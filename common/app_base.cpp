//
// Created by w on 22-12-14.
//

#include "app_base.h"

Camera* AppBase::camera = new Camera();

GLint AppBase::windowWidth = 1920;
GLint AppBase::windowHeight = 1080;

bool AppBase::watchIt = false;
bool AppBase::firstMouse = true;
float AppBase::lastX = static_cast<float>(windowWidth) / 2.0f;
float AppBase::lastY = static_cast<float>(windowHeight) / 2.0f;

AppBase::AppBase() {
    glfwSetErrorCallback(ErrorCallback);

    // initialize glfw
    if (!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW\n");
        std::exit(1);
    }

    const char *glsl_version = "#version 460";

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_RESIZABLE,GL_FALSE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create window with graphics context
    window = glfwCreateWindow(windowWidth, windowHeight, "Default App", nullptr, nullptr);
    if (window == nullptr) {
        fprintf(stderr,
                "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
        std::exit(1);
    }

    glfwSetWindowSize(window, windowWidth, windowHeight);
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    // Add window based callbacks to the underlying app
    glfwSetMouseButtonCallback(window, MouseButtonCallBack);
    glfwSetCursorPosCallback(window, CursorPosCallback);
    glfwSetFramebufferSizeCallback(window, FramebufferSizeCallBack);
    glfwSetScrollCallback(window, ScrollCallback);

    // Initialize GLEW
    glewExperimental = true;
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        std::exit(1);
    }

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    {
    // Setup Dear ImGui style
//        ImVec4* colors                         = ImGui::GetStyle().Colors;
//        colors[ImGuiCol_Text]                  = ImVec4(252.f / 255.f, 224.f / 255.f, 176.f / 255.f, 1.f);
//        colors[ImGuiCol_TextDisabled]          = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
//        colors[ImGuiCol_WindowBg]              = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
//        colors[ImGuiCol_ChildBg]               = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
//        colors[ImGuiCol_PopupBg]               = ImVec4(0.19f, 0.19f, 0.19f, 0.92f);
//        colors[ImGuiCol_Border]                = ImVec4(0.19f, 0.19f, 0.19f, 0.29f);
//        colors[ImGuiCol_BorderShadow]          = ImVec4(0.00f, 0.00f, 0.00f, 0.24f);
//        colors[ImGuiCol_FrameBg]               = ImVec4(0.05f, 0.05f, 0.05f, 0.54f);
//        colors[ImGuiCol_FrameBgHovered]        = ImVec4(0.19f, 0.19f, 0.19f, 0.54f);
//        colors[ImGuiCol_FrameBgActive]         = ImVec4(0.20f, 0.22f, 0.23f, 1.00f);
//        colors[ImGuiCol_TitleBg]               = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
//        colors[ImGuiCol_TitleBgActive]         = ImVec4(0.06f, 0.06f, 0.06f, 1.00f);
//        colors[ImGuiCol_TitleBgCollapsed]      = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
//        colors[ImGuiCol_MenuBarBg]             = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
//        colors[ImGuiCol_ScrollbarBg]           = ImVec4(0.05f, 0.05f, 0.05f, 0.54f);
//        colors[ImGuiCol_ScrollbarGrab]         = ImVec4(0.34f, 0.34f, 0.34f, 0.54f);
//        colors[ImGuiCol_ScrollbarGrabHovered]  = ImVec4(0.40f, 0.40f, 0.40f, 0.54f);
//        colors[ImGuiCol_ScrollbarGrabActive]   = ImVec4(0.56f, 0.56f, 0.56f, 0.54f);
//        colors[ImGuiCol_CheckMark]             = ImVec4(0.33f, 0.67f, 0.86f, 1.00f);
//        colors[ImGuiCol_SliderGrab]            = ImVec4(0.34f, 0.34f, 0.34f, 0.54f);
//        colors[ImGuiCol_SliderGrabActive]      = ImVec4(0.56f, 0.56f, 0.56f, 0.54f);
//        colors[ImGuiCol_Button]                = ImVec4(0.15f, 0.05f, 0.05f, 0.54f);
//        colors[ImGuiCol_ButtonHovered]         = ImVec4(0.19f, 0.19f, 0.19f, 0.54f);
//        colors[ImGuiCol_ButtonActive]          = ImVec4(0.20f, 0.22f, 0.23f, 1.00f);
//        colors[ImGuiCol_Header]                = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
//        colors[ImGuiCol_HeaderHovered]         = ImVec4(0.00f, 0.00f, 0.00f, 0.36f);
//        colors[ImGuiCol_HeaderActive]          = ImVec4(0.20f, 0.22f, 0.23f, 0.33f);
//        colors[ImGuiCol_Separator]             = ImVec4(0.28f, 0.28f, 0.28f, 0.29f);
//        colors[ImGuiCol_SeparatorHovered]      = ImVec4(0.44f, 0.44f, 0.44f, 0.29f);
//        colors[ImGuiCol_SeparatorActive]       = ImVec4(0.40f, 0.44f, 0.47f, 1.00f);
//        colors[ImGuiCol_ResizeGrip]            = ImVec4(0.28f, 0.28f, 0.28f, 0.29f);
//        colors[ImGuiCol_ResizeGripHovered]     = ImVec4(0.44f, 0.44f, 0.44f, 0.29f);
//        colors[ImGuiCol_ResizeGripActive]      = ImVec4(0.40f, 0.44f, 0.47f, 1.00f);
//        colors[ImGuiCol_Tab]                   = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
//        colors[ImGuiCol_TabHovered]            = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
//        colors[ImGuiCol_TabActive]             = ImVec4(0.20f, 0.20f, 0.20f, 0.36f);
//        colors[ImGuiCol_TabUnfocused]          = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
//        colors[ImGuiCol_TabUnfocusedActive]    = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
//        colors[ImGuiCol_PlotLines]             = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
//        colors[ImGuiCol_PlotLinesHovered]      = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
//        colors[ImGuiCol_PlotHistogram]         = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
//        colors[ImGuiCol_PlotHistogramHovered]  = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
//        colors[ImGuiCol_TableHeaderBg]         = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
//        colors[ImGuiCol_TableBorderStrong]     = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
//        colors[ImGuiCol_TableBorderLight]      = ImVec4(0.28f, 0.28f, 0.28f, 0.29f);
//        colors[ImGuiCol_TableRowBg]            = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
//        colors[ImGuiCol_TableRowBgAlt]         = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
//        colors[ImGuiCol_TextSelectedBg]        = ImVec4(0.20f, 0.22f, 0.23f, 1.00f);
//        colors[ImGuiCol_DragDropTarget]        = ImVec4(0.33f, 0.67f, 0.86f, 1.00f);
//        colors[ImGuiCol_NavHighlight]          = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
//        colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 0.00f, 0.00f, 0.70f);
//        colors[ImGuiCol_NavWindowingDimBg]     = ImVec4(1.00f, 0.00f, 0.00f, 0.20f);
//        colors[ImGuiCol_ModalWindowDimBg]      = ImVec4(1.00f, 0.00f, 0.00f, 0.35f);
//
//        ImGuiStyle& style       = ImGui::GetStyle();
//        style.WindowPadding     = ImVec2(8.00f, 8.00f);
//        style.FramePadding      = ImVec2(5.00f, 2.00f);
//        style.CellPadding       = ImVec2(6.00f, 6.00f);
//        style.ItemSpacing       = ImVec2(6.00f, 6.00f);
//        style.ItemInnerSpacing  = ImVec2(6.00f, 6.00f);
//        style.TouchExtraPadding = ImVec2(0.00f, 0.00f);
//        style.IndentSpacing     = 25;
//        style.ScrollbarSize     = 15;
//        style.GrabMinSize       = 10;
//        style.WindowBorderSize  = 1;
//        style.ChildBorderSize   = 1;
//        style.PopupBorderSize   = 1;
//        style.FrameBorderSize   = 1;
//        style.TabBorderSize     = 1;
//        style.WindowRounding    = 7;
//        style.ChildRounding     = 4;
//        style.FrameRounding     = 3;
//        style.PopupRounding     = 4;
//        style.ScrollbarRounding = 9;
//        style.GrabRounding      = 3;
//        style.LogSliderDeadzone = 4;
//        style.TabRounding       = 4;
}

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Add custom fonts
    ImGuiIO& io = ImGui::GetIO();
    io.Fonts->AddFontFromFileTTF("../resource/fonts/Roboto-Medium.ttf", 18.0f);
}

