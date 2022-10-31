#version 430 core

in vec2 vTexCoord;

uniform sampler2D texture1;

out vec4 fragmentColor;


const float offset = 1.0 / 300.0;
const int count = 9;

void main() {
    // offsets
    vec2 offsets[count] = vec2[](
        vec2(-offset, offset),
        vec2(0.0, offset),
        vec2(offset, offset),
        vec2(-offset, 0.0),
        vec2(0.0, 0.0),
        vec2(offset, 0.0),
        vec2(-offset, -offset),
        vec2(0.0, -offset),
        vec2(offset, -offset)
    );

    // convolution matrix
    const float sharpenKernel[count] = float[](
        -1, -1, -1,
        -1, 9, -1,
        -1, -1, -1
    );

    const float blurKernel[count] = float[](
        1 / 16.0, 2 / 16.0, 1 / 16.0,
        2 / 16.0, 4 / 16.0, 2 / 16.0,
        1 / 16.0, 2 / 16.0, 1 / 16.0
    );

    const float edageDetectionKernel[count] = float[](
        1, 1, 1,
        1, -8, 1,
        1, 1, 1
    );


    vec3 samplers[count];
    for (int i = 0; i < count; i++) {
        samplers[i] = vec3(texture(texture1, vTexCoord.st + offsets[i]));
    }

    // 一定要对局部变量进行初始化
    vec3 color = vec3(0.0);
    for (int i = 0; i < count; i++) {
        color += samplers[i] * edageDetectionKernel[i];
    }

    fragmentColor = vec4(color, 1.0);

}
