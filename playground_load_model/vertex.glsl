#version 330 core

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;

// out block
out VS_OUT {
    vec2 vTexCoord;
    vec3 vNormal;
    vec3 vFragmentPosition;
} vsOut;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform mat3 norMatrix;

void main() {
    gl_Position = projection * view * model * vec4(aPosition, 1.0);

    // 片段位置
    vsOut.vFragmentPosition = vec3(model * vec4(aPosition, 1.0));
    // 该点在世界空间中的 法向量
    vsOut.vNormal = normalize(norMatrix * aNormal);
    // 纹理坐标
    vsOut.vTexCoord = aTexCoord;
}
