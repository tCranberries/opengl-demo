//
// Created by wzw on 2022/11/16.
//

#include <iostream>
#include <glm/glm.hpp>


int main_tangent() {

    // positions
    glm::vec3 p1(-1.0f, 1.0f, 0.0f);
    glm::vec3 p2(-1.0f, -1.0f, 0.0f);
    glm::vec3 p3(1.0f, -1.0f, 0.0f);
    glm::vec3 p4(1.0f, 1.0f, 0.0f);

    // uv coordinates  texture coordinates
    glm::vec2 t1(0.0f, 1.0f);
    glm::vec2 t2(0.0f, 0.0f);
    glm::vec2 t3(1.0f, 0.0f);
    glm::vec2 t4(1.0f, 1.0f);

    glm::vec3 nm(0.0f, 0.0f, 1.0f);

    //
    glm::vec3 e1 = p2 - p1;
    glm::vec3 e2 = p3 - p1;
    glm::vec2 deltaUV1 = t2 - t1;
    glm::vec2 deltaUV2 = t3 - t1;

    glm::vec3 tangent;

    float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);
    tangent.x = f * (deltaUV2.y * e1.x - deltaUV1.y * e2.x);
    tangent.y = f * (deltaUV2.y * e1.y - deltaUV1.y * e2.y);
    tangent.z = f * (deltaUV2.y * e1.z - deltaUV1.y * e2.z);

    glm::vec3 biTangent = glm::cross(nm, tangent);

    std::cout << "done" << std::endl;
    return 0;
}





















