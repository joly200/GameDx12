#include "Tool.h"

#include <corecrt_wstdio.h>
#pragma comment(lib,"DirectXTex.lib")
using namespace DirectX;


ID3D12Resource* Tool::uploadresoure(ID3D12Device* lpDevice, ID3D12Resource** uploadBuffer, ID3D12GraphicsCommandList* lpCommandList,
    ID3D12CommandQueue* lpCommandQueue, ID3D12CommandAllocator* lpCommandAllocator, void* data, size_t Size)
{
    UINT size = 0;
    size = Size;
    ID3D12Resource* defaultBuffer;

    HRESULT hr = lpDevice->CreateCommittedResource(&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
        D3D12_HEAP_FLAG_NONE, &CD3DX12_RESOURCE_DESC::Buffer(size),
        D3D12_RESOURCE_STATE_COMMON, nullptr,
        IID_PPV_ARGS(&defaultBuffer));

    hr = lpDevice->CreateCommittedResource(&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
        D3D12_HEAP_FLAG_NONE, &CD3DX12_RESOURCE_DESC::Buffer(size),
        D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
        IID_PPV_ARGS(uploadBuffer));
    D3D12_SUBRESOURCE_DATA SubResData;

    SubResData.pData = data;
    SubResData.RowPitch = size;
    SubResData.SlicePitch = size;
    lpCommandList->Reset(lpCommandAllocator, nullptr);
    lpCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(defaultBuffer,
        D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_COPY_DEST));
    UINT64 Size0 = UpdateSubresources<1>(lpCommandList, defaultBuffer, *uploadBuffer, 0, 0, 1, &SubResData);
    lpCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(defaultBuffer, D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_GENERIC_READ));
    lpCommandList->Close();
    ID3D12CommandList* cmdsLists[] = { lpCommandList };
    lpCommandQueue->ExecuteCommandLists(1, cmdsLists);
    return defaultBuffer;
}

void* Tool::conveyGpu(ID3D12Device* lpDevice, ID3D12Resource** AimResource, UINT  CbvDescSize, UINT Size, CD3DX12_CPU_DESCRIPTOR_HANDLE& CbvHeapHandle, int* indexBuffer)
{
    UINT size = 0;
    if (Size % 256) {
        size = (Size / 256 + 1) * 256;
    }
    else
    {
        size = (Size / 256) * 256;
    }

    lpDevice->CreateCommittedResource(&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
        D3D12_HEAP_FLAG_NONE, &CD3DX12_RESOURCE_DESC::Buffer(size),
        D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
        IID_PPV_ARGS(AimResource));

    D3D12_CONSTANT_BUFFER_VIEW_DESC pDesc;
    pDesc.BufferLocation = (*AimResource)->GetGPUVirtualAddress();
    pDesc.SizeInBytes = size;

    lpDevice->CreateConstantBufferView(&pDesc, CbvHeapHandle);
    //CbvHeapHandle.Offset(1, CbvDescSize);

    void* MapPtr;
    (*AimResource)->Map(0, nullptr, &MapPtr);

    if (indexBuffer)*indexBuffer = CbvIndex;
    CbvIndex++;
    return MapPtr;
}

