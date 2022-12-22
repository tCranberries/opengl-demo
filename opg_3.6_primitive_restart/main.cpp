
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
            // cube vertex
            const float position[] = {
                    -1.0f, -1.0f, -1.0f,
                    -1.0f, -1.0f,  1.0f,
                    -1.0f,  1.0f, -1.0f,
                    -1.0f,  1.0f,  1.0f,
                    1.0f, -1.0f, -1.0f,
                    1.0f, -1.0f,  1.0f,
                    1.0f,  1.0f, -1.0f,
                    1.0f,  1.0f,  1.0f
            };

            const float color[] = {
                    1.0f, 1.0f, 1.0f,
                    1.0f, 1.0f, 0.0f,
                    1.0f, 0.0f, 1.0f,
                    1.0f, 0.0f, 0.0f,
                    0.0f, 1.0f, 1.0f,
                    0.0f, 1.0f, 0.0f,
                    0.0f, 0.0f, 1.0f,
                    0.5f, 0.5f, 0.5f
            };

            const unsigned int index[] = {
                    0, 1, 2, 3, 6, 7, 4, 5,         // First strip
                    0xFFFF,                         // <<-- This is the restart index
                    2, 6, 0, 4, 1, 5, 3, 7          // Second strip
            };


            glGenVertexArrays(1, &mTriangleVAO);
            glBindVertexArray(mTriangleVAO);
            glGenBuffers(1, &mTriangleVBO);
            glBindBuffer(GL_ARRAY_BUFFER, mTriangleVBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(position) + sizeof(color), nullptr, GL_STATIC_DRAW);

            glGenBuffers(1, &mTriangleEBO);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mTriangleEBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(index), index, GL_STATIC_DRAW);


            /**
             * 采用这种方式传递顶点属性数据时， 注意数据是紧密排列的
             */
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(position), position);
            glBufferSubData(GL_ARRAY_BUFFER, sizeof(position), sizeof(color), color);

            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)(sizeof(position)));

            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindVertexArray(0);
        }
        glUseProgram(mProgram);

        glBindVertexArray(mTriangleVAO);
//        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mTriangleEBO);

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-2.0f, 0.0f, -5.0f));
        SetUniform(mProgram, "model", model);

        //glDrawElements(GL_TRIANGLE_STRIP, 17, GL_UNSIGNED_INT, nullptr);
        glDrawElements(GL_TRIANGLE_STRIP, 8, GL_UNSIGNED_INT, nullptr);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-2.0f, -1.0f, -5.0f));
        SetUniform(mProgram, "model", model);
        glDrawElements(GL_TRIANGLE_STRIP, 8, GL_UNSIGNED_INT, (void*)(sizeof(float) * 9));
    }



    // Anything that needs to be called once outside the main application loop
    void StartUp() override {
        const char* vertexPath = "/home/w/CLionProjects/opengl-demo/opg_3.6_primitive_restart/vertex.glsl";
        const char* fragmentPath = "/home/w/CLionProjects/opengl-demo/opg_3.6_primitive_restart/fragment.glsl";

        mProgram = LoadShaders(vertexPath, fragmentPath);

        glEnable(GL_DEPTH_TEST);
        camera->setPosition(glm::vec3(0.0f, 0.0f, 3.0f));

        // 开启图元重启动
//        glEnable(GL_PRIMITIVE_RESTART);
//        glPrimitiveRestartIndex(0xFFFF);
    }

    // Anything that needs to be called cyclically inside the main application loop
    void Update() override {

        {
            static bool b{};
            static float f = 0.0f;
            static int counter = 0;

            ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

            ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
            ImGui::Checkbox("Use Texture", &b);      // Edit bools storing our window open/close state
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

        glUseProgram(mProgram);
        SetUniform(mProgram, "projection", projection);
        SetUniform(mProgram, "view", view);

        drawTriangle();
    }

private:
    GLuint mTriangleEBO{};
    GLuint mTriangleVAO{};
    GLuint mTriangleVBO{};
    GLuint mProgram{};

};


int main() {
    App app;
    app.Run();


    return 0;
}