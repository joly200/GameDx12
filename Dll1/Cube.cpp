#include "pch.h"
#include "Cube.h"

struct Vertex {
    float x, y, z;     // 顶点位置
    float r, g, b, a;  // 颜色 (红, 绿, 蓝, 透明度)
    float u, v;        // 纹理坐标
};

Cube::Cube(virtualBase::UpdataArg* Paraml):r(2)
{
    const wchar_t Name[] = L"Cube";
    printf("Cube");
    memcpy(ObjectName, Name, OBJECT_NAME);
    fun = Paraml->fun;
    Vertex cubeBuffer[] = {


    { -1.0f,  1.0f,  1.0f, 1.0f, 0.0f, 0.0f, 1.0f,  0.0f, 0.0f }, // Top-left
    {  1.0f,  1.0f,  1.0f, 0.0f, 1.0f, 0.0f, 1.0f,  1.0f, 0.0f }, // Top-right
    {  1.0f, -1.0f,  1.0f,  0.0f, 0.0f, 1.0f,1.0f,  1.0f, 1.0f }, // Bottom-right
    { -1.0f, -1.0f,  1.0f,  1.0f, 1.0f, 0.0f, 1.0f,  0.0f, 1.0f }, // Bottom-left

    // Back face (Z = -1.0f)
    { -1.0f,  1.0f, -1.0f,  1.0f, 0.0f, 1.0f, 1.0f,  0.0f, 0.0f }, // Top-left
    {  1.0f,  1.0f, -1.0f,  0.0f, 1.0f, 1.0f, 1.0f,  1.0f, 0.0f }, // Top-right
    {  1.0f, -1.0f, -1.0f,  1.0f, 1.0f, 0.0f, 1.0f,  1.0f, 1.0f }, // Bottom-right
    { -1.0f, -1.0f, -1.0f,  0.0f, 0.0f, 1.0f, 1.0f,  0.0f, 1.0f }, // Bottom-left

    // Top face (Y = 1.0f)
    { -1.0f,  1.0f, -1.0f,  1.0f, 0.0f, 1.0f, 1.0f,  0.0f, 0.0f }, // Top-left
    {  1.0f,  1.0f, -1.0f,  0.0f, 1.0f, 1.0f, 1.0f,  1.0f, 0.0f }, // Top-right
    {  1.0f,  1.0f,  1.0f,  0.0f, 1.0f, 0.0f, 1.0f,  1.0f, 1.0f }, // Bottom-right
    { -1.0f,  1.0f,  1.0f,  1.0f, 0.0f, 0.0f, 1.0f,  0.0f, 1.0f }, // Bottom-left

    // Bottom face (Y = -1.0f)
    { -1.0f, -1.0f, -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,  0.0f, 0.0f }, // Top-left
    {  1.0f, -1.0f, -1.0f,  0.0f, 1.0f, 0.0f, 1.0f,  1.0f, 0.0f }, // Top-right
    {  1.0f, -1.0f,  1.0f,  0.0f, 0.0f, 0.0f, 1.0f,  1.0f, 1.0f }, // Bottom-right
    { -1.0f, -1.0f,  1.0f,  1.0f, 1.0f, 0.0f, 1.0f,  0.0f, 1.0f }, // Bottom-left

    // Left face (X = -1.0f)
    { -1.0f,  1.0f,  1.0f,  1.0f, 0.0f, 0.0f, 1.0f,  0.0f, 0.0f }, // Top-left
    { -1.0f,  1.0f, -1.0f,  0.0f, 1.0f, 0.0f, 1.0f,  1.0f, 0.0f }, // Top-right
    { -1.0f, -1.0f, -1.0f,  0.0f, 0.0f, 1.0f, 1.0f,  1.0f, 1.0f }, // Bottom-right
    { -1.0f, -1.0f,  1.0f,  1.0f, 1.0f, 0.0f, 1.0f,  0.0f, 1.0f }, // Bottom-left

    // Right face (X = 1.0f)
    {  1.0f,  1.0f,  1.0f,  1.0f, 0.0f, 1.0f, 1.0f,  0.0f, 0.0f }, // Top-left
    {  1.0f,  1.0f, -1.0f,  0.0f, 1.0f, 1.0f, 1.0f,  1.0f, 0.0f }, // Top-right
    {  1.0f, -1.0f, -1.0f,  0.0f, 0.0f, 0.0f, 1.0f,  1.0f, 1.0f }, // Bottom-right
    {  1.0f, -1.0f,  1.0f,  1.0f, 1.0f, 1.0f, 1.0f,  0.0f, 1.0f }, // Bottom-left
    };

    cubeSize = sizeof(cubeBuffer);
    //memcpy(cube, cubeBuffer, cubeSize);
    unsigned int cubeindicesBuffer[]{

    1, 0, 2, 2, 0, 3,
    // Back face
    4, 5, 6, 4, 6, 7,
    // Top face
    9, 8, 10 ,10, 8, 11,
    // Bottom face
    12, 13, 14, 12, 14, 15,
    // Left face
    16, 17, 18, 16, 18, 19,
    // Right face
    21, 20, 22, 22, 20, 23
    };
    cubeindicesSize = sizeof(cubeindicesBuffer);
    //memcpy(cubeindices, cubeindicesBuffer, cubeindicesSize);
    //uint64_t ptr = (uint64_t)lpDevice.Get();
    
    cubedefaultBuffer = Paraml->tool->uploadresoure(Paraml->lpDevice.Get(), cubeuploadBuffer.GetAddressOf(), Paraml->lpCommandList.Get(), Paraml->lpCommandQueue.Get(), Paraml->lpCommandAllocator.Get()
        , cubeBuffer, cubeSize
    );
    Paraml->FlushCommandQueue();
    cubeuploadBuffer->Release();
    *cubeuploadBuffer.GetAddressOf() = nullptr;

    cubeindicesdefaultBuffer = Paraml->tool->uploadresoure(Paraml->lpDevice.Get(), cubeuploadBuffer.GetAddressOf(), Paraml->lpCommandList.Get(), Paraml->lpCommandQueue.Get(), Paraml->lpCommandAllocator.Get()
        , cubeindicesBuffer, cubeindicesSize
    );
    Paraml->FlushCommandQueue();
    cubeuploadBuffer->Release();
    *cubeuploadBuffer.GetAddressOf() = nullptr;
    cubeState = {
        1.0f,0.0f,0.0f,0.0f,
        0.0f,1.0f,0.0f,0.0f,
        0.0f,0.0f,1.0f,0.0f,
        0.0f,0.0f,0.0f,1.0f
    };
    //wchar_t Current[MAX_PATH];
    //DWORD length = GetModuleFileName(nullptr, Current, MAX_PATH);
    dXstring path;
    //path.resize(MAX_PATH);
    //memcpy((void*)path.c_str(), Current, MAX_PATH);
    //Current
    path = Paraml->tool->gotoPath(L"src\\xiaobao.dds");

    lpcubeState = Paraml->tool->conveyGpu(Paraml->lpDevice.Get(), &stateBuffer, Paraml->CbvSrvUavDescSize, sizeof(cubeState), Paraml->CbvHeapHandle);
   
    Paraml->CbvHeapHandle.Offset(1, Paraml->CbvSrvUavDescSize);
    Mr1.ptr = Paraml->mCbvHeap->GetGPUDescriptorHandleForHeapStart().ptr +
        *Paraml->CbvHeapindex * Paraml->CbvSrvUavDescSize;
    *Paraml->CbvHeapindex += 1;
   
    HRESULT hr = Paraml->tool->CreateDDSToBuffer(Paraml->lpDevice, Paraml->lpCommandList,
        path.c_str(), Paraml->lpCommandAllocator, textureUploadBuffer,
        texturedefaultBuffer,
        Paraml->lpCommandQueue);

    if (hr != S_OK)return;
    D3D12_SHADER_RESOURCE_VIEW_DESC XiaobaoTextureView;
    D3D12_RESOURCE_DESC ptr = texturedefaultBuffer->GetDesc();
    XiaobaoTextureView.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
    XiaobaoTextureView.Format = texturedefaultBuffer->GetDesc().Format;
    XiaobaoTextureView.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
    XiaobaoTextureView.Texture2D.PlaneSlice = 0;
    XiaobaoTextureView.Texture2D.MostDetailedMip = 0;
    XiaobaoTextureView.Texture2D.MipLevels = texturedefaultBuffer->GetDesc().MipLevels;
    XiaobaoTextureView.Texture2D.ResourceMinLODClamp = 0.0f;
    Paraml->lpDevice->CreateShaderResourceView(texturedefaultBuffer.Get(), &XiaobaoTextureView, Paraml->CbvHeapHandle);


    Paraml->CbvHeapHandle.Offset(1, Paraml->CbvSrvUavDescSize);
    Mr2.ptr = Paraml->mCbvHeap->GetGPUDescriptorHandleForHeapStart().ptr +
        *Paraml->CbvHeapindex * Paraml->CbvSrvUavDescSize;
    *Paraml->CbvHeapindex += 1;
    //DirectXTool::SrvHeapHandle.Offset()
    samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;  // 设置线性过滤
    samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
    samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
    samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
    samplerDesc.MinLOD = 0;
    samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;
    samplerDesc.MipLODBias = 0.0f;
    samplerDesc.MaxAnisotropy = 1;
    samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
    Paraml->lpDevice->CreateSampler(&samplerDesc, Paraml->SamplerHeapHandle);
    Paraml->SamplerHeapHandle.Offset(1, Paraml->lpDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER));
    memcpy(lpcubeState, &cubeState, sizeof(DirectX::XMFLOAT4X4));
    Setpipe(Paraml->lpDevice, Paraml);
    Paraml->FlushCommandQueue();
    textureUploadBuffer->Release();
    *textureUploadBuffer.GetAddressOf() = nullptr;
 
}

