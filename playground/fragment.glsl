#version 330 core

in vec2 vTexCoord;

out vec4 fragmentColor;

//uniform float greenColor;

uniform sampler2D texture1;
uniform sampler2D texture2;

void main() {

    vec4 texColor = mix(texture(texture1, vTexCoord), texture(texture2, vTexCoord), 0.5);

    fragmentColor = texColor;
}
