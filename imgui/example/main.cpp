
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
                    -0.5f, -0.5f, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0,// Triangle 1
                    0.5f, -0.5f, 0.0, 0.0, 1.0, 0.0, 1.0, 0.0,
                    0.0f, 0.5f, 0.0, 0.0, 0.0, 1.0, 0.5, 1.0
            };

            glGenVertexArrays(1, &mTriangleVAO);
            glBindVertexArray(mTriangleVAO);
            glGenBuffers(1, &mTriangleVBO);
            glBindBuffer(GL_ARRAY_BUFFER, mTriangleVBO);
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
        glBindVertexArray(mTriangleVAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);
    }


    // Anything that needs to be called once outside the main application loop
    void StartUp() override {
        const char* vertexPath = "/home/w/CLionProjects/opengl-demo/imgui/example/vertex.glsl";
        const char* fragmentPath = "/home/w/CLionProjects/opengl-demo/imgui/example/fragment.glsl";
        const char* gridPath = "/home/w/CLionProjects/opengl-demo/resource/textures/grid.jpg";
        mProgram = LoadShaders(vertexPath, fragmentPath);
        gridTexture = LoadTexture(gridPath);

        subroutine1 = glGetSubroutineIndex(mProgram, GL_FRAGMENT_SHADER, "renderWithTex");
        subroutine2 = glGetSubroutineIndex(mProgram, GL_FRAGMENT_SHADER, "renderWithoutTex");


        GLint activeSubroutines{};
        glGetProgramStageiv(mProgram, GL_FRAGMENT_SHADER, GL_ACTIVE_SUBROUTINES, &activeSubroutines);


        glUseProgram(mProgram);
        SetUniform(mProgram, "gridTexture", 0);


        glEnable(GL_DEPTH_TEST);

        camera->setPosition(glm::vec3(0.0f, 0.0f, 3.0f));
    }

    GLuint LoadTexture(const char* path) {
        GLuint texture{};
        GLint width{};
        GLint height{};
        GLint channel{};

        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        unsigned char* data = stbi_load(path, &width, &height, &channel, 0);
        if (data) {
            GLint format{};
            if (channel == 1) {
                format = GL_RED;
            }
            else if (channel == 3) {
                format = GL_RGB;
            }
            else if (channel == 4) {
                format = GL_RGBA;
            }
            glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        }
        else {
            std::cout << "failed create texture " << std::endl;
        }
        glBindTexture(GL_TEXTURE_2D, 0);
        stbi_image_free(data);

        return texture;
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
        glm::mat4 model(1.0f);

        glUseProgram(mProgram);
        SetUniform(mProgram, "projection", projection);
        SetUniform(mProgram, "view", view);
        SetUniform(mProgram, "model", model);


        glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, 1, &subroutine1);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, gridTexture);
        drawTriangle();
        glBindTexture(GL_TEXTURE_2D, 0);


        glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, 1, &subroutine2);
        model = glm::translate(model, glm::vec3(1.0, 0.0, 0.0));
        SetUniform(mProgram, "model", model);
        drawTriangle();
    }

private:
    GLuint mTriangleVAO{};
    GLuint mTriangleVBO{};
    GLuint mProgram{};

    GLuint gridTexture{};

    GLuint subroutine1{};
    GLuint subroutine2{};

};


int main() {
    App app;
    app.Run();


    return 0;
}