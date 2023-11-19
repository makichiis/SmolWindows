#include "smol.h"

#include <gl/GL.h>

#include <stdio.h>
#include <string.h>
#include <winuser.h>

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

WndCtx* SmolCreateContext(const char* windowTitle, int width, int height) {
    char* className = Smol__GenerateWindowHandleClassName(windowTitle);

    // Register Window class
    WNDCLASS wc = {};
    wc.lpfnWndProc = Smol__DefaultWndProc; // DEFAULT WND PROC WAS REPLACED HERE
    wc.hInstance = GetModuleHandle(NULL);
    wc.lpszClassName = className;
    RegisterClass(&wc);

    // Create window
    HWND hwnd = CreateWindowEx(
        0,
        className,
        windowTitle,
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
        .mState__TimerID = SetTimer(hwnd, 1, 16, NULL),
        .mClientSizeUpdatedCallback = Smol__DefaultClientSizeUpdatedCallback
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
