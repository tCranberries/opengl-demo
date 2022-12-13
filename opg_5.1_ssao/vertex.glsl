#version 460 core

// ssao geometry vertex shader
layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

out struct Info {
    vec3 vNormal;
    vec2 vTexCoord;
    // fragment pos in view space
    vec3 vFragmentViewSpacePos;
} info;

uniform bool inverseNormal;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main() {
    // 所有的变量都必须在声明的时候进行初始化
    mat3 normalMatrix = transpose(inverse(mat3(view * model)));

    info.vTexCoord = aTexCoord;
    info.vNormal = normalMatrix * (inverseNormal ? -aNormal : aNormal);
    info.vFragmentViewSpacePos = vec3(view * model * vec4(aPosition, 1.0));
    gl_Position = projection * view * model * vec4(aPosition, 1.0);
}
