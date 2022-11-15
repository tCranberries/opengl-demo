
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

const char* vertexPath = "/home/wzw/documents/ogl-master/playground_shadow_mapping_point_light/vertex.glsl";
const char* geometryPath = "/home/wzw/documents/ogl-master/playground_shadow_mapping_point_light/geometry.glsl";
const char* fragmentPath = "/home/wzw/documents/ogl-master/playground_shadow_mapping_point_light/fragment.glsl";

const char* shadowVertexPath = "/home/wzw/documents/ogl-master/playground_shadow_mapping_point_light/shadow_mapping.vs.glsl";
const char* shadowFragmentPath = "/home/wzw/documents/ogl-master/playground_shadow_mapping_point_light/shadow_mapping.fs.glsl";

const char* debugLightVertexPath = "/home/wzw/documents/ogl-master/playground_shadow_mapping_point_light/debug_light.vs.glsl";
const char* debugLightFragmentPath = "/home/wzw/documents/ogl-master/playground_shadow_mapping_point_light/debug_light.fs.glsl";

const char* floorTexPath = "/home/wzw/documents/ogl-master/resource/textures/wood.png";
const char* wallTexPath = "/home/wzw/documents/ogl-master/resource/textures/wall.jpg";

const int WIDTH = 2560;
const int HEIGHT = 1440;

/**
 *  camera
 */
Camera camera(glm::vec3(0.0f, 3.0f, 7.0f));  /* NOLINT */
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

