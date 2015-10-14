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
#include "Capabilities.hpp"

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

static void
advertiseDisplaySettingsError(LONG err, const char** name = nullptr)
{
    const char* reason;

    switch (err)
    {
    case DISP_CHANGE_BADDUALVIEW:
        reason = "DISP_CHANGE_BADDUALVIEW";
        break;
    case DISP_CHANGE_BADFLAGS:
        reason = "DISP_CHANGE_BADFLAGS";
        break;
    case DISP_CHANGE_BADMODE:
        reason = "DISP_CHANGE_BADMODE";
        break;
    case DISP_CHANGE_BADPARAM:
        reason = "DISP_CHANGE_BADPARAM";
        break;
    case DISP_CHANGE_FAILED:
        reason = "DISP_CHANGE_FAILED";
        break;
    case DISP_CHANGE_NOTUPDATED:
        reason = "DISP_CHANGE_NOTUPDATED";
        break;
    case DISP_CHANGE_RESTART:
        reason = "DISP_CHANGE_RESTART";
        break;
    case DISP_CHANGE_SUCCESSFUL:
        reason = "DISP_CHANGE_RESTART";
        break;
    default:
        reason = "Unknown :(";
    }

    if (name)
        *name = reason;
    else
        MessageBox(nullptr, reason, "ChangeDisplaySettings failed !", MB_OK);
}

// Create a dummy window & context to load OpenGL pointers.
static void
loadGlApi()
{
    HDC dc;
    int pf;
    HGLRC gl;
    HINSTANCE hInstance = GetModuleHandle(NULL);
    HWND hwnd = CreateWindow("LMWindow", "Initializing Lums...", 0, 0, 0, 4, 4, nullptr, nullptr, hInstance, nullptr);
    int loaderResult;

    if (!hwnd)
    {
        MessageBox(NULL, "Window creation failed", "Lums Error", MB_OK | MB_ICONERROR);
        exit(EXIT_FAILURE);
    }
    dc = GetWindowDC(hwnd);
    pf = ChoosePixelFormat(dc, &pfd);
    SetPixelFormat(dc, pf, &pfd);
    gl = wglCreateContext(dc);
    wglMakeCurrent(dc, gl);
    ReleaseDC(hwnd, dc);

    loaderResult = ogl_LoadFunctions();
    if (loaderResult == ogl_LOAD_FAILED)
    {
        MessageBox(nullptr, "Lums failed to load OpenGL functions. Are we missing a context ?", "Lums Error", MB_OK | MB_ICONERROR);
        exit(EXIT_FAILURE);
    }
	wglMakeCurrent(nullptr, nullptr);
    wglDeleteContext(gl);
	DestroyWindow(hwnd);
}

static void protectValues() {}
template<typename T, typename ...Ts> static void
protectValues(T& n, Ts& ...Args)
{
    // A 0 height could lead to dividing by 0 in 3rd party code,
    // while Windows can BSOD upon a mere ChangeDisplaySettings call with unsupported display modes
	// 1 is an unreasonnable value but won't cause arithmetic errors.
    if (n <= static_cast<T>(0))
        n = static_cast<T>(1);
    protectValues(Args...);
}

