#ifndef __SMOLINE_CTX_H__
#define __SMOLINE_CTX_H__

#include <Windows.h>

#include <stdbool.h>

typedef WNDCLASS CtxWndClass;
typedef HGLRC CtxWglContext;
typedef HWND CtxWindowHandle;

struct NativeWindowContext;

typedef void (*SmolWindowResizeCallback)(struct NativeWindowContext*, int width, int height);

struct NativeWindowContext {
    CtxWndClass                 wndClass;
    CtxWglContext               wglContext;
    CtxWindowHandle             handle;

    MSG                         m_state__eventmsg;

    RECT                        m_state_clientsize;
    SmolWindowResizeCallback    m_WindowResizeCallback;
};

typedef struct NativeWindowContext WndCtx;


/// Initialize a native window context. Allocates memory. Must be freed with `SmolDestroyContext(WndCtx*)`.

/**
 * @brief Initialize a native window context. Allocates memory, which should be freed when the context
 is no longer necessary using `SmolDestroyContext(WndCtx*)`.
 * 
 * @param windowTitle The title of the window/
 * @param width The width (in pixels) of the window.
 * @param height The height (in pixels) of the window.
 * @return WndCtx* A new window context. Will return NULL if unsuccessful.
 */
WndCtx* SmolCreateContext(const char* windowTitle, int width, int height);


// -- Core context functions 

/**
 * @brief Sets the current GL context to the context managed by `ctx`.
 * 
 * @param ctx The context.
 */
void SmolMakeContextCurrent(WndCtx* ctx);

/**
 * @brief Destroys the context and frees resources.
 * 
 * @param ctx The context to destroy.
 */
void SmolDestroyContext(WndCtx* ctx);


// -- State

/**
 * @brief Returns whether the context should exit. `SmolDestroyContext(WndCtx*)` must be called to free resources.
 * 
 * @param ctx - The context to check against.
 * @return true - The context should close.
 * @return false - The context should not close.
 */
bool SmolContextShouldClose(WndCtx* ctx);

/**
 * @brief Processes internal events necessary for window context to function, as well as primes callbacks. These
 callbacks can be overridden using the various `SmolSetXCallback()` functions.
 * 
 * @param ctx The window context to handle events for.
 */
void SmolHandleEvents(WndCtx* ctx);


// -- Context manager callbacks

/**
 * @brief Specify callback to window resize.
 By default, the OpenGL viewport will be scaled with the client resolution of the window handle
 via a call to `glViewport()`. Overriding this behavior will not preserve viewport scaling.
 * 
 * @param window - The window context `cb` will be attached to.
 * @param cb - The callback. If set to NULL, nothing will happen in this event.
 */
static inline void SmolOnClientSizeUpdated(WndCtx* window, void (*cb)(WndCtx*, int width, int height)) {
    window->m_WindowResizeCallback = cb;
}


// -- Default callbacks

/// For internal use only. The default base window process callback.
LRESULT CALLBACK Smol__DefaultWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
/// For internal user only. The default window resize callback, though it can be overriden via `SmolSetWindowResizeCallback()`.
void Smol__DefaultClientSizeCallback(WndCtx* window, int width, int height);

#endif