Cube::~Cube()
{
    if (stateBuffer)stateBuffer->Unmap(0, nullptr);
}

int Cube::updatacCubeState(ULONGLONG starttoCurrentTick)
{
    DirectX::XMVECTOR Vector1 = { 0.0f,1.0f,0.0f };
    return 1;
}

void Cube::SetShowPostion(DirectX::XMFLOAT3 Vector)
{
    DirectX::XMFLOAT4X4* lpcube = (DirectX::XMFLOAT4X4*)lpcubeState;
    lpcube->_41 = Vector.x;
    lpcube->_42 = Vector.y;
    lpcube->_43 = Vector.z;
}

void Cube::Setpipe(ComPtr<ID3D12Device>& lpDevice, virtualBase::UpdataArg* Paraml)
{
    HRESULT hr;
    D3D12_INPUT_ELEMENT_DESC vertexDesc[] = {
        {
            "POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,
            D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
        },
        {
            "COLOR",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,12,
            D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
        }
        ,
        {
            "TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,28,
            D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
        }
    };
    //"C:\\Users\\Administrator\\source\\repos\\GameDx12\\GameDx12
    ID3DBlob* VSCode = nullptr;
    ID3DBlob* errors = nullptr;
    dXstring Result;
    
  
    hr = D3DReadFileToBlob(Paraml->tool->gotoPath(L"Shader\\CubeVertexShader.cso"), &VSCode);
    if (FAILED(hr))MessageBox(NULL, Paraml->tool->gotoPath(L"Shader\\CubeVertexShader.cso"), L"D3DCompileFromFile", 0);
    ID3DBlob* PSCode = nullptr;

    
    hr = D3DReadFileToBlob(Paraml->tool->gotoPath(L"Shader\\CubePixelShader.cso"), &PSCode);
    if (FAILED(hr))MessageBox(NULL, L"CubePixelShader Error", L"D3DCompileFromFile", 0);
    CD3DX12_ROOT_PARAMETER slotRootParameter[4];

    CD3DX12_DESCRIPTOR_RANGE Sampler;
    CD3DX12_DESCRIPTOR_RANGE cbvTable[3];
    // srbTable.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);
    cbvTable[0].Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 0);
    cbvTable[1].Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 1);
    cbvTable[2].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);
    Sampler.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER, 1, 0);

    slotRootParameter[0].InitAsDescriptorTable(1, &cbvTable[0]);
    slotRootParameter[1].InitAsDescriptorTable(1, &cbvTable[1]);
    slotRootParameter[2].InitAsDescriptorTable(1, &cbvTable[2]);
    slotRootParameter[3].InitAsDescriptorTable(1, &Sampler);
    //slotRootParameter[3].InitAsConstantBufferView(0);
   // slotRootParameter[1].InitAsDescriptorTable(1, &srbTable);

    CD3DX12_ROOT_SIGNATURE_DESC rootSigDesc(4, slotRootParameter, 0, nullptr,
        D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);
    ID3DBlob* serializedRootSig = nullptr;
    ID3DBlob* errorBlob = nullptr;

    hr = D3D12SerializeRootSignature(&rootSigDesc, D3D_ROOT_SIGNATURE_VERSION_1,
        &serializedRootSig, &errorBlob);

    hr = lpDevice->CreateRootSignature(
        0,
        serializedRootSig->GetBufferPointer(),
        serializedRootSig->GetBufferSize(),
        IID_PPV_ARGS(mRootSignature.GetAddressOf()));
    D3D12_BLEND_DESC blendDesc = {};
    blendDesc.AlphaToCoverageEnable = FALSE;
    blendDesc.IndependentBlendEnable = FALSE;
    blendDesc.RenderTarget[0].BlendEnable = FALSE; // 启用混合
    blendDesc.RenderTarget[0].LogicOpEnable = FALSE; // 禁用逻辑运算
    blendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA; // 源因子：源颜色的 Alpha
    blendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_INV_SRC_ALPHA; // 目标因子：1 - 源颜色的 Alpha
    blendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD; // 混合操作：相加
    blendDesc.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE; // Alpha 源因子：1
    blendDesc.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO; // Alpha 目标因子：0
    blendDesc.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD; // Alpha 混合操作：相加
    blendDesc.RenderTarget[0].LogicOp = D3D12_LOGIC_OP_NOOP; // 逻辑运算：无
    blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL; // 启用所有颜色通道
    D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc;
    ZeroMemory(&psoDesc, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));
    psoDesc.InputLayout = { vertexDesc , sizeof(vertexDesc) / sizeof(D3D12_INPUT_ELEMENT_DESC) };
    psoDesc.pRootSignature = mRootSignature.Get();
    psoDesc.VS = { VSCode->GetBufferPointer(),VSCode->GetBufferSize() };
    psoDesc.PS = { PSCode->GetBufferPointer(),PSCode->GetBufferSize() };
    psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
    psoDesc.BlendState = blendDesc;

    psoDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
    psoDesc.SampleMask = UINT_MAX;
    psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
    psoDesc.NumRenderTargets = 1;
    psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
    psoDesc.SampleDesc.Count = 1;
    psoDesc.SampleDesc.Quality = 0;
    psoDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;

    hr = lpDevice->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&PSO));


}



