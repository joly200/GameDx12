#pragma once
#include "pch.h"

//#include "../GameDx12/DxWin.h"
using namespace Microsoft::WRL;
class Cube: public virtualBase
{
public:
	Cube(virtualBase::UpdataArg* Paraml) ;
	~Cube();
public:

	ComPtr<ID3D12Resource> cubeuploadBuffer;
	ComPtr<ID3D12Resource> cubeindicesuploadBuffer;
	ComPtr<ID3D12Resource>  cubedefaultBuffer;
	ComPtr<ID3D12Resource> cubeindicesdefaultBuffer;
	ComPtr<ID3D12Resource>  stateBuffer;
	ComPtr<ID3D12Resource>  texturedefaultBuffer;
	ComPtr<ID3D12Resource>  textureUploadBuffer;

	ComPtr<ID3D12RootSignature> mRootSignature;
	D3D12_GPU_DESCRIPTOR_HANDLE Mr1;
	D3D12_GPU_DESCRIPTOR_HANDLE Mr2;
private:
	DirectX::XMFLOAT3 postion;
private:
	float cube[864];
	unsigned int cubeindices[36];
	D3D12_SAMPLER_DESC samplerDesc;
	DirectX::XMFLOAT4X4 cubeState;
	void* lpcubeState;
	float r;
public:
	int updatacCubeState(ULONGLONG starttoCurrentTick);
	void SetShowPostion(DirectX::XMFLOAT3 Vector);
	void Setpipe(ComPtr<ID3D12Device>& lpDevice, virtualBase::UpdataArg* Paraml);

public:
	UINT cubeSize;
	UINT cubeindicesSize;
private:
	virtual bool CheckForIntersection(CameraInformation*) override;
	virtual int64_t BaseObjectFun(UpdataArg& Arg) override;
	virtual int64_t ShowMethod(UpdataArg& Arg) override;
};
