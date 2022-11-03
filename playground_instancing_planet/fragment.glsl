#version 330 core

in vec2 vTexCoord;

uniform sampler2D texture_diffuse1;

out vec4 fragmentColor;

void main() {

    fragmentColor = texture(texture_diffuse1, vTexCoord);
}
