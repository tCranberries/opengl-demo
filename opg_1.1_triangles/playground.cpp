
#include <iostream>
#include <cmath>
#include <cstdio>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../common/shader.hpp"
#include "../common/stb_image.h"
#include "../common/camera.h"


GLFWwindow* window;

const char* vertexPath = "/home/w/CLionProjects/opengl-demo/opg_1.1_triangles/vertex.glsl";
const char* fragmentPath = "/home/w/CLionProjects/opengl-demo/opg_1.1_triangles/fragment.glsl";

const int WIDTH = 1080;
const int HEIGHT = 1080;


/**
 *  camera
 */
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));  /* NOLINT */
float lastX = WIDTH / 2.0f;
float lastY = HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;


void processInput(GLFWwindow* _window) {
    if (glfwGetKey(_window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(_window, true);
    }
    if (glfwGetKey(_window, GLFW_KEY_W) == GLFW_PRESS) {
        camera.processKeyboard(FORWARD, deltaTime);
    }
    if (glfwGetKey(_window, GLFW_KEY_A) == GLFW_PRESS) {
        camera.processKeyboard(LEFT, deltaTime);
    }
    if (glfwGetKey(_window, GLFW_KEY_S) == GLFW_PRESS) {
        camera.processKeyboard(BACKWARD, deltaTime);
    }
    if (glfwGetKey(_window, GLFW_KEY_D) == GLFW_PRESS) {
        camera.processKeyboard(RIGHT, deltaTime);
    }
}


void framebufferSizeCallBack(GLFWwindow* _window, int width, int height) {
    glViewport(0, 0, width, height);
}

void mouseCallback(GLFWwindow* _window, double xposIn, double yposIn) {
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

    camera.processMouseMovement(xoffset, yoffset);
}

void scrollCallback(GLFWwindow* _window, double xoffset, double yoffset) {
    camera.processMouseScroll(static_cast<float>(xoffset), static_cast<float>(yoffset));
}

// generate circle vertices
void drawCircle(glm::vec3 center, GLfloat radius, GLint numbersOfSides, std::vector<GLfloat>& vertices) {
    vertices.push_back(center.x);
    vertices.push_back(center.y);
    vertices.push_back(center.z);
    for (int i = 0; i <= numbersOfSides; i++) {
        vertices.push_back(center.x + radius * (float)std::cos((float)i * 2.0f * M_PI / numbersOfSides));
        vertices.push_back(center.y + radius * (float)std::sin((float)i * 2.0f * M_PI / numbersOfSides));
        vertices.push_back(center.z);
    }
}

int main() {
	// Initialise GLFW
	if (!glfwInit()) {
		fprintf(stderr, "Failed to initialize GLFW\n");
		getchar();
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_RESIZABLE,GL_FALSE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Open a window and create its OpenGL context
	window = glfwCreateWindow( WIDTH, HEIGHT, "Playground", nullptr, nullptr);
	if (window == nullptr) {
		fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
		getchar();
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallBack);
    glfwSetCursorPosCallback(window, mouseCallback);
    glfwSetScrollCallback(window, scrollCallback);

	// Initialize GLEW
    glewExperimental = true;
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return -1;
	}

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    // get version and vendor info
    const unsigned char* vendor = glGetString(GL_VENDOR);
    const unsigned char* render = glGetString(GL_RENDERER);
    const unsigned char* version = glGetString(GL_VERSION);
    const unsigned char* shaderVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);

    int major{};
    int minor{};
    glGetIntegerv(GL_MAJOR_VERSION, &major);
    glGetIntegerv(GL_MINOR_VERSION, &minor);


    int binaryFormats{};
    glGetIntegerv(GL_NUM_PROGRAM_BINARY_FORMATS, &binaryFormats);


    // triangle vertices
    const float vertices[] = {
            -0.5f, -0.5f, 1.0, 0.0, 0.0, // Triangle 1
            0.5f, -0.5f, 0.0, 1.0, 0.0,
            0.0f, 0.5f, 0.0, 0.0, 1.0
    };

    /**
     * ****************************************************
     * INIT
     */
	// Dark blue background
    glEnable(GL_DEPTH_TEST);
    glViewport(0, 0, WIDTH, HEIGHT);
	glClearColor(127 / 255.0f, 255 / 255.0f, 212 / 255.0f, 0.0f);


    std::vector<GLfloat> circleVertices;
    drawCircle(glm::vec3(0, 0, 0), 1, 200, circleVertices);


    GLuint VAO;
    GLuint VBO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, (long)(circleVertices.size() * sizeof(GLfloat)), &circleVertices[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)nullptr);
    glBindVertexArray(0);

    GLuint program = LoadShaders(vertexPath, fragmentPath);

    /**
     *
     * can list of active attributes  opengl 4.3 or later
     */
    int numAttribs{};
    glGetProgramInterfaceiv(program, GL_PROGRAM_INPUT, GL_ACTIVE_RESOURCES, &numAttribs);
    GLenum properties[] = {GL_NAME_LENGTH, GL_TYPE, GL_LOCATION};
    for (int i = 0; i < numAttribs; i++) {
        int result[3];
        glGetProgramResourceiv(program, GL_PROGRAM_INPUT,
                               i,
                               3,
                               properties,
                               3,
                               nullptr,
                               result
                               );
        int nameBufferSize = result[0] + 1;
        char* name = new char[nameBufferSize];
        glGetProgramResourceName(program, GL_PROGRAM_INPUT,
                                 i,
                                 nameBufferSize,
                                 nullptr,
                                 name);
        std::cout << result[2] << "   " << name << "   " << result[1] << std::endl;
        delete[] name;
    }



    while (glfwWindowShouldClose(window) == 0) {
        auto currentTime = (float)glfwGetTime();
        deltaTime = currentTime - lastFrame;
        lastFrame = currentTime;

        processInput(window);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        /**
         * mvp  matrix
         */
        glm::mat4 projection = glm::perspective(glm::radians(camera.zoom), WIDTH / (float)HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.getViewMatrix();
        glm::mat4 model(1.0f);
        glUseProgram(program);
//        SetUniform(program, "projection", projection);
//        SetUniform(program, "view", view);



        SetUniform(program, "model", model);

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLE_FAN, 0, (GLint)circleVertices.size());

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}


    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
    glDeleteProgram(program);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}













