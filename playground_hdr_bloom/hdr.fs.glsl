#version 330 core

in vec2 vTexCoord;

out vec4 fragmentColor;

uniform sampler2D hdrTexture;

void main() {

    vec3 hdrColor = texture(hdrTexture, vTexCoord).rgb;

    // two ways calcaulate  tone mapping
//    vec3 res = hdrColor / (hdrColor + vec3(1.0));
    float expose = 1.0;
    vec3 res = vec3(1.0) - exp(-hdrColor * expose);

    // gamma correction
    res = pow(res, vec3(1.0 / 2.2));
    fragmentColor = vec4(res, 1.0);
}