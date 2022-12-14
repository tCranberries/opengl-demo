
#include "app_base.h"

class App : public AppBase {
public:
    App() = default;

    ~App() override {
        glDeleteBuffers(1, &mTriangleVBO);
        glDeleteVertexArrays(1, &mTriangleVAO);
        glDeleteProgram(mProgram);
    };


    void drawTriangle() {
        if (mTriangleVAO == 0) {
            const float vertices[] = {
                    -0.5f, -0.5f, 1.0, 0.0, 0.0, // Triangle 1
                    0.5f, -0.5f, 0.0, 1.0, 0.0,
                    0.0f, 0.5f, 0.0, 0.0, 1.0
            };

            glGenVertexArrays(1, &mTriangleVAO);
            glBindVertexArray(mTriangleVAO);
            glGenBuffers(1, &mTriangleVBO);
            glBindBuffer(GL_ARRAY_BUFFER, mTriangleVBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), nullptr);
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindVertexArray(0);
        }
        glBindVertexArray(mTriangleVAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);
    }


    // Anything that needs to be called once outside the main application loop
    void StartUp() override {
        const char* vertexPath = "/home/w/CLionProjects/opengl-demo/opg_1.1_triangles/vertex.glsl";
        const char* fragmentPath = "/home/w/CLionProjects/opengl-demo/opg_1.1_triangles/fragment.glsl";
        mProgram = LoadShaders(vertexPath, fragmentPath);

        glEnable(GL_DEPTH_TEST);

        camera->setPosition(glm::vec3(0.0f, 0.0f, 3.0f));
    }

    // Anything that needs to be called cyclically inside the main application loop
    void Update() override {

        {
            static float f = 0.0f;
            static int counter = 0;

            ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

            ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
//            ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
//            ImGui::Checkbox("Another Window", &show_another_window);

            ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
            ImGui::ColorEdit3("clear color", (float*)&clearColor); // Edit 3 floats representing a color

            if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
                counter++;
            ImGui::SameLine();
            ImGui::Text("counter = %d", counter);

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::End();
        }


        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 projection = glm::perspective(glm::radians(camera->zoom),
                                                (float)getWindowWidth() / (float)getWindowHeight(),
                                                0.1f, 100.f);
        glm::mat4 view = camera->getViewMatrix();
        glm::mat4 model(1.0f);

        glUseProgram(mProgram);
        SetUniform(mProgram, "projection", projection);
        SetUniform(mProgram, "view", view);
        SetUniform(mProgram, "model", model);

        drawTriangle();
    }

private:
    GLuint mTriangleVAO{};
    GLuint mTriangleVBO{};
    GLuint mProgram{};
};


int main() {
    App app;
    app.Run();


    return 0;
}