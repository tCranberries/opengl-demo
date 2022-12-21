
#include "app_base.h"
#include <cmath>

#include "model.h"

class App : public AppBase {
public:
    App() = default;

    ~App() override {
        glDeleteBuffers(1, &quadVBO);
        glDeleteVertexArrays(1, &quadVAO);

        glDeleteProgram(shadowPassProgram);
        glDeleteProgram(lightingPassProgram);
    };


    void renderQuad() {
        if (debugVAO == 0) {
            float vertices[] = {
                    // positions        // texture Coords
                    -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
                    -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
                    1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
                    1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
            };
            glGenVertexArrays(1, &debugVAO);
            glBindVertexArray(debugVAO);
            glGenBuffers(1, &debugVBO);
            glBindBuffer(GL_ARRAY_BUFFER, debugVBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), nullptr);
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindVertexArray(0);
        }
        glBindVertexArray(debugVAO);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        glBindVertexArray(0);
    }

    void drawQuad() {
        if (quadVAO == 0) {
            const float vertices[] = {
                    // positions                        // normals                      // texCoords
                    20.0f, -0.5f,  20.0f,  0.0f, 1.0f, 0.0f,  20.0f,  0.0f,
                    -20.0f, -0.5f,  20.0f,  0.0f, 1.0f, 0.0f,   0.0f,  0.0f,
                    -20.0f, -0.5f, -20.0f,  0.0f, 1.0f, 0.0f,   0.0f, 20.0f,

                    20.0f, -0.5f,  20.0f,  0.0f, 1.0f, 0.0f,  20.0f,  0.0f,
                    -20.0f, -0.5f, -20.0f,  0.0f, 1.0f, 0.0f,   0.0f, 20.0f,
                    20.0f, -0.5f, -20.0f,  0.0f, 1.0f, 0.0f,  20.0f, 20.0f
            };

            glGenVertexArrays(1, &quadVAO);
            glBindVertexArray(quadVAO);
            glGenBuffers(1, &quadVBO);
            glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, nullptr);
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(3 * sizeof(float)));
            glEnableVertexAttribArray(2);
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(6 * sizeof(float)));
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindVertexArray(0);
        }

        glBindVertexArray(quadVAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);
    }

    void drawCube() {
        if (cubeVAO == 0) {
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

            glGenVertexArrays(1, &cubeVAO);
            glBindVertexArray(cubeVAO);
            glGenBuffers(1, &cubeVBO);
            glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, nullptr);
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(3 * sizeof(float)));
            glEnableVertexAttribArray(2);
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(6 * sizeof(float)));
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindVertexArray(0);
        }
        glBindVertexArray(cubeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
    }


    static GLuint LoadTexture(const char* path, bool gamma);

    // Anything that needs to be called once outside the main application loop
    void StartUp() override {
        const char* debugVertexPath = "/home/w/CLionProjects/opengl-demo/opg_8.1_shadow_map/debug.vs.glsl";
        const char* debugFragPath = "/home/w/CLionProjects/opengl-demo/opg_8.1_shadow_map/debug.fs.glsl";

        const char* vertexPath = "/home/w/CLionProjects/opengl-demo/opg_8.1_shadow_map/vertex.glsl";
        const char* fragmentPath = "/home/w/CLionProjects/opengl-demo/opg_8.1_shadow_map/fragment.glsl";
        // floor texture
        const char* floorPath = "/home/w/CLionProjects/opengl-demo/resource/textures/wood.png";
        const char* wallTexPath = "/home/w/CLionProjects/opengl-demo/resource/textures/wall.jpg";
        const char* backpackPath =  "/home/w/CLionProjects/opengl-demo/resource/model/nanosuit/nanosuit.obj";

        // lighting pass vs and fs
        const char* lightingVertexPath = "/home/w/CLionProjects/opengl-demo/opg_8.1_shadow_map/lighting.vs.glsl";
        const char* lightingFragmentPath = "/home/w/CLionProjects/opengl-demo/opg_8.1_shadow_map/lighting.fs.glsl";

        debugProgram = LoadShaders(debugVertexPath, debugFragPath);
        shadowPassProgram = LoadShaders(vertexPath, fragmentPath);
        lightingPassProgram = LoadShaders(lightingVertexPath, lightingFragmentPath);

        camera->setPosition(glm::vec3(0.0f, 3.0f, 7.0f));
        windowWidth = (int)getWindowWidth();
        windowHeight = (int)getWindowHeight();
        floorTexture = LoadTexture(floorPath, false);
        wallTexture = LoadTexture(wallTexPath, false);

        backpack = new Model(backpackPath);
        if (!backpack) {
            std::cout << "failed create model" << std::endl;
            exit(EXIT_FAILURE);
        }

        // set
        glUseProgram(lightingPassProgram);
        SetUniform(lightingPassProgram, "texture_diffuse1", 0);
        SetUniform(lightingPassProgram, "shadowMap", 1);

        /**
         * generate framebuffer and shadow map
         */
        glGenFramebuffers(1, &frameBuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
        glGenTextures(1, &shadowMap);
        glBindTexture(GL_TEXTURE_2D, shadowMap);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
                     depthWidth, depthHeight,
                     0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glm::vec4 borderColor(1.0f);
        glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, glm::value_ptr(borderColor));
        glBindTexture(GL_TEXTURE_2D, 0);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowMap, 0);
        glReadBuffer(GL_NONE);
        glDrawBuffer(GL_NONE);
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            std::cout << "failed create framebuffer" << std::endl;
            exit(EXIT_FAILURE);
        }
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        glEnable(GL_DEPTH_TEST);

        glm::mat4 modelMat(1.0f);
        modelMat = glm::mat4(1.0f);
        modelMat = glm::translate(modelMat, glm::vec3(0.0f, 1.5f, 0.0));
        modelMat = glm::scale(modelMat, glm::vec3(0.5f));
        models.push_back(modelMat);

        modelMat = glm::mat4(1.0f);
        modelMat = glm::translate(modelMat, glm::vec3(2.0f, 0.0f, 1.0));
        modelMat = glm::scale(modelMat, glm::vec3(0.5f));
        models.push_back(modelMat);

        modelMat = glm::mat4(1.0f);
        modelMat = glm::translate(modelMat, glm::vec3(-1.0f, 0.0f, 2.0));
        modelMat = glm::rotate(modelMat, glm::radians(60.0f), glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
        modelMat = glm::scale(modelMat, glm::vec3(0.25));
        models.push_back(modelMat);
    }



    // Anything that needs to be called cyclically inside the main application loop
    void Update() override {

        /**
         * -1, imgui set some parameters
         */
        {
            static int counter = 0;
            ImGui::Begin("Point Sprite!");
            ImGui::Text("this is point sprite parameter control");
            ImGui::SliderFloat3("camera position", (float*)&camera->position, -20.0f, 20.0f);

            ImGui::SliderFloat("light fov", &lightFOV, 0.0f, 90.0f);
            ImGui::SliderFloat("light near", &near, 0.0f, 2.0f);
            ImGui::SliderFloat("light far", &far, 0.0f, 100.0f);
            ImGui::SliderFloat3("light position", (float*)&lightPosition, -5.0f, 15.0f);
            ImGui::ColorEdit3("light color", (float*)&lightColor);
            ImGui::ColorEdit3("clear color", (float*)&clearColor);
            if (ImGui::Button("Button"))
                counter++;
            ImGui::SameLine();
            ImGui::Text("counter = %d", counter);

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::End();
        }

        // matrix
        glm::mat4 projection = glm::perspective(glm::radians(camera->zoom),
                                                (float)windowWidth / (float)windowHeight,
                                                0.1f, 100.0f);
        glm::mat4 view = camera->getViewMatrix();
        glm::mat4 model(1.0f);
        glm::mat3 normalMatrix(1.0f);
        glm::mat4 lightProjection = glm::perspective(glm::radians(lightFOV),1.0f,
                                                     near, far);
        glm::mat4 lightView = glm::lookAt(lightPosition,
                                          glm::vec3(0.0f, 0.0f, -1.0f),
                                          glm::vec3(0.0f, 1.0f, 0.0f));

        /**
         * first pass
         */
        glViewport(0, 0, depthWidth, depthHeight);
        glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
        glClear(GL_DEPTH_BUFFER_BIT);
        glUseProgram(shadowPassProgram);
        SetUniform(shadowPassProgram, "lightProjection", lightProjection);
        SetUniform(shadowPassProgram, "lightView", lightView);
        // draw floor
        model = glm::mat4(1.0f);
        SetUniform(shadowPassProgram, "model", model);
        drawQuad();
        for (auto mat : models) {
            SetUniform(shadowPassProgram, "model", mat);
            drawCube();
        }
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(4.0f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.3f));
        SetUniform(shadowPassProgram, "model", model);
        if (backpack) {
            backpack->draw(shadowPassProgram);
        }
        glBindFramebuffer(GL_FRAMEBUFFER, 0);


        /**
         * second pass lighting
         */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glViewport(0, 0, windowWidth, windowHeight);

        // debug
