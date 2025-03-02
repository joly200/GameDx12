#include "pch.h"
#include "sky.h"


sky::sky(virtualBase::UpdataArg* Paraml)
{
    HRESULT hr;
    const wchar_t Name[] = L"sky";
    memcpy(ObjectName, Name, OBJECT_NAME);
    const int n = 50;  // 纬度段数
    const int m = 60;  // 经度段数
    const float r = 10.0f;  // 球的半径
    levels = 10;
    std::vector<Vertex> vertices;  // 顶点位置
    std::vector<uint32_t> indices;  // 纹理坐标

    for (int i = 0; i <= n; ++i) {
        float theta = pi / 2 - (pi * (float)i / n);
        for (int j = 0; j <= m; ++j) {
            float phi = (float)j / m * 2.0f * pi;  // 经度，范围是 0 到 2*pi
            float x = r * cos(theta) * cos(phi);
            float y = r * sin(theta);
            float z = r * cos(theta) * sin(phi);    
            float u = (float)j / m;  // 经度映射
            float v = ((pi / 2) - theta)/pi;  // 纬度映射  
            vertices.push_back({ x,y,z,1.0f,1.0f, 1.0f,1.0f,u, v ,-x,-y,-z });
        }
    }
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            int first = i * (m + 1) + j;
            int second = first + m + 1;

            // 第一个三角形
            indices.push_back(first + 1);
            indices.push_back(first);
            
            indices.push_back(second);

            //// 第二个三角形
            indices.push_back(second + 1);
            indices.push_back(first + 1);

            indices.push_back(second);
        }
    }
    skySize = vertices.size() * sizeof(Vertex);
    skyindicesSize = indices.size() * sizeof(uint32_t);

    skydefaultBuffer = Paraml->tool->uploadresoure(Paraml->lpDevice.Get(), skyuploadBuffer.GetAddressOf(), Paraml->lpCommandList.Get(),
        Paraml->lpCommandQueue.Get(), Paraml->lpCommandAllocator.Get()
        , (void*)vertices.data(), skySize
    );
    Paraml->FlushCommandQueue();
    skyuploadBuffer->Release();
    *skyuploadBuffer.GetAddressOf() = nullptr;
    skyindicesdefaultBuffer = Paraml->tool->uploadresoure(Paraml->lpDevice.Get(), skyuploadBuffer.GetAddressOf(), Paraml->lpCommandList.Get(),
        Paraml->lpCommandQueue.Get(), Paraml->lpCommandAllocator.Get()
        , (void*)indices.data(), skyindicesSize
    );
    Paraml->FlushCommandQueue();
    skyuploadBuffer->Release();
    *skyuploadBuffer.GetAddressOf() = nullptr;
    skyState = {
       1.0f,0.0f,0.0f,0.0f,
       0.0f,1.0f,0.0f,0.0f,
       0.0f,0.0f,1.0f,0.0f,
       0.0f,0.0f,0.0f,1.0f
    };
    lpskyState = Paraml->tool->conveyGpu(Paraml->lpDevice.Get(), &stateBuffer, Paraml->CbvSrvUavDescSize, sizeof(skyState), Paraml->CbvHeapHandle);
    Paraml->CbvHeapHandle.Offset(1, Paraml->CbvSrvUavDescSize);
    Mr1.ptr = Paraml->mCbvHeap->GetGPUDescriptorHandleForHeapStart().ptr +
        *Paraml->CbvHeapindex * Paraml->CbvSrvUavDescSize;
    *Paraml->CbvHeapindex += 1;


    hr = Paraml->tool->LoadHDRTexture(Paraml->tool->gotoPath(L"src\\sunflowers_puresky_8k.hdr"),
         textureUploadBuffer, texturedefaultBuffer,Mr2,(void*)Paraml);

    samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR; // 线性过滤
    samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP; // U 轴的包裹方式
    samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP; // V 轴的包裹方式
    samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP; // W 轴的包裹方式
    samplerDesc.MipLODBias = 0.0f;
    samplerDesc.MaxAnisotropy = 1;
    samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER; // 不进行比较
    samplerDesc.BorderColor[0] = 0.0f;
    samplerDesc.BorderColor[1] = 0.0f;
    samplerDesc.BorderColor[2] = 0.0f;
    samplerDesc.BorderColor[3] = 0.0f;
    samplerDesc.MinLOD = 0;
    samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;
    Paraml->lpDevice->CreateSampler(&samplerDesc, Paraml->SamplerHeapHandle);
    Paraml->SamplerHeapHandle.Offset(1, Paraml->lpDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER));
    memcpy(lpskyState, &skyState, sizeof(DirectX::XMFLOAT4X4));
    Setpipe(Paraml->lpDevice, Paraml);
    Paraml->FlushCommandQueue();
    textureUploadBuffer->Release();
    *textureUploadBuffer.GetAddressOf() = nullptr;
}

sky::~sky()
{
}

void sky::SetShowPostion(DirectX::XMFLOAT3 Vector)
{
}

