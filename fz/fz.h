#pragma once
struct Vertex {
	float x, y, z;     // 顶点位置
	float r, g, b, a;  // 颜色 (红, 绿, 蓝, 透明度)
	float u, v;        // 纹理坐标
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
		float4 position; // 光源位置
		float4 color;    // 光源颜色
	};
	struct Material
	{
		float4 ambient;  // 环境光反射系数
		float4 diffuse;  // 漫反射系数
		float4 specular; // 镜面反射系数
		float shininess; // 高光指数
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

