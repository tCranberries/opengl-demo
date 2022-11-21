#version 330 core

in vec2 vTexCoord;

out vec4 fragmentColor;

uniform sampler2D image;
uniform bool horizontal;
uniform float wight[5] = float[] (0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216);

void main() {

    vec2 texCoordOffset = 1.0 / textureSize(image, 0);
    vec3 res = texture(image, vTexCoord).rgb *  wight[0];

    if (horizontal) {
        for (int i = 1; i < 5; i++) {
            res += texture(image, vTexCoord + vec2(texCoordOffset.x * i, 0.0)).rgb * wight[i];
            res += texture(image, vTexCoord - vec2(texCoordOffset.x * i, 0.0)).rgb * wight[i];
        }
    }
    else {
        for (int i = 1; i < 5; i++) {
            res += texture(image, vTexCoord + vec2(0.0, texCoordOffset.y * i)).rgb * wight[i];
            res += texture(image, vTexCoord - vec2(0.0, texCoordOffset.y * i)).rgb * wight[i];
        }
    }

    fragmentColor = vec4(res, 1.0);

}
