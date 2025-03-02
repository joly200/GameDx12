#include "pch.h"
#include "fz.h"

fz::fz(virtualBase::UpdataArg* Paraml):r(1.0)
{
    const wchar_t Name[] = L"fz";
    fun = Paraml->fun;
    memcpy(ObjectName, Name, OBJECT_NAME);
    tinyobj::ObjReader reader;
    tinyobj::ObjReaderConfig reader_config;
    std::vector<tinyobj::material_t> materials1;
    BOOL w = reader.ParseFromFile(Paraml->tool->WStringToString(Paraml->tool->gotoPath(L"src\\The_axe_has_a_gold_ca_0210113953_texture_obj\\The_axe_has_a_gold_ca_0210113953_texture.obj")));
    //BOOL a = tinyobj::LoadMtl(DirectXTool::WStringToString(GOTOPATH(L"src\\fz__0209071254_texture_obj\\fz__0209071254_texture.mtl")), materials1);
    const auto& attrib = reader.GetAttrib();
    const auto& shapes = reader.GetShapes();
    const auto& materials = reader.GetMaterials();
    std::string error = reader.Warning();
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
    std::vector<float> texture;
    uint64_t max = 0;
    float v = 0.0f;
    //attrib.texcoords.
    for (uint64_t i = 0; i < shapes[0].mesh.indices.size(); i+=3) {
        Vertex w,n,t;
        w = {
            attrib.vertices[shapes[0].mesh.indices[i].vertex_index*3],
            attrib.vertices[shapes[0].mesh.indices[i].vertex_index * 3 +1],
            attrib.vertices[shapes[0].mesh.indices[i].vertex_index * 3 +2],
            1.0f,
            1.0f,
            1.0f,
            1.0f,
            attrib.texcoords[shapes[0].mesh.indices[i].texcoord_index * 2]+ v,
            attrib.texcoords[shapes[0].mesh.indices[i].texcoord_index * 2+1] + v,
            attrib.normals[shapes[0].mesh.indices[i].normal_index*3],
            attrib.normals[shapes[0].mesh.indices[i].normal_index * 3+1],
            attrib.normals[shapes[0].mesh.indices[i].normal_index * 3+2]
            
        };
        vertices.push_back(w);
        n = {
            attrib.vertices[shapes[0].mesh.indices[i+1].vertex_index * 3],
            attrib.vertices[shapes[0].mesh.indices[i+1].vertex_index * 3 + 1],
            attrib.vertices[shapes[0].mesh.indices[i+1].vertex_index * 3 + 2],
            1.0f,
            1.0f,
            1.0f,
            1.0f,
            attrib.texcoords[shapes[0].mesh.indices[i + 1].texcoord_index * 2] + v,
            attrib.texcoords[shapes[0].mesh.indices[i + 1].texcoord_index * 2 + 1] + v,
            attrib.normals[shapes[0].mesh.indices[i + 1].normal_index * 3],
            attrib.normals[shapes[0].mesh.indices[i + 1].normal_index * 3 + 1],
            attrib.normals[shapes[0].mesh.indices[i + 1].normal_index * 3 + 2]
        };
        vertices.push_back(n);
        t = {
            attrib.vertices[shapes[0].mesh.indices[i + 2].vertex_index * 3],
            attrib.vertices[shapes[0].mesh.indices[i + 2].vertex_index * 3 + 1],
            attrib.vertices[shapes[0].mesh.indices[i + 2].vertex_index * 3 + 2],
            1.0f,
            1.0f,
            1.0f,
            1.0f,
            attrib.texcoords[shapes[0].mesh.indices[i + 2].texcoord_index * 2 ] + v,
            attrib.texcoords[shapes[0].mesh.indices[i + 2].texcoord_index * 2+ 1] + v,
            attrib.normals[shapes[0].mesh.indices[i + 2].normal_index * 3],
            attrib.normals[shapes[0].mesh.indices[i + 2].normal_index * 3 + 1],
            attrib.normals[shapes[0].mesh.indices[i + 2].normal_index * 3 + 2]
        };
        vertices.push_back(t);

        indices.push_back(i);
        indices.push_back(i+1);
        indices.push_back(i+2);
    }
  

    fzSize = vertices.size() * sizeof(Vertex);
    fzindicesSize = indices.size()* sizeof(uint32_t);

    fzdefaultBuffer = Paraml->tool->uploadresoure(Paraml->lpDevice.Get(), fzuploadBuffer.GetAddressOf(), Paraml->lpCommandList.Get(),
        Paraml->lpCommandQueue.Get(), Paraml->lpCommandAllocator.Get()
        , (void*)vertices.data(), fzSize
    );
    Paraml->FlushCommandQueue();
    fzuploadBuffer->Release();
    *fzuploadBuffer.GetAddressOf() = nullptr;
    fzindicesdefaultBuffer = Paraml->tool->uploadresoure(Paraml->lpDevice.Get(), fzuploadBuffer.GetAddressOf(), Paraml->lpCommandList.Get(),
        Paraml->lpCommandQueue.Get(), Paraml->lpCommandAllocator.Get()
        , (void*)indices.data(), fzindicesSize
    );
    Paraml->FlushCommandQueue();
    fzuploadBuffer->Release();
    *fzuploadBuffer.GetAddressOf() = nullptr;
     fzState = {
        1.0f,0.0f,0.0f,0.0f,
        0.0f,1.0f,0.0f,0.0f,
        0.0f,0.0f,1.0f,0.0f,
        0.0f,0.0f,0.0f,1.0f
    };


    lpfzState = Paraml->tool->conveyGpu(Paraml->lpDevice.Get(), &stateBuffer, Paraml->CbvSrvUavDescSize, sizeof(fzState), Paraml->CbvHeapHandle);
    Paraml->CbvHeapHandle.Offset(1, Paraml->CbvSrvUavDescSize);
    Mr1.ptr = Paraml->mCbvHeap->GetGPUDescriptorHandleForHeapStart().ptr +
        *Paraml->CbvHeapindex * Paraml->CbvSrvUavDescSize;
    *Paraml->CbvHeapindex += 1;
    Lighting = Paraml->tool->conveyGpu(Paraml->lpDevice.Get(), &LightingParamsBuffer, Paraml->CbvSrvUavDescSize, sizeof(LightingParams), Paraml->CbvHeapHandle);
    Paraml->CbvHeapHandle.Offset(1, Paraml->CbvSrvUavDescSize);
    Mr3.ptr = Paraml->mCbvHeap->GetGPUDescriptorHandleForHeapStart().ptr +
        *Paraml->CbvHeapindex * Paraml->CbvSrvUavDescSize;
    *Paraml->CbvHeapindex += 1;

    memset(Lighting, 0x00, sizeof(LightingParams));
    LightingParams* lpPtr = (LightingParams*)Lighting;
    ((LightingParams*)Lighting)->g_light.position = {  1.0, 0.0,0.0,0.0 };
    ((LightingParams*)Lighting)->g_light.color = { 1.0f,1.0f,1.0f,0.0f };
    ((LightingParams*)Lighting)->g_material = { 
        {0.2, 0.2, 0.2, 1.0},  // 环境光
        {0.8, 0.8, 0.8, 1.0},  // 漫反射
        {1.0, 1.0, 1.0, 1.0},  // 镜面反射
                        1.0f
    };
    ((LightingParams*)Lighting)->g_viewPos = { Paraml->Pos[0] , Paraml->Pos[1] , Paraml->Pos[2] };
    HRESULT hr = Paraml->tool->CreateDDSToBuffer(Paraml->lpDevice, Paraml->lpCommandList,
        Paraml->tool->gotoPath(L"src\\The_axe_has_a_gold_ca_0210113953_texture_obj\\The_axe_has_a_gold_ca_0210113953_texture.dds"), Paraml->lpCommandAllocator, textureUploadBuffer,
        texturedefaultBuffer,
        Paraml->lpCommandQueue);
    D3D12_RESOURCE_DESC lp;

    if (hr != S_OK)return;
    D3D12_SHADER_RESOURCE_VIEW_DESC fzTextureView;
    D3D12_RESOURCE_DESC ptr = texturedefaultBuffer->GetDesc();
    fzTextureView.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
    fzTextureView.Format = texturedefaultBuffer->GetDesc().Format;
    fzTextureView.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
    fzTextureView.Texture2D.PlaneSlice = 0;
    fzTextureView.Texture2D.MostDetailedMip = 0;
    fzTextureView.Texture2D.MipLevels = texturedefaultBuffer->GetDesc().MipLevels;
    fzTextureView.Texture2D.ResourceMinLODClamp = 0.0f;
    Paraml->lpDevice->CreateShaderResourceView(texturedefaultBuffer.Get(), &fzTextureView, Paraml->CbvHeapHandle);


    Paraml->CbvHeapHandle.Offset(1, Paraml->CbvSrvUavDescSize);
    Mr2.ptr = Paraml->mCbvHeap->GetGPUDescriptorHandleForHeapStart().ptr +
        *Paraml->CbvHeapindex * Paraml->CbvSrvUavDescSize;
    *Paraml->CbvHeapindex += 1;
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
    memcpy(lpfzState, &fzState, sizeof(DirectX::XMFLOAT4X4));
    Setpipe(Paraml->lpDevice, Paraml);
    Paraml->FlushCommandQueue();

    textureUploadBuffer->Release();
    *textureUploadBuffer.GetAddressOf() = nullptr;

}

