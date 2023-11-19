#ifndef __SMOLINE_UTILS_H__
#define __SMOLINE_UTILS_H__

#include "smol.h"

struct SmolSize2D {
    int width;
    int height;
};

typedef struct SmolSize2D SmolSize2D;

/**
 * @brief Retrieves the size of the window. This is not equal to the size of the viewport.
 To get the size of the GL viewport, use `SmolGetViewportSize(WndCtx*)`.
 * 
 * @return SmolSize2D The width and height of the window.
 */
SmolSize2D SmolGetWindowSize(SmolWindow*);

/**
 * @brief Retrieves the size of the GL viewport.
 * 
 * @return SmolSize2D The width and height of the viewport.
 */
SmolSize2D SmolGetViewportSize(SmolWindow*);

/**
 * @brief Sets the dimensions of the window to `width, height`. The size of the internal GL
 viewport will be slightly smaller than this. If you want to set the side othe GL viewport
 specifically, use `SmolSetViewportSize(WndCtx*, int width, int height)`.
 * 
 * @param ctx The window context.
 * @param width The width of the new window size.
 * @param height The height of the new window size.
 */
void SmolSetWindowSize(SmolWindow* ctx, int width, int height);

/**
 * @brief Sets the dimensions of the window to `width, height` without changing the dimensions
 of the viewport.
 * 
 * @param ctx The window context.
 * @param width The width of the new window size.
 * @param height The height of the new window size.
 */
void SmolSetWindowSizeNoGLAdjust(SmolWindow* ctx, int width, int height);

/**
 * @brief Sets the dimensions of the internal GL viewport to `width, height`. The size of
 the encompassing window will scale and be slightly larger than the viewport. If you do not
 want this functionality, call `glViewport(0, 0, width, height)` instead. This will force the
 `ctx` to be the current context.
 * 
 * @param ctx The window context.
 * @param width The width of the new viewport size.
 * @param height The height of the new viewport size.
 */
void SmolSetViewportSize(SmolWindow* ctx, int width, int height);

/**
 * @brief Sets the dimensions of the internal GL viewprot to `width, height` without adjusting
 the size of the encompassing window. This is semantically equivalent to `glViewport(0, 0, width, height)`.
 This will force the `ctx` to be the current context.
 * 
 * @param ctx The window context.
 * @param width The width of the new viewport size.
 * @param height The height of the new viewport size.
 */
void SmolSetViewportSizeNoWindowAdjust(SmolWindow* ctx, int width, int height);

#endif