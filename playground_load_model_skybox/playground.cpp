

#include <cstdio>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../common/shader.hpp"
#include "../common/stb_image.h"
#include "../common/camera.h"
#include "../common/model.h"


GLFWwindow* window;

const char* vertexPath = "/home/w/CLionProjects/opengl-demo/playground_load_model_skybox/vertex.glsl";
const char* lampVertexPath = "/home/w/CLionProjects/opengl-demo/playground_load_model_skybox/lampVertex.glsl";
const char* fragmentPath = "/home/w/CLionProjects/opengl-demo/playground_load_model_skybox/fragment.glsl";
const char* lampFragmentPath = "/home/w/CLionProjects/opengl-demo/playground_load_model_skybox/lampFragment.glsl";
const char* skyboxVertexPath = "/home/w/CLionProjects/opengl-demo/playground_load_model_skybox/skyboxVertex.glsl";
const char* skyboxFragmentPath = "/home/w/CLionProjects/opengl-demo/playground_load_model_skybox/skyboxFragment.glsl";

const char* modelPath = "/home/w/CLionProjects/opengl-demo/resource/model/nanosuit/nanosuit.obj";
const char* skyRightPath = "/home/w/CLionProjects/opengl-demo/resource/textures/skybox/right.jpg";
const char* skyLeftPath = "/home/w/CLionProjects/opengl-demo/resource/textures/skybox/left.jpg";
const char* skyTopPath = "/home/w/CLionProjects/opengl-demo/resource/textures/skybox/top.jpg";
const char* skyBottomPath = "/home/w/CLionProjects/opengl-demo/resource/textures/skybox/bottom.jpg";
const char* skyBackPath = "/home/w/CLionProjects/opengl-demo/resource/textures/skybox/back.jpg";
const char* skyFrontPath = "/home/w/CLionProjects/opengl-demo/resource/textures/skybox/front.jpg";

// view port size
const int WIDTH = 2560;
const int HEIGHT = 1440;

// clear color
const float CLEAR_R = 0;
const float CLEAR_G = 0;
const float CLEAR_B = 0;

/**
 * camera
 */
Camera camera(glm::vec3(0.0f, 10.0f, 25.0f));  /* NOLINT */
float lastX = WIDTH / 2.0f;
float lastY = HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

// point light
const int NR_POINT_LIGHTS = 4;
// dir light

// spotlight
const float CUTOFF = 8.5f;
const float OUTER_CUTOFF = 10.0f;


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


