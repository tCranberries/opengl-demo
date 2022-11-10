#version 330 core

in vec2 vTexCoord;
in vec3 vNormal;
in vec3 vFragmentPosition;

out vec4 fragmentColor;

uniform sampler2D texture1;

uniform vec3 cameraPosition;
uniform vec3 lightPositions[4];
uniform vec3 lightColors[4];
uniform bool gamma;


vec3 calcLight(vec3 _normal, vec3 lightPosition, vec3 fragmentPosition, vec3 lightColor) {
    vec3 normal = normalize(_normal);
    vec3 lightDir = normalize(lightPosition - fragmentPosition);
    // diffuse
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * lightColor;

    // specular
    vec3 viewDir = normalize(cameraPosition - fragmentPosition);
    vec3 halfDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(halfDir, normal), 0.0), 64.0);
    vec3 specular = spec * lightColor;

    // attenuation
    float distance = length(lightPosition - fragmentPosition);
    float attenuation = 1.0 / (gamma ? distance * distance : distance);

    diffuse *= attenuation;
    specular *= attenuation;

    return diffuse + specular;
}

void main() {

    vec3 color = texture(texture1, vTexCoord).rgb;
    vec3 lighting = vec3(0.0);
    for (int i = 0; i < 4; i++) {
        lighting += calcLight(vNormal, lightPositions[i], vFragmentPosition, lightColors[i]);
    }
    color *= lighting;
    if (gamma) {
        color = pow(color, vec3(1 / 2.2));
    }

    fragmentColor = vec4(color, 1.0);
}
