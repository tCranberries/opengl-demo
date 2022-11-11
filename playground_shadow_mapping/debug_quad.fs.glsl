#version 330 core

in vec2 vTexCoord;

uniform sampler2D texture1;

out vec4 FragmentColor;
//
void main() {

    float depthValue = texture(texture1, vTexCoord).r;
    FragmentColor = vec4(vec3(depthValue), 1.0);
}
