#version 330 core

in VS_OUT {
    vec2 vTexCoord;
    vec3 vLightPosTanSpace;
    vec3 vViewPosTanSpace;
    vec3 vFragPosTanSpace;
} vsin;

out vec4 fragmentColor;

uniform sampler2D diffuseTexture;
uniform sampler2D normalMap;
uniform sampler2D displaceMap;

vec2 getOffsetCoords(vec2 coords, vec3 viewDir) {
    float height = texture(displaceMap, coords).r;
    vec2 p = viewDir.xy / viewDir.z * height * 0.1;
    return coords - p;
}

void main() {

    vec3 viewDir = normalize(vsin.vViewPosTanSpace - vsin.vFragPosTanSpace);
    vec2 offsetCoords = getOffsetCoords(vsin.vTexCoord, viewDir);
    if (offsetCoords.x < 0.0 || offsetCoords.x > 1.0 || offsetCoords.y < 0.0 || offsetCoords.y > 1.0) {
        discard;
    }

    vec3 color = texture(diffuseTexture, offsetCoords).rgb;
    vec3 normal = texture(normalMap, offsetCoords).rgb;
    normal = normalize(normal * 2.0 - 1.0);

    // ambient
    vec3 ambient = 0.1 * color;
    // diffuse
    vec3 lightDir = normalize(vsin.vLightPosTanSpace - vsin.vFragPosTanSpace);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * color;

    // specular blinn phong
    vec3 halfDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(halfDir, normal), 0.0), 32.0);
    vec3 specular = spec * vec3(0.2);

    vec3 res = ambient + diffuse + specular;

    fragmentColor = vec4(res, 1.0);
}
