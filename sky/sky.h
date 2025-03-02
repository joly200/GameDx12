#pragma once
struct Vertex {
	float x, y, z;     // ����λ��
	float r, g, b, a;  // ��ɫ (��, ��, ��, ͸����)
	float u, v;        // ��������
	float xn, yn, zn;
};
struct float3
{
	float x, y, z;
};
struct float4
{
	float r, b, g, a;
};
class sky: public virtualBase
{
public:
	sky(virtualBase::UpdataArg* Paraml);
	~sky();
private:
	ComPtr<ID3D12Resource> skyuploadBuffer;
	ComPtr<ID3D12Resource> skyindicesuploadBuffer;
	ComPtr<ID3D12Resource>  skydefaultBuffer;
	ComPtr<ID3D12Resource> skyindicesdefaultBuffer;
	ComPtr<ID3D12Resource>  stateBuffer;
	ComPtr<ID3D12Resource>  LightingParamsBuffer;
	ComPtr<ID3D12Resource>  texturedefaultBuffer;
	ComPtr<ID3D12Resource>  textureUploadBuffer;
	ComPtr<ID3D12RootSignature> mRootSignature;
	D3D12_GPU_DESCRIPTOR_HANDLE Mr1;
	D3D12_GPU_DESCRIPTOR_HANDLE Mr2;
	D3D12_GPU_DESCRIPTOR_HANDLE Mr3;
	uint64_t skySize;
	uint64_t skyindicesSize;
	DirectX::XMFLOAT4X4 skyState;
	void SetShowPostion(DirectX::XMFLOAT3 Vector);
	void* lpskyState;

private:
	D3D12_SAMPLER_DESC samplerDesc;
	
	void Setpipe(ComPtr<ID3D12Device>& lpDevice,virtualBase::UpdataArg* Paraml);
private:
	virtual bool CheckForIntersection(CameraInformation* ) override;
	virtual int64_t BaseObjectFun(UpdataArg& Arg) override;
	virtual int64_t ShowMethod(UpdataArg& Arg) override;
};

