#version 460 core

const float PI = 3.1415926;

layout (location = 0) out vec4 fragmentColor;

in vec3 vColor;


void main() {
    float factor = 1.0 + 0.5 * cos(gl_FragCoord.x * 100 / PI);
    fragmentColor = vec4(factor * vColor, 1.0);
}
