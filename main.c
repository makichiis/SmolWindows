#include "ctx.h"

#include <gl/GL.h>
#include <windef.h>
#include <winuser.h>

#define NOTE_1 This include takes up like 30kb of space lmao
#include <stdio.h>

void RenderTest();

int main() {
    WndCtx* window = SmolCreateContext("Smol window :3", 800, 600);
    if (!window) {
        perror("Window context could not be opened.");
        return 1;
    }
    SmolMakeContextCurrent(window);

    // main loop
    while (!SmolContextShouldClose(window)) {
        SmolHandleEvents(window);

        RenderTest();
    }

    SmolDestroyContext(window);
}

void RenderTest() {
    glClear(GL_COLOR_BUFFER_BIT);

    glBegin(GL_TRIANGLES);
    glColor3f(1.0f, 0.0f, 0.0f); // Red color
    glVertex2f(0.0f, 1.0f);     // Top vertex
    glColor3f(0.0f, 1.0f, 0.0f); // Green color
    glVertex2f(-1.0f, -1.0f);   // Bottom-left vertex
    glColor3f(0.0f, 0.0f, 1.0f); // Blue color
    glVertex2f(1.0f, -1.0f);    // Bottom-right vertex
    glEnd();

    glFlush();
    SwapBuffers(wglGetCurrentDC());
}