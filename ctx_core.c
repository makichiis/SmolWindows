#include "ctx.h"
#include <gl/GL.h>
#include <minwindef.h>
#include <stdio.h>
#include <string.h>
#include <windef.h>
#include <wingdi.h>
#include <winuser.h>

LRESULT CALLBACK Smol__DefaultWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_CLOSE:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

#define WINDOW_HANDLE_CLASS_NAME_PREFIX "SmolContextWindow-"
/// For internal use. In `SmolCreateContext`, memory pointed to by return value is freed before end of function.
static inline char* Smol__GenerateWindowHandleClassName(const char* windowTitle) {
    char* s = calloc(strlen(windowTitle) + sizeof WINDOW_HANDLE_CLASS_NAME_PREFIX, sizeof (char));
    strncpy_s(s, sizeof WINDOW_HANDLE_CLASS_NAME_PREFIX, WINDOW_HANDLE_CLASS_NAME_PREFIX, _TRUNCATE);
    char* offset = s + sizeof WINDOW_HANDLE_CLASS_NAME_PREFIX-1;
    // Thanks, Windows
    strncpy_s(offset, strlen(windowTitle)+1, windowTitle, strlen(windowTitle));
    
    return s;
}

WndCtx* SmolCreateContext(const char* title, int width, int height) {
    char* className = Smol__GenerateWindowHandleClassName(title);

    // Register Window class
    WNDCLASS wc = {};
    wc.lpfnWndProc = Smol__DefaultWndProc;
    wc.hInstance = GetModuleHandle(NULL);
    wc.lpszClassName = className;
    RegisterClass(&wc);

    // Create window
    HWND hwnd = CreateWindowEx(
        0,
        className,
        title,
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        CW_USEDEFAULT, CW_USEDEFAULT, width, height,
        NULL, NULL, GetModuleHandle(NULL), NULL
    );

    // Create and set up opengl context
    HDC hdc = GetDC(hwnd);

    // TODO: Should I make this configurable?
    PIXELFORMATDESCRIPTOR pfd = {
        sizeof (PIXELFORMATDESCRIPTOR),
        1,
        PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
        PFD_TYPE_RGBA,
        32, // color depth
        0, 0, 0, 0, 0, 0,
        0,
        0,
        0,
        0, 0, 0, 0,
        24, // depth buffer
        8,  // stencil buffer
        0,
        PFD_MAIN_PLANE,
        0,
        0, 0, 0
    };
    // I do not know what this does
    int pixelFormat = ChoosePixelFormat(hdc, &pfd);
    SetPixelFormat(hdc, pixelFormat, &pfd);

    HGLRC hglrc = wglCreateContext(hdc);
    //wglMakeCurrent(hdc, hglrc);

    WndCtx* ctx = malloc(sizeof *ctx);
    if (!ctx) return NULL;
    
    *ctx = (WndCtx) {
        .wndClass = wc,
        .wglContext = hglrc,
        .handle = hwnd,
        .m_WindowResizeCallback = Smol__DefaultWindowResizeCallback
    };

    free(className);
    return ctx;
}

void SmolMakeContextCurrent(WndCtx *ctx) {
    wglMakeCurrent(GetDC(ctx->handle), ctx->wglContext);
}

void SmolDestroyContext(WndCtx* ctx) {
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(ctx->wglContext);

    ReleaseDC(ctx->handle, GetDC(ctx->handle));

    free(ctx);
}

bool SmolContextShouldClose(WndCtx *ctx) {
    return !GetMessage(&ctx->m_state__eventmsg, NULL, 0, 0);
}

#define RectEquals(rect1, rect2) (rect1.bottom == rect2.bottom\
                                && rect1.top == rect2.top\
                                && rect1.left == rect2.left\
                                && rect1.right == rect2.right)
static inline void Smol__DetermineClientSizeChangeCallback(WndCtx* ctx) {
    static RECT state_snapshot_clientsize;
    RECT rect;
    GetClientRect(ctx->handle, &rect);
    if (!RectEquals(state_snapshot_clientsize, rect)) {
        ctx->m_WindowResizeCallback(ctx, rect.right - rect.left, rect.bottom - rect.top);
        state_snapshot_clientsize = rect;
    }
}

void SmolHandleEvents(WndCtx *ctx) {
    TranslateMessage(&ctx->m_state__eventmsg);
    DispatchMessage(&ctx->m_state__eventmsg);

    // TODO: We do not want to run every single callback every single frame. Find a way to snapshot the state and compare.
    Smol__DetermineClientSizeChangeCallback(ctx);
}