Window::Window(int w, int h, const char* name, bool fullscreen)
	: _fullscreen(false)
    , _fbo()
    , _texBuffer()
{
	DWORD dwStyle, dwExStyle;
    HINSTANCE hInstance = GetModuleHandle(nullptr);
    RECT winRekt;
    static bool launched = false;

    protectValues(w, h);
    if (!launched)
    {
        WNDCLASSEX wc;

        wc.cbSize = sizeof(WNDCLASSEX);
        wc.style = CS_NOCLOSE | CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
        wc.lpfnWndProc = LMWindowProc;
        wc.cbClsExtra = 0;
        wc.cbWndExtra = 0;
        wc.hInstance = hInstance;
        wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
        wc.hCursor = LoadCursor(NULL, IDC_ARROW);
        wc.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1);
        wc.lpszMenuName = NULL;
        wc.lpszClassName = "LMWindow";
        wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
        if (0 == RegisterClassEx(&wc))
        {
            MessageBox(nullptr, "Failed to register window class", "Lums error", MB_OK | MB_ICONERROR);
            exit(EXIT_FAILURE);
        }
        loadGlApi();
        launched = true;
    }

    if (fullscreen)
    {
        dwExStyle = WS_EX_APPWINDOW;
        dwStyle = WS_POPUP;
    }
    else
    {
        dwExStyle = WS_EX_WINDOWEDGE | WS_EX_APPWINDOW;
        dwStyle = WS_OVERLAPPEDWINDOW;
    }

    winRekt.left = 0;
    winRekt.right = w;
    winRekt.top = 0;
    winRekt.bottom = h;
    AdjustWindowRectEx(&winRekt, dwStyle, FALSE, dwExStyle);

    HWND hwnd = CreateWindowEx(
        dwExStyle
        , "LMWindow"
        , name
        , dwStyle | WS_CLIPSIBLINGS | WS_CLIPCHILDREN
        , CW_USEDEFAULT, CW_USEDEFAULT
        , winRekt.right - winRekt.left
        , winRekt.bottom - winRekt.top
        , nullptr, nullptr, hInstance, nullptr);
    _windowHandle = hwnd;
    if (!_windowHandle)
    {
        MessageBoxA(nullptr, "Window creation failed", "Lums fucked up", MB_OK);
        exit(EXIT_FAILURE);
    }


    HDC dc = GetWindowDC(hwnd);
    int pf = ChoosePixelFormat(dc, &pfd);

    SetPixelFormat(dc, pf, &pfd);

    HGLRC gl = wglCreateContext(dc);
    wglMakeCurrent(dc, gl);

    _openGlHandle = gl;

    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glGenFramebuffers(1, &_fbo);
    glGenTextures(2, _texBuffer);
    glGenTextures(1, &_depthBuffer);
    for (int i = 0; i < 2; ++i)
    {
        glBindTexture(GL_TEXTURE_2D, _texBuffer[i]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
    }
    glBindTexture(GL_TEXTURE_2D, _depthBuffer);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);

    ReleaseDC(hwnd, dc);
    resize(w, h, fullscreen);
}


