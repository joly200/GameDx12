#include "DxWin.h"
#include <tchar.h>
#include "BaseObject.h"
DxWin::DxWin()
{

}

int DxWin::initializeDxWin(HWND hWnd, UINT width, UINT height)
{
    if (!SetClientSize(width, height)) {
        return 0;
    }
    if (!initialize(hWnd)) {
        return 0;
    }
    if (!SetRenderPipeline()) {
        return 0;
    }
    camera = new Camera(ClientWidth, ClientHeight, lpDevice.Get(), CbvSrvUavDescSize, mCbvHeap.Get(), CbvHeapHandle,&tool ,& CbvHeapindex);
    //cube = new Cube(lpDevice, lpCommandList, lpCommandQueue, lpCommandAllocator, mCbvHeap, CbvSrvUavDescSize);
    return 1;
}

DxWin::~DxWin()
{
    delete camera;
}

int DxWin::initialize(HWND hWnd)
{
#if defined(DEBUG)||defined(_DEBUG)
	{
		D3D12GetDebugInterface(IID_PPV_ARGS(lpDebug.GetAddressOf()));
		lpDebug->EnableDebugLayer();
	}
#endif
//HRESULT hr = D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_12_1, IID_PPV_ARGS(lpDevice.GetAddressOf()));

HRESULT hr = CreateDXGIFactory(IID_PPV_ARGS(lpFactory.GetAddressOf()));
if (hr != S_OK) {
	return 0;
}
ComPtr<IDXGIAdapter>lpAdapter;
UINT selectedAdapter = 0 , maxDedicatedVideoMemory = 0;
for (UINT i = 0; i < 100; i++)
{


	hr = lpFactory->EnumAdapters(i, &lpAdapter);
	if (hr != S_OK) {
		break;
	}
    DXGI_ADAPTER_DESC desc;
	lpAdapter->GetDesc(&desc);
	if (desc.DedicatedVideoMemory > maxDedicatedVideoMemory)
	{
		maxDedicatedVideoMemory = desc.DedicatedVideoMemory;
		selectedAdapter = i;
	}

}
if (FAILED(lpFactory->EnumAdapters(selectedAdapter, &lpAdapter)))return 0;
hr = D3D12CreateDevice(lpAdapter.Get(), D3D_FEATURE_LEVEL_12_1, IID_PPV_ARGS(lpDevice.GetAddressOf()));
if (hr != S_OK) {
	return 0;
}

hr = lpDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(lpFence.GetAddressOf()));
if (hr != S_OK) {
	return 0;
}

RtvDescSize = lpDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
DsvDescSize = lpDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
CbvSrvUavDescSize = lpDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
SamplerSize = lpDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER);
D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS QualityLevels;
QualityLevels.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
QualityLevels.SampleCount = 4;
QualityLevels.Flags = D3D12_MULTISAMPLE_QUALITY_LEVELS_FLAG_NONE;
QualityLevels.NumQualityLevels = 0;
hr = lpDevice->CheckFeatureSupport(D3D12_FEATURE_MULTISAMPLE_QUALITY_LEVELS, &QualityLevels, sizeof(D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS));


D3D12_FEATURE_DATA_FORMAT_SUPPORT formatSupport = {};
formatSupport.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
hr = lpDevice->CheckFeatureSupport(D3D12_FEATURE_FORMAT_SUPPORT, &formatSupport, sizeof(formatSupport));
if (hr == S_OK) {
    if (formatSupport.Support1 & D3D12_FORMAT_SUPPORT1_TYPED_UNORDERED_ACCESS_VIEW) {
        printf("Format supports UAV.\n");
    }
    else {
        printf("Format does not support UAV.\n");
    }
}


if (hr != S_OK) return 0;
D3D12_COMMAND_QUEUE_DESC DescQueue;
DescQueue.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
DescQueue.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
DescQueue.NodeMask = 0;
DescQueue.Priority = 0;
hr = lpDevice->CreateCommandQueue(&DescQueue, IID_PPV_ARGS(lpCommandQueue.GetAddressOf()));
if (hr != S_OK) return 0;

hr = lpDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(lpCommandAllocator.GetAddressOf()));
if (hr != S_OK) return 0;

hr = lpDevice->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, lpCommandAllocator.Get(), nullptr, IID_PPV_ARGS(&lpCommandList));
lpCommandList->Close();
if (hr != S_OK) return 0;

