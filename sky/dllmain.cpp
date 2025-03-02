// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "pch.h"
#include "sky.h"
#ifdef MYDLL_EXPORTS
#define MYDLL_API __declspec(dllexport)
#else
#define MYDLL_API __declspec(dllimport)
#endif
extern "C" MYDLL_API virtualBase* GetTheObject(uint64_t param1, void* param2);
BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

extern "C" virtualBase* GetTheObject(uint64_t param1, void* param2)
{
    virtualBase::UpdataArg* Paraml = (virtualBase::UpdataArg*)param1;
    virtualBase* lpsky = (virtualBase*)new sky(Paraml);
    return lpsky;
}
