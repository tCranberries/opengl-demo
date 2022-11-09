//
// Created by wzw on 22-10-26.
//

#include <iostream>
#include <glm/glm.hpp>

int main() {

    glm::vec3 origin(0.5f, 0.6f, 0.7f);

    glm::vec3 color(1.0f, 1.0f, 1.0f);
    glm::vec3 diffuse(0.4f, 0.4f, 0.4f);
    float diff = 0.4f;

    glm::vec3 res1 = origin * diffuse * diff;
    glm::vec3 res2 = origin * diffuse * diff * color;


    std::cout << "done" << std::endl;

    return 0;
}