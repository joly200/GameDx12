#include "BaseObject.h"

BaseObject::InputArg assa;


bool BaseObject::CheckForIntersectionFun(CameraInformation* infPtr, void* lpState, float r)
{
    float result;
    DirectX::XMFLOAT4X4* lp = (DirectX::XMFLOAT4X4*)lpState;
    result = (infPtr->n.A * lp->_41 + infPtr->n.B * lp->_42 + infPtr->n.C * lp->_43 + infPtr->n.D) /
        sqrtf(infPtr->n.A * infPtr->n.A + infPtr->n.B * infPtr->n.B + infPtr->n.C * infPtr->n.C);
    if (result - r > 0)return false;
    result = (infPtr->f.A * lp->_41 + infPtr->f.B * lp->_42 + infPtr->f.C * lp->_43 + infPtr->f.D) /
        sqrtf(infPtr->f.A * infPtr->f.A + infPtr->f.B * infPtr->f.B + infPtr->f.C * infPtr->f.C);
    if (result - r > 0)return false;
    result = (infPtr->l.A * lp->_41 + infPtr->l.B * lp->_42 + infPtr->l.C * lp->_43 + infPtr->l.D) /
        sqrtf(infPtr->l.A * infPtr->l.A + infPtr->l.B * infPtr->l.B + infPtr->l.C * infPtr->l.C);
    if (result - r > 0)return false;
    result = (infPtr->r.A * lp->_41 + infPtr->r.B * lp->_42 + infPtr->r.C * lp->_43 + infPtr->r.D) /
        sqrtf(infPtr->r.A * infPtr->r.A + infPtr->r.B * infPtr->r.B + infPtr->r.C * infPtr->r.C);
    if (result - r > 0)return false;
    result = (infPtr->t.A * lp->_41 + infPtr->t.B * lp->_42 + infPtr->t.C * lp->_43 + infPtr->t.D) /
        sqrtf(infPtr->t.A * infPtr->t.A + infPtr->t.B * infPtr->t.B + infPtr->t.C * infPtr->t.C);
    if (result - r > 0)return false;
    result = (infPtr->b.A * lp->_41 + infPtr->b.B * lp->_42 + infPtr->b.C * lp->_43 + infPtr->b.D) /
        sqrtf(infPtr->b.A * infPtr->b.A + infPtr->b.B * infPtr->b.B + infPtr->b.C * infPtr->b.C);
    if (result - r > 0)return false;
    return true;
}

