
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

const char* vertexPath = "/home/w/CLionProjects/opengl-demo/playground_hdr_bloom/vertex.glsl";
const char* fragmentPath = "/home/w/CLionProjects/opengl-demo/playground_hdr_bloom/fragment.glsl";
const char* lightVertexPath = "/home/w/CLionProjects/opengl-demo/playground_hdr_bloom/light.vs.glsl";
const char* lightFragmentPath = "/home/w/CLionProjects/opengl-demo/playground_hdr_bloom/light.fs.glsl";
const char* blurVertexPath = "/home/w/CLionProjects/opengl-demo/playground_hdr_bloom/blur.vs.glsl";
const char* blurFragmentPath = "/home/w/CLionProjects/opengl-demo/playground_hdr_bloom/blur.fs.glsl";
const char* bloomVertexPath = "/home/w/CLionProjects/opengl-demo/playground_hdr_bloom/bloom.vs.glsl";
const char* bloomFragmentPath = "/home/w/CLionProjects/opengl-demo/playground_hdr_bloom/bloom.fs.glsl";

const char* wallTexPath = "/home/w/CLionProjects/opengl-demo/resource/textures/wall.jpg";
const char* woodTexPath = "/home/w/CLionProjects/opengl-demo/resource/textures/wood.png";

const int WIDTH = 2560;
const int HEIGHT = 1440;

/**
 *  camera
 */
Camera camera(glm::vec3(0.0f, 3.0f, 15.0f));  /* NOLINT */
float lastX = WIDTH / 2.0f;
float lastY = HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

bool glfwBPressed{};
bool glfwBEnabled{};

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

    if (glfwGetKey(_window, GLFW_KEY_B) == GLFW_PRESS && !glfwBPressed) {
        glfwBEnabled = !glfwBEnabled;
        glfwBPressed = true;
    }
    if (glfwGetKey(_window, GLFW_KEY_B) == GLFW_RELEASE) {
        glfwBPressed = false;
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


GLuint loadTexture(const char* path, bool gammaCorrection) {
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    int width{};
    int height{};
    int channels{};
    unsigned char* data = stbi_load(path, &width, &height, &channels, 0);
    GLint format{};
    GLint internalFormat{};
    if (data) {
        switch (channels) {
            case 1:
                format = GL_RED;
                internalFormat = GL_RED;
                break;
            case 3:
                format = GL_RGB;
                internalFormat = gammaCorrection ? GL_SRGB : GL_RGB;
                break;
            case 4:
                format = GL_RGBA;
                internalFormat = gammaCorrection ? GL_SRGB_ALPHA : GL_RGBA;
                break;
            default:
                break;
        }
        glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    }
    else {
        std::cout << "create texture failed case the data is nullptr" << std::endl;
    }
    stbi_image_free(data);

    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    return texture;
}

void renderCube(GLuint& VAO, GLuint& VBO) {
    if (VAO == 0) {
        const float vertices[] = {
                // back face
                -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
                1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
                1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f, // bottom-right
                1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
                -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
                -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f, // top-left
                // front face
                -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
                1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f, // bottom-right
                1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
                1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
                -1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f, // top-left
                -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
                // left face
                -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
                -1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-left
                -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
                -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
                -1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-right
                -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
                // right face
                1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
                1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
                1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-right
                1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
                1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
                1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-left
                // bottom face
                -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
                1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f, // top-left
                1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
                1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
                -1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f, // bottom-right
                -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
                // top face
                -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
                1.0f,  1.0f , 1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
                1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f, // top-right
                1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
                -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
                -1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f  // bottom-left
        };

        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);
        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), nullptr);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}