void* Tool::AddComtexture(ID3D12Device* lpDevice, ID3D12Resource** AimResource, UINT width, UINT height, UINT CbvSrvUavDescSize, CD3DX12_CPU_DESCRIPTOR_HANDLE& UavHeapHandle)
{
    UINT size = 0;
    UINT Size = width * height * 4;
    if (Size % 256) {
        size = (Size / 256 + 1) * 256;
    }
    else
    {
        size = (Size / 256) * 256;
    }
    D3D12_RESOURCE_DESC textureDesc = {};
    ZeroMemory(&textureDesc, sizeof(textureDesc));
    textureDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
    textureDesc.Alignment = 0;
    textureDesc.Width = width;
    textureDesc.Height = height;
    textureDesc.DepthOrArraySize = 1;
    textureDesc.MipLevels = 1;
    textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    textureDesc.SampleDesc.Count = 1;
    textureDesc.SampleDesc.Quality = 0;
    textureDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
    textureDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;

    if (FAILED(lpDevice->CreateCommittedResource(&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
        D3D12_HEAP_FLAG_NONE, &textureDesc, D3D12_RESOURCE_STATE_COMMON, nullptr,
        IID_PPV_ARGS(AimResource))))
    {
        return nullptr;
    }
    D3D12_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};
    ZeroMemory(&uavDesc, sizeof(uavDesc));
    D3D12_RESOURCE_DESC res;
    res = (*AimResource)->GetDesc();
    uavDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    uavDesc.ViewDimension = D3D12_UAV_DIMENSION_TEXTURE2D;
    uavDesc.Texture2D.MipSlice = 0;
    lpDevice->CreateUnorderedAccessView(*AimResource, nullptr, &uavDesc, UavHeapHandle);

    if (AimResource)
    {
        UavHeapHandle.Offset(1, CbvSrvUavDescSize);
        return AimResource;
    }
    return nullptr;
}

void Tool::revolve(float x, float y, float z, DirectX::XMFLOAT4X4* lpData)
{
    DirectX::XMMATRIX XMF = DirectX::XMMatrixMultiply(DirectX::XMMatrixRotationX(x), DirectX::XMMatrixMultiply(DirectX::XMMatrixRotationY(y), DirectX::XMMatrixRotationZ(z)));
    DirectX::XMStoreFloat4x4(lpData, XMF);
}

void Tool::RotationNormal(float Rotation, DirectX::FXMVECTOR NormalAxis, DirectX::XMFLOAT4X4* lpData)
{
    DirectX::XMMATRIX XMF = DirectX::XMMatrixRotationNormal(NormalAxis, Rotation);
    DirectX::XMStoreFloat4x4(lpData, XMF);
}

void Tool::SetCamera(DirectX::XMFLOAT4X4* lpData, float distance,
    UINT width, UINT height,
    float r, float l, float t, float b, float n, float f, float j

)
{

    float nearPotX = j;
    DirectX::XMMATRIX XMF = {
        (static_cast<float>(height) / static_cast<float>(width)) * (2.0f / (r - l)),0.0f,0.0f,
       0.0f,
        0.0f, (2.0f / (t - b)),0.0f,0.0f,
        0.0f,0.0f,(2.0f / (f - n)),0.5f/** (2.0f / (f - n))*(500)*/,
        0.0f,0.0f,1.0f / j,1.0f
    };

    DirectX::XMStoreFloat4x4(lpData, XMF);
}

void Tool::SetView(DirectX::XMFLOAT4X4* lpData, DirectX::XMFLOAT4X4* combinedRotation, float x, float y, float z
    , float pitch, float yaw, float j)
{
    DirectX::XMMATRIX XMF = {
        1.0f,0.0f,0.0f,-x,
        0.0f,1.0f,0.0f,-y,
        0.0f,0.0f,1.0f,-z - j,
        0.0f,0.0f,0.0f,1.0f
    };
    DirectX::XMFLOAT4X4 angle;
    DirectX::XMStoreFloat4x4(&angle, XMF);
    yaw = DirectX::XMConvertToRadians(yaw);
    pitch = DirectX::XMConvertToRadians(pitch);

    // 创建绕 y 轴的旋转矩阵（Yaw）
    DirectX::XMMATRIX rotationMatrixY = DirectX::XMMatrixRotationY(yaw);

    // 创建绕 x 轴的旋转矩阵（Pitch）
    DirectX::XMMATRIX rotationMatrixX = DirectX::XMMatrixRotationX(pitch);

    // 组合旋转矩阵
    DirectX::XMMATRIX rotationMatrix = DirectX::XMMatrixMultiply(rotationMatrixY, rotationMatrixX);

    DirectX::XMStoreFloat4x4(lpData, XMF);
    DirectX::XMStoreFloat4x4(combinedRotation, rotationMatrix);
}

