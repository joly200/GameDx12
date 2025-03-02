// GameDx12.cpp : 定义应用程序的入口点。
//
#pragma once
#include "application.h"
#include "framework.h"
#include "GameDx12.h"
#include"BaseObject.h"
#include <thread>



#define CLOSE_THE_THREAD (WM_USER+1)
#define MAX_LOADSTRING 100

UINT ProState = 1;
void UpDate(void* arg);
DWORD WINAPI UpDateFunc(
    LPVOID lpParameter
);
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);


    // 执行应用程序初始化:
    if (!application::lpapplication->initializeApplication(hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_GAMEDX12));

    MSG msg = { 0 };
    int result = DxWin::lpDxWin->initializeDxWin(application::lpapplication->hWnd, 
        application::lpapplication->width,
        application::lpapplication->height);
    BaseObject::InputArg Arg;
    ZeroMemory(&Arg, sizeof(BaseObject::InputArg));
    result =  BaseObject::initializeObject(Arg);
    if (result == 0) {
        return 0;
    }
    GameTimer::lpGameTimer->initializeGameTimer();
    HANDLE hThread = (HANDLE)_beginthread(UpDate, 0, nullptr);

    while (msg.message != WM_QUIT)
    {
        int test = 0;
        if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
            if (msg.message == WM_KEYDOWN)
            {
                switch (msg.wParam)
                {
                case 'W':
                    DxWin::lpDxWin->camera->WkeyState(TRUE);
                    break;
                case 'S':
                    DxWin::lpDxWin->camera->SkeyState(TRUE);
                    break;
                case 'D':
                    DxWin::lpDxWin->camera->DkeyState(TRUE);
                    break;
                case 'A':
                    DxWin::lpDxWin->camera->AkeyState(TRUE);
                    break;
                case ' ':
                    DxWin::lpDxWin->camera->SPACEkeyState(TRUE);
                    break;
                case 'C':
                    DxWin::lpDxWin->camera->CkeyState(TRUE);
                    break;
                case VK_ESCAPE:
                    ProState = 0;
                    Sleep(10);
                    PostQuitMessage(0);
                    break;

                }
            }
            if (msg.message == WM_KEYUP) {
                switch (msg.wParam)
                {
                case 'W':
                    if (DxWin::lpDxWin->camera->WkeyDown)DxWin::lpDxWin->camera->WkeyState(FALSE);
                    break;
                case 'S':                    
                    if (DxWin::lpDxWin->camera->SkeyDown)DxWin::lpDxWin->camera->SkeyState(FALSE);
                    break;
                case 'D':  
                    if (DxWin::lpDxWin->camera->DkeyDown)DxWin::lpDxWin->camera->DkeyState(FALSE);
                    break;
                case 'A':
                    
                    if (DxWin::lpDxWin->camera->AkeyDown)DxWin::lpDxWin->camera->AkeyState(FALSE);
                    break;
                case ' ':
                    if (DxWin::lpDxWin->camera->SPACEkeyDown)DxWin::lpDxWin->camera->SPACEkeyState(FALSE);
                    break;
                case 'C':
                    if (DxWin::lpDxWin->camera->CkeyDown)DxWin::lpDxWin->camera->CkeyState(FALSE);
                    break;
                }
            }

        }
        else
        {


        }

    }

    return (int) msg.wParam;
}


void UpDate(void* arg) {
    UpDateFunc(nullptr);
    _endthread();
}
UINT count = 0;

DWORD WINAPI UpDateFunc(
    LPVOID lpParameter
) {

    while (ProState)
    {
        DxWin::lpDxWin->camera->UpdateViewMatrix(GameTimer::lpGameTimer->GetStartToCurrentTick());
        DxWin::lpDxWin->DrawGraphics();
    }
    return 0;
}