/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*    Windows/Window.cpp                             oooooo       oooooo      */
/*                                                 oooooooooo   oooooooooo    */
/*                                                         o%%%%%o            */
/*                                                         %:::::%            */
/*                                                        %:::::::%           */
/*    This file is part of the                             %:::::%            */
/*    Lums library.                                         %%%%%             */
/*                                                                            */
/* ************************************************************************** */

#include <LumsInclude/Graphics/Window.hpp>
#include <LumsInclude/Graphics/OpenGL.hpp>

// TODO: proper graphical warning / note system
#include <iostream>

using namespace lm;

static Window* currentWin;

static const PIXELFORMATDESCRIPTOR pfd =
{
    sizeof(PIXELFORMATDESCRIPTOR),
    1,
    PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
    PFD_TYPE_RGBA,
    32,
    0, 0, 0, 0, 0, 0,
    0,
    0,
    0,
    0, 0, 0, 0,
    32,
    8,
    0,
    PFD_MAIN_PLANE,
    0,
    0, 0, 0
};

static LRESULT CALLBACK
LMWindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    Event event;

    switch(msg)
    {
        case WM_KEYDOWN:
            event.type = Event::Type::KeyDown;
            event.key = static_cast<Key>(wParam);
            currentWin->pushEvent(event);
            break;
        case WM_KEYUP:
            event.type = Event::Type::KeyUp;
            event.key = static_cast<Key>(wParam);
            currentWin->pushEvent(event);
            break;
        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

Window::Window(int w, int h, const char* name, bool fullscreen)
{
    HINSTANCE hInstance = GetModuleHandle(NULL);
    static bool launched = false;

	static_cast<void>(fullscreen); // FIXME
    if (!launched)
    {
        WNDCLASSEX wc;

        wc.cbSize        = sizeof(WNDCLASSEX);
        wc.style         = CS_NOCLOSE | CS_OWNDC;
        wc.lpfnWndProc   = LMWindowProc;
        wc.cbClsExtra    = 0;
        wc.cbWndExtra    = 0;
        wc.hInstance     = hInstance;
        wc.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
        wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
        wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
        wc.lpszMenuName  = NULL;
        wc.lpszClassName = "LMWindow";
        wc.hIconSm       = LoadIcon(NULL, IDI_APPLICATION);
        RegisterClassEx(&wc);
        launched = true;
    }

    HWND hwnd = CreateWindowEx(
        WS_EX_CLIENTEDGE,
        "LMWindow",
        name,
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU,
        CW_USEDEFAULT, CW_USEDEFAULT, w, h,
        NULL, NULL, hInstance, NULL);
    _windowHandle = hwnd;

    HDC dc = GetWindowDC(hwnd);
    int pf = ChoosePixelFormat(dc, &pfd);

    SetPixelFormat(dc, pf, &pfd);

    HGLRC gl = wglCreateContext(dc);
    wglMakeCurrent(dc, gl);
    
    _openGlHandle = gl;

    ShowWindow(hwnd, SW_SHOW);
    UpdateWindow(hwnd);
    ReleaseDC(hwnd, dc);
}


void
Window::resize(int w, int h, bool fullscreen = false)
{
    WINDOWINFO wInfo;
    HWND hWnd = static_cast<HWND>(_windowHandle);
    HMONITOR hMonitor;
    MONITORINFO monInfo;
    LONG nuX, nuY, monW, monH;

    nuX = nuY = 0;
    hMonitor = MonitorFromWindow(hWnd, MONITOR_DEFAULTTONEAREST);
    monInfo.cbSize = sizeof(MONITORINFO);
    if (FALSE == GetMonitorInfo(hMonitor, &monInfo))
        goto resize_do_move;
    wInfo.cbSize = sizeof(WINDOWINFO);
    if (FALSE == GetWindowInfo(hWnd, &wInfo))
        goto resize_do_move;

    // If the window's new H or W is larger than its current monitor's working area,
    // set X or Y (resp.) to 0 .
    // Else If upon resizing part of the window should get offscreen,
    // subtract delta(W or H) from X or Y (resp.) clamped to 0 .

    monW = monInfo.rcWork.right - monInfo.rcWork.left;
    monH = monInfo.rcWork.bottom - monInfo.rcWork.top; 

    auto computeWinOrigin = [](LONG& nuOrig, LONG monDim, int reqWinDim, LONG winOrig, LONG winEnd, LONG monOrig, LONG monEnd)
    {
        if (monDim < reqWinDim)
        {
            nuOrig = 0;
        }
        else
        {
            LONG deltaDim = winEnd - winOrig - reqWinDim;

            nuOrig = (winEnd + deltaDim > monEnd)
                ? winOrig - ((winEnd + deltaDim) - monEnd)
                : winOrig;
        }
    };
    computeWinOrigin(nuX, monW, w, wInfo.rcWindow.left, wInfo.rcWindow.right, monInfo.rcWork.left, monInfo.rcWork.right);
    computeWinOrigin(nuY, monH, h, wInfo.rcWindow.top, wInfo.rcWindow.bottom, monInfo.rcWork.top, monInfo.rcWork.bottom);

resize_do_move:
    MoveWindow(hWnd, nuX, nuY, w, h, (fullscreen == false));
    if (fullscreen)
    {

    }
}

//
// If the window is in fullscreen mode, return the primary monitor's dimensions.
// Else, return the window's main (area-based) monitor's dimensions.
// It doesn't account for a windows's non client area.
//
Vector2i 
Window::maxSize() const
{
    int width, height;

    if (_fullscreen)
    {
        width = GetSystemMetrics(SM_CXFULLSCREEN);
        height = GetSystemMetrics(SM_CYFULLSCREEN);
    }
    else
    {
        HMONITOR hMonitor = MonitorFromWindow(static_cast<HWND>(_windowHandle), MONITOR_DEFAULTTONEAREST);
        MONITORINFO monInfo;

        monInfo.cbSize = sizeof(MONITORINFO);
        if (FALSE == GetMonitorInfo(hMonitor, &monInfo))
        {
            // TODO: proper graphical warning
            std::cerr << "[Lums][Critical] Failed to fetch monitor data !" << std::endl;
            return{ 0, 0 };
        }
        width = monInfo.rcWork.right - monInfo.rcWork.left;
        height = monInfo.rcWork.bottom - monInfo.rcWork.top;
    }

    return {width, height};
}

void   
Window::pumpEvent()
{
    currentWin = this;
    MSG msg;

    while (PeekMessage(&msg, static_cast<HWND>(_windowHandle), 0, 0, PM_REMOVE))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

void   
Window::swap()
{
    HWND hwnd = static_cast<HWND>(_windowHandle);
    HDC dc = GetWindowDC(hwnd);
    SwapBuffers(dc);
    UpdateWindow(hwnd);
    ReleaseDC(hwnd, dc);
}

bool
Window::visible() const
{
// TODO: test
    return IsWindowVisible(static_cast<HWND>(_windowHandle)) != 0;
}

Window::~Window()
{
    DestroyWindow(static_cast<HWND>(_windowHandle));
}