void sky::Setpipe(ComPtr<ID3D12Device>& lpDevice, virtualBase::UpdataArg* Paraml)
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
        ,
        {
            "NORMAL",0,DXGI_FORMAT_R32G32B32_FLOAT,0,36,
            D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
        }
    };

    ID3DBlob* VSCode = nullptr;
    ID3DBlob* errors = nullptr;
    dXstring Result;
    hr = D3DReadFileToBlob(Paraml->tool->gotoPath(L"Shader\\skyVertexShader.cso"), &VSCode);
    if (FAILED(hr))MessageBox(NULL, L"CubeVerTex Error", L"D3DCompileFromFile", 0);
    ID3DBlob* PSCode = nullptr;


    hr = D3DReadFileToBlob(Paraml->tool->gotoPath(L"Shader\\skyPixelShader.cso"), &PSCode);
    if (FAILED(hr))MessageBox(NULL, L"Cubeframent Error", L"D3DCompileFromFile", 0);
    CD3DX12_ROOT_PARAMETER slotRootParameter[4];

    CD3DX12_DESCRIPTOR_RANGE Sampler;
    CD3DX12_DESCRIPTOR_RANGE cbvTable[3];
    cbvTable[0].Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 0);
    cbvTable[1].Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 1);
    cbvTable[2].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);

    Sampler.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER, 1, 0);

    slotRootParameter[0].InitAsDescriptorTable(1, &cbvTable[0]);
    slotRootParameter[1].InitAsDescriptorTable(1, &cbvTable[1]);
    slotRootParameter[2].InitAsDescriptorTable(1, &cbvTable[2]);
    slotRootParameter[3].InitAsDescriptorTable(1, &Sampler);



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

bool sky::CheckForIntersection(CameraInformation* infPtr)
{
    return true;
}

int64_t sky::BaseObjectFun(UpdataArg& Arg)
{

    DirectX::XMFLOAT4X4* lpsky = (DirectX::XMFLOAT4X4*)lpskyState;
    lpsky->_41 = Arg.Pos[0];
    lpsky->_42 = Arg.Pos[1];
    lpsky->_43 = Arg.Pos[2];
    lpsky->_44 = 1.0f;
	return 0;
}

int64_t sky::ShowMethod(UpdataArg& Arg)
{
    HRESULT hr;
    ID3D12DescriptorHeap* descriptorHeaps[] = { Arg.mCbvHeap.Get(),Arg.mSamplerHeap.Get() };
    D3D12_VERTEX_BUFFER_VIEW VertexView = { skydefaultBuffer->GetGPUVirtualAddress(),
    skySize,
    sizeof(Vertex)
    };
    D3D12_INDEX_BUFFER_VIEW IndexView = { skyindicesdefaultBuffer->GetGPUVirtualAddress() ,
    skyindicesSize ,DXGI_FORMAT_R32_UINT };
    CD3DX12_GPU_DESCRIPTOR_HANDLE mCbvGpuHeap(Arg.mCbvHeap->GetGPUDescriptorHandleForHeapStart());

    CD3DX12_GPU_DESCRIPTOR_HANDLE mSamplerGpuHeap(Arg.mSamplerHeap->GetGPUDescriptorHandleForHeapStart());

    D3D12_GPU_DESCRIPTOR_HANDLE BaseDescriptor0 = mCbvGpuHeap;


    hr = Arg.lpCommandList->Reset(Arg.lpCommandAllocator.Get(), PSO.Get());
    Arg.lpCommandList->SetPipelineState(PSO.Get());
    Arg.lpCommandList->RSSetViewports(1, &Arg.Vp);
    Arg.lpCommandList->RSSetScissorRects(1, &Arg.SciRect);

    Arg.lpCommandList->OMSetRenderTargets(1, &CD3DX12_CPU_DESCRIPTOR_HANDLE(Arg.mRtvHeap->GetCPUDescriptorHandleForHeapStart(), Arg.CurrebtSwapChainBuffer, Arg.RtvDescSize)
        , true, &Arg.mDsvHeap->GetCPUDescriptorHandleForHeapStart());

    Arg.lpCommandList->SetDescriptorHeaps(2, &descriptorHeaps[0]);

    Arg.lpCommandList->SetGraphicsRootSignature(mRootSignature.Get());

    Arg.lpCommandList->IASetVertexBuffers(0, 1, &VertexView);

    Arg.lpCommandList->IASetIndexBuffer(&IndexView);

    Arg.lpCommandList->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);//D3D_PRIMITIVE_TOPOLOGY_LINELIST

    static UINT i = 1000;//
    Arg.lpCommandList->SetGraphicsRootDescriptorTable(0, BaseDescriptor0);


    Arg.lpCommandList->SetGraphicsRootDescriptorTable(1, Mr1);

    Arg.lpCommandList->SetGraphicsRootDescriptorTable(2, Mr2);

    Arg.lpCommandList->SetGraphicsRootDescriptorTable(3, mSamplerGpuHeap.Offset(2, Arg.CbvSrvUavDescSize));
    Arg.lpCommandList->DrawIndexedInstanced(skyindicesSize / 4, 1, 0, 0, 0);
    Arg.lpCommandList->ClearDepthStencilView(Arg.mDsvHeap->GetCPUDescriptorHandleForHeapStart(),
        D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, nullptr);
    hr = Arg.lpCommandList->Close();
    ID3D12CommandList* cmdsLists[] = { Arg.lpCommandList.Get() };
    Arg.lpCommandQueue->ExecuteCommandLists(1, cmdsLists);

    Arg.FlushCommandQueue();
	return 0;
}
