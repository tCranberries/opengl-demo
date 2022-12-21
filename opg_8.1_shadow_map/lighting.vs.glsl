#version 460 core

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

out struct Info {
    vec3 vNormal;
    vec2 vTexCoord;
    vec3 vFragWorldSpacePos;
    vec4 vFragLightSpacePos;
} info;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform mat3 normalMatrix;

uniform mat4 lightProjection;
uniform mat4 lightView;

void main() {
    info.vNormal = normalMatrix * aNormal;
    info.vTexCoord = aTexCoord;
    info.vFragWorldSpacePos = vec3(model * vec4(aPosition, 1.0));
    info.vFragLightSpacePos = lightProjection * lightView * model * vec4(aPosition, 1.0);
    gl_Position = projection * view * model * vec4(aPosition, 1.0);
}
