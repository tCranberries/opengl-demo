
#include <iostream>
#include <cstdio>
#include <cmath>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../common/shader.hpp"
#include "../common/stb_image.h"
#include "../common/camera.h"


GLFWwindow* window;

const char* vertexPath = "/home/w/CLionProjects/opengl-demo/opg_4.1_pbr/vertex.glsl";
const char* fragmentPath = "/home/w/CLionProjects/opengl-demo/opg_4.1_pbr/fragment.glsl";

const int WIDTH = 1920;
const int HEIGHT = 1080;

/**
 * camera
 */
Camera camera(glm::vec3(0.0f, 0.0f, 23.0f));  /* NOLINT */
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

// render a sphere
GLuint SphereVAO;
GLuint IndexCount;

void drawSphere() {
    if (SphereVAO == 0) {
        float radius = 1.0f;
        std::vector<glm::vec3> positions;
        std::vector<glm::vec3> normals;
        std::vector<glm::vec2> uvs;
        std::vector<unsigned int> indices;

        const int xSegments = 64;
        const int ySegments = 64;

        // generate (64 + 1) * (64 + 1) vertices
        for (unsigned int i = 0; i <= xSegments; i++) {
            for (unsigned int j = 0; j <= ySegments; j++) {
                float xSegment = (float)i / (float)xSegments;
                float ySegment = (float)j / (float)ySegments;
                float x = radius * (float)std::sin(ySegment * M_PI) * (float)std::cos(2.0f * M_PI * xSegment);
                float y = radius * (float)std::cos(ySegment * M_PI);
                float z = radius * (float)std::sin(ySegment * M_PI) * (float)std::sin(2.0f * M_PI * xSegment);

                positions.emplace_back(x, y, z);
                normals.emplace_back(x, y, z);
                uvs.emplace_back(xSegment, ySegment);
            }
        }

        // fill indices
        bool oddRow = false;
        for (unsigned int y = 0; y < ySegments; y++) {
            if (!oddRow) {
                for (unsigned int x = 0; x <= xSegments; x++) {
                    indices.push_back(y * (xSegments + 1) + x);
                    indices.push_back((y + 1) * (xSegments + 1) + x);
                }
            }
            else {
                for (int x = xSegments; x >= 0; x--) {
                    indices.push_back((y + 1) * (xSegments + 1) + x);
                    indices.push_back(y * (xSegments + 1) + x);
                }
            }
            oddRow = !oddRow;
        }

        IndexCount = indices.size();

        // push all data to one container
        std::vector<float> data;
        for (unsigned int i = 0; i < positions.size(); i++) {
            data.push_back(positions[i].x);
            data.push_back(positions[i].y);
            data.push_back(positions[i].z);
            if (!normals.empty()) {
                data.push_back(normals[i].x);
                data.push_back(normals[i].y);
                data.push_back(normals[i].z);
            }
            if (!uvs.empty()) {
                data.push_back(uvs[i].x);
                data.push_back(uvs[i].y);
            }
        }

        // generate VAO VBO EBO
        GLuint VBO;
        GLuint EBO;
        glGenVertexArrays(1, &SphereVAO);
        glBindVertexArray(SphereVAO);
        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, (long)(data.size() * sizeof(float)), &data[0], GL_STATIC_DRAW);
        glGenBuffers(1, &EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, (long)(indices.size() * sizeof(unsigned int)), &indices[0], GL_STATIC_DRAW);
        int stride = (3 + 3 + 2) * sizeof(float);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, nullptr);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)(6 * sizeof(float)));
        glBindVertexArray(0);
    }

    glBindVertexArray(SphereVAO);
    glDrawElements(GL_TRIANGLE_STRIP, (int)IndexCount, GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(SphereVAO);
}

