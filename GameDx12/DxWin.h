#pragma once
#include<d3d12.h>
#include<dxgi.h>
#include"d3dx12.h"
#include<d3dcompiler.h>
#include<DirectXColors.h>
#include<string>
#include "DirectXTool.h"
#include"Camera.h"
#include"virtualBase.h"
#include"Tool.h"
#include<list>
#pragma comment(lib,"D3DCompiler.lib")
#pragma comment(lib,"D3d12.lib")
#pragma comment(lib,"DXGI.lib")
#define BUFFERCOUNT 2

class DxWin
{
public:
	static DxWin* DxWinInstance() {
		if (!lpDxWin) {
			lpDxWin = new DxWin();
			return lpDxWin;
		}
		return lpDxWin;
	}
	static DxWin* lpDxWin;
public:
	int initializeDxWin(HWND hWnd, UINT width, UINT height);
public:
	DxWin();
	~DxWin();

	friend class virtualBase;
public:
	int initialize( HWND hWnd);
	int SetRenderPipeline();
	UINT GetBufferIndex();
	
	int DrawGraphics();
	int ShowObject();
	int SetViewPort(UINT width,UINT height);
	int SetClientSize(UINT width, UINT height);
	void  FlushCommandQueue() ;
	static void  StaticFlushCommandQueue();

public:
	ComPtr<IDXGIFactory> lpFactory;
	ComPtr<ID3D12Device> lpDevice;
	ComPtr<ID3D12CommandAllocator> lpCommandAllocator;
	ComPtr<ID3D12GraphicsCommandList> lpCommandList;
	ComPtr<ID3D12CommandQueue> lpCommandQueue;
	ComPtr<ID3D12CommandSignature> lpCommandSignature;
	ComPtr<ID3D12Fence> lpFence;
	ComPtr<IDXGISwapChain> lpSwapChain;
	ComPtr<ID3D12DescriptorHeap> mRtvHeap;
	ComPtr<ID3D12DescriptorHeap> mDsvHeap;
	ComPtr<ID3D12DescriptorHeap> mCbvHeap;
	ComPtr<ID3D12DescriptorHeap> mSrvHeap;
	ComPtr<ID3D12DescriptorHeap> mUavHeap;
	ComPtr<ID3D12DescriptorHeap> mSamplerHeap;
	ComPtr<ID3D12Resource> SwapBufferRes[BUFFERCOUNT];
	ComPtr<ID3D12Resource> InputRes;
	ComPtr<ID3D12Resource> OutputRes;
	ComPtr<ID3D12Resource> DepthStencilRes;
	ComPtr<ID3D12PipelineState> PSO;
	ComPtr<ID3D12PipelineState> CSO;
	ComPtr<ID3D12RootSignature> mRootSignature;
	ComPtr<ID3D12RootSignature> cRootSignature;

	ComPtr<ID3D12Debug> lpDebug;
	D3D12_RECT SciRect;

	UINT BufferCount = BUFFERCOUNT;
public:
	D3D12_VIEWPORT Vp;
	UINT RtvDescSize, DsvDescSize, CbvSrvUavDescSize,SamplerSize;
public:
	UINT VertexArraySize = 0;
	UINT IndexArraySize = 0;
	UINT ClientWidth;
	UINT ClientHeight;
	UINT CurrentFence = 0;
	UINT CurrebtSwapChainBuffer = 0;
	UINT ThreadX, ThreadY, ThreadZ;
	static CD3DX12_CPU_DESCRIPTOR_HANDLE CbvHeapHandle;
	static CD3DX12_CPU_DESCRIPTOR_HANDLE RtvHeapHandle;
	static CD3DX12_CPU_DESCRIPTOR_HANDLE DsvHeapHandle;
	static CD3DX12_CPU_DESCRIPTOR_HANDLE SrvHeapHandle;
	static CD3DX12_CPU_DESCRIPTOR_HANDLE UavHeapHandle;
	static CD3DX12_CPU_DESCRIPTOR_HANDLE SamplerHeapHandle;
	UINT CbvHeapindex = 0;
public:
	static void releaseInstance()
	{
		if (!lpDxWin)delete lpDxWin;
	}
	class DxWinHelper {
	public:
		DxWinHelper() {
			DxWin::DxWinInstance();
		}
		~DxWinHelper() {
		
			
			DxWin::releaseInstance();
		}
	};
	static DxWinHelper dxWinhelper;
public:
	Camera* camera;
	Tool tool;
};
