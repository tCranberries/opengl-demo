#version 460 core

layout (location = 0) out vec4 fragmentColor;

layout (binding = 1) uniform BlobSettings {
    vec4 InnerColor;
    vec4 OuterColor;
    float InnerRadius;
    float OuterRadius;
} blob;

in vec2 vTexCoord;

void main() {

    float dx = vTexCoord.x - 0.5;
    float dy = vTexCoord.y - 0.5;
    float dis = sqrt(dx * dx + dy * dy);

    fragmentColor = mix(blob.InnerColor, blob.OuterColor, smoothstep(blob.InnerRadius, blob.OuterRadius, dis));
}