fz::~fz()
{

}

void fz::SetShowPostion(DirectX::XMFLOAT3 Vector)
{
    DirectX::XMFLOAT4X4* lpcube = (DirectX::XMFLOAT4X4*)lpfzState;
    lpcube->_41 = Vector.x;
    lpcube->_42 = Vector.y;
    lpcube->_43 = Vector.z;
}

void fz::Setpipe(ComPtr<ID3D12Device>& lpDevice, virtualBase::UpdataArg* Paraml)
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
    hr = D3DReadFileToBlob(Paraml->tool->gotoPath(L"Shader\\fzVertexShader.cso"), &VSCode);
    if (FAILED(hr))MessageBox(NULL, L"CubeVerTex Error", L"D3DCompileFromFile", 0);
    ID3DBlob* PSCode = nullptr;

   
    hr = D3DReadFileToBlob(Paraml->tool->gotoPath(L"Shader\\fzPixelShader.cso"), &PSCode);
    if (FAILED(hr))MessageBox(NULL, L"Cubeframent Error", L"D3DCompileFromFile", 0);
    CD3DX12_ROOT_PARAMETER slotRootParameter[5];

    CD3DX12_DESCRIPTOR_RANGE Sampler;
    CD3DX12_DESCRIPTOR_RANGE cbvTable[4];
    // srbTable.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);
    cbvTable[0].Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 0);
    cbvTable[1].Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 1);
    cbvTable[2].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);
    cbvTable[3].Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 2);
    Sampler.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER, 1, 0);

    slotRootParameter[0].InitAsDescriptorTable(1, &cbvTable[0]);
    slotRootParameter[1].InitAsDescriptorTable(1, &cbvTable[1]);
    slotRootParameter[2].InitAsDescriptorTable(1, &cbvTable[2]);
    slotRootParameter[3].InitAsDescriptorTable(1, &Sampler);
    slotRootParameter[4].InitAsDescriptorTable(1, &cbvTable[3]);
    //slotRootParameter[3].InitAsConstantBufferView(0);
   // slotRootParameter[1].InitAsDescriptorTable(1, &srbTable);

    CD3DX12_ROOT_SIGNATURE_DESC rootSigDesc(5, slotRootParameter, 0, nullptr,
        D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);
    ID3DBlob* serializedRootSig = nullptr;
    ID3DBlob* errorBlob = nullptr;

    hr = D3D12SerializeRootSignature(&rootSigDesc, D3D_ROOT_SIGNATURE_VERSION_1,
        &serializedRootSig, &errorBlob);

    //MessageBox(NULL, L"test1", L"Test2", 0);
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

