#version 330 core

// 使用深度贴图 创建阴影效果的 vs
layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

out VS_DATA {
    vec2 vTexCoord;
    vec3 vNormal;
    vec3 vFragmentPosition;
    vec4 vFragPosLightSpace;
} vsout;


uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform mat3 normalMat;

uniform mat4 lightSpaceMat;

void main() {


    // Vertex Shader的输出就是在Clip Space上, 接着由GPU自己做透视除法将顶点转到NDC
    // https://zhuanlan.zhihu.com/p/65969162
    gl_Position = projection * view * model * vec4(aPosition, 1.0);

    vsout.vTexCoord = aTexCoord;
    vsout.vNormal = normalMat * aNormal;
    vsout.vFragmentPosition = vec3(model * vec4(aPosition, 1.0));
    vsout.vFragPosLightSpace = lightSpaceMat * vec4(vsout.vFragmentPosition, 1.0);
}