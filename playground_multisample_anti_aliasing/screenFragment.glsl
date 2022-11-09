#version 330 core

in vec2 vTexCoord;

out vec4 fragmentColor;

uniform sampler2D screenTexture;

void main() {
    vec3 color = texture(screenTexture, vTexCoord).rgb;
    float gray = 0.2 * color.r + 0.79 * color.g + 0.01 * color.b;
    fragmentColor = vec4(vec3(gray), 1.0);
//    fragmentColor = vec4(color, 1.0);
}
