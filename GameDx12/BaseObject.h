#pragma once

#include"DirectXTool.h"
#include <wrl/client.h>

#include"DxWin.h"
class BaseObject:public virtualBase
{
public:
	static BaseObject* BaseObjectInstance() {
		if (!lpBaseObject) {
			lpBaseObject = new BaseObject();
			return lpBaseObject;
		}
		return lpBaseObject;
	}
	static BaseObject* lpBaseObject;

public:
	BaseObject() {};
	~BaseObject() {};
public:
	struct InputArg 
	{
		int8_t ObjectName[0x100];
	};
	static UpdataArg arg;
public:
	struct DllsAndObjects
	{
		DllsAndObjects(virtualBase* lpObject, HMODULE Dll) {
			this->lpObject = lpObject;
			this->Dll = Dll;
		}
		~DllsAndObjects() {
			
		}
		DllsAndObjects& operator=(DllsAndObjects arg) {
			if (this != &arg) {  // ��ֹ���Ҹ�ֵ
				this->lpObject = arg.lpObject;
				this->Dll = arg.Dll;
			}
			return *this;  // ���ص�ǰ���������
		}
		virtualBase* lpObject;
		HMODULE Dll;
	};
	static std::list<DllsAndObjects> lpObjects;
	static bool CompareLevels(BaseObject::DllsAndObjects a, BaseObject::DllsAndObjects b) {
		return a.lpObject->levels > b.lpObject->levels; // �������У��ϴ�� levels ��ǰ��
	}
	static bool CheckForIntersectionFun(CameraInformation* infPtr, void* lpState ,float r);
	static std::unordered_map<std::wstring, ComPtr<ID3D12PipelineState>> mPSOs;

public:
	static int64_t initializeObject(InputArg Arg);
public:
	typedef virtualBase* (*GetTheObjectFun)(UpdataArg*, void*);
protected:
	static void BaseObjectreleaseInstance();
	class BaseObjectHelper {
	public:
		BaseObjectHelper() {
			BaseObject::BaseObjectInstance();
		}
		~BaseObjectHelper() {


			BaseObject::BaseObjectreleaseInstance();
		}
	};
	virtual int64_t BaseObjectFun(UpdataArg& Arg) { return 0; };
	virtual int64_t ShowMethod(UpdataArg& Arg) { return 0;};
	virtual bool CheckForIntersection(CameraInformation*) { return 0; };
	static BaseObjectHelper BaseObjectHelperhelper;
};

