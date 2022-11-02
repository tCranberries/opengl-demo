#version 330 core

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;

// out block
out VS_OUT {
    vec3 vNormal;
} vsOut;


uniform mat4 view;
uniform mat4 model;
uniform mat3 norMatrix;

void main() {
    gl_Position = view * model * vec4(aPosition, 1.0);
    // 该点在世界空间中的 法向量
    vsOut.vNormal = normalize(norMatrix * aNormal);
}
