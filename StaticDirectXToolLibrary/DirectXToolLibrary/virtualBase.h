#pragma once
#include"DirectXTool.h"
#include <unordered_map>
using namespace Microsoft::WRL;
#define OBJECT_NAME 0x20
typedef bool(*CheckForIntersectionfunc)(CameraInformation*, void*, float);
class virtualBase {
public:
	//virtualBase() {};
	virtual ~virtualBase() {};
	typedef void(*lpFlushCommandQueue)();
	struct UpdataArg
	{
		
		ComPtr<IDXGIFactory> lpFactory;
		ComPtr<ID3D12Device> lpDevice;
		ComPtr<ID3D12CommandAllocator> lpCommandAllocator;
		ComPtr<ID3D12GraphicsCommandList> lpCommandList;
		ComPtr<ID3D12CommandQueue> lpCommandQueue;
		ComPtr<ID3D12Fence> lpFence;
		ComPtr<IDXGISwapChain> lpSwapChain;
		ComPtr<ID3D12Resource>* SwapBufferRes;
		ComPtr<ID3D12Resource> InputRes;
		ComPtr<ID3D12Resource> OutputRes;
		ComPtr<ID3D12Resource> DepthStencilRes;
		ComPtr<ID3D12DescriptorHeap> mRtvHeap;
		ComPtr<ID3D12DescriptorHeap> mDsvHeap;
		ComPtr<ID3D12DescriptorHeap> mCbvHeap;
		ComPtr<ID3D12DescriptorHeap> mSrvHeap;
		ComPtr<ID3D12DescriptorHeap> mUavHeap;
		ComPtr<ID3D12DescriptorHeap> mSamplerHeap;
		ComPtr<ID3D12RootSignature> mRootSignature;
		ComPtr<ID3D12RootSignature> cRootSignature;
		ComPtr<ID3D12PipelineState> PSO;
		ComPtr<ID3D12PipelineState> CSO;
		lpFlushCommandQueue FlushCommandQueue;
		UINT ThreadX;
		UINT ThreadY;
		UINT ThreadZ;
		UINT CurrebtSwapChainBuffer;
		D3D12_VIEWPORT Vp;
		D3D12_RECT SciRect;
		UINT RtvDescSize;
		UINT DsvDescSize;
		UINT CbvSrvUavDescSize;
		UINT SamplerSize;
		CD3DX12_CPU_DESCRIPTOR_HANDLE CbvHeapHandle;
		CD3DX12_CPU_DESCRIPTOR_HANDLE RtvHeapHandle;
		CD3DX12_CPU_DESCRIPTOR_HANDLE DsvHeapHandle;
		CD3DX12_CPU_DESCRIPTOR_HANDLE SrvHeapHandle;
		CD3DX12_CPU_DESCRIPTOR_HANDLE UavHeapHandle;
		CD3DX12_CPU_DESCRIPTOR_HANDLE SamplerHeapHandle;
		UINT* CbvHeapindex;
		float Pos[3];
		uint64_t timestamp;
		DirectXTool* tool;
		CheckForIntersectionfunc fun;
	};

	uint32_t levels = 0;
	std::unordered_map<std::wstring, ComPtr<ID3D12PipelineState>>* mPSOs;
	ComPtr<ID3D12PipelineState> PSO;
	wchar_t ObjectName[OBJECT_NAME];
	CheckForIntersectionfunc fun;
	virtual bool CheckForIntersection(CameraInformation*) = 0;
	virtual int64_t BaseObjectFun(UpdataArg& Arg) = 0;
	virtual int64_t ShowMethod(UpdataArg& Arg) = 0;
};