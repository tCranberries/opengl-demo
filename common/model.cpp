
#include "model.h"
#include "stb_image.h"


unsigned int TextureFromFile(const char* path, const std::string& directory, bool gamma) {
    std::string fileName = std::string(path);
    fileName = directory + '/' + fileName;

    unsigned int textureID;
    glGenTextures(1, &textureID);
    int width{};
    int height{};
    int nChannels{};

    unsigned char* data = stbi_load(fileName.c_str(), &width, &height, &nChannels, 0);
    if (data) {
        GLenum format{};
        if (nChannels == 1) {
            format = GL_RED;
        }
        else if (nChannels == 3) {
            format = GL_RGB;
        }
        else if (nChannels == 4) {
            format = GL_RGBA;
        }
        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, (GLint)format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
    else {
        std::cout << "failed to create texture at path" << path << gamma << std::endl;
    }
    stbi_image_free(data);

    return textureID;
}