DirectX::XMFLOAT4X4 Tool::Identity4x4()
{
    static DirectX::XMFLOAT4X4 I(
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f);

    return I;
}

DirectX::XMFLOAT2 Tool::SetCameraj(float iAx, float iAy, float iAz)
{
    // 计算偏航角（Yaw）
    float yaw = atan2f(iAz, iAx);

    // 计算俯仰角（Pitch）
    float pitch = atan2f(iAy, sqrtf(iAx * iAx + iAz * iAz));

    // 如果需要将弧度转换为度数
    yaw = DirectX::XMConvertToDegrees(yaw);
    pitch = DirectX::XMConvertToDegrees(pitch);

    return DirectX::XMFLOAT2(yaw, pitch);
}

DirectX::XMFLOAT3 Tool::SetCameraAj(float yaw, float pitch)
{
    // 如果输入角度是度数，首先将其转换为弧度
    yaw = DirectX::XMConvertToRadians(yaw);
    pitch = DirectX::XMConvertToRadians(pitch);

    // 计算方向向量的分量
    float x = cosf(pitch) * sinf(yaw);
    float y = sinf(pitch);
    float z = cosf(pitch) * cosf(yaw);

    return DirectX::XMFLOAT3(x, y, z);
}

HRESULT Tool::CreateDDSToBuffer(Microsoft::WRL::ComPtr<ID3D12Device>& lpDevice, Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& lpCommandList,
    std::wstring string, Microsoft::WRL::ComPtr<ID3D12CommandAllocator>& lpCommandAllocator, Microsoft::WRL::ComPtr<ID3D12Resource>& textureUploadHeap
    , Microsoft::WRL::ComPtr<ID3D12Resource>& textureDefaultHeap,
    Microsoft::WRL::ComPtr<ID3D12CommandQueue> lpCommandQueue)
{
    HRESULT hr;
    ID3D12Resource* texture;
    WCHAR Path[0x100];
    WORD StrCount = GetCurrentDirectoryW(0x100, Path);
    lpCommandList->Reset(lpCommandAllocator.Get(), nullptr);
    hr = CreateDDSTextureFromFile12(lpDevice.Get(), lpCommandList.Get(), string.c_str(),
        textureDefaultHeap, textureUploadHeap);
    lpCommandList->Close();
    ID3D12CommandList* cmdsLists[] = { lpCommandList.Get() };
    lpCommandQueue->ExecuteCommandLists(1, cmdsLists);

    return hr;
}
#ifdef _DEBUG
int64_t Tool::gotoPath(dXstring& currentPath, dXstring relatepath)
{
    {
        wchar_t Current[0x100];
        ZeroMemory(Current, MAX_PATH);
        DWORD length = GetModuleFileName(nullptr, Current, MAX_PATH);
        currentPath.resize(MAX_PATH);
        memcpy((void*)currentPath.c_str(), Current, MAX_PATH);

        size_t pos = currentPath.find_last_of(L"\\");
        if (pos != std::string::npos) {
            currentPath = currentPath.substr(0, pos);
        }
        currentPath = currentPath + L"\\" + relatepath;
        return 0;
    }
    return 0;
}
wchar_t* Tool::gotoPath(wchar_t* RelateBuffer)
{
    static dXstring Agrstring;
    Agrstring.resize(MAX_PATH);
    DWORD length = GetModuleFileName(nullptr, (LPWSTR)Agrstring.c_str(), MAX_PATH);

    size_t pos = Agrstring.find(L"GameDx12.exe");
    if (pos != std::string::npos) {
        Agrstring = Agrstring.substr(0, pos);
    }
    Agrstring += RelateBuffer;

    return (wchar_t*)Agrstring.c_str();
}
#endif // DEBUG
#ifndef _DEBUG
int64_t Tool::gotoPath(dXstring& currentPath, dXstring relatepath)
{
    {
        wchar_t Current[0x100];
        ZeroMemory(Current, MAX_PATH);
        DWORD length = GetModuleFileName(nullptr, Current, MAX_PATH);
        currentPath.resize(MAX_PATH);
        memcpy((void*)currentPath.c_str(), Current, MAX_PATH);

        size_t pos = currentPath.find_last_of(L"Debug");
        if (pos != std::string::npos) {
            currentPath = currentPath.substr(0, pos);
        }
        currentPath = currentPath +L"Release"+L"\\" + relatepath;
        return 0;
    }
    return 0;
}
wchar_t* Tool::gotoPath(wchar_t* RelateBuffer)
{
    static dXstring Agrstring;
    Agrstring.resize(MAX_PATH);
    DWORD length = GetModuleFileName(nullptr, (LPWSTR)Agrstring.c_str(), MAX_PATH);

    size_t pos = Agrstring.find(L"GameDx12.exe");
    if (pos != std::string::npos) {
        Agrstring = Agrstring.substr(0, pos);
    }
    Agrstring += RelateBuffer;

    return (wchar_t*)Agrstring.c_str();
}
#endif // RELEASE




