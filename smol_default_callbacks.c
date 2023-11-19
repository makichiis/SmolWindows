#include "smol.h"

#include <gl/GL.h>

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

void Smol__DefaultClientSizeUpdatedCallback(WndCtx* window, int width, int height) {
    glViewport(0, 0, width, height);
}
