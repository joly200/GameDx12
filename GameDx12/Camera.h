#pragma once
#include<d3d12.h>
#include<DirectXMath.h>
#include "d3dx12.h"
#include <atomic>
#include "DirectXTool.h"
#include "GameTimer.h"


using namespace DirectX;
class Camera
{
public:
	Camera(UINT width,UINT height,ID3D12Device* lpDevice, UINT  CbvDescSize, ID3D12DescriptorHeap* Cbvheap,
		CD3DX12_CPU_DESCRIPTOR_HANDLE& CbvHeapHandle, DirectXTool* tool,UINT* CbvHeapindex = nullptr);
	~Camera();



public:
	// Get/Set world camera position.
	XMVECTOR GetPosition()const;
	XMFLOAT3 GetPosition3f()const;
	void SetPosition(float x, float y, float z);
	void SetPosition(const DirectX::XMFLOAT3& v);

	// Get camera basis vectors.
	XMVECTOR GetRight()const;
	XMFLOAT3 GetRight3f()const;
	XMVECTOR GetUp()const;
	XMFLOAT3 GetUp3f()const;
	XMVECTOR GetLook()const;
	XMFLOAT3 GetLook3f()const;

	// Get frustum properties.
	float GetNearZ()const;
	float GetFarZ()const;
	float GetAspect()const;
	float GetFovY()const;
	float GetFovX()const;

	// Get near and far plane dimensions in view space coordinates.
	float GetNearWindowWidth()const;
	float GetNearWindowHeight()const;
	float GetFarWindowWidth()const;
	float GetFarWindowHeight()const;

	// Set frustum.
	void SetLens(float fovY, float aspect, float zn, float zf);

	// Define camera space via LookAt parameters.
	void LookAt(FXMVECTOR pos,FXMVECTOR target, FXMVECTOR worldUp);
	void LookAt(const XMFLOAT3& pos, const XMFLOAT3& target, const XMFLOAT3& up);

	// Get View/Proj matrices.
	XMMATRIX GetView()const;
	XMMATRIX GetProj()const;

	XMFLOAT4X4 GetView4x4f()const;
	XMFLOAT4X4 GetProj4x4f()const;


	void Strafe(float d);
	void Walk(float d);
	void Up(float d);

	void Pitch(float angle);
	void RotateY(float angle);


	void UpdateViewMatrix(ULONGLONG starttoCurrentTick);
	void UpdateFrustumPlanes();
	plane ComputePlane(XMFLOAT3 p0, XMFLOAT3 p1, XMFLOAT3 p2);
	CameraInformation* GetCameraInformation();
private:


	XMFLOAT3 mPosition = { 0.0f, 0.0f, -50.0f };
	XMFLOAT3 mRight = { 1.0f, 0.0f, 0.0f };
	XMFLOAT3 mUp = { 0.0f, 1.0f, 0.0f };
	XMFLOAT3 mLook = { 0.0f, 0.0f, 1.0f };

	float mNearZ = 0.0f;
	float mFarZ = 0.0f;
	float mAspect = 0.0f;
	float mFovY = 0.0f;
	float mNearWindowHeight = 0.0f;
	float mFarWindowHeight = 0.0f;
	float mSpeed = 0.02f;
	std::atomic<bool> mViewDirty = true;


	struct camera{
		XMFLOAT4X4 mProj;
		XMFLOAT4X4 mView;
	};
	camera cameraState;

private:
	void* MapPtr1;
	DirectXTool* tool;
	UINT width;
	UINT height;
	CameraInformation camInfo;
	ID3D12Resource* camerauploadBuffer;

public:
	bool lMouseDown = 0,showcCursor = 0 , WkeyDown = 0, 
		SkeyDown = 0, DkeyDown = 0, AkeyDown = 0, SPACEkeyDown = 0, CkeyDown = 0;
	UINT dWkeyDownTick = 0, dSkeyDownTick = 0, dDkeyDownTick = 0, dAkeyDownTick = 0,
		dSPACEkeyDownTick = 0, dCkeyDownTick = 0;
	int LastMouseX = 0, LastMouseY = 0;
	void TestTick(UINT Tick)
	{
	}
	void WkeyState(bool state) {

		if (WkeyDown == state)return;
		dWkeyDownTick = GameTimer::lpGameTimer->GetStartToCurrentTick();
		WkeyDown = state;
		if (WkeyDown || SkeyDown || DkeyDown || AkeyDown || SPACEkeyDown || CkeyDown)
		{
			mViewDirty = TRUE;
			return;
		}
		mViewDirty = state;

	}
	void SkeyState(bool state) {
		if (SkeyDown == state)return;
		SkeyDown = state;
		dSkeyDownTick = GameTimer::lpGameTimer->GetStartToCurrentTick();
		if (WkeyDown || SkeyDown || DkeyDown || AkeyDown || SPACEkeyDown || CkeyDown)
		{
			mViewDirty = TRUE;
			return;
		}
		mViewDirty = state;
	}
	void DkeyState(bool state) {
		if (DkeyDown == state)return;
		DkeyDown = state;
		dDkeyDownTick = GameTimer::lpGameTimer->GetStartToCurrentTick();
		if (WkeyDown || SkeyDown || DkeyDown || AkeyDown || SPACEkeyDown || CkeyDown)
		{
			mViewDirty = TRUE;
			return;
		}
		mViewDirty = state;
	}
	void AkeyState(bool state) {
		if (AkeyDown == state)return;
		AkeyDown = state;
		dAkeyDownTick = GameTimer::lpGameTimer->GetStartToCurrentTick();
		if (WkeyDown || SkeyDown || DkeyDown || AkeyDown || SPACEkeyDown || CkeyDown)
		{
			mViewDirty = TRUE;
			return;
		}
		mViewDirty = state;
	}
	void SPACEkeyState(bool state) {
		if (SPACEkeyDown == state)return;
		SPACEkeyDown = state;
		dSPACEkeyDownTick = GameTimer::lpGameTimer->GetStartToCurrentTick();
		if (WkeyDown || SkeyDown || DkeyDown || AkeyDown || SPACEkeyDown || CkeyDown)
		{
			mViewDirty = TRUE;
			return;
		}
		mViewDirty = state;
	}
	void CkeyState(bool state) {
		if (CkeyDown == state)return;
		CkeyDown = state;
		dCkeyDownTick = GameTimer::lpGameTimer->GetStartToCurrentTick();
		if (WkeyDown || SkeyDown || DkeyDown || AkeyDown || SPACEkeyDown || CkeyDown)
		{
			mViewDirty = TRUE;
			return;
		}
		mViewDirty = state;
	}
	void LMouseDown(int X, int Y) {

	}
	void LMouseUp() {

	}
	void LTurnPitchAndRotateY(int X, int Y) {
		if (showcCursor)return;

		float dx = 0.001 * static_cast<float>(X);
		float dy = 0.001 * static_cast<float>(Y);
		if(dy)Pitch(dy);
		if(dx)RotateY(dx);
	}
	void CursotState(bool state) {
		ShowCursor(state);
	}
};