int64_t BaseObject::initializeObject(InputArg Arg)
{
    DxWin* lpDxWin = DxWin::DxWinInstance();
    uint64_t ptr1 = (uint64_t)lpDxWin->lpDevice.Get();
    ComPtr<ID3D12Device> lpDevice1 = lpDxWin->lpDevice;
    uint64_t ptr2 = (uint64_t)lpDevice1.Get();
    arg = {
    lpDxWin->lpFactory,              // ComPtr<IDXGIFactory>& lpFactory
    lpDxWin->lpDevice,               // ComPtr<ID3D12Device>& lpDevice
    lpDxWin->lpCommandAllocator,     // ComPtr<ID3D12CommandAllocator>& lpCommandAllocator
    lpDxWin->lpCommandList,          // ComPtr<ID3D12GraphicsCommandList>& lpCommandList
    lpDxWin->lpCommandQueue,         // ComPtr<ID3D12CommandQueue>& lpCommandQueue
    lpDxWin->lpFence,                // ComPtr<ID3D12Fence>& lpFence
    lpDxWin->lpSwapChain,            // ComPtr<IDXGISwapChain>& lpSwapChain
    lpDxWin->SwapBufferRes,          // ComPtr<ID3D12Resource>* SwapBufferRes
    lpDxWin->InputRes,               // ComPtr<ID3D12Resource> InputRes
    lpDxWin->OutputRes,              // ComPtr<ID3D12Resource> OutputRes
    lpDxWin->DepthStencilRes,        // ComPtr<ID3D12Resource> DepthStencilRes
    lpDxWin->mRtvHeap,               // ComPtr<ID3D12DescriptorHeap>& mRtvHeap
    lpDxWin->mDsvHeap,               // ComPtr<ID3D12DescriptorHeap>& mDsvHeap
    lpDxWin->mCbvHeap,               // ComPtr<ID3D12DescriptorHeap>& mCbvHeap
    lpDxWin->mSrvHeap,               // ComPtr<ID3D12DescriptorHeap>& mSrvHeap
    lpDxWin->mUavHeap,               // ComPtr<ID3D12DescriptorHeap>& mUavHeap
    lpDxWin->mSamplerHeap,           // ComPtr<ID3D12DescriptorHeap>& mSamplerHeap
    lpDxWin->mRootSignature,         // ComPtr<ID3D12RootSignature> mRootSignature
    lpDxWin->cRootSignature,
    lpDxWin->PSO,
    lpDxWin->CSO,                    // ComPtr<ID3D12PipelineState> CSO
    reinterpret_cast<lpFlushCommandQueue>(DxWin::StaticFlushCommandQueue),      // lpFlushCommandQueue FlushCommandQueue
    lpDxWin->ThreadX,                // UINT ThreadX
    lpDxWin->ThreadY,                // UINT ThreadY
    lpDxWin->ThreadZ,                // UINT ThreadZ
    lpDxWin->CurrebtSwapChainBuffer, // UINT CurrebtSwapChainBuffer
    lpDxWin->Vp,                     // D3D12_VIEWPORT Vp
    lpDxWin->SciRect,                // D3D12_RECT SciRect
    lpDxWin->RtvDescSize,            // UINT RtvDescSize
    lpDxWin->DsvDescSize,            // UINT DsvDescSize
    lpDxWin->CbvSrvUavDescSize,      // UINT CbvSrvUavDescSize
    lpDxWin->SamplerSize,            // UINT SamplerSize
    lpDxWin->CbvHeapHandle,
    lpDxWin->RtvHeapHandle,
    lpDxWin->DsvHeapHandle,
    lpDxWin->SrvHeapHandle,
    lpDxWin->UavHeapHandle,
    lpDxWin->SamplerHeapHandle,
    &lpDxWin->CbvHeapindex,
    {
    lpDxWin->camera->GetPosition3f().x,
    lpDxWin->camera->GetPosition3f().y,
    lpDxWin->camera->GetPosition3f().z
    }
    };
    arg.tool = &lpDxWin->tool;
    arg.fun = CheckForIntersectionFun;
    uint64_t ptr = (uint64_t)arg.lpDevice.Get();
    if (arg.lpDevice.Get() == nullptr) {
        return 0;
    }

    std::vector<std::wstring> dllFiles;
    lpDxWin->tool.GetDllFilesRecursive(lpDxWin->tool.gotoPath(L"dll"), dllFiles);
    HMODULE hDll;
    for (const auto& file : dllFiles) {
        HMODULE hDll = LoadLibrary(file.c_str());
        if (hDll == 0)return -1;
        
        GetTheObjectFun GetTheObject = (GetTheObjectFun)GetProcAddress(hDll, "GetTheObject");

        virtualBase* Object = GetTheObject(&arg, nullptr);

        mPSOs.insert(std::pair<std::wstring, ComPtr<ID3D12PipelineState>>(Object->ObjectName, Object->PSO));
        lpObjects.push_back(DllsAndObjects(Object, hDll));
    } 
    lpObjects.sort(BaseObject::CompareLevels);

    return 1;
}

void BaseObject::BaseObjectreleaseInstance()
{
    if (!lpBaseObject)delete lpBaseObject;
    for (std::list<BaseObject::DllsAndObjects>::iterator it = BaseObject::lpObjects.begin(); it != BaseObject::lpObjects.end(); ++it) {
        delete it->lpObject;
        FreeLibrary(it->Dll);
    }
}

std::list<BaseObject::DllsAndObjects> BaseObject::lpObjects;
virtualBase::UpdataArg BaseObject::arg;
BaseObject::BaseObjectHelper BaseObject::BaseObjectHelperhelper;
BaseObject* BaseObject::lpBaseObject = BaseObjectInstance();
std::unordered_map<std::wstring, ComPtr<ID3D12PipelineState>> BaseObject::mPSOs;