D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS qualityLevels = {};
qualityLevels.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // 常用后备缓冲区格式
qualityLevels.SampleCount = 8; // 检查4x采样
hr = lpDevice->CheckFeatureSupport(D3D12_FEATURE_MULTISAMPLE_QUALITY_LEVELS,&qualityLevels,sizeof(qualityLevels));
if (hr == S_OK)
{
    if (qualityLevels.NumQualityLevels > 0)
    {
        // 支持4x MSAA
    }
}


RECT rect;
GetWindowRect(hWnd, &rect);

DXGI_SWAP_CHAIN_DESC SwapDesc;
SwapDesc.BufferDesc.Width = ClientWidth;
SwapDesc.BufferDesc.Height = ClientHeight;
SwapDesc.BufferDesc.RefreshRate.Numerator = 60;
SwapDesc.BufferDesc.RefreshRate.Denominator = 1;
SwapDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
SwapDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
SwapDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
SwapDesc.SampleDesc.Count = 1;
SwapDesc.SampleDesc.Quality = 0;
SwapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
SwapDesc.BufferCount = BufferCount;
SwapDesc.OutputWindow = hWnd;
SwapDesc.Windowed = true;
SwapDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
SwapDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
//|D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;

hr = lpFactory->CreateSwapChain(lpCommandQueue.Get(), &SwapDesc, lpSwapChain.GetAddressOf());
if (FAILED(hr)) 
    return 0;

D3D12_DESCRIPTOR_HEAP_DESC RtvHeapDesc;
RtvHeapDesc.NumDescriptors = BUFFERCOUNT;
RtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
RtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
RtvHeapDesc.NodeMask = 0;
hr = lpDevice->CreateDescriptorHeap(&RtvHeapDesc, IID_PPV_ARGS(mRtvHeap.GetAddressOf()));
if (hr != S_OK) return 0;
DxWin::RtvHeapHandle.InitOffsetted(mRtvHeap->GetCPUDescriptorHandleForHeapStart(), 0, RtvDescSize);

D3D12_DESCRIPTOR_HEAP_DESC DsvHeapDesc;
DsvHeapDesc.NumDescriptors = 1;
DsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
DsvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
DsvHeapDesc.NodeMask = 0;
hr = lpDevice->CreateDescriptorHeap(&DsvHeapDesc, IID_PPV_ARGS(mDsvHeap.GetAddressOf()));
if (hr != S_OK) return 0;
DsvHeapHandle.InitOffsetted(mDsvHeap->GetCPUDescriptorHandleForHeapStart(), 0, DsvDescSize);
//lpCommandList->Reset(lpCommandAllocator.Get(), nullptr);

D3D12_DESCRIPTOR_HEAP_DESC CbvHeapDesc;
CbvHeapDesc.NumDescriptors = 10;
CbvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
CbvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
CbvHeapDesc.NodeMask = 0;
hr = lpDevice->CreateDescriptorHeap(&CbvHeapDesc, IID_PPV_ARGS(mCbvHeap.GetAddressOf()));
if (hr != S_OK) return 0;
DxWin::CbvHeapHandle.InitOffsetted(mCbvHeap->GetCPUDescriptorHandleForHeapStart(), 0, CbvSrvUavDescSize);
D3D12_DESCRIPTOR_HEAP_DESC SrvHeapDesc;
SrvHeapDesc.NumDescriptors = 1;
SrvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
SrvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
SrvHeapDesc.NodeMask = 0;
hr = lpDevice->CreateDescriptorHeap(&SrvHeapDesc, IID_PPV_ARGS(mSrvHeap.GetAddressOf()));
if (hr != S_OK) return 0;
DxWin::SrvHeapHandle.InitOffsetted(mSrvHeap->GetCPUDescriptorHandleForHeapStart(), 0, CbvSrvUavDescSize);
D3D12_DESCRIPTOR_HEAP_DESC UavHeapDesc;
UavHeapDesc.NumDescriptors = 2;
UavHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
UavHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
UavHeapDesc.NodeMask = 0;
hr = lpDevice->CreateDescriptorHeap(&UavHeapDesc, IID_PPV_ARGS(mUavHeap.GetAddressOf()));
if (hr != S_OK) 
    return 0;
