#include "Camera.h"
#include <cmath> 
using namespace DirectX;

Camera::Camera(UINT width, UINT height, ID3D12Device* lpDevice, UINT  CbvDescSize, ID3D12DescriptorHeap* Cbvheap, 
	CD3DX12_CPU_DESCRIPTOR_HANDLE& CbvHeapHandle, DirectXTool* tool, UINT* CbvHeapindex)
{
	this->width = width;
	this->height = height;
	this->tool = tool;
	cameraState.mProj = tool->Identity4x4();
	cameraState.mView = tool->Identity4x4();
	CursotState(showcCursor);
	SetLens(0.25f * tool->PI, static_cast<float>(width)/static_cast<float>(height), 0.0001f, 1000.0f);
	MapPtr1 = tool->conveyGpu(lpDevice, &camerauploadBuffer, CbvDescSize, sizeof(Camera::camera), CbvHeapHandle);
	CbvHeapHandle.Offset(1, CbvDescSize);
	*CbvHeapindex += 1;
	memcpy(MapPtr1, &cameraState, sizeof(cameraState));
}

Camera::~Camera()
{
	if(camerauploadBuffer)camerauploadBuffer->Unmap(0, nullptr);
	
}


XMVECTOR Camera::GetPosition()const
{
	return XMLoadFloat3(&mPosition);
}

XMFLOAT3 Camera::GetPosition3f()const
{
	return mPosition;
}

void Camera::SetPosition(float x, float y, float z)
{
	mPosition = XMFLOAT3(x, y, z);
	mViewDirty = true;
}

void Camera::SetPosition(const XMFLOAT3& v)
{
	mPosition = v;
	mViewDirty = true;
}

XMVECTOR Camera::GetRight()const
{
	return XMLoadFloat3(&mRight);
}

XMFLOAT3 Camera::GetRight3f()const
{
	return mRight;
}

XMVECTOR Camera::GetUp()const
{
	return XMLoadFloat3(&mUp);
}

XMFLOAT3 Camera::GetUp3f()const
{
	return mUp;
}

XMVECTOR Camera::GetLook()const
{
	return XMLoadFloat3(&mLook);
}

XMFLOAT3 Camera::GetLook3f()const
{
	return mLook;
}

float Camera::GetNearZ()const
{
	return mNearZ;
}

float Camera::GetFarZ()const
{
	return mFarZ;
}

float Camera::GetAspect()const
{
	return mAspect;
}

float Camera::GetFovY()const
{
	return mFovY;
}

float Camera::GetFovX()const
{
	float halfWidth = 0.5f * GetNearWindowWidth();
	return 2.0f * atan(halfWidth / mNearZ);
}

float Camera::GetNearWindowWidth()const
{
	return mAspect * mNearWindowHeight;
}

float Camera::GetNearWindowHeight()const
{
	return mNearWindowHeight;
}

float Camera::GetFarWindowWidth()const
{
	return mAspect * mFarWindowHeight;
}

float Camera::GetFarWindowHeight()const
{
	return mFarWindowHeight;
}

void Camera::SetLens(float fovY, float aspect, float zn, float zf)
{
	// cache properties
	mFovY = fovY;
	mAspect = aspect;
	mNearZ = zn;
	mFarZ = zf;

	mNearWindowHeight = 2.0f * mNearZ * tanf(0.5f * mFovY);
	mFarWindowHeight = 2.0f * mFarZ * tanf(0.5f * mFovY);

	XMMATRIX P = XMMatrixPerspectiveFovLH(mFovY, mAspect, mNearZ, mFarZ);
	XMStoreFloat4x4(&cameraState.mProj, P);
}

