#version 330 core

in vec2 vTexCoord;

uniform sampler2D colorBuffer;
uniform sampler2D bloomLight;
uniform bool enableBloom;

out vec4 fragmentColor;

void main() {

    vec3 color = texture(colorBuffer, vTexCoord).rgb;
    vec3 bloom = texture(bloomLight, vTexCoord).rgb;

    if (enableBloom) {
        color += bloom;
    }

    float exposure = 1.0;
    float gamma = 2.2;
    vec3 res = vec3(1.0) - exp(-color * exposure);
    res = pow(res, vec3(1.0 / 2.2));

    fragmentColor = vec4(res, 1.0);
}