DxWin::UavHeapHandle.InitOffsetted(mUavHeap->GetCPUDescriptorHandleForHeapStart(), 0, CbvSrvUavDescSize);
D3D12_DESCRIPTOR_HEAP_DESC SamplerHeapDesc;
SamplerHeapDesc.NumDescriptors = 6;
SamplerHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER;
SamplerHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
SamplerHeapDesc.NodeMask = 0;
hr = lpDevice->CreateDescriptorHeap(&SamplerHeapDesc, IID_PPV_ARGS(mSamplerHeap.GetAddressOf()));
if (hr != S_OK) 
    return 0;
DxWin::SamplerHeapHandle.InitOffsetted(mSamplerHeap->GetCPUDescriptorHandleForHeapStart(), 0, SamplerSize);


for (UINT i = 0; i < BufferCount; i++)
{
	hr = lpSwapChain->GetBuffer(i, IID_PPV_ARGS(SwapBufferRes[i].GetAddressOf()));
	if (hr != S_OK) return 0;
    lpDevice->CreateRenderTargetView(SwapBufferRes[i].Get(), nullptr, DxWin::RtvHeapHandle);

    DxWin::RtvHeapHandle.Offset(1, RtvDescSize);

}
if (!tool.AddComtexture(lpDevice.Get(), &InputRes, ClientWidth, ClientHeight, CbvSrvUavDescSize, UavHeapHandle)) {
	return 0;
}
if (!tool.AddComtexture(lpDevice.Get(), &OutputRes, ClientWidth, ClientHeight, CbvSrvUavDescSize,UavHeapHandle)) {
	return 0;
}

D3D12_RESOURCE_DESC ResDepDesc;
ResDepDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
ResDepDesc.Alignment = 0;
ResDepDesc.Width = ClientWidth;
ResDepDesc.Height = ClientHeight;
ResDepDesc.DepthOrArraySize = 1;
ResDepDesc.MipLevels = 1;
ResDepDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
ResDepDesc.SampleDesc.Count = 1;
ResDepDesc.SampleDesc.Quality = 0;
ResDepDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
ResDepDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;//| D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;
D3D12_CLEAR_VALUE optClear;
optClear.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
optClear.DepthStencil.Depth = 1.0f;
optClear.DepthStencil.Stencil = 0;

hr = lpDevice->CreateCommittedResource(&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT), D3D12_HEAP_FLAG_NONE, &ResDepDesc, 
    D3D12_RESOURCE_STATE_COMMON, &optClear, IID_PPV_ARGS(&DepthStencilRes));
if (hr != S_OK) return 0;
CD3DX12_CPU_DESCRIPTOR_HANDLE DsvHeapHandle(mDsvHeap->GetCPUDescriptorHandleForHeapStart());
lpDevice->CreateDepthStencilView(DepthStencilRes.Get(), nullptr, DsvHeapHandle);
DsvHeapHandle.Offset(1, DsvDescSize);

lpCommandList->Reset(lpCommandAllocator.Get(), nullptr);
lpCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(DepthStencilRes.Get(), D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_DEPTH_WRITE));

Vp.Width = static_cast<float>(ClientWidth);
Vp.Height = static_cast<float>(ClientHeight);
Vp.TopLeftX = 0;
Vp.TopLeftY = 0;
Vp.MaxDepth = 1.0f;
Vp.MinDepth = 0.0f;
lpCommandList->RSSetViewports(1, &Vp);

SciRect = { 0, 0, (long)ClientWidth , (long)ClientHeight };
lpCommandList->RSSetScissorRects(1, &SciRect);

lpCommandList->Close();

ID3D12CommandList* cmdsLists[] = { lpCommandList.Get()};
lpCommandQueue->ExecuteCommandLists(1, cmdsLists);

return 1;
}

