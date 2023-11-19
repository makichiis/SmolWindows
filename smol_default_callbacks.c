#include "smol.h"

#include <gl/GL.h>

void Smol__DefaultClientSizeUpdatedCallback(WndCtx* window, int width, int height) {
    glViewport(0, 0, width, height);
}