void Camera::LookAt(FXMVECTOR pos, FXMVECTOR target, FXMVECTOR worldUp)
{
	XMVECTOR L = XMVector3Normalize(XMVectorSubtract(target, pos));
	XMVECTOR R = XMVector3Normalize(XMVector3Cross(worldUp, L));
	XMVECTOR U = XMVector3Cross(L, R);

	XMStoreFloat3(&mPosition, pos);
	XMStoreFloat3(&mLook, L);
	XMStoreFloat3(&mRight, R);
	XMStoreFloat3(&mUp, U);

	mViewDirty = true;
}

void Camera::LookAt(const XMFLOAT3& pos, const XMFLOAT3& target, const XMFLOAT3& up)
{
	XMVECTOR P = XMLoadFloat3(&pos);
	XMVECTOR T = XMLoadFloat3(&target);
	XMVECTOR U = XMLoadFloat3(&up);

	LookAt(P, T, U);

	mViewDirty = true;
}

XMMATRIX Camera::GetView()const
{
	assert(!mViewDirty);
	return XMLoadFloat4x4(&cameraState.mView);
}

XMMATRIX Camera::GetProj()const
{
	return XMLoadFloat4x4(&cameraState.mProj);
}


XMFLOAT4X4 Camera::GetView4x4f()const
{
	assert(!mViewDirty);
	return cameraState.mView;
}

XMFLOAT4X4 Camera::GetProj4x4f()const
{
	return cameraState.mProj;
}

void Camera::Strafe(float d)
{
	// mPosition += d*mRight
	XMVECTOR s = XMVectorReplicate(d);
	XMVECTOR r = XMLoadFloat3(&mRight);
	XMVECTOR p = XMLoadFloat3(&mPosition);
	XMStoreFloat3(&mPosition, XMVectorMultiplyAdd(s, r, p));

	mViewDirty = true;
}

void Camera::Walk(float d)
{
	// mPosition += d*mLook
	XMVECTOR s = XMVectorReplicate(d);
	XMVECTOR l = XMLoadFloat3(&mLook);
	XMVECTOR p = XMLoadFloat3(&mPosition);
	XMStoreFloat3(&mPosition, XMVectorMultiplyAdd(s, l, p));

	mViewDirty = true;
}

void Camera::Up(float d)
{
	XMVECTOR s = XMVectorReplicate(d);
	XMVECTOR u = { 0.0f,1.0f,0.0f };//XMLoadFloat3(&mUp);
	XMVECTOR p = XMLoadFloat3(&mPosition);
	XMStoreFloat3(&mPosition, XMVectorMultiplyAdd(s, u, p));
	
	mViewDirty = true;
}

void Camera::Pitch(float angle)
{
	// Rotate up and look vector about the right vector.
	 // 计算新的仰角（Pitch），限制在[-89, 89]度之间

	float dotValue = mLook.y;

	float angleInRadians = std::acos(dotValue);


	if ((angleInRadians + angle) < (pi / 170) || (angleInRadians + angle) > (pi - pi / 170)) {
		angle = 0;
	}
	XMMATRIX R = XMMatrixRotationAxis(XMLoadFloat3(&mRight), angle);

	XMStoreFloat3(&mUp, XMVector3TransformNormal(XMLoadFloat3(&mUp), R));
	XMStoreFloat3(&mLook, XMVector3TransformNormal(XMLoadFloat3(&mLook), R));

	mViewDirty = true;
}

void Camera::RotateY(float angle)
{
	// Rotate the basis vectors about the world y-axis.

	XMMATRIX R = XMMatrixRotationY(angle);

	XMStoreFloat3(&mRight, XMVector3TransformNormal(XMLoadFloat3(&mRight), R));
	XMStoreFloat3(&mUp, XMVector3TransformNormal(XMLoadFloat3(&mUp), R));
	XMStoreFloat3(&mLook, XMVector3TransformNormal(XMLoadFloat3(&mLook), R));

	mViewDirty = true;
}

