
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

const char* vertexPath = "/home/wzw/documents/ogl-master/playground_parrallax_mapping/vertex.glsl";
const char* fragmentPath = "/home/wzw/documents/ogl-master/playground_parrallax_mapping/fragment.glsl";
const char* textureWallPath = "/home/wzw/documents/ogl-master/resource/textures/bricks2.jpg";
const char* textureWallNMPath = "/home/wzw/documents/ogl-master/resource/textures/bricks2_normal.jpg";
const char* textureWallDisplacePath = "/home/wzw/documents/ogl-master/resource/textures/bricks2_disp.jpg";

const int WIDTH = 2560;
const int HEIGHT = 1440;

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


GLuint loadTexture(const char* path) {
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    int width{};
    int height{};
    int channels{};
    unsigned char* data = stbi_load(path, &width, &height, &channels, 0);
    GLint format{};
    if (data) {
        switch (channels) {
            case 1:
                format = GL_RED;
                break;
            case 3:
                format = GL_RGB;
                break;
            case 4:
                format = GL_RGBA;
                break;
            default:
                break;
        }
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    }
    else {
        std::cout << "create texture failed case the data is nullptr" << std::endl;
    }
    stbi_image_free(data);

    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,  format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    return texture;
}

void renderScene(GLuint& VAO, GLuint& VBO) {
    if (VAO == 0) {
        // calculate tangent space TBN
        // NDC vertex positions
        glm::vec3 p1(-1.0f, 1.0f, 0.0f);
        glm::vec3 p2(-1.0f, -1.0f, 0.0f);
        glm::vec3 p3(1.0f, -1.0f, 0.0f);
        glm::vec3 p4(1.0f, 1.0f, 0.0f);

        // texture coordinates
        glm::vec2 t1(0.0f, 1.0f);
        glm::vec2 t2(0.0f, 0.0f);
        glm::vec2 t3(1.0f, 0.0f);
        glm::vec2 t4(1.0f, 1.0f);

        glm::vec3 normal(0.0f, 0.0f, 1.0f);

        glm::vec3 e1 = p2 - p1;
        glm::vec3 e2 = p3 - p1;
        glm::vec2 deltaUV1 = t2 - t1;
        glm::vec2 deltaUV2 = t3 - t1;

        // first triangle   p1 p2 p3
        float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);
        glm::vec3 tangent1{};
        tangent1.x = f * (deltaUV2.y * e1.x - deltaUV1.y * e2.x);
        tangent1.y = f * (deltaUV2.y * e1.y - deltaUV1.y * e2.y);
        tangent1.z = f * (deltaUV2.y * e1.z - deltaUV1.y * e2.z);

        // second triangle   p1 p3 p4
        e1 = p3 - p1;
        e2 = p4 - p1;
        deltaUV1 = t3 - t1;
        deltaUV2 = t4 - t1;

        f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);
        glm::vec3 tangent2{};
        tangent2.x = f * (deltaUV2.y * e1.x - deltaUV1.y * e2.x);
        tangent2.y = f * (deltaUV2.y * e1.y - deltaUV1.y * e2.y);
        tangent2.z = f * (deltaUV2.y * e1.z - deltaUV1.y * e2.z);

        const float vertices[] = {
                p1.x, p1.y, p1.z, normal.x, normal.y, normal.z, t1.x, t1.y, tangent1.x, tangent1.y, tangent1.z,
                p2.x, p2.y, p2.z, normal.x, normal.y, normal.z, t2.x, t2.y, tangent1.x, tangent1.y, tangent1.z,
                p3.x, p3.y, p3.z, normal.x, normal.y, normal.z, t3.x, t3.y, tangent1.x, tangent1.y, tangent1.z,

                p1.x, p1.y, p1.z, normal.x, normal.y, normal.z, t1.x, t1.y, tangent2.x, tangent2.y, tangent2.z,
                p3.x, p3.y, p3.z, normal.x, normal.y, normal.z, t3.x, t3.y, tangent2.x, tangent2.y, tangent2.z,
                p4.x, p4.y, p4.z, normal.x, normal.y, normal.z, t4.x, t4.y, tangent2.x, tangent2.y, tangent2.z,
        };

        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);
        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), nullptr);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(8 * sizeof(float)));
        glBindVertexArray(0);
    }
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
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
	// Dark blue background
    glEnable(GL_DEPTH_TEST);
    glViewport(0, 0, WIDTH, HEIGHT);
	glClearColor(20 / 255.0f, 20 / 255.0f, 25 / 255.0f, 0.0f);

    GLuint program = LoadShaders(vertexPath, fragmentPath);

    stbi_set_flip_vertically_on_load(true);
    GLuint diffTexture = loadTexture(textureWallPath);
    GLuint normalMap = loadTexture(textureWallNMPath);
    GLuint displaceMap = loadTexture(textureWallDisplacePath);

    glUseProgram(program);
    SetUniform(program, "diffuseTexture", 0);
    SetUniform(program, "normalMap", 1);
    SetUniform(program, "displaceMap", 2);

    glm::vec3 lightPos(0.5f, 1.0f, 0.3f);
    GLuint VAO{};
    GLuint VBO{};
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
        model = glm::rotate(model, glm::radians(currentTime * -10.0f), glm::vec3(1.0f, 0.0f, 1.0f));
        SetUniform(program, "projection", projection);
        SetUniform(program, "view", view);
        SetUniform(program,"model", model);
        SetUniform(program, "lightPos", lightPos);
        SetUniform(program, "viewPos", camera.position);
        // normal matrix
        glm::mat3 normalMat = glm::transpose(glm::inverse(glm::mat3(model)));
        SetUniform(program, "normalMat", normalMat);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, diffTexture);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, normalMap);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, displaceMap);
        renderScene(VAO, VBO);

        // render a small light
        model = glm::mat4(1.0f);
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(0.1f));
        glm::mat3 normalMat1 = glm::transpose(glm::inverse(glm::mat3(model)));
        SetUniform(program, "normalMat", normalMat1);
        SetUniform(program, "model", model);
        renderScene(VAO, VBO);

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













