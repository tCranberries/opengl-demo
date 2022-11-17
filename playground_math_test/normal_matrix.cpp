//
// Created by wzw on 2022/11/17.
//

#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

int main() {

    glm::mat4 model(1.0f);
    model = glm::translate(model, glm::vec3(1.0f, 2.0f, 3.0f));
    model = glm::rotate(model, glm::radians(45.0f), glm::vec3(1.0f, 0.0f, 0.0f));
//    model = glm::scale(model, glm::vec3(1.0f, 2.0f, 2.0f));

    glm::mat3 normalMat = glm::transpose(glm::inverse(glm::mat3(model)));

    glm::vec3 tangent(2.0f, 0.0f, 0.0f);

    glm::vec3 t1 = glm::vec3(glm::normalize(model * glm::vec4(tangent, 0.0f)));
    glm::vec3 t2 = glm::normalize(normalMat * tangent);


    glm::vec3 f1;
    return 0;
}