//        glUseProgram(debugProgram);
//        SetUniform(debugProgram, "projection", projection);
//        SetUniform(debugProgram, "view", view);
//        glBindTexture(GL_TEXTURE_2D, shadowMap);
//        renderQuad();

        glUseProgram(lightingPassProgram);
        SetUniform(lightingPassProgram, "lightProjection", lightProjection);
        SetUniform(lightingPassProgram, "lightView", lightView);
        SetUniform(lightingPassProgram, "projection", projection);
        SetUniform(lightingPassProgram, "view", view);
        SetUniform(lightingPassProgram, "lightWorldSpacePos", lightPosition);
        SetUniform(lightingPassProgram, "lightColor", lightColor);
        SetUniform(lightingPassProgram, "viewerWorldSpacePos", camera->position);

        // draw floor

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, shadowMap);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, floorTexture);
        model = glm::mat4(1.0f);
        SetUniform(lightingPassProgram, "model", model);
        normalMatrix = glm::transpose(glm::inverse(glm::mat3(model)));
        SetUniform(lightingPassProgram, "normalMatrix", normalMatrix);
        drawQuad();

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, wallTexture);
        for (auto mat : models) {
            SetUniform(lightingPassProgram, "model", mat);
            normalMatrix = glm::transpose(glm::inverse(glm::mat3(mat)));
            SetUniform(lightingPassProgram, "normalMatrix", normalMatrix);
            drawCube();
        }
        glBindTexture(GL_TEXTURE_2D, 0);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(4.0f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.3f));
        SetUniform(lightingPassProgram, "model", model);
        normalMatrix = glm::transpose(glm::inverse(glm::mat3(model)));
        SetUniform(lightingPassProgram, "normalMatrix", normalMatrix);
        if (backpack) {
            backpack->draw(lightingPassProgram);
        }

        // draw light
        model = glm::mat4(1.0f);
        model = glm::translate(model, lightPosition);
        model = glm::scale(model, glm::vec3(0.1f));
        SetUniform(lightingPassProgram, "model", model);
        normalMatrix = glm::transpose(glm::inverse(glm::mat3(model)));
        SetUniform(lightingPassProgram, "normalMatrix", normalMatrix);
        drawCube();

    }

