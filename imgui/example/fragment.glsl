#version 460 core

out vec4 fragmentColor;

in vec2 vTexCoord;
in vec3 vColor;

subroutine vec3 RenderPassType();
subroutine uniform RenderPassType RenderPass;

uniform sampler2D gridTexture;

vec3 colorBias() {
    return vec3(0.2) * RenderPass();
}

subroutine (RenderPassType)
vec3 renderWithTex() {
    return texture(gridTexture, vTexCoord).rgb;
}

subroutine (RenderPassType)
vec3 renderWithoutTex() {
    return vColor;
}


void main() {

    vec3 color = colorBias();

    fragmentColor = vec4(color, 1.0);
}