int DxWin::SetRenderPipeline()
{
    HRESULT hr;
    ID3DBlob* errorBlob = nullptr;
    ID3DBlob* CSCode = nullptr;
    //hr = D3DCompileFromFile(GOTOPATH(L"Shader\\ComputerShader.hlsl"), NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, "CSMain", "cs_5_0", 5, 0, &CSCode, &errorBlob);
    hr = D3DReadFileToBlob(tool.gotoPath(L"Shader\\ComputerShader.cso"), &CSCode);
    if (FAILED(hr))
        return 0;
    //cRootSignature
    CD3DX12_DESCRIPTOR_RANGE uavRange;
    uavRange.Init(D3D12_DESCRIPTOR_RANGE_TYPE_UAV, 1, 0);
    CD3DX12_DESCRIPTOR_RANGE uavRange1;
    uavRange1.Init(D3D12_DESCRIPTOR_RANGE_TYPE_UAV, 1, 1);
    CD3DX12_ROOT_PARAMETER rootParameters[2];
    rootParameters[0].InitAsDescriptorTable(1, &uavRange, D3D12_SHADER_VISIBILITY_ALL);
    rootParameters[1].InitAsDescriptorTable(1, &uavRange1, D3D12_SHADER_VISIBILITY_ALL);
    // 定义根签名描述
    CD3DX12_ROOT_SIGNATURE_DESC rootSignatureDesc(2, rootParameters, 0, 
        nullptr, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

    ID3DBlob* csignatureBlob = nullptr;

    hr = D3D12SerializeRootSignature(
        &rootSignatureDesc,
        D3D_ROOT_SIGNATURE_VERSION_1,
        &csignatureBlob,
        &errorBlob
    );
    if (FAILED(hr))return 0;
    // 创建根签名

    lpDevice->CreateRootSignature(
        0,
        csignatureBlob->GetBufferPointer(),
        csignatureBlob->GetBufferSize(),
        IID_PPV_ARGS(&cRootSignature)
    );

	D3D12_COMPUTE_PIPELINE_STATE_DESC cpsoDesc;
	cpsoDesc.pRootSignature = cRootSignature.Get();
	cpsoDesc.CS = { CSCode->GetBufferPointer(),CSCode->GetBufferSize() };
    cpsoDesc.NodeMask = 0;
	cpsoDesc.CachedPSO = { nullptr,0 };
	cpsoDesc.Flags = D3D12_PIPELINE_STATE_FLAG_NONE;
	hr = lpDevice->CreateComputePipelineState(&cpsoDesc, IID_PPV_ARGS(&CSO));
    ThreadX = ceil(ClientWidth / 32);
    ThreadY = ceil(ClientHeight / 32);
    ThreadZ = 1;
    return 1;
}

UINT DxWin::GetBufferIndex()
{
    static UINT i = BUFFERCOUNT -1;
    i++;
    if (i == BUFFERCOUNT) i = 0;
    return i;
}

int DxWin::DrawGraphics()
{
    HRESULT hr;
    static UINT ping = 0;
    CurrebtSwapChainBuffer = GetBufferIndex();
    //lpCommandList->ClearState();

	//cube->updatacCubeState(GameTimer::lpGameTimer->GetStartToCurrentTick());
    ShowObject();
    


    hr = lpSwapChain->Present(0, 0);

    lpCommandAllocator->Reset();
    return 0;
}

int DxWin::ShowObject()
{
    CD3DX12_GPU_DESCRIPTOR_HANDLE mUavGpuHeap(mUavHeap->GetGPUDescriptorHandleForHeapStart());
    D3D12_GPU_DESCRIPTOR_HANDLE Uav1 = mUavGpuHeap.Offset(0, CbvSrvUavDescSize);
    D3D12_GPU_DESCRIPTOR_HANDLE Uav2 = mUavGpuHeap.Offset(1, CbvSrvUavDescSize);
    BaseObject::arg.CurrebtSwapChainBuffer = CurrebtSwapChainBuffer;
    HRESULT hr = lpCommandList->Reset(lpCommandAllocator.Get(),nullptr);
    lpCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(
        SwapBufferRes[CurrebtSwapChainBuffer].Get(),
        D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET));
    lpCommandList->ClearRenderTargetView(CD3DX12_CPU_DESCRIPTOR_HANDLE(mRtvHeap->GetCPUDescriptorHandleForHeapStart(), CurrebtSwapChainBuffer, RtvDescSize),
        DirectX::Colors::Black, 0, nullptr);
    lpCommandList->ClearDepthStencilView(mDsvHeap->GetCPUDescriptorHandleForHeapStart(), D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, nullptr);
    hr = lpCommandList->Close();
    ID3D12CommandList* cmdsLists[] = { lpCommandList.Get() };
    lpCommandQueue->ExecuteCommandLists(1, cmdsLists);
    FlushCommandQueue();
    BaseObject::arg.Pos[0] = camera->GetPosition3f().x;
    BaseObject::arg.Pos[1] = camera->GetPosition3f().y;
    BaseObject::arg.Pos[2] = camera->GetPosition3f().z;
    BaseObject::arg.timestamp = GameTimer::GameTimerInstance()->GetStartToCurrentTick();
    for (std::list<BaseObject::DllsAndObjects>::iterator it = BaseObject::lpObjects.begin(); it != BaseObject::lpObjects.end(); ++it) {
        if ((*it).lpObject->CheckForIntersection(camera->GetCameraInformation())) {
            (*it).lpObject->BaseObjectFun(BaseObject::arg);
            (*it).lpObject->ShowMethod(BaseObject::arg);
        }
    }


    hr = lpCommandList->Reset(lpCommandAllocator.Get(), (CSO.Get()));
    lpCommandList->RSSetViewports(1, &(Vp));
    lpCommandList->RSSetScissorRects(1, &SciRect);
    lpCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(
        SwapBufferRes[CurrebtSwapChainBuffer].Get(),
        D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_COPY_SOURCE));
    lpCommandList->CopyResource(InputRes.Get(), SwapBufferRes[CurrebtSwapChainBuffer].Get());
    lpCommandList->SetComputeRootSignature(cRootSignature.Get());
    lpCommandList->SetPipelineState(CSO.Get());

    ID3D12DescriptorHeap* ppHeaps[] = { mUavHeap.Get() };
    lpCommandList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
    lpCommandList->SetComputeRootDescriptorTable(0, Uav1);
    lpCommandList->SetComputeRootDescriptorTable(1, Uav2);
    lpCommandList->Dispatch(ThreadX, ThreadY, 1);
    lpCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(
        SwapBufferRes[CurrebtSwapChainBuffer].Get(),
        D3D12_RESOURCE_STATE_COPY_SOURCE, D3D12_RESOURCE_STATE_COPY_DEST));
    lpCommandList->CopyResource(SwapBufferRes[CurrebtSwapChainBuffer].Get(), OutputRes.Get());
    lpCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(
        SwapBufferRes[CurrebtSwapChainBuffer].Get(),
        D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_PRESENT));
    lpCommandList->Close();
    cmdsLists[0] = { lpCommandList.Get() };
    lpCommandQueue->ExecuteCommandLists(1, cmdsLists);
    FlushCommandQueue();
    return 0;
}

