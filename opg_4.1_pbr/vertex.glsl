#version 460 core

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

out struct Info {
    vec3 Normal;
    vec2 TexCoord;
    // fragment pos in world space
    vec3 FragmentWorldPos;
} info;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform mat3 normalMatrix;

void main() {
    // 所有的变量都必须在声明的时候进行初始化
    info.TexCoord = aTexCoord;
    info.Normal = normalMatrix * aNormal;
    info.FragmentWorldPos = vec3(model * vec4(aPosition, 1.0));
    gl_Position = projection * view * model * vec4(aPosition, 1.0);
}