AppBase::~AppBase() {
    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    delete camera;
}

void AppBase::MouseButtonCallBack(GLFWwindow* window, int button, int action, int mods) {
    if (GLFW_MOUSE_BUTTON_RIGHT == button && action == GLFW_PRESS) {
        if (!watchIt) {
            watchIt = true;
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            glfwSetCursorPos(window, static_cast<double>(lastX), static_cast<double>(lastY));
        }
        else {
            watchIt = false;
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
    }
}

void AppBase::processInput() {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
    if (watchIt) {
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
            camera->processKeyboard(FORWARD, ImGui::GetIO().DeltaTime);
        }
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
            camera->processKeyboard(LEFT, ImGui::GetIO().DeltaTime);
        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
            camera->processKeyboard(BACKWARD, ImGui::GetIO().DeltaTime);
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
            camera->processKeyboard(RIGHT, ImGui::GetIO().DeltaTime);
        }
    }
}

void AppBase::ErrorCallback(int error, const char* description) {
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}


void AppBase::ScrollCallback([[maybe_unused]] GLFWwindow* _window, double xoffset, double yoffset) {
    if (watchIt) {
        camera->processMouseScroll(static_cast<float>(xoffset), static_cast<float>(yoffset));
    }
}

void AppBase::FramebufferSizeCallBack([[maybe_unused]] GLFWwindow* _window, int width, int height) {
    if (watchIt) {
        windowWidth = width;
        windowHeight = height;
        glViewport(0, 0, windowWidth, windowHeight);
    }
}

void AppBase::CursorPosCallback([[maybe_unused]] GLFWwindow* _window, double xposIn, double yposIn) {
    if (watchIt) {
        auto xpos = static_cast<float>(xposIn);
        auto ypos = static_cast<float>(yposIn);

        if (firstMouse) {
            lastX = xpos;
            lastY = ypos;
            firstMouse = false;
        }

        float xoffset = xpos - lastX;
        float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

        lastX = xpos;
        lastY = ypos;

        camera->processMouseMovement(xoffset, yoffset);
    }
}


void AppBase::Run() {
    // Initialize the underlying app
    StartUp();

    while (!glfwWindowShouldClose(window)) {
        // Poll events like key presses, mouse movements etc.
        glfwPollEvents();
        processInput();

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Main loop of the underlying app
        Update();

        // Rendering imgui
        ImGui::Render();
        int displayW, displayH;
        glfwGetFramebufferSize(window, &displayW, &displayH);
        glViewport(0, 0, displayW, displayH);
        glClearColor(clearColor.x * clearColor.w, clearColor.y * clearColor.w, clearColor.z * clearColor.w, clearColor.w);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }
}


[[nodiscard]] GLint AppBase::getWindowHeight() {
    return windowHeight;
}

[[nodiscard]] GLint AppBase::getWindowWidth() {
    return windowWidth;
}

