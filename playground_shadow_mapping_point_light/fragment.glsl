#version 330 core

in vec4 FragPos;

uniform vec3 lightPosition;
uniform float farPlane;

void main() {

    float distance = length(FragPos.xyz - lightPosition);

    float depth = distance / farPlane;

    gl_FragDepth = depth;
}