std::string Tool::WStringToString(const std::wstring& wstr)
{

    int bufferSize = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, nullptr, 0, nullptr, nullptr);

    std::string str(bufferSize, 0);


    WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, &str[0], bufferSize, nullptr, nullptr);

    return str;
}

void Tool::GetDllFilesRecursive(const std::wstring& directory, std::vector<std::wstring>& dllFiles)
{
    // 查找 DLL 文件
    std::wstring searchPath = directory + L"\\*.dll";
    WIN32_FIND_DATA findFileData;
    HANDLE hFind = FindFirstFile(searchPath.c_str(), &findFileData);

    if (hFind != INVALID_HANDLE_VALUE) {
        do {
            if (!(findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
                std::wstring filePath = directory + L"\\" + findFileData.cFileName;
                dllFiles.push_back(filePath);
            }
        } while (FindNextFile(hFind, &findFileData) != 0);
        FindClose(hFind);
    }

    // 递归遍历子文件夹
    std::wstring subdirSearchPath = directory + L"\\*";
    hFind = FindFirstFile(subdirSearchPath.c_str(), &findFileData);

    if (hFind != INVALID_HANDLE_VALUE) {
        do {
            if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
                if (wcscmp(findFileData.cFileName, L".") != 0 && wcscmp(findFileData.cFileName, L"..") != 0) {
                    std::wstring subdirPath = directory + L"\\" + findFileData.cFileName;
                    GetDllFilesRecursive(subdirPath, dllFiles);
                }
            }
        } while (FindNextFile(hFind, &findFileData) != 0);
        FindClose(hFind);
    }
}

std::wstring Tool::getProgramName(const std::wstring& path)
{
    wchar_t drive[_MAX_DRIVE];
    wchar_t dir[_MAX_DIR];
    wchar_t fname[_MAX_FNAME];
    wchar_t ext[_MAX_EXT];

    // 使用宽字符版本的 _wsplitpath_s
    _wsplitpath_s(path.c_str(), drive, dir, fname, ext);
    return std::wstring(fname) + std::wstring(ext);
}

void Tool::DebugString(const wchar_t* format, ...)
{
    wchar_t buffer[512];  // 临时存储格式化后的字符串
    va_list args;
    va_start(args, format);
    vswprintf(buffer, sizeof(buffer) / sizeof(wchar_t), format, args);  // 格式化输出
    va_end(args);

    OutputDebugString(buffer);
}

