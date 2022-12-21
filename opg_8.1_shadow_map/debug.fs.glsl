#version 460 core

layout (location = 0) out vec4 fragmentColor;

in vec2 vTexCoord;

uniform sampler2D shadowMap;

void main() {

    float value = texture(shadowMap, vTexCoord).r;
    fragmentColor = vec4(vec3(value), 1.0);
}
