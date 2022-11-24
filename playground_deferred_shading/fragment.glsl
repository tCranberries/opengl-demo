#version 330 core

// G-buffer MRT
layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec3 gAlbedo;

in VS_OUT {
    vec2 vTexCoord;
    vec3 vNormal;
    vec3 vFragPos;
} vsin;

uniform sampler2D diffuseTexture;

void main() {

    gPosition = vsin.vFragPos;
    gNormal = normalize(vsin.vNormal);
    gAlbedo = texture(diffuseTexture, vsin.vTexCoord).rgb;
}