bool fz::CheckForIntersection(CameraInformation* infPtr)
{
    return fun(infPtr, lpfzState,r);
}

int64_t fz::BaseObjectFun(UpdataArg& Arg)
{
    ((LightingParams*)Lighting)->g_viewPos.x = Arg.Pos[0];
    ((LightingParams*)Lighting)->g_viewPos.y = Arg.Pos[1];
    ((LightingParams*)Lighting)->g_viewPos.z = Arg.Pos[2];
    //((LightingParams*)Lighting)->g_light = { { 0.0, 0.0,2210.0f}, {1.0, 1.0, 2210.0f} };
    ((LightingParams*)Lighting)->g_light.position =
    { sinf(static_cast<float>(static_cast<double>(Arg.timestamp) / 1000)),0.0f,0.0f };
	return 0;
}

int64_t fz::ShowMethod(UpdataArg& Arg)
{
    HRESULT hr;
    ID3D12DescriptorHeap* descriptorHeaps[] = { Arg.mCbvHeap.Get(),Arg.mSamplerHeap.Get() };
    D3D12_VERTEX_BUFFER_VIEW VertexView = { fzdefaultBuffer->GetGPUVirtualAddress(),
    fzSize,
    sizeof(Vertex)
    };
    D3D12_INDEX_BUFFER_VIEW IndexView = { fzindicesdefaultBuffer->GetGPUVirtualAddress() ,
    fzindicesSize ,DXGI_FORMAT_R32_UINT };
    CD3DX12_GPU_DESCRIPTOR_HANDLE mCbvGpuHeap(Arg.mCbvHeap->GetGPUDescriptorHandleForHeapStart());

    CD3DX12_GPU_DESCRIPTOR_HANDLE mSamplerGpuHeap(Arg.mSamplerHeap->GetGPUDescriptorHandleForHeapStart());


    SetShowPostion({ -10.0,0,0 });
    D3D12_GPU_DESCRIPTOR_HANDLE BaseDescriptor0 = mCbvGpuHeap;
    D3D12_GPU_DESCRIPTOR_HANDLE BaseDescriptor1 = mCbvGpuHeap.Offset(3, Arg.CbvSrvUavDescSize);
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

    Arg.lpCommandList->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);//D3D_PRIMITIVE_TOPOLOGY_LINELIST

    static UINT i = 1000;//
    Arg.lpCommandList->SetGraphicsRootDescriptorTable(0, BaseDescriptor0);


    Arg.lpCommandList->SetGraphicsRootDescriptorTable(1, Mr1);

    Arg.lpCommandList->SetGraphicsRootDescriptorTable(2, Mr2);

    Arg.lpCommandList->SetGraphicsRootDescriptorTable(3, mSamplerGpuHeap.Offset(1, Arg.CbvSrvUavDescSize));
    Arg.lpCommandList->SetGraphicsRootDescriptorTable(4, Mr3);
    Arg.lpCommandList->DrawIndexedInstanced(fzindicesSize/4, 1, 0, 0, 0);
    hr = Arg.lpCommandList->Close();
    ID3D12CommandList* cmdsLists[] = { Arg.lpCommandList.Get() };
    Arg.lpCommandQueue->ExecuteCommandLists(1, cmdsLists);

    Arg.FlushCommandQueue();
	return 0;
}
