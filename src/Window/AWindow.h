#pragma once

#include <stdio.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>


class AWindow
{
public:

    AWindow(GLuint windowWidth, GLuint windowHeight, const char* title);
    ~AWindow();

    int Init();

    void PreUpdate();
    void Update();
    void PostUpdate();


    GLuint GetWindowBufferWidth() { return m_windowBufferWidth;}
    GLuint GetWindowBufferHeight() { return m_windowBufferHeight;}

    bool ShouldClose() { return glfwWindowShouldClose(m_mainWindow); }
    void SwapBuffers() { glfwSwapBuffers(m_mainWindow);}

    bool IsKeyPressed(int keyId);
    void GetMouseChange(GLfloat& mousePosChangeX, GLfloat& mousePosChangeY, bool consume);

    void CheckForGLError(const char* msg);



private:
    GLFWwindow* m_mainWindow;

    GLuint m_windowWidth, m_windowHeight;
    int m_windowBufferWidth, m_windowBufferHeight;

    const char* m_title;

    //keyboard input
    bool m_keys[1024];
    //Mouse input
    GLfloat m_mouseLastPosX, m_mouseLastPosY;
    GLfloat m_mousePosChangeX, m_mousePosChangeY;
    bool m_mouseHasDoneInitialPositioning;

    bool m_mouseHasMoved;

    void InitCallbacks();


    // input
    static void HandleKeyboardInput(GLFWwindow* window, int key, int code, int action, int mode);
    static void HandleMouseInput(GLFWwindow* window, double mousePosX, double mousePosY);

};