//
// Created by wzw on 22-10-21.
//

#ifndef TUTORIALS_MESH_H
#define TUTORIALS_MESH_H

#include <iostream>
#include <cstddef>
#include <string>
#include <utility>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "shader.hpp"

const int MAX_BONE_INFLUENCE = 4;

struct Vertex {
    // position
    glm::vec3 position;
    // normal
    glm::vec3 normal;
    // texCoords
    glm::vec2 texCoords;
    // tangent
    glm::vec3 tangent;
    // biTangent
    glm::vec3 biTangent;
    // bone indices witch will influence this vertex
    int boneIDs[MAX_BONE_INFLUENCE];
    // weight from each bone
    float weights[MAX_BONE_INFLUENCE];
};

struct Texture {
    unsigned int id;
    std::string type;
    std::string path;
};

class Mesh {
public:
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    unsigned int VAO{};

    explicit Mesh(std::vector<Vertex> _vertices, std::vector<unsigned int> _indices, std::vector<Texture> _textures) {
        this->vertices = std::move(_vertices);
        this->indices = std::move(_indices);
        this->textures = std::move(_textures);

        setupMesh();
    }

    void draw(unsigned int program) {

        unsigned int diffuseNr = 1;
        unsigned int specularNr = 1;
        unsigned int normalNr = 1;
        unsigned int heightNr = 1;


        for (unsigned int i = 0; i < textures.size(); i++) {
            glActiveTexture(GL_TEXTURE0 + i);
            std::string number;
            std::string name = textures[i].type;
            if (name == "texture_diffuse") {
                number = std::to_string(diffuseNr++);
            }
            else if (name == "texture_specular") {
                number = std::to_string(specularNr++); // transfer unsigned int to string
            }
            else if (name == "texture_normal") {
                number = std::to_string(normalNr++);  // transfer unsigned int to string
            }
            else if (name == "texture_height") {
                number = std::to_string(heightNr++); // transfer unsigned int to string
            }

            SetUniform(program, (name + number).c_str(), (int)i);
            glBindTexture(GL_TEXTURE_2D, textures[i].id);
        }


        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, (int)indices.size(), GL_UNSIGNED_INT, nullptr);
        glBindVertexArray(0);

        // always good practice to set everything back to default once configured.
        glActiveTexture(GL_TEXTURE0);
    }


private:
    unsigned int VBO{};
    unsigned int EBO{};

    void setupMesh() {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, (long)(vertices.size() * sizeof(Vertex)), &vertices[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, (long)(indices.size() * sizeof(unsigned int)), &indices[0], GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));

        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tangent));

        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, biTangent));

        glEnableVertexAttribArray(5);
        glVertexAttribPointer(5, 4, GL_INT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, boneIDs));

        glEnableVertexAttribArray(6);
        glVertexAttribPointer(6, 4, GL_INT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, weights));

        glBindVertexArray(0);
    }

};


#endif //TUTORIALS_MESH_H
















