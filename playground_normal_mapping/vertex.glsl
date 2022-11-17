#version 330 core

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in vec3 aTangent;

out VS_OUT {
    vec2 vTexCoord;
    vec3 vLightPosTanSpace;
    vec3 vViewPosTanSpace;
    vec3 vFragPosTanSpace;
} vsout;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform mat3 normalMat;

uniform vec3 lightPos;
uniform vec3 viewPos;

void main() {
    gl_Position = projection * view * model * vec4(aPosition, 1.0);
    vec3 fragPos = vec3(model * vec4(aPosition, 1.0));
    vsout.vTexCoord = aTexCoord;

    vec3 N = normalize(normalMat * aNormal);
    vec3 T = normalize(normalMat * aTangent);
    T = normalize(T - dot(N, T) * N);
    vec3 B = cross(N, T);
    mat3 TBN = transpose(mat3(T, B, N));

    vsout.vLightPosTanSpace = TBN * lightPos;
    vsout.vViewPosTanSpace = TBN * viewPos;
    vsout.vFragPosTanSpace = TBN * fragPos;
}
