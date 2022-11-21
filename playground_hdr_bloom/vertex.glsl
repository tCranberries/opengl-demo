#version 330 core

// render scene
layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

out VS_OUT {
    vec2 vTexCoord;
    vec3 vNormal;
    vec3 vFragPos;
} vsout;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform mat3 normalMat;

void main() {
    gl_Position = projection * view * model * vec4(aPosition, 1.0);
    vsout.vFragPos = vec3(model * vec4(aPosition, 1.0));
    vsout.vTexCoord = aTexCoord;
    vsout.vNormal = normalize(normalMat * aNormal);
}
