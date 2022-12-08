
#include <iostream>
#include <cstdio>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../common/shader.hpp"
#include "../common/stb_image.h"
#include "../common/camera.h"


GLFWwindow* window;

const char* vertexPath = "/home/w/CLionProjects/opengl-demo/opg_1.3_program_pipeline/vertex.glsl";
const char* fragmentPath = "/home/w/CLionProjects/opengl-demo/opg_1.3_program_pipeline/fragment.glsl";
const char* fragment2Path = "/home/w/CLionProjects/opengl-demo/opg_1.3_program_pipeline/fragment2.glsl";

const int WIDTH = 1920;
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

    // triangle vertices
    const float vertices[] = {
            // positions                  //  texture coord
            -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,// Triangle 1
            0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
            0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f
    };

    /**
     * ****************************************************
     * INIT
     */
	// Dark blue background
    glEnable(GL_DEPTH_TEST);
    glViewport(0, 0, WIDTH, HEIGHT);
	glClearColor(12 / 255.0f, 25 / 255.0f, 21 / 255.0f, 0.0f);

    // create vao and vbo
    GLuint VAO;
    GLuint VBO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)nullptr);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glBindVertexArray(0);

    //GLuint program = LoadShaders(vertexPath, fragmentPath);
    // create separable shader program
    std::string vertexShaderCode = LoadShaderCode(vertexPath);
    std::string fragmentShaderCode1 = LoadShaderCode(fragmentPath);
    std::string fragmentShaderCode2 = LoadShaderCode(fragment2Path);
    const char* code[] = {
            vertexShaderCode.c_str(),
            fragmentShaderCode1.c_str(),
            fragmentShaderCode2.c_str()
    };

    GLuint program[3]{};
    program[0] = glCreateShaderProgramv(GL_VERTEX_SHADER, 1, code);
    CheckProgramLinkStatus(program[0]);
    program[1] = glCreateShaderProgramv(GL_FRAGMENT_SHADER, 1, code + 1);
    CheckProgramLinkStatus(program[1]);
    program[2] = glCreateShaderProgramv(GL_FRAGMENT_SHADER, 1, code + 2);
    CheckProgramLinkStatus(program[2]);

    // then create two program pipelines
    GLuint pipeline[2];
    glCreateProgramPipelines(2, pipeline);
    glUseProgramStages(pipeline[0], GL_VERTEX_SHADER_BIT, program[0]);
    glUseProgramStages(pipeline[0], GL_FRAGMENT_SHADER_BIT, program[1]);
    // second
    glUseProgramStages(pipeline[1], GL_VERTEX_SHADER_BIT, program[0]);
    glUseProgramStages(pipeline[1], GL_FRAGMENT_SHADER_BIT, program[2]);

    //

    while (glfwWindowShouldClose(window) == 0) {
        auto currentTime = (float)glfwGetTime();
        deltaTime = currentTime - lastFrame;
        lastFrame = currentTime;

        processInput(window);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        /**
         * mvp  matrix
         */
        glm::mat4 projection = glm::perspective(glm::radians(camera.zoom), (float)((WIDTH / 2.0) / (float)HEIGHT), 0.1f, 100.0f);
        glm::mat4 view = camera.getViewMatrix();
        glm::mat4 model(1.0f);
        glm::vec3 colorMask(0.2f, 1.0f, 0.1f);

        glUseProgram(0);

        SetUniform(program[0], "projection", projection);
        SetUniform(program[0], "view", view);
        SetUniform(program[0], "model", model);
        SetUniform(program[0], "colorMask", colorMask);

        glBindVertexArray(VAO);

        // draw first pipeline on the left
        glViewport(0, 0, WIDTH / 2, HEIGHT);
        glBindProgramPipeline(pipeline[0]);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // draw second pipeline on the right
        glViewport(WIDTH / 2, 0, WIDTH / 2, HEIGHT);
        glBindProgramPipeline(pipeline[1]);
        glDrawArrays(GL_TRIANGLES, 0, 3);

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}


    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}













