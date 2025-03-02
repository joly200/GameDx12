#pragma once
#include<Windows.h>  
#include "DxWin.h"
LRESULT CALLBACK MainWndProc(HWND , UINT , WPARAM , LPARAM );
class application
{
private:
    application():szWindowClass(L"DxWinClass"),szTitle(L"DxWin") {};
public:
    HWND hWnd;
    HKL Statehlayout;
    WCHAR szWindowClass[0x20];
    WCHAR szTitle[0x20];
public:
    static application* ApplicationInstance() {
        if (!application::lpapplication) {
            lpapplication = new application();
            return lpapplication;
        }
        return lpapplication;
    }
    static application* lpapplication;
public:

    int initializeApplication(HINSTANCE hInstance, int nCmdShow);
    LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
private:
    ATOM MyRegisterClass(HINSTANCE hInstance)
    {
        WNDCLASSEXW wcex;

        wcex.cbSize = sizeof(WNDCLASSEX);

        wcex.style = CS_HREDRAW | CS_VREDRAW;
        wcex.lpfnWndProc = MainWndProc;
        wcex.cbClsExtra = 0;
        wcex.cbWndExtra = 0;
        wcex.hInstance = hInstance;
        wcex.hIcon = nullptr;
        wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
        wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
        wcex.lpszMenuName = NULL;//MAKEINTRESOURCEW(IDC_GAMEDX12);
        wcex.lpszClassName = szWindowClass;
        wcex.hIconSm = NULL;

        return RegisterClassExW(&wcex);
    }
    BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
    {
        width = GetSystemMetrics(SM_CXSCREEN);
        height = GetSystemMetrics(SM_CYSCREEN);
        HMODULE hmodule = GetModuleHandle(NULL);
        hWnd = CreateWindowW(szWindowClass, szTitle, WS_POPUP,
            0, 0, width, height, nullptr, nullptr, hInstance, nullptr);
        SetWindowLong(hWnd, GWL_STYLE, 0);
        if (!hWnd)
        {
            return FALSE;
        }

        int i = ShowWindow(hWnd, nCmdShow);
        Statehlayout = GetKeyboardLayout(0);
        HKL m_hlayout = LoadKeyboardLayout(L"00000409", KLF_REPLACELANG);
        ActivateKeyboardLayout(m_hlayout, 0);

        UpdateWindow(hWnd);

        return TRUE;
    }
private:
    const int Xmuose{ GetSystemMetrics(SM_CXSCREEN)/2}, Ymuose{ GetSystemMetrics(SM_CYSCREEN) / 2 };
protected:
    static void releaseInstance()
    {
        if (lpapplication)
        {
            HKL m_hlayout = LoadKeyboardLayout(L"00000804", KLF_REPLACELANG);
            ActivateKeyboardLayout(m_hlayout, 0);
            delete lpapplication;
        }
        
    }
    class ApplicationHelper {
    public:
        ApplicationHelper() {
            application::ApplicationInstance();
        }
        ~ApplicationHelper() {
            application::releaseInstance();
        }
    };
    static ApplicationHelper dxWinhelper;
public:
    UINT width, height;
};
