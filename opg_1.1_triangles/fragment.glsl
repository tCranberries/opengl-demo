#version 460 core

in vec3 color;
out vec4 fragmentColor;

//uniform float greenColor;

void main() {

    fragmentColor = vec4(color, 1.0);
}