GLuint loadCubeMap(const std::vector<const char*>& paths) {
    int width{};
    int height{};
    int channels{};
    GLuint cubeMapID;
    glGenTextures(1, &cubeMapID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapID);
    for (unsigned int i = 0; i < paths.size(); i++) {
        unsigned char* data = stbi_load(paths[i], &width, &height, &channels, 0);
        int format{};
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
        if (data) {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        }
        else {
            std::cout << "failed to create " << i << "'s image" << std::endl;
        }
        stbi_image_free(data);
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    return cubeMapID;
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

    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Initialize GLEW
    glewExperimental = true;
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return -1;
	}


    const float skyboxVertices[] = {
            -1.0f,  1.0f, -1.0f,
            -1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,

            -1.0f, -1.0f,  1.0f,
            -1.0f, -1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,

            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,

            -1.0f, -1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f, -1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,

            -1.0f,  1.0f, -1.0f,
            1.0f,  1.0f, -1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f, -1.0f,

            -1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
            1.0f, -1.0f,  1.0f
    };


    /**
     * ****************************************************
     * INIT
     */
	// Dark blue background
    glEnable(GL_DEPTH_TEST);
    glViewport(0, 0, WIDTH, HEIGHT);
	glClearColor(CLEAR_R / 255.0f, CLEAR_G / 255.0f, CLEAR_B / 255.0f, 0.0f);

    /**
     * light
     */
    unsigned int VAO;
    unsigned int VBO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    glBindVertexArray(0);

    /*
     * skybox
     */
    GLuint skyVAO;
    glGenVertexArrays(1, &skyVAO);
    glBindVertexArray(skyVAO);
    GLuint skyVBO;
    glGenBuffers(1, &skyVBO);
    glBindBuffer(GL_ARRAY_BUFFER, skyVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    glBindVertexArray(0);

    // lamp
    GLuint lampProgram = LoadShaders(lampVertexPath, lampFragmentPath);
    // skybox
    GLuint skyProgram = LoadShaders(skyboxVertexPath, skyboxFragmentPath);
    std::vector<const char*> paths;
    paths.push_back(skyRightPath);
    paths.push_back(skyLeftPath);
    paths.push_back(skyTopPath);
    paths.push_back(skyBottomPath);
    paths.push_back(skyBackPath);
    paths.push_back(skyFrontPath);
    GLuint skyboxTexture = loadCubeMap(paths);

    // model
    GLuint program = LoadShaders(vertexPath, fragmentPath);
    stbi_set_flip_vertically_on_load(true);
    Model hunter((std::string(modelPath)));

    // lamp color
    glm::vec3 initialLightColors[NR_POINT_LIGHTS] = {
            glm::vec3(1.0f, 0.0f, 0.0f),
            glm::vec3(0.0f, 1.0f, 0.0f),
            glm::vec3(0.0f, 0.0f, 1.0f),
            glm::vec3(1.0f, 1.0f, 1.0f)
    };

    // lamp position
    glm::vec3 initialLightPositions[NR_POINT_LIGHTS] = {
            glm::vec3(-5.0f, 8.0f, -5.0f),
            glm::vec3(7.0f, 9.0f, 7.0f),
            glm::vec3(-3.0f, 2.0f, -3.0f),
            glm::vec3(6.0f, 15.0f, 4.0f)
    };

    while (!glfwWindowShouldClose(window)) {
        auto currentTime = (float)glfwGetTime();
        deltaTime = currentTime - lastFrame;
        lastFrame = currentTime;

        processInput(window);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        /**
         * mvp  matrix
         */
        glm::mat4 projection = glm::perspective(glm::radians(camera.zoom), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.getViewMatrix();

        /**
         * draw light
         */
        glUseProgram(lampProgram);
        SetUniform(lampProgram, "projection", projection);
        SetUniform(lampProgram, "view", view);
        glBindVertexArray(VAO);
        for (int i = 0; i < NR_POINT_LIGHTS; i++) {
            glm::mat4 lightModel(1.0f);
            lightModel = glm::translate(lightModel, initialLightPositions[i]);
            lightModel = glm::scale(lightModel, glm::vec3(0.3f));
            SetUniform(lampProgram, "model", lightModel);
            SetUniform(lampProgram, "lightColor", initialLightColors[i]);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        glBindVertexArray(0);

        // draw model
        {
            glUseProgram(program);
            SetUniform(program, "projection", projection);
            SetUniform(program, "view", view);
            glm::mat4 model(1.0f);
            model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
            model = glm::rotate(model, glm::radians(20.0f) * (float)glfwGetTime(), glm::vec3(0.0f, 1.0f, 0.0f));
            model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
            SetUniform(program, "model", model);
            // hunter normal matrix
            glm::mat3 normalMatrix(1.0f);
            normalMatrix = glm::mat3(glm::transpose(glm::inverse(model)));
            SetUniform(program, "norMatrix", normalMatrix);

            for (int i = 0; i < NR_POINT_LIGHTS; i++) {
                // hunter light
                SetUniform(program, std::string("pointLights[" + std::to_string(i) + "].lightColor").c_str(), initialLightColors[i]);
                SetUniform(program, std::string("pointLights[" + std::to_string(i) + "].lightAmbient").c_str(), glm::vec3(0.05f, 0.05f, 0.05f));
                SetUniform(program, std::string("pointLights[" + std::to_string(i) + "].lightDiffuse").c_str(), glm::vec3(0.8f, 0.8f, 0.8f));
                SetUniform(program, std::string("pointLights[" + std::to_string(i) + "].lightSpecular").c_str(), glm::vec3(1.0f, 1.0f, 1.0f));
                SetUniform(program, std::string("pointLights[" + std::to_string(i) + "].constant").c_str(), 1.0f);
                SetUniform(program, std::string("pointLights[" + std::to_string(i) + "].linear").c_str(), 0.09f);
                SetUniform(program, std::string("pointLights[" + std::to_string(i) + "].quadratic").c_str(), 0.032f);
                SetUniform(program, std::string("pointLights[" + std::to_string(i) + "].lightPosition").c_str(), initialLightPositions[i]);
            }
            // dir light
            SetUniform(program, "dirLight.dirLightVec", glm::vec3(0.0f, -1.0f, 0.0f));
            SetUniform(program, "dirLight.dirLightColor", initialLightColors[NR_POINT_LIGHTS - 1]);
            SetUniform(program, "dirLight.dirLightAmbient", glm::vec3(0.05f, 0.05f, 0.05f));
            SetUniform(program, "dirLight.dirLightDiffuse", glm::vec3(0.6f, 0.6f, 0.6f));
            SetUniform(program, "dirLight.dirLightSpecular", glm::vec3(0.9f, 0.9f, 0.9f));
            // spotlight
            SetUniform(program, "spotLight.spotLightPosition", camera.position);
            SetUniform(program, "spotLight.spotLightVec", camera.front);
            SetUniform(program, "spotLight.spotLightColor", initialLightColors[NR_POINT_LIGHTS - 1]);
            SetUniform(program, "spotLight.cutoff", glm::cos(glm::radians(CUTOFF)));
            SetUniform(program, "spotLight.outerCutoff", glm::cos(glm::radians(OUTER_CUTOFF)));
            SetUniform(program, "spotLight.spotLightAmbient", glm::vec3(0.01f, 0.01f, 0.01f));
            SetUniform(program, "spotLight.spotLightDiffuse", glm::vec3(0.9f, 0.9f, 0.9f));
            SetUniform(program, "spotLight.spotLightSpecular", glm::vec3(1.0f, 1.0f, 1.0f));
            SetUniform(program, "spotLight.constant", 1.0f);
            SetUniform(program, "spotLight.linear", 0.09f);
            SetUniform(program, "spotLight.quadratic", 0.032f);
            SetUniform(program, "cameraPosition", camera.position);
            glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTexture);
            hunter.draw(program);
        }

        /**
         * draw skybox
         */
        glDepthFunc(GL_LEQUAL);
        glUseProgram(skyProgram);
        SetUniform(skyProgram, "projection", projection);
        glm::mat4 tmpView = glm::mat4(glm::mat3(view));
        SetUniform(skyProgram, "view", tmpView);
        glBindVertexArray(skyVAO);
        glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTexture);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        glDepthFunc(GL_LESS);

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &skyVAO);
    glDeleteBuffers(1, &skyVBO);
    glDeleteProgram(program);
    glDeleteProgram(lampProgram);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}













