#version 460 core

// MRT gBuffer
layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec3 gAlbedo;

in struct Info {
    vec3 vNormal;
    vec2 vTexCoord;
// fragment pos in view space
    vec3 vFragmentViewSpacePos;
} info;


void main() {
    gPosition = info.vFragmentViewSpacePos;
    gNormal = normalize(info.vNormal);

    // and the diffuse per-fragment color
    gAlbedo = vec3(0.95);
}