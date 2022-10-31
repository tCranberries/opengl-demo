#version 430 core

in vec2 vTexCoord;

out vec4 fragmentColor;

uniform sampler2D texture1;

void main() {

    vec4 tmpColor = texture(texture1, vTexCoord);
    fragmentColor = tmpColor;
}