// floor
void renderQuad(GLuint& VAO, GLuint& VBO) {
    if (VAO == 0) {
        const float vertices[] = {
                // positions                    // texture Coords
                -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
                -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
                1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
                1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        };
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);
        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), nullptr);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
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
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
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
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    /**
     * ****************************************************
     * INIT
     */
    // container models
    std::vector<glm::mat4> models{};
    {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 1.5f, 0.0));
        model = glm::scale(model, glm::vec3(0.5f));
        models.push_back(model);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(2.0f, 0.0f, 1.0));
        model = glm::scale(model, glm::vec3(0.5f));
        models.push_back(model);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-1.0f, -1.0f, 2.0));
        model = glm::rotate(model, glm::radians(60.0f), glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
        models.push_back(model);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 2.7f, 4.0));
        model = glm::rotate(model, glm::radians(23.0f), glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
        model = glm::scale(model, glm::vec3(1.25));
        models.push_back(model);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-2.0f, 1.0f, -3.0));
        model = glm::rotate(model, glm::radians(124.0f), glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
        models.push_back(model);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-3.0f, 0.0f, 0.0));
        model = glm::scale(model, glm::vec3(0.5f));
        models.push_back(model);
    }

	// Dark blue background
    glEnable(GL_DEPTH_TEST);
    glViewport(0, 0, WIDTH, HEIGHT);
	glClearColor(20 / 255.0f, 20 / 255.0f, 15 / 255.0f, 0.0f);

    // create program and textures
    GLuint program = LoadShaders(vertexPath, fragmentPath);
    GLuint lightProgram = LoadShaders(lightVertexPath, lightFragmentPath);
    GLuint blurProgram = LoadShaders(blurVertexPath, blurFragmentPath);
    GLuint bloomProgram = LoadShaders(bloomVertexPath, bloomFragmentPath);

    GLuint floorTexture = loadTexture(woodTexPath, true);
    GLuint containerTexture = loadTexture(wallTexPath, true);

    /**
     * create floating point framebuffer
     */
    GLuint hdrFBO;
    glGenFramebuffers(1, &hdrFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);
    // color attachment
    GLuint colorTexture[2];
    for (int i = 0; i < 2; i++) {
        glGenTextures(1, &colorTexture[i]);
        glBindTexture(GL_TEXTURE_2D, colorTexture[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F,
                     WIDTH, HEIGHT, 0, GL_RGBA, GL_FLOAT, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // attach buffers
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, colorTexture[i], 0);
    }

    // depth attachment
    GLuint rbo;
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, WIDTH, HEIGHT);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo);

    // tell opengl witch color attachments we'll use
    unsigned int attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
    glDrawBuffers(2, attachments);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cout << "create framebuffer failed" << std::endl;
        exit(EXIT_FAILURE);
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    /**
     * create ping pong framebuffer
     */
    GLuint pingpongFramebuffer[2];
    GLuint pingpongColorBuffer[2];
    glGenFramebuffers(2, pingpongFramebuffer);
    glGenTextures(2, pingpongColorBuffer);
    for (unsigned int i = 0; i < 2; i++) {
        glBindFramebuffer(GL_FRAMEBUFFER, pingpongFramebuffer[i]);
        glBindTexture(GL_TEXTURE_2D, pingpongColorBuffer[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F,
                     WIDTH, HEIGHT, 0, GL_RGBA, GL_FLOAT, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pingpongColorBuffer[i], 0);
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            std::cout << "create pingpong framebuffer failed" << std::endl;
            exit(EXIT_FAILURE);
        }
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);


    // light positions
    std::vector<glm::vec3> lightPositions;
    lightPositions.emplace_back(0.0f, 0.5f,  1.5f); // back light
    lightPositions.emplace_back(-4.0f, 0.5f, -3.0f);
    lightPositions.emplace_back(3.0f, 0.5f,  1.0f);
    lightPositions.emplace_back(-0.8f,  2.4f, -1.0f);
    // colors
    std::vector<glm::vec3> lightColors;
    lightColors.emplace_back(5.0f,   5.0f,  5.0f);
    lightColors.emplace_back(10.0f,  0.0f,  0.0f);
    lightColors.emplace_back(0.0f,   0.0f,  15.0f);
    lightColors.emplace_back(0.0f,   5.0f,  0.0f);


    glUseProgram(program);
    SetUniform(program, "diffuseTexture", 0);
    glUseProgram(blurProgram);
    SetUniform(blurProgram, "image", 0);
    glUseProgram(bloomProgram);
    SetUniform(bloomProgram, "colorBuffer", 0);
    SetUniform(bloomProgram, "bloomLight", 1);

    GLuint cubeVAO{};
    GLuint cubeVBO{};
    GLuint blurVAO{};
    GLuint blurVBO{};
    while (glfwWindowShouldClose(window) == 0) {
        auto currentTime = (float)glfwGetTime();
        deltaTime = currentTime - lastFrame;
        lastFrame = currentTime;
        processInput(window);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // render scene to floating framebuffer
        glm::mat4 projection = glm::perspective(glm::radians(camera.zoom), WIDTH / (float)HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.getViewMatrix();
        glm::mat4 model(1.0f);
        glm::mat3 normalMat(1.0f);

        glUseProgram(program);
        SetUniform(program, "projection", projection);
        SetUniform(program, "view", view);
        for (unsigned int i = 0; i < lightPositions.size(); i++) {
            SetUniform(program, std::string("lightPos[" + std::to_string(i) + "]").c_str(), lightPositions[i]);
            SetUniform(program, std::string("lightColors[" + std::to_string(i) + "]").c_str(), lightColors[i]);
        }
        SetUniform(program, "viewPos", camera.position);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, floorTexture);
        // render floor
        model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(12.5f, 0.2f, 12.5f));
        normalMat = glm::transpose(glm::inverse(glm::mat3(model)));
        SetUniform(program,"model", model);
        SetUniform(program, "normalMat", normalMat);
        renderCube(cubeVAO, cubeVBO);

        // render containers
        glBindTexture(GL_TEXTURE_2D, containerTexture);
        for (auto mat : models) {
            normalMat = glm::transpose(glm::inverse(glm::mat3(mat)));
            SetUniform(program,"model", mat);
            SetUniform(program, "normalMat", normalMat);
            renderCube(cubeVAO, cubeVBO);
        }

        // render light
        glUseProgram(lightProgram);
        SetUniform(lightProgram, "projection", projection);
        SetUniform(lightProgram, "view", view);
        for (unsigned int i = 0; i < lightPositions.size(); i++) {
            model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(lightPositions[i]));
            model = glm::scale(model, glm::vec3(0.25f));
            SetUniform(lightProgram, "model", model);
            SetUniform(lightProgram, "lightColor", lightColors[i]);
            renderCube(cubeVAO, cubeVBO);
        }
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        /**
         * use generated brightColor buffer to blur
         */
        glUseProgram(blurProgram);
        bool horizontal = true;
        bool isFirstIteration = true;
        int amount = 20;
        for (int i = 0; i < amount; i++) {
            glBindFramebuffer(GL_FRAMEBUFFER, pingpongFramebuffer[horizontal]);
            glBindTexture(GL_TEXTURE_2D, isFirstIteration ? colorTexture[1] : pingpongColorBuffer[!horizontal]);
            SetUniform(blurProgram, "horizontal", horizontal);
            renderQuad(blurVAO, blurVBO);
            horizontal = !horizontal;
            if (isFirstIteration) {
                isFirstIteration = false;
            }
        }
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        /**
         * use
         */
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
        glUseProgram(bloomProgram);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, colorTexture[0]);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, pingpongColorBuffer[!horizontal]);
        SetUniform(bloomProgram, "enableBloom", glfwBEnabled);
        renderQuad(blurVAO, blurVBO);

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}


    glDeleteBuffers(1, &cubeVBO);
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteBuffers(1, &blurVBO);
    glDeleteVertexArrays(1, &blurVAO);
    glDeleteProgram(program);


	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}













