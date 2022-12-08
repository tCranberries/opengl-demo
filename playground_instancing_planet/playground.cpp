
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

const char* vertexPath = "/home/w/CLionProjects/opengl-demo/playground_instancing_planet/vertex.glsl";
const char* rockVertexPath = "/home/w/CLionProjects/opengl-demo/playground_instancing_planet/rockVertex.glsl";
const char* fragmentPath = "/home/w/CLionProjects/opengl-demo/playground_instancing_planet/fragment.glsl";
const char* planetPath = "/home/w/CLionProjects/opengl-demo/resource/model/planet/planet.obj";
const char* rockPath = "/home/w/CLionProjects/opengl-demo/resource/model/rock/rock.obj";


const int WIDTH = 2560;
const int HEIGHT = 1440;

/**
 *  camera
 */
Camera camera(glm::vec3(0.0f, 120.0f, 255.0f));  /* NOLINT */
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

void calcModels(std::vector<glm::mat4>& matrices, const int mount, const float radius, const float offset) {
    srand(static_cast<unsigned int>(glfwGetTime()));
    for (int i = 0; i < mount; i++) {
        glm::mat4 model{1.0f};

        float angle = ((float)i / 100.0f) * 360.0f;
        float dispatch = (float)(random() % (int)(2 * offset * 100.0f)) / 100.0f - offset;
        float x = sinf(angle) * radius + dispatch;
        float y = dispatch * 0.4f;
        float z = cosf(angle) * radius + dispatch;

        model = glm::translate(model, glm::vec3(x, y, z));

        auto rotate = (float)(random() % 360);
        model = glm::rotate(model, glm::radians(rotate), glm::vec3(0.3f, 0.4f, 0.8f));

        auto scale = (float)(random() % 20) / 100.0f;
        model = glm::scale(model, glm::vec3(scale));

        matrices.push_back(model);
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
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    /**
     * coordinates  and indices
     */
    const float boxVertices[] = {
            -0.05f,  0.05f, 0.0f, 1.0f, 0.0f, 0.0f,
            0.05f, -0.05f, 0.0f, 0.0f, 1.0f, 0.0f,
            -0.05f, -0.05f, 0.0f, 0.0f, 0.0f, 1.0f,

            -0.05f,  0.05f, 0.0f, 1.0f, 0.0f, 0.0f,
            0.05f, -0.05f, 0.0f, 0.0f, 1.0f, 0.0f,
            0.05f,  0.05f, 0.0f, 0.0f, 1.0f, 1.0f
    };

    glm::vec3 translations[100]{};
    int index = 0;
    float offset = 0.1f;
    for(int y = -10; y < 10; y += 2) {
        for(int x = -10; x < 10; x += 2) {
            glm::vec3 translation;
            translation.x = (float)x / 10.0f + offset;
            translation.y = (float)y / 10.0f + offset;
            translations[index++] = translation;
        }
    }

    /**
     * ****************************************************
     * INIT
     */
	// Dark blue background
    glEnable(GL_DEPTH_TEST);
    glViewport(0, 0, WIDTH, HEIGHT);
	glClearColor(127 / 255.0f, 255 / 255.0f, 212 / 255.0f, 0.0f);

    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    GLuint VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(boxVertices), boxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), nullptr);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    GLuint transVBO;
    glGenBuffers(1, &transVBO);
    glBindBuffer(GL_ARRAY_BUFFER, transVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * 100, &translations[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), nullptr);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glVertexAttribDivisor(2, 1);
    glBindVertexArray(0);


    GLuint program = LoadShaders(vertexPath, fragmentPath);
    Model planet(planetPath);
    GLuint rockProgram = LoadShaders(rockVertexPath, fragmentPath);
    Model rock(rockPath);

    // generate models;
    std::vector<glm::mat4> modelMats;
    const int amount = 100000;
    calcModels(modelMats, amount, 150.0f, 25.0f);

    //
    GLuint buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * amount, &modelMats[0], GL_STATIC_DRAW);

    for (const auto& mesh : rock.meshes) {
        glBindVertexArray(mesh.VAO);
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), nullptr);
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
        glEnableVertexAttribArray(5);
        glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
        glEnableVertexAttribArray(6);
        glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));

        glVertexAttribDivisor(3, 1);
        glVertexAttribDivisor(4, 1);
        glVertexAttribDivisor(5, 1);
        glVertexAttribDivisor(6, 1);

        glBindVertexArray(0);
    }

    auto lastTime = (float)glfwGetTime();
    int nbFrame = 0;
    while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0) {
        auto currentTime = (float)glfwGetTime();
        deltaTime = currentTime - lastFrame;
        lastFrame = currentTime;
        processInput(window);

        {
            // fps
            nbFrame++;
            if (currentTime - lastTime >= 1.0f) {
                std::cout << 1000.0f / (float)nbFrame << " ms/frame" << std::endl;
                nbFrame = 0;
                lastTime = currentTime;
            }
        }

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        /**
         * mvp  matrix
         */
        glUseProgram(program);
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), WIDTH / (float)HEIGHT, 0.1f, 1000.0f);
        glm::mat4 view = camera.getViewMatrix();
        SetUniform(program, "projection", projection);
        SetUniform(program, "view", view);
        glm::mat4 model(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, -3.0f, 0.0f));
        model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
        SetUniform(program, "model", model);

        planet.draw(program);


        glUseProgram(rockProgram);
        SetUniform(rockProgram, "projection", projection);
        SetUniform(rockProgram, "view", view);
        for (const auto& mesh : rock.meshes) {
            glBindVertexArray(mesh.VAO);
            glDrawElementsInstanced(GL_TRIANGLES, (int)mesh.indices.size(), GL_UNSIGNED_INT, nullptr, amount);
        }

		glfwSwapBuffers(window);
		glfwPollEvents();
	}


    glDeleteBuffers(1, &transVBO);
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
    glDeleteProgram(program);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}