unsigned int loadTexture(const char* path) {
    unsigned int textureID{};
    glGenTextures(1, &textureID);

    int width{};
    int height{};
    int channels{};
    unsigned char* data = stbi_load(path, &width, &height, &channels, 0);
    if (data) {
        GLint format{};
        if (channels == 1) {
            format = GL_RED;
        }
        else if (channels == 3) {
            format = GL_RGB;
        }
        else if (channels == 4) {
            format = GL_RGBA;
        }

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0,
                     format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    }
    else {
        std::cout << "load file data failed!" << std::endl;
        exit(EXIT_FAILURE);
    }
    stbi_image_free(data);

    return textureID;
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


    /**
     * ****************************************************
     * INIT
     */
	// Dark blue background
    glEnable(GL_DEPTH_TEST);
    glViewport(0, 0, WIDTH, HEIGHT);
	glClearColor(12 / 255.0f, 25 / 255.0f, 21 / 255.0f, 0.0f);

    // lights
    // ------
    glm::vec3 lightPositions[] = {
            glm::vec3(-10.0f,  10.0f, 10.0f),
            glm::vec3( 10.0f,  10.0f, 10.0f),
            glm::vec3(-10.0f, -10.0f, 10.0f),
            glm::vec3( 10.0f, -10.0f, 10.0f),
    };
    glm::vec3 lightColors[] = {
            glm::vec3(300.0f, 300.0f, 300.0f),
            glm::vec3(300.0f, 300.0f, 300.0f),
            glm::vec3(300.0f, 300.0f, 300.0f),
            glm::vec3(300.0f, 300.0f, 300.0f)
    };
    int nrRows    = 7;
    int nrColumns = 7;
    float spacing = 2.5;

    GLuint program = LoadShaders(vertexPath, fragmentPath);
    glUseProgram(program);
    SetUniform(program, "albedo", glm::vec3(0.5f, 0.0f, 0.0f));
    SetUniform(program, "ao", 1.0f);


    while (glfwWindowShouldClose(window) == 0) {
        auto currentTime = (float)glfwGetTime();
        deltaTime = currentTime - lastFrame;
        lastFrame = currentTime;

        processInput(window);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        /**
         * mvp  matrix
         */
        glUseProgram(program);
        glm::mat4 projection = glm::perspective(glm::radians(camera.zoom), WIDTH / (float)HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.getViewMatrix();
        SetUniform(program, "projection", projection);
        SetUniform(program, "view", view);
        SetUniform(program, "viewerPos", camera.position);

        glm::mat4 model(1.0f);
        // row * column spheres
        for (int row = 0; row < nrRows; row++) {
            float metallic = (float)row / (float)nrRows;
            SetUniform(program, "metallic", metallic);
            for (int column = 0; column < nrColumns; column++) {
                float roughness = glm::clamp((float)column / (float)nrColumns, 0.05f, 1.0f);
                SetUniform(program, "roughness", roughness);

                model = glm::mat4(1.0f);
                model = glm::translate(model,
                                       glm::vec3((column - (nrColumns / 2)) * spacing,
                                                 (row - (nrRows / 2)) * spacing,
                                                 0.0f));
                glm::mat3 normalMatrix = glm::transpose(glm::inverse(glm::mat3(model)));
                SetUniform(program, "model", model);
                SetUniform(program, "normalMatrix", normalMatrix);

                drawSphere();
            }
        }

        for (unsigned int i = 0; i < 4; i++) {
            SetUniform(program, std::string("lightPositions[" + std::to_string(i) + "]").c_str(), lightPositions[i]);
            SetUniform(program, std::string("lightColors[" + std::to_string(i) + "]").c_str(), lightColors[i]);

            model = glm::mat4(1.0f);
            model = glm::translate(model, lightPositions[i]);
            model = glm::scale(model, glm::vec3(0.5f));
            SetUniform(program, "model", model);

            drawSphere();
        }

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

    glDeleteVertexArrays(1, &SphereVAO);
    glDeleteProgram(program);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}