bool Cube::CheckForIntersection(CameraInformation* infPtr)
{
    return fun(infPtr, lpcubeState, r);
}

int64_t Cube::BaseObjectFun(UpdataArg& Arg)
{
    return 0;
}

int64_t Cube::ShowMethod(UpdataArg& Arg)
{
    HRESULT hr;
    ID3D12DescriptorHeap* descriptorHeaps[] = { Arg.mCbvHeap.Get(),Arg.mSamplerHeap.Get() };
    D3D12_VERTEX_BUFFER_VIEW VertexView = { cubedefaultBuffer->GetGPUVirtualAddress(),
    cubeSize,
    36
    };
    D3D12_INDEX_BUFFER_VIEW IndexView = { cubeindicesdefaultBuffer->GetGPUVirtualAddress() ,
    cubeindicesSize ,DXGI_FORMAT_R32_UINT };
    CD3DX12_GPU_DESCRIPTOR_HANDLE mCbvGpuHeap(Arg.mCbvHeap->GetGPUDescriptorHandleForHeapStart());
    
    CD3DX12_GPU_DESCRIPTOR_HANDLE mSamplerGpuHeap(Arg.mSamplerHeap->GetGPUDescriptorHandleForHeapStart());


    SetShowPostion({ 10,0,0 });
    D3D12_GPU_DESCRIPTOR_HANDLE BaseDescriptor0 = mCbvGpuHeap;
    D3D12_GPU_DESCRIPTOR_HANDLE BaseDescriptor1 = mCbvGpuHeap.Offset(1, Arg.CbvSrvUavDescSize);
    D3D12_GPU_DESCRIPTOR_HANDLE BaseDescriptor2 = mCbvGpuHeap.Offset(1, Arg.CbvSrvUavDescSize);

    hr = Arg.lpCommandList->Reset(Arg.lpCommandAllocator.Get(), PSO.Get());
    Arg.lpCommandList->RSSetViewports(1, &Arg.Vp);

    Arg.lpCommandList->RSSetScissorRects(1, &Arg.SciRect);
    
    Arg.lpCommandList->OMSetRenderTargets(1, &CD3DX12_CPU_DESCRIPTOR_HANDLE(Arg.mRtvHeap->GetCPUDescriptorHandleForHeapStart(), Arg.CurrebtSwapChainBuffer, Arg.RtvDescSize)
        , true, &Arg.mDsvHeap->GetCPUDescriptorHandleForHeapStart());

    Arg.lpCommandList->SetDescriptorHeaps(2, &descriptorHeaps[0]);

    Arg.lpCommandList->SetGraphicsRootSignature(mRootSignature.Get());

    Arg.lpCommandList->IASetVertexBuffers(0, 1, &VertexView);

    Arg.lpCommandList->IASetIndexBuffer(&IndexView);

    Arg.lpCommandList->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    Arg.lpCommandList->SetGraphicsRootDescriptorTable(0, BaseDescriptor0);


    Arg.lpCommandList->SetGraphicsRootDescriptorTable(1, Mr1);

    Arg.lpCommandList->SetGraphicsRootDescriptorTable(2, Mr2);

    Arg.lpCommandList->SetGraphicsRootDescriptorTable(3, mSamplerGpuHeap);

   


    Arg.lpCommandList->DrawIndexedInstanced(36, 1, 0, 0, 0);

    hr = Arg.lpCommandList->Close();
    ID3D12CommandList* cmdsLists[] = { Arg.lpCommandList.Get() };
    Arg.lpCommandQueue->ExecuteCommandLists(1, cmdsLists);

    Arg.FlushCommandQueue();


    return 0;
}