HRESULT Tool::LoadHDRTexture(dXstring filePath,Microsoft::WRL::ComPtr<ID3D12Resource>& textureUploadHeap,
    Microsoft::WRL::ComPtr<ID3D12Resource>& textureDefaultHeap, D3D12_GPU_DESCRIPTOR_HANDLE& Mr, void* lparam)
{
    TexMetadata metadata;
    ScratchImage scratchImg;
    virtualBase::UpdataArg* arg = (virtualBase::UpdataArg*)lparam;
    HRESULT hr = LoadFromHDRFile(filePath.c_str(), &metadata, scratchImg);
    D3D12_RESOURCE_DESC textureDesc = {};
    textureDesc.MipLevels = metadata.mipLevels;
    textureDesc.Format = metadata.format;
    textureDesc.Width = metadata.width;
    textureDesc.Height = metadata.height;
    textureDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
    textureDesc.DepthOrArraySize = metadata.arraySize;
    textureDesc.SampleDesc.Count = 1;
    textureDesc.SampleDesc.Quality = 0;
    textureDesc.Dimension = static_cast<D3D12_RESOURCE_DIMENSION>(metadata.dimension);


    CD3DX12_HEAP_PROPERTIES heapProps(D3D12_HEAP_TYPE_DEFAULT);
     hr = arg->lpDevice->CreateCommittedResource(
        &heapProps,
        D3D12_HEAP_FLAG_NONE,
        &textureDesc,
        D3D12_RESOURCE_STATE_COPY_DEST,
        nullptr,
        IID_PPV_ARGS(&textureDefaultHeap));

     if (FAILED(hr))
     {
         return hr;
     }



    std::vector<D3D12_SUBRESOURCE_DATA> subresources;
    D3D12_SUBRESOURCE_DATA subresourceData = {};
    subresourceData.pData = scratchImg.GetPixels(); // 纹理数据指针
    subresourceData.RowPitch = metadata.width * 4 * sizeof(float); // 每行数据的字节数（RGBA 32F）
    subresourceData.SlicePitch = subresourceData.RowPitch * metadata.height;
    subresources.push_back(subresourceData);

    UINT64 requiredSize = GetRequiredIntermediateSize(textureDefaultHeap.Get(), 0, subresources.size());
    CD3DX12_HEAP_PROPERTIES uploadHeapProps(D3D12_HEAP_TYPE_UPLOAD);
    CD3DX12_RESOURCE_DESC uploadBufferDesc = CD3DX12_RESOURCE_DESC::Buffer(requiredSize);

    hr = arg->lpDevice->CreateCommittedResource(
        &uploadHeapProps,
        D3D12_HEAP_FLAG_NONE,
        &uploadBufferDesc,
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&textureUploadHeap));
    if (FAILED(hr))
    {
        return hr;
    }
    arg->lpCommandList->Reset(arg->lpCommandAllocator.Get(), nullptr);
    UpdateSubresources(arg->lpCommandList.Get(), textureDefaultHeap.Get(), textureUploadHeap.Get(), 0, 0, subresources.size(), subresources.data());
    arg->lpCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(textureDefaultHeap.Get(), 
        D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE));
    arg->lpCommandList->Close();
    ID3D12CommandList* foo[] = { arg->lpCommandList.Get()};
    arg->lpCommandQueue->ExecuteCommandLists(1, foo);



    D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
    srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
    srvDesc.Format = textureDefaultHeap->GetDesc().Format;
    srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MipLevels = textureDefaultHeap->GetDesc().MipLevels;
    srvDesc.Texture2D.MostDetailedMip = 0;
    srvDesc.Texture2D.ResourceMinLODClamp = 0.0f;
    
    arg->lpDevice->CreateShaderResourceView(textureDefaultHeap.Get(), &srvDesc, arg->CbvHeapHandle);
    arg->CbvHeapHandle.Offset(1, arg->CbvSrvUavDescSize);
    Mr.ptr = arg->mCbvHeap->GetGPUDescriptorHandleForHeapStart().ptr +
        *arg->CbvHeapindex * arg->CbvSrvUavDescSize;
    *arg->CbvHeapindex += 1;
}
