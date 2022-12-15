
#include "app_base.h"

#include <random>

class App : public AppBase {
public:
    App() = default;

    ~App() override {
        glDeleteBuffers(1, &mTriangleVBO);
        glDeleteVertexArrays(1, &mTriangleVAO);
        glDeleteProgram(mProgram);
    };

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

    void drawTriangle() {
        if (mTriangleVAO == 0) {

            std::random_device device;
            std::uniform_real_distribution<float> floats(0.1, 1.0);
            std::default_random_engine engine(device());

            std::vector<glm::vec3> vertices{};
            for (int i = 0; i < 10; i++) {
                vertices.emplace_back(
                        floats(engine) * 2.0 - 1.0,
                        floats(engine) * 2.0 - 1.0,
                        floats(engine) * 2.0 - 1.0
                        );
            }
            numPoints = vertices.size();
            glGenVertexArrays(1, &mTriangleVAO);
            glBindVertexArray(mTriangleVAO);
            glGenBuffers(1, &mTriangleVBO);
            glBindBuffer(GL_ARRAY_BUFFER, mTriangleVBO);
            glBufferData(GL_ARRAY_BUFFER, (long)(vertices.size() * 3 * sizeof(float)), &vertices[0], GL_STATIC_DRAW);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindVertexArray(0);
        }
        glBindVertexArray(mTriangleVAO);
        glDrawArrays(GL_POINTS, 0, (int)numPoints);
        glBindVertexArray(0);
    }


    // Anything that needs to be called once outside the main application loop
    void StartUp() override {
        const char* vertexPath = "/home/w/CLionProjects/opengl-demo/opg_7.1_point_sprite/vertex.glsl";
        const char* geometryPath = "/home/w/CLionProjects/opengl-demo/opg_7.1_point_sprite/geometry.glsl";
        const char* fragmentPath = "/home/w/CLionProjects/opengl-demo/opg_7.1_point_sprite/fragment.glsl";
        const char* texturePath =  "/home/w/CLionProjects/opengl-demo/resource/textures/brickwall.jpg";
        tex1 = LoadTexture(texturePath);

        mProgram = LoadShaders(vertexPath, geometryPath, fragmentPath);
        glUseProgram(mProgram);
        SetUniform(mProgram, "texture1", 0);

        camera->setPosition(glm::vec3(0.0f, 0.0f, 3.0f));

        glEnable(GL_DEPTH_TEST);
    }

    // Anything that needs to be called cyclically inside the main application loop
    void Update() override {

        {
            static int counter = 0;

            ImGui::Begin("Point Sprite!");                          // Create a window called "Hello, world!" and append into it.

            ImGui::Text("this is point sprite parameter control");               // Display some text (you can use a format strings too)
//            ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
//            ImGui::Checkbox("Another Window", &show_another_window);

            ImGui::SliderFloat("size2", &size2, 0.0f, 5.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
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
        SetUniform(mProgram, "size2", size2);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, tex1);
        drawTriangle();
    }

private:
    GLuint mTriangleVAO{};
    GLuint mTriangleVBO{};

    GLuint mProgram{};

    GLfloat size2 = 0.1;
    GLuint numPoints{};
    GLuint tex1{};
};


int main() {
    App app;
    app.Run();


    return 0;
}