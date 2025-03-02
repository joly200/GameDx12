#pragma once
#include"DirectXTool.h"
#include"virtualBase.h"
#include"../../DirectXTex-main/DirectXTex/DirectXTex.h"

class Tool : public DirectXTool
{
public:
    Tool() {};
    ~Tool() {};

    virtual ID3D12Resource* uploadresoure(ID3D12Device* lpDevice, ID3D12Resource** uploadBuffer,
        ID3D12GraphicsCommandList* lpCommandList, ID3D12CommandQueue* lpCommandQueue,
        ID3D12CommandAllocator* lpCommandAllocator, void* data, size_t Size) override;

    virtual void* conveyGpu(ID3D12Device* lpDevice, ID3D12Resource** AimResource, UINT width, UINT height,
        CD3DX12_CPU_DESCRIPTOR_HANDLE& CbvHeapHandle, int* indexBuffer = nullptr) override;

    virtual void* AddComtexture(ID3D12Device* lpDevice, ID3D12Resource** AimResource, UINT width,
        UINT height, UINT CbvSrvUavDescSize, CD3DX12_CPU_DESCRIPTOR_HANDLE& UavHeapHandle) override;

    virtual void revolve(float x, float y, float z, DirectX::XMFLOAT4X4* lpData) override;

    virtual void RotationNormal(float Rotation, DirectX::FXMVECTOR NormalAxis, DirectX::XMFLOAT4X4* lpData) override;

    virtual void SetCamera(DirectX::XMFLOAT4X4* lpData, float distance, UINT width, UINT height,
        float r, float l, float t, float b, float n, float f, float j) override;

    virtual  void SetView(DirectX::XMFLOAT4X4* lpData, DirectX::XMFLOAT4X4* combinedRotation,
        float x, float y, float z, float pitch, float yaw, float j) override;

    DirectX::XMFLOAT4X4 Identity4x4() override;

    DirectX::XMFLOAT2 SetCameraj(float iAx, float iAy, float iAz) override;

    DirectX::XMFLOAT3 SetCameraAj(float ijx, float ijy) override;

    virtual HRESULT CreateDDSToBuffer(Microsoft::WRL::ComPtr<ID3D12Device>& lpDevice,
        Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& lpCommandList, std::wstring string,
        Microsoft::WRL::ComPtr<ID3D12CommandAllocator>& lpCommandAllocator,
        Microsoft::WRL::ComPtr<ID3D12Resource>& textureUploadHeap,
        Microsoft::WRL::ComPtr<ID3D12Resource>& textureDefaultHeap,
        Microsoft::WRL::ComPtr<ID3D12CommandQueue> lpCommandQueue) override;

    virtual  int64_t gotoPath(dXstring& currentPath, dXstring relatepath) override;

    virtual wchar_t* gotoPath(wchar_t* RelateBuffer) override;

    virtual std::string WStringToString(const std::wstring& wstr) override;

    virtual void GetDllFilesRecursive(const std::wstring& directory, std::vector<std::wstring>& dllFiles) override;

    virtual std::wstring getProgramName(const std::wstring& path) override;

    virtual void DebugString(const wchar_t* format, ...) override;
    virtual HRESULT LoadHDRTexture(dXstring filePath,  Microsoft::WRL::ComPtr<ID3D12Resource>& textureUploadHeap,
        Microsoft::WRL::ComPtr<ID3D12Resource>& textureDefaultHeap, D3D12_GPU_DESCRIPTOR_HANDLE& Mr, void* lparam) override;
};
