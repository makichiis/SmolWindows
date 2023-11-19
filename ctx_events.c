#include "ctx.h"

#include <gl/GL.h>

void Smol__DefaultWindowResizeCallback(WndCtx* window, int width, int height) {
    glViewport(0, 0, width, height);
}
