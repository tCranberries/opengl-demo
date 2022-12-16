
#include "app_base.h"

#include "model.h"
#include <random>


class App : public AppBase {
public:
    App() = default;

    ~App() override {
        glDeleteBuffers(1, &mTriangleVBO);
        glDeleteVertexArrays(1, &mTriangleVAO);
        glDeleteProgram(mProgram);

        delete backpack;
    };

    // Anything that needs to be called once outside the main application loop
    void StartUp() override {
        const char* vertexPath = "/home/w/CLionProjects/opengl-demo/opg_7.2_wireframe/vertex.glsl";
        const char* geometryPath = "/home/w/CLionProjects/opengl-demo/opg_7.2_wireframe/geometry.glsl";
        const char* fragmentPath = "/home/w/CLionProjects/opengl-demo/opg_7.2_wireframe/fragment.glsl";
        const char* backpackPath =  "/home/w/CLionProjects/opengl-demo/resource/model/backpack/backpack.obj";

        // set some matrix

        glm::mat4 projection = glm::perspective(glm::radians(camera->zoom),
                                                (float)getWindowWidth() / (float)getWindowHeight(),
                                                0.1f, 100.f);
        float w2 = (float)getWindowWidth() / 2.0f;
        float h2 = (float)getWindowHeight() / 2.0f;
        glm::mat4 viewportMatrix = glm::mat4(glm::vec4(w2, 0.0f, 0.0f, 0.0f),
                                             glm::vec4(0.0f, h2, 0.0f, 0.0f),
                                             glm::vec4(0.0f, 0.0f, 1.0f, 0.0f),
                                             glm::vec4(w2 + 0.0f, h2 + 0.0f, 0.0f, 1.0f));

        backpack = new Model(backpackPath);

        mProgram = LoadShaders(vertexPath, geometryPath, fragmentPath);
        glUseProgram(mProgram);
        SetUniform(mProgram, "projection", projection);
        SetUniform(mProgram, "viewportMatrix", viewportMatrix);

        camera->setPosition(glm::vec3(0.0f, 0.0f, 13.0f));

        glEnable(GL_DEPTH_TEST);
    }

    // Anything that needs to be called cyclically inside the main application loop
    void Update() override {

        {
            static int counter = 0;

            ImGui::Begin("Point Sprite!");                          // Create a window called "Hello, world!" and append into it.

            ImGui::Text("this is point sprite parameter control");               // Display some text (you can use a format strings too)
            ImGui::Checkbox("Wireframe", &isWireframe);      // Edit bools storing our window open/close state
//            ImGui::Checkbox("Another Window", &show_another_window);

            ImGui::SliderFloat("line width", &lineWidth, 0.0f, 2.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
            ImGui::ColorEdit3("line color", (float*)&lineColor);

            ImGui::ColorEdit3("light color", (float*)&lightColor);

            ImGui::ColorEdit3("clear color", (float*)&clearColor); // Edit 3 floats representing a color

            if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
                counter++;
            ImGui::SameLine();
            ImGui::Text("counter = %d", counter);

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::End();
        }


        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        glm::mat4 view = camera->getViewMatrix();
        glm::mat4 model(1.0f);

        glUseProgram(mProgram);
        SetUniform(mProgram, "view", view);
        SetUniform(mProgram, "model", model);
        glm::mat3 normalMatrix = glm::transpose(glm::inverse(glm::mat3(model)));
        SetUniform(mProgram, "normalMatrix", normalMatrix);

        // light info
        SetUniform(mProgram, "lightPosition", lightPosition);
        SetUniform(mProgram, "lightColor", lightColor);

        // line info
        SetUniform(mProgram, "lineWidth", lineWidth);
        SetUniform(mProgram, "lineColor", lineColor);

        // camera position
        SetUniform(mProgram, "viewerPosition", camera->position);

        // is wireframe
        SetUniform(mProgram, "wireframe", isWireframe);

        if (backpack) {
            backpack->draw(mProgram);
        }
    }

private:
    GLuint mTriangleVAO{};
    GLuint mTriangleVBO{};

    GLuint mProgram{};

    Model* backpack{};

    glm::vec3 lightPosition = glm::vec3(0.0f, 2.0f, 3.0f);
    glm::vec3 lightColor = glm::vec3(0.5f, 0.7f, 0.8f);
    glm::vec3 lineColor = glm::vec3(0.0f, 0.0f, 0.0f);
    GLfloat lineWidth = 1.0f;

    bool isWireframe{};

};


int main() {
    App app;
    app.Run();


    return 0;
}