void Camera::UpdateViewMatrix(ULONGLONG starttoCurrentTick)
{
	if (mViewDirty)
	{

		XMVECTOR R = XMLoadFloat3(&mRight);
		XMVECTOR U = XMLoadFloat3(&mUp);
		XMVECTOR L = XMLoadFloat3(&mLook);
		XMVECTOR P = XMLoadFloat3(&mPosition);

		// Keep camera's axes orthogonal to each other and of unit length.
		L = XMVector3Normalize(L);
		U = XMVector3Normalize(XMVector3Cross(L, R));

		// U, L already ortho-normal, so no need to normalize cross product.
		R = XMVector3Cross(U, L);

		// Fill in the view matrix entries.
		float x = -XMVectorGetX(XMVector3Dot(P, R));
		float y = -XMVectorGetX(XMVector3Dot(P, U));
		float z = -XMVectorGetX(XMVector3Dot(P, L));

		XMStoreFloat3(&mRight, R);
		XMStoreFloat3(&mUp, U);
		XMStoreFloat3(&mLook, L);

		cameraState.mView(0, 0) = mRight.x;
		cameraState.mView(1, 0) = mRight.y;
		cameraState.mView(2, 0) = mRight.z;
		cameraState.mView(3, 0) = x;

		cameraState.mView(0, 1) = mUp.x;
		cameraState.mView(1, 1) = mUp.y;
		cameraState.mView(2, 1) = mUp.z;
		cameraState.mView(3, 1) = y;

		cameraState.mView(0, 2) = mLook.x;
		cameraState.mView(1, 2) = mLook.y;
		cameraState.mView(2, 2) = mLook.z;
		cameraState.mView(3, 2) = z;

		cameraState.mView(0, 3) = 0.0f;
		cameraState.mView(1, 3) = 0.0f;
		cameraState.mView(2, 3) = 0.0f;
		cameraState.mView(3, 3) = 1.0f;
		UpdateFrustumPlanes();

		camera* ptr = (camera*)MapPtr1;
		memcpy(&ptr->mView, &cameraState.mView, sizeof(cameraState.mView));
		mViewDirty = false;
		if (WkeyDown)
		{
			UINT Tick = GameTimer::lpGameTimer->GetStartToCurrentTick(), dt;
			dt = Tick - dWkeyDownTick;
			Walk(mSpeed * static_cast<float>(dt));
			dWkeyDownTick = Tick;
		}
		if (SkeyDown)
		{
			UINT Tick = GameTimer::lpGameTimer->GetStartToCurrentTick(), dt;
			dt = Tick - dSkeyDownTick;
			Walk(-mSpeed * static_cast<float>(dt));
			dSkeyDownTick = Tick;
		}
		if (DkeyDown)
		{
			UINT Tick = GameTimer::lpGameTimer->GetStartToCurrentTick(), dt;
			dt = Tick - dDkeyDownTick;
			Strafe(mSpeed * static_cast<float>(dt));
			dDkeyDownTick = Tick;
		}
		if (AkeyDown)
		{
			UINT Tick = GameTimer::lpGameTimer->GetStartToCurrentTick(), dt;
			dt = Tick - dAkeyDownTick;
			Strafe(-mSpeed * static_cast<float>(dt));
			dAkeyDownTick = Tick;
			
		}
		if (SPACEkeyDown)
		{
			UINT Tick = GameTimer::lpGameTimer->GetStartToCurrentTick(), dt;
			dt = Tick - dSPACEkeyDownTick;
 			Up(mSpeed * static_cast<float>(dt));
			dSPACEkeyDownTick = Tick;
			
		}
		if (CkeyDown)
		{
			UINT Tick = GameTimer::lpGameTimer->GetStartToCurrentTick(), dt;
			dt = Tick - dCkeyDownTick;
  			Up(-mSpeed * static_cast<float>(dt));
			dCkeyDownTick = Tick;

		}

	}
}