private:
    GLuint frameBuffer{};
    GLuint shadowMap{};
    GLuint floorTexture{};
    GLuint wallTexture{};

    GLuint shadowPassProgram{};
    GLuint lightingPassProgram{};

    GLuint debugVAO{};
    GLuint debugVBO{};
    GLuint debugProgram{};

    std::vector<glm::mat4> models;
    GLuint cubeVAO{};
    GLuint cubeVBO{};

    Model* backpack{};

    // floor
    GLuint quadVAO{};
    GLuint quadVBO{};


    float near = 1.0f;
    float far = 20.0f;

    GLint depthWidth = 1024;
    GLint depthHeight = 1024;
    GLint windowWidth{};
    GLint windowHeight{};

    float lightFOV = 90.0f;
    glm::vec3 lightPosition = glm::vec3(-2.0f, 8.0f, -5.0f);
    glm::vec3 lightColor = glm::vec3(0.7f);
};

GLuint App::LoadTexture(const char* path, bool gamma) {
    if (!path) {
        return 0;
    }

    GLuint texture{};
    GLint width{};
    GLint height{};
    GLint channels;

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    unsigned char* data = stbi_load(path, &width, &height, &channels, 0);
    if (data) {
        GLint sFormat{};
        GLint format{};
        if (channels == 1) {
            sFormat = GL_RED;
            format = GL_RED;
        }
        else if (channels == 3) {
            sFormat = gamma ? GL_SRGB : GL_RGB;
            format = GL_RGB;
        }
        else if (channels == 4) {
            sFormat = gamma ? GL_SRGB_ALPHA : GL_RGBA;
            format = GL_RGBA;
        }
        glTexImage2D(GL_TEXTURE_2D, 0, sFormat, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        // trick1
        glGenerateMipmap(GL_TEXTURE_2D);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    else {
        std::cout << "failed generate texture" << std::endl;
    }
    stbi_image_free(data);

    return texture;
}


int main() {
    App app;
    app.Run();

    return 0;
}