#pragma once

#include<d3d12.h>
#include <wrl/client.h>
#include<dxgi.h>
#include"../../Dxhead/d3dx12.h"
#include<d3dcompiler.h>
#include<DirectXColors.h>
#include<string>
#include"../../Dxhead/DDSTextureLoader.h"
#include <vector>
#pragma comment(lib,"D3DCompiler.lib")
#pragma comment(lib,"D3d12.lib")
#pragma comment(lib,"DXGI.lib")
#ifdef UNICODE
#define dXstring  std::wstring
#else
#define dXstring  std::string
#endif // !UNICODE
#define pi 3.1415926535

using namespace DirectX;
using namespace Microsoft::WRL;
struct plane {
    float A = 0.0;
    float B = 0.0;
    float C = 0.0;
    float D = 0.0;
};
struct CameraInformation {
    plane n = { 0.0 ,0.0 ,0.0 ,0.0 };
    plane f = { 0.0 ,0.0 ,0.0 ,0.0 };
    plane r = { 0.0 ,0.0 ,0.0 ,0.0 };
    plane l = { 0.0 ,0.0 ,0.0 ,0.0 };
    plane t = { 0.0 ,0.0 ,0.0 ,0.0 };
    plane b = { 0.0 ,0.0 ,0.0 ,0.0 };
    XMFLOAT3 LookTo;

};
class DirectXTool
{
public:
     DirectXTool():PI(pi), CbvIndex(0){};
    virtual ~DirectXTool() {};
    // ´¿Ðéº¯Êý
    virtual ID3D12Resource* uploadresoure(ID3D12Device* lpDevice, ID3D12Resource** uploadBuffer,
        ID3D12GraphicsCommandList* lpCommandList, ID3D12CommandQueue* lpCommandQueue,
        ID3D12CommandAllocator* lpCommandAllocator, void* data, size_t Size) = 0;

    virtual void* conveyGpu(ID3D12Device* lpDevice, ID3D12Resource** AimResource, UINT width, UINT height,
        CD3DX12_CPU_DESCRIPTOR_HANDLE& CbvHeapHandle, int* indexBuffer = nullptr) = 0;

    virtual void* AddComtexture(ID3D12Device* lpDevice, ID3D12Resource** AimResource, UINT width,
        UINT height, UINT CbvSrvUavDescSize, CD3DX12_CPU_DESCRIPTOR_HANDLE& UavHeapHandle) = 0;

    virtual void revolve(float x, float y, float z, DirectX::XMFLOAT4X4* lpData) = 0;

    virtual void RotationNormal(float Rotation, DirectX::FXMVECTOR NormalAxis, DirectX::XMFLOAT4X4* lpData) = 0;

    virtual void SetCamera(DirectX::XMFLOAT4X4* lpData, float distance, UINT width, UINT height,
        float r, float l, float t, float b, float n, float f, float j) = 0;

    virtual void SetView(DirectX::XMFLOAT4X4* lpData, DirectX::XMFLOAT4X4* combinedRotation,
        float x, float y, float z, float pitch, float yaw, float j) = 0;

    virtual DirectX::XMFLOAT4X4 Identity4x4() = 0;

    virtual DirectX::XMFLOAT2 SetCameraj(float iAx, float iAy, float iAz) = 0;

    virtual DirectX::XMFLOAT3 SetCameraAj(float ijx, float ijy) = 0;

    virtual HRESULT CreateDDSToBuffer(Microsoft::WRL::ComPtr<ID3D12Device>& lpDevice,
        Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& lpCommandList, std::wstring string,
        Microsoft::WRL::ComPtr<ID3D12CommandAllocator>& lpCommandAllocator,
        Microsoft::WRL::ComPtr<ID3D12Resource>& textureUploadHeap,
        Microsoft::WRL::ComPtr<ID3D12Resource>& textureDefaultHeap,
        Microsoft::WRL::ComPtr<ID3D12CommandQueue> lpCommandQueue) = 0;

    virtual int64_t gotoPath(dXstring& currentPath, dXstring relatepath) = 0;

    virtual wchar_t* gotoPath(wchar_t* RelateBuffer) = 0;

    virtual std::string WStringToString(const std::wstring& wstr) = 0;

    virtual void GetDllFilesRecursive(const std::wstring& directory, std::vector<std::wstring>& dllFiles) = 0;

    virtual std::wstring getProgramName(const std::wstring& path) = 0;

    virtual void DebugString(const wchar_t* format, ...) = 0;
    virtual HRESULT LoadHDRTexture(dXstring filePath, Microsoft::WRL::ComPtr<ID3D12Resource>& textureUploadHeap,
        Microsoft::WRL::ComPtr<ID3D12Resource>& textureDefaultHeap, D3D12_GPU_DESCRIPTOR_HANDLE& Mr, void* lparam) = 0;
    const float PI;
    int CbvIndex;
};