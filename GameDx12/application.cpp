#include "application.h"
#include"windowsx.h"

application::ApplicationHelper application::dxWinhelper;
application* application::lpapplication = application::ApplicationInstance();
LRESULT CALLBACK
MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{

    return application::lpapplication->WndProc(hwnd, msg, wParam, lParam);
}

int application::initializeApplication(HINSTANCE hInstance, int nCmdShow)
{

    MyRegisterClass(hInstance);
    return InitInstance(hInstance, nCmdShow);
}


LRESULT application::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    UINT height, width;
 
    static UINT startR = 0;
    static UINT startL = 0;
    UINT dt = 0;
    switch (message)
    {
    case WM_COMMAND:
    {
        break;
    }
    case WM_CREATE:
        
        break;
    case WM_USER:
        break;
    case WM_KEYDOWN:
        switch (wParam)
        {
        case VK_LEFT:
        
            break;

        case VK_RIGHT:

            break;

        case VK_UP:

            break;

        case VK_DOWN:


        case VK_F11:
            DxWin::lpDxWin->camera->showcCursor = !DxWin::lpDxWin->camera->showcCursor;
            DxWin::lpDxWin->camera->CursotState(DxWin::lpDxWin->camera->showcCursor);

            break;
        case VK_ESCAPE:
            PostQuitMessage(0); 
            break;
        }


        break;
    case WM_KEYUP:
        
        startR = 0;
        break;

    case WM_RBUTTONDOWN:


        break;
    

    case WM_RBUTTONUP: {

        break;
    }
    
    case WM_MOUSEMOVE: 
        if (!DxWin::lpDxWin->camera)break;
        DxWin::lpDxWin->camera->LTurnPitchAndRotateY(GET_X_LPARAM(lParam)-Xmuose, GET_Y_LPARAM(lParam)-Ymuose);
        if(!DxWin::lpDxWin->camera->showcCursor)SetCursorPos(Xmuose, Ymuose);
      break;
        /////////////
    case WM_LBUTTONDOWN:

       break;


    case WM_LBUTTONUP: 

        break;
    case WM_SIZE:
        height = HIWORD(lParam);
        width = LOWORD(lParam);

        DxWin::lpDxWin->SetViewPort(width, height);
        break;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        EndPaint(hWnd, &ps);
    }
    break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}
