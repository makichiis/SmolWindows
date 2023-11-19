#include "smol__window_utils.h"
#include "smol_utils.h"


#include <stdio.h>
#include <windef.h>
#include <winuser.h>


SmolSize2D SmolGetWindowSize(SmolWindow* ctx) {
    RECT rect;
    GetWindowRect(ctx->handle, &rect);
    return (SmolSize2D){
        .width = RectResolutionHorizontal(rect),
        .height = RectResolutionVertical(rect)
    };
}

SmolSize2D SmolGetViewportSize(SmolWindow* ctx) {
    RECT rect;
    GetClientRect(ctx->handle, &rect);
    return (SmolSize2D){
        .width = RectResolutionHorizontal(rect),
        .height = RectResolutionVertical(rect)
    };
}

void SmolSetWindowSize(SmolWindow* ctx, int width, int height) {
    perror("Unimplemented.");
}

void SmolSetViewportSize(SmolWindow* ctx, int width, int height) {
    perror("Unimplemented.");
}
