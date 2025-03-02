#pragma once
struct Vertex {
	float x, y, z;     // ����λ��
	float r, g, b, a;  // ��ɫ (��, ��, ��, ͸����)
	float u, v;        // ��������
	float xn, yn,zn;
};
struct float3
{
	float x, y, z;
}; 
struct float4
{
	float r, b, g, a;
};
class fz : public virtualBase
{
public:
	fz(virtualBase::UpdataArg* Paraml);
	~fz();
private:
	ComPtr<ID3D12Resource> fzuploadBuffer;
	//ComPtr<ID3D12Resource> fzindicesuploadBuffer;
	ComPtr<ID3D12Resource>  fzdefaultBuffer;
	ComPtr<ID3D12Resource> fzindicesdefaultBuffer;
	ComPtr<ID3D12Resource>  stateBuffer;
	ComPtr<ID3D12Resource>  LightingParamsBuffer;
	ComPtr<ID3D12Resource>  texturedefaultBuffer;
	ComPtr<ID3D12Resource>  textureUploadBuffer;
	ComPtr<ID3D12RootSignature> mRootSignature;
	D3D12_GPU_DESCRIPTOR_HANDLE Mr1;
	D3D12_GPU_DESCRIPTOR_HANDLE Mr2;
	D3D12_GPU_DESCRIPTOR_HANDLE Mr3;
	uint64_t fzSize;
	uint64_t fzindicesSize;
	DirectX::XMFLOAT4X4 fzState;
	void SetShowPostion(DirectX::XMFLOAT3 Vector);
	void* lpfzState;
	float r;
	struct Light
	{
		float4 position; // ��Դλ��
		float4 color;    // ��Դ��ɫ
	};
	struct Material
	{
		float4 ambient;  // �����ⷴ��ϵ��
		float4 diffuse;  // ������ϵ��
		float4 specular; // ���淴��ϵ��
		float shininess; // �߹�ָ��
		float3 pad;
	};
	struct LightingParams {
		Light g_light;
		Material g_material;
		float3 g_viewPos;
	};
	void* Lighting;
private:
	D3D12_SAMPLER_DESC samplerDesc;

	void Setpipe(ComPtr<ID3D12Device>& lpDevice, virtualBase::UpdataArg* Paraml);
private:
	virtual bool CheckForIntersection(CameraInformation*) override;
	virtual int64_t BaseObjectFun(UpdataArg& Arg) override;
	virtual int64_t ShowMethod(UpdataArg& Arg) override;
};

