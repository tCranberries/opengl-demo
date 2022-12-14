//
// Created by wzw on 22-10-20.
//

#ifndef TUTORIALS_CAMERA_H
#define TUTORIALS_CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>


enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.05f;
const float ZOOM = 45.0f;

class Camera {
public:
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up{};
    glm::vec3 right;
    glm::vec3 worldUp;

    float yaw;
    float pitch;
    float movementSpeed;
    float mouseSensitivity;
    float zoom;

    explicit Camera(glm::vec3 _position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 _up = glm::vec3(0.0f, 1.0f, 0.0f),
           float _yaw = YAW,
           float _pitch = PITCH) :

           front(glm::vec3(0.0f, 0.0f, -1.0f)),
           right(glm::vec3(1.0f, 0.0f, 0.0f)),
           movementSpeed(SPEED),
           mouseSensitivity(SENSITIVITY),
           zoom(ZOOM),
           yaw(_yaw),
           position(_position),
           worldUp(_up),
           pitch(_pitch) {

        updateCameraVectors();
    }

    void setPosition(glm::vec3 pos) {
        position = pos;
    }

    [[nodiscard]] glm::mat4 getViewMatrix() const {
        return glm::lookAt(position, position + front, up);
    }

    void processKeyboard(Camera_Movement direction, float deltaTime) {
        float velocity = movementSpeed * deltaTime;
        if (direction == FORWARD) {
            position += front * velocity;
        }
        else if (direction == BACKWARD) {
            position -= front * velocity;
        }
        else if (direction == LEFT) {
            position -= right * velocity;
        }
        else if (direction == RIGHT) {
            position += right * velocity;
        }
    }

    void processMouseMovement(float xoffset, float yoffset, bool constrainPitch = true) {
        xoffset *= mouseSensitivity;
        yoffset *= mouseSensitivity;
        yaw += xoffset;
        pitch += yoffset;

        if (constrainPitch) {
            if (pitch > 89.0f) {
                pitch = 89.0f;
            }
            if (pitch < -89.0f) {
                pitch = -89.0f;
            }
        }

        updateCameraVectors();
    }


    void processMouseScroll(float xoffset, float yoffset) {
        zoom += (float)xoffset;
        zoom -= (float)yoffset;
        if (zoom < 1.0f) {
            zoom = 1.0f;
        }
        if (zoom > 45.0f) {
            zoom = 45.0f;
        }
    }

private:

    void updateCameraVectors() {
        glm::vec3 tmpFront;
        tmpFront.x = cosf(glm::radians(yaw)) * cosf(glm::radians(pitch));
        tmpFront.y = sinf(glm::radians(pitch));
        tmpFront.z = sinf(glm::radians(yaw)) * cosf(glm::radians(pitch));
        front = glm::normalize(tmpFront);

        right = glm::normalize(glm::cross(front, worldUp));
        up = glm::normalize(glm::cross(right, front));
    }
};












#endif //TUTORIALS_CAMERA_H