void
Window::resize(int w, int h, bool fullscreen)
{
    HWND hWnd = static_cast<HWND>(_windowHandle);
    // Vector2i maxSz = prospectiveMaxSize(hWnd, fullscreen);
    int res;

    protectValues(w, h);
    if (_fullscreen)
    {
        // If we were already in fullscreen,
        // we need to reset the display in any case.
        if ((res = ChangeDisplaySettings(nullptr, 0)) != DISP_CHANGE_SUCCESSFUL)
        {
            char text[2048];
            const char* reason;

            advertiseDisplaySettingsError(res, &reason);
            snprintf(text, sizeof(text), "Failed to ChangeDisplaySettings: %s", reason);
            MessageBox(hWnd, text, "Lums error", MB_OK | MB_ICONERROR);
            return;
        }
    }
    LONG_PTR lpStyle = WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
    LONG_PTR lpExStyle = 0;
    
    // Set window style, ext. style, cursor visibility
    if (!fullscreen)
    {
        lpExStyle |= WS_EX_WINDOWEDGE | WS_EX_APPWINDOW;
        lpStyle |= WS_OVERLAPPEDWINDOW;
        ShowCursor(TRUE);
    }
    else
    {
        lpExStyle |= WS_EX_APPWINDOW;
        lpStyle |= WS_POPUP;
        ShowCursor(FALSE);
    }

    // Perform fullscreen specifics
    if (fullscreen)
    {
        DEVMODE dmScreenSettings;
        winnt::Capabilities cap;
        winnt::DisplaySetting currentDs, userSupplDs;
        int res;

        dmScreenSettings.dmSize = sizeof(DEVMODE);
        userSupplDs = cap.current(currentDs);
        userSupplDs.width = w;
        userSupplDs.height = h;
        if (cap.exists(userSupplDs))
        {
            // Then use the provided width and height.
            dmScreenSettings.dmPelsWidth = w;
            dmScreenSettings.dmPelsHeight = h;
        }
        else
        {
            // Fallback on current display mode (i.e.: desktop, most likely)
            dmScreenSettings.dmPelsWidth = w = currentDs.width;
            dmScreenSettings.dmPelsHeight = h = currentDs.height;
        }
     
        dmScreenSettings.dmBitsPerPel = currentDs.bpp;
		// Not taking the hassle to set frequency; hope it's auto selected
        dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;
        if ((res = ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN)) != DISP_CHANGE_SUCCESSFUL)
        {
            char text[2048];
            const char* reason;

            advertiseDisplaySettingsError(res, &reason);
            snprintf(text, sizeof(text), "Failed to ChangeDisplaySettings: %s", reason);
            MessageBox(hWnd, text, "Lums error", MB_OK | MB_ICONERROR);
            return;
        }
    }
    _fullscreen = fullscreen;

    if (0 == SetWindowLongPtr(hWnd, GWL_STYLE, lpStyle)
        || 0 == SetWindowLongPtr(hWnd, GWL_EXSTYLE, lpExStyle))
    {
        MessageBox(hWnd, "Failed to set window style", "Lums error", MB_OK | MB_ICONERROR);
        return;
    }

    RECT winRekt = { 0, 0, w, h };
    AdjustWindowRectEx(&winRekt
        , static_cast<DWORD>(lpStyle)
        , FALSE
        , static_cast<DWORD>(lpExStyle)
        );

    // Keep the window onscreen.
    if (winRekt.left < 0)
    {
        winRekt.right -= winRekt.left;
        winRekt.left = 0;
    }
    if (winRekt.top < 0)
    {
        winRekt.bottom -= winRekt.top;
        winRekt.top = 0;
    }
    if (FALSE == SetWindowPos(hWnd, HWND_TOP, winRekt.left, winRekt.top, winRekt.right, winRekt.bottom, SWP_SHOWWINDOW))
    {
        MessageBox(hWnd, "Failed to SetwindowPos", "Lums error", MB_OK | MB_ICONERROR);
        return;
    }

    _size = { w, h };

    glViewport(0, 0, w, h);
    glBindFramebuffer(GL_FRAMEBUFFER, _fbo);
    glBindTexture(GL_TEXTURE_2D, _texBuffer[0]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, w, h, 0, GL_BGRA, GL_UNSIGNED_INT_8_8_8_8_REV, nullptr);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _texBuffer[0], 0);
    glBindTexture(GL_TEXTURE_2D, _texBuffer[1]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, w, h, 0, GL_BGRA, GL_UNSIGNED_INT_8_8_8_8_REV, nullptr);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, _texBuffer[1], 0);
    glBindTexture(GL_TEXTURE_2D, _depthBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, w, h, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_SHORT, nullptr);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, _depthBuffer, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);

    ShowWindow(hWnd, SW_SHOW);
    SetForegroundWindow(hWnd);
    SetFocus(hWnd);
    UpdateWindow(hWnd);
}

static Vector2i
prospectiveMaxSize(const HWND hWnd, bool fullscreen)
{
    int width, height;
    HMONITOR hMonitor = MonitorFromPoint({0, 0}, MONITOR_DEFAULTTOPRIMARY);
    MONITORINFO monInfo;

    monInfo.cbSize = sizeof(MONITORINFO);
    if (FALSE == GetMonitorInfo(hMonitor, &monInfo))
        return{ 1, 1 };
    if (fullscreen)
    {
        width = monInfo.rcMonitor.right - monInfo.rcMonitor.left;
        height = monInfo.rcMonitor.bottom - monInfo.rcMonitor.top;
    }
    else
    {
        width = monInfo.rcWork.right - monInfo.rcWork.left;
        height = monInfo.rcWork.bottom - monInfo.rcWork.top;
    }
    return{ width, height };
}

Vector2i
Window::maxSize() const
{
    return prospectiveMaxSize(static_cast<HWND>(_windowHandle), _fullscreen);
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
    return IsWindowVisible(static_cast<HWND>(_windowHandle)) != 0;
}

Window::~Window()
{
    if (_fullscreen && visible())
        ChangeDisplaySettings(nullptr, 0);
    wglMakeCurrent(nullptr, nullptr);
    wglDeleteContext(static_cast<HGLRC>(_openGlHandle));
    DestroyWindow(static_cast<HWND>(_windowHandle));
}