void Camera::UpdateFrustumPlanes()
{
	// 将 XMFLOAT3 转换为 XMVECTOR
	XMVECTOR position = XMLoadFloat3(&mPosition);
	XMVECTOR right = XMLoadFloat3(&mRight);
	XMVECTOR up = XMLoadFloat3(&mUp);
	XMVECTOR look = XMLoadFloat3(&mLook);

	// 计算近平面和远平面的距离
	float nearHeight = 2.0f * tanf(mFovY * 0.5f) * mNearZ;
	float nearWidth = nearHeight * mAspect;
	float farHeight = 2.0f * tanf(mFovY * 0.5f) * mFarZ;
	float farWidth = farHeight * mAspect;

	// 计算近平面和远平面的中心点
	XMVECTOR nearCenter = position + look * mNearZ;
	XMVECTOR farCenter = position + look * mFarZ;

	// 计算近平面和远平面的四个角点
	XMFLOAT3 nearTopLeft, nearTopRight, nearBottomLeft, nearBottomRight;
	XMFLOAT3 farTopLeft, farTopRight, farBottomLeft, farBottomRight;

	XMStoreFloat3(&nearTopLeft, nearCenter + (up * (nearHeight * 0.5f)) - (right * (nearWidth * 0.5f)));
	XMStoreFloat3(&nearTopRight, nearCenter + (up * (nearHeight * 0.5f)) + (right * (nearWidth * 0.5f)));
	XMStoreFloat3(&nearBottomLeft, nearCenter - (up * (nearHeight * 0.5f)) - (right * (nearWidth * 0.5f)));
	XMStoreFloat3(&nearBottomRight, nearCenter - (up * (nearHeight * 0.5f)) + (right * (nearWidth * 0.5f)));

	XMStoreFloat3(&farTopLeft, farCenter + (up * (farHeight * 0.5f)) - (right * (farWidth * 0.5f)));
	XMStoreFloat3(&farTopRight, farCenter + (up * (farHeight * 0.5f)) + (right * (farWidth * 0.5f)));
	XMStoreFloat3(&farBottomLeft, farCenter - (up * (farHeight * 0.5f)) - (right * (farWidth * 0.5f)));
	XMStoreFloat3(&farBottomRight, farCenter - (up * (farHeight * 0.5f)) + (right * (farWidth * 0.5f)));

	// 计算六个平面方程
	camInfo.n = ComputePlane(nearTopLeft, nearTopRight, nearBottomLeft); // 近平面
	camInfo.f = ComputePlane(farTopRight, farTopLeft, farBottomRight);   // 远平面
	camInfo.r = ComputePlane(nearTopRight, farTopRight, nearBottomRight); // 右平面
	camInfo.l = ComputePlane(farTopLeft, nearTopLeft, farBottomLeft);     // 左平面
	camInfo.t = ComputePlane(farTopLeft, farTopRight, nearTopLeft);       // 顶平面
	camInfo.b = ComputePlane(nearBottomRight, farBottomRight, nearBottomLeft); // 底平面

	// 更新视线方向
	camInfo.LookTo = mLook;
}

plane Camera::ComputePlane(XMFLOAT3 p0, XMFLOAT3 p1, XMFLOAT3 p2)
{
	XMVECTOR v0 = XMLoadFloat3(&p0);
	XMVECTOR v1 = XMLoadFloat3(&p1);
	XMVECTOR v2 = XMLoadFloat3(&p2);

	// 计算两个向量
	XMVECTOR edge1 = v1 - v0;
	XMVECTOR edge2 = v2 - v0;

	// 计算平面的法向量
	XMVECTOR normal = XMVector3Normalize(XMVector3Cross(edge1, edge2));

	// 计算平面方程的 D 值
	float D = -XMVectorGetX(XMVector3Dot(normal, v0));

	// 返回平面方程
	return { XMVectorGetX(normal), XMVectorGetY(normal), XMVectorGetZ(normal), D };
}

CameraInformation* Camera::GetCameraInformation()
{

	return &camInfo;
}


