//
// Created by w on 22-12-14.
//

#ifndef TUTORIALS_APP_BASE_H
#define TUTORIALS_APP_BASE_H

#include <cstdlib>
#include <functional>
#include <iostream>
#include <cstdio>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "shader.hpp"
#include "stb_image.h"
#include "camera.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"


class AppBase {

public:
    AppBase();

    virtual ~AppBase();

    void processInput();

    static void ErrorCallback(int error, const char* description);

    static void MouseButtonCallBack(GLFWwindow* window, int button, int action, int mods);

    static void FramebufferSizeCallBack(GLFWwindow* _window, int width, int height);

    static void CursorPosCallback(GLFWwindow* _window, double xposIn, double yposIn);

    static void ScrollCallback(GLFWwindow* _window, double xoffset, double yoffset);

    void Run();

    virtual void StartUp() = 0;

    virtual void Update() = 0;

    [[nodiscard]] static GLint getWindowWidth();
    [[nodiscard]] static GLint getWindowHeight();

protected:
    static Camera* camera;
    // 鼠标是充当摄像机看向场景
    static bool watchIt;
    static bool firstMouse;
    static float lastX;
    static float lastY;

    glm::vec4 clearColor = glm::vec4(0.45f, 0.55f, 0.60f, 1.00f);

private:
    GLFWwindow* window{};
    static GLint windowWidth;
    static GLint windowHeight;
};


#endif //TUTORIALS_APP_BASE_H