int DxWin::SetViewPort(UINT width, UINT height)
{
    Vp.Height = height;
    Vp.Width = width;
    return 1;
}

int DxWin::SetClientSize(UINT width, UINT height)
{
    ClientWidth = width;
    ClientHeight = height;
    return 1;
}
void DxWin::FlushCommandQueue()
{
    // Advance the fence value to mark commands up to this fence point.
    CurrentFence++;

    // Add an instruction to the command queue to set a new fence point.  Because we 
    // are on the GPU timeline, the new fence point won't be set until the GPU finishes
    // processing all the commands prior to this Signal().
    lpCommandQueue->Signal(lpFence.Get(), CurrentFence);

    // Wait until the GPU has completed commands up to this fence point.
    if (lpFence->GetCompletedValue() < CurrentFence)
    {
        HANDLE eventHandle = CreateEventEx(nullptr, false, false, EVENT_ALL_ACCESS);

        // Fire event when GPU hits current fence.  
        lpFence->SetEventOnCompletion(CurrentFence, eventHandle);

        // Wait until the GPU hits current fence event is fired.
        WaitForSingleObject(eventHandle, INFINITE);
        CloseHandle(eventHandle);
    }
}
void DxWin::StaticFlushCommandQueue()
{
    lpDxWin->FlushCommandQueue();
}
DxWin::DxWinHelper DxWin::dxWinhelper;
DxWin* DxWin::lpDxWin = DxWinInstance();

CD3DX12_CPU_DESCRIPTOR_HANDLE DxWin::CbvHeapHandle;
CD3DX12_CPU_DESCRIPTOR_HANDLE DxWin::RtvHeapHandle;
CD3DX12_CPU_DESCRIPTOR_HANDLE DxWin::DsvHeapHandle;
CD3DX12_CPU_DESCRIPTOR_HANDLE DxWin::SrvHeapHandle;
CD3DX12_CPU_DESCRIPTOR_HANDLE DxWin::UavHeapHandle;
CD3DX12_CPU_DESCRIPTOR_HANDLE DxWin::SamplerHeapHandle;
