#include "smol__window_utils.h"
#include "smol.h"

bool SmolContextShouldClose(SmolWindow *ctx) {
    return !GetMessage(&ctx->mWindows__EventMsg, NULL, 0, 0);
}

/// Dev note: Determines whether to call the OnClientSizeUpdated event callback.
static inline void Smol__CheckOnClientSizeUpdated(SmolWindow* ctx) {
    RECT rect;
    GetClientRect(ctx->handle, &rect);
    if (!RectResolutionEquals(ctx->mState__ClientSize, rect)) {
        ctx->mOnClientSizeUpdated(ctx,
            RectResolutionHorizontal(rect),
            RectResolutionVertical(rect));
        ctx->mState__ClientSize = rect;
    }
}

void SmolHandleEvents(SmolWindow *ctx) {
    TranslateMessage(&ctx->mWindows__EventMsg);
    DispatchMessage(&ctx->mWindows__EventMsg);

    if (ctx->mOnClientSizeUpdated) Smol__CheckOnClientSizeUpdated(ctx);
}

/// Must remain at bottom of file.
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