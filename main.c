#include "smol.h"

#include <gl/GL.h>
#include <gl/GLU.h>
#include <math.h>
#include <minwinbase.h>
#include <synchapi.h>

#define NOTE_1 This include takes up like 30kb of space lmao
#include <stdio.h>

#define PI 3.14159265358979323846

void RenderTest();

void DrawCube() {
    glBegin(GL_QUADS);
    // Front face
    glVertex3f(-0.5, -0.5, 0.5);
    glVertex3f(0.5, -0.5, 0.5);
    glVertex3f(0.5, 0.5, 0.5);
    glVertex3f(-0.5, 0.5, 0.5);

    // Back face
    glVertex3f(-0.5, -0.5, -0.5);
    glVertex3f(0.5, -0.5, -0.5);
    glVertex3f(0.5, 0.5, -0.5);
    glVertex3f(-0.5, 0.5, -0.5);

    // Left face
    glVertex3f(-0.5, -0.5, 0.5);
    glVertex3f(-0.5, 0.5, 0.5);
    glVertex3f(-0.5, 0.5, -0.5);
    glVertex3f(-0.5, -0.5, -0.5);

    // Right face
    glVertex3f(0.5, -0.5, 0.5);
    glVertex3f(0.5, 0.5, 0.5);
    glVertex3f(0.5, 0.5, -0.5);
    glVertex3f(0.5, -0.5, -0.5);

    // Top face
    glVertex3f(-0.5, 0.5, 0.5);
    glVertex3f(0.5, 0.5, 0.5);
    glVertex3f(0.5, 0.5, -0.5);
    glVertex3f(-0.5, 0.5, -0.5);

    // Bottom face
    glVertex3f(-0.5, -0.5, 0.5);
    glVertex3f(0.5, -0.5, 0.5);
    glVertex3f(0.5, -0.5, -0.5);
    glVertex3f(-0.5, -0.5, -0.5);

    glEnd();
}

int main() {
    SmolWindow* window = SmolCreateContext("Smol window :3", 800, 600);
    if (!window) {
        perror("Window context could not be opened.");
        return 1;
    }
    SmolMakeContextCurrent(window);

    glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);

    // main loop
    while (!SmolContextShouldClose(window)) {

        SYSTEMTIME time; GetLocalTime(&time);
        float angle = (float)(time.wSecond * 1000 + time.wMilliseconds) / 1000;
        
        glClear(GL_COLOR_BUFFER_BIT);

        glPushMatrix();
        glRotatef(angle * 50, 1.0f, 0.3f, 0.1f);

        glColor3f(1.0, 1.0, 1.0);
        DrawCube();

        glPopMatrix();

        Sleep(10);
        SmolHandleEvents(window);
        SwapBuffers(wglGetCurrentDC());
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