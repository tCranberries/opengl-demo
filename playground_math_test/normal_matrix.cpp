//
// Created by wzw on 2022/11/17.
//

#include <iostream>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <random>
#include <ctime>

void generateRandom() {

    std::uniform_real_distribution<float> floats(0.1, 1.0);
    std::random_device device;
    std::default_random_engine generator(device());
    for (int i = 0; i < 10; i++) {
        std::cout << floats(generator) << "  " << std::endl;
    }

}


int main() {

    generateRandom();

    return 0;
}