GLuint loadTexture(const char* path, bool correction) {
    int width{};
    int height{};
    int channel{};
    unsigned char* data = stbi_load(path, &width, &height, &channel, 0);
    GLint format{};
    GLint sFormat{};
    switch(channel) {
        case 1:
            format = GL_RED;
            break;
        case 3:
            sFormat = correction ? GL_SRGB : GL_RGB;
            format = GL_RGB;
            break;
        case 4:
            sFormat = correction ? GL_SRGB_ALPHA : GL_RGBA;
            format = GL_RGBA;
            break;
        default:
            break;
    }

    GLuint texture1;
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, sFormat, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    }
    else {
        std::cout << "create texture failed case the data is nullptr" << std::endl;
        exit(EXIT_FAILURE);
    }
    stbi_image_free(data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);

    return texture1;
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
     * coordinates  and indices
     */
    const float cubeVertices[] = {
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

    /**
     * three cube translate rotate and scale
     */
    std::vector<glm::mat4> models;
    glm::mat4 modelMat(1.0f);
    modelMat = glm::mat4(1.0f);
    modelMat = glm::translate(modelMat, glm::vec3(4.0f, -3.5f, 0.0f));
    modelMat = glm::scale(modelMat, glm::vec3(0.5f));
    models.push_back(modelMat);

    modelMat = glm::mat4(1.0f);
    modelMat = glm::translate(modelMat, glm::vec3(2.0f, 3.0f, 1.0f));
    modelMat = glm::scale(modelMat, glm::vec3(0.75f));
    models.push_back(modelMat);

    modelMat = glm::mat4(1.0f);
    modelMat = glm::translate(modelMat, glm::vec3(-3.0f, -1.0f, 0.0f));
    modelMat = glm::scale(modelMat, glm::vec3(0.5f));
    models.push_back(modelMat);

    modelMat = glm::mat4(1.0f);
    modelMat = glm::translate(modelMat, glm::vec3(-1.5f, 1.0f, 1.5f));
    modelMat = glm::scale(modelMat, glm::vec3(0.5f));
    models.push_back(modelMat);

    modelMat = glm::mat4(1.0f);
    modelMat = glm::translate(modelMat, glm::vec3(-1.5f, 2.0f, -3.0f));
    modelMat = glm::rotate(modelMat, glm::radians(60.0f), glm::normalize(glm::vec3(1.0f, 0.0f, 1.0f)));
    modelMat = glm::scale(modelMat, glm::vec3(0.75f));
    models.push_back(modelMat);

    /**
     * ****************************************************
     * INIT
     */
    glEnable(GL_DEPTH_TEST);
    glViewport(0, 0, WIDTH, HEIGHT);
	glClearColor(20 / 255.0f, 20 / 255.0f, 20 / 255.0f, 0.0f);

    GLuint cubeVAO;
    glGenVertexArrays(1, &cubeVAO);
    glBindVertexArray(cubeVAO);
    GLuint cubeVBO;
    glGenBuffers(1, &cubeVBO);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)nullptr);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glBindVertexArray(0);

    // draw light
    GLuint lightVAO;
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);
    GLuint lightVBO;
    glGenBuffers(1, &lightVBO);
    glBindBuffer(GL_ARRAY_BUFFER, lightVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)nullptr);
    glBindVertexArray(0);

    // framebuffer object
    const int DEPTH_WIDTH = 1024;
    const int DEPTH_HEIGHT = 1024;
    GLuint framebuffer;
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    GLuint cubeMap;
    glGenTextures(1, &cubeMap);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMap);
    for (GLuint i = 0; i < 6; i++) {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT,
                     DEPTH_WIDTH, DEPTH_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, cubeMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cout << "create framebuffer failed..." << std::endl;
        exit(EXIT_FAILURE);
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    GLuint planeTexture = loadTexture(floorTexPath, false);
    GLuint wallTexture = loadTexture(wallTexPath, false);

    // draw 3D sense
    GLuint shadowProgram = LoadShaders(shadowVertexPath, shadowFragmentPath);
    // generate cube depth map
    GLuint depthProgram = LoadShaders(vertexPath, geometryPath, fragmentPath);
    // draw light
    GLuint lightProgram = LoadShaders(debugLightVertexPath, debugLightFragmentPath);

    glUseProgram(shadowProgram);
    SetUniform(shadowProgram, "texture1", 0);
    SetUniform(shadowProgram, "depthMap", 1);

    // light position
    glm::vec3 lightPosition(0.0f);

    while (glfwWindowShouldClose(window) == 0) {
        auto currentTime = (float)glfwGetTime();
        deltaTime = currentTime - lastFrame;
        lastFrame = currentTime;
        processInput(window);

        lightPosition = glm::vec3(0.0f, 2.0f * sin(currentTime), 0.0f);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // use shadow map
        glViewport(0, 0, WIDTH, HEIGHT);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // 0. create depth cube map transformation matrices
        float nearPlane = 1.0f;
        float farPlane = 25.0f;
        glm::mat4 shadowProj = glm::perspective(glm::radians(90.0f), (float)DEPTH_WIDTH / DEPTH_HEIGHT, nearPlane, farPlane);
        std::vector<glm::mat4> shadowTransforms;
        shadowTransforms.push_back(shadowProj * glm::lookAt(lightPosition, lightPosition + glm::vec3( 1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)));
        shadowTransforms.push_back(shadowProj * glm::lookAt(lightPosition, lightPosition + glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)));
        shadowTransforms.push_back(shadowProj * glm::lookAt(lightPosition, lightPosition + glm::vec3( 0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)));
        shadowTransforms.push_back(shadowProj * glm::lookAt(lightPosition, lightPosition + glm::vec3( 0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)));
        shadowTransforms.push_back(shadowProj * glm::lookAt(lightPosition, lightPosition + glm::vec3( 0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)));
        shadowTransforms.push_back(shadowProj * glm::lookAt(lightPosition, lightPosition + glm::vec3( 0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f)));

        // 1. render scene to depth cube map
        glViewport(0, 0, DEPTH_WIDTH, DEPTH_HEIGHT);
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
        glClear(GL_DEPTH_BUFFER_BIT);
        glUseProgram(depthProgram);
        for (unsigned int i = 0; i < shadowTransforms.size(); i++) {
            SetUniform(depthProgram, std::string("shadowMatrices[" + std::to_string(i) + "]").c_str(), shadowTransforms[i]);
        }
        SetUniform(depthProgram, "lightPosition", lightPosition);
        SetUniform(depthProgram, "farPlane", farPlane);
        // draw big box
        {
            glBindVertexArray(cubeVAO);
            glm::mat4 model(1.0f);
            model = glm::scale(model, glm::vec3(5.0f));
            glDisable(GL_CULL_FACE);
            SetUniform(depthProgram, "model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
            glEnable(GL_CULL_FACE);
            // draw cube
            for (auto mat : models) {
                SetUniform(depthProgram, "model", mat);
                glDrawArrays(GL_TRIANGLES, 0, 36);
            }
        }
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        // use depth map to render scene
        glViewport(0, 0, WIDTH, HEIGHT);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glm::mat4 projection = glm::perspective(glm::radians(camera.zoom), WIDTH / (float)HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.getViewMatrix();

        // draw light
        glUseProgram(lightProgram);
        SetUniform(lightProgram, "projection", projection);
        SetUniform(lightProgram, "view", view);
        glm::mat4 lightModel(1.0f);
        lightModel = glm::translate(lightModel, glm::vec3(0.0f, 2.0f * sin(currentTime), 0.0f));
        lightModel = glm::scale(lightModel, glm::vec3(0.05f));
        SetUniform(lightProgram, "model", lightModel);
        glBindVertexArray(cubeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);


        glUseProgram(shadowProgram);
        SetUniform(shadowProgram, "projection", projection);
        SetUniform(shadowProgram, "view", view);
        SetUniform(shadowProgram, "lightPosition", lightPosition);
        SetUniform(shadowProgram, "cameraPosition", camera.position);
        SetUniform(shadowProgram, "farPlane", farPlane);
        // draw big box
        glBindVertexArray(cubeVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, planeTexture);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMap);
        glm::mat4 model(1.0f);
        model = glm::scale(model, glm::vec3(5.0f));
        glm::mat3 normalMat = glm::transpose(glm::inverse(glm::mat3(model)));
        glDisable(GL_CULL_FACE);
        SetUniform(shadowProgram, "model", model);
        SetUniform(shadowProgram, "normalMat", normalMat);
        SetUniform(shadowProgram, "reversNormal", 1);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        SetUniform(shadowProgram, "reversNormal", 0);
        glEnable(GL_CULL_FACE);
        // draw cube
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, wallTexture);
        for (auto mat : models) {
            SetUniform(shadowProgram, "model", mat);
            glm::mat3 tmpNormalMat = glm::transpose(glm::inverse(glm::mat3(mat)));
            SetUniform(shadowProgram, "normalMat", tmpNormalMat);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

    glDeleteTextures(1, &planeTexture);
    glDeleteTextures(1, &wallTexture);
    glDeleteBuffers(1, &cubeVBO);
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteProgram(shadowProgram);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}











