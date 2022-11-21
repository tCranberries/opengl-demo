#version 330 core

layout (location = 0) out vec4 fragmentColor;
layout (location = 1) out vec4 brightColor;

uniform vec3 lightColor;

void main() {

    float gray = dot(lightColor, vec3(0.2, 0.7, 0.1));
    if (gray <= 1.0) {
        brightColor = vec4(vec3(0.0), 1.0);
    }
    else {
        brightColor = vec4(lightColor, 1.0);
    }
    fragmentColor = vec4(lightColor, 1.0);
}
