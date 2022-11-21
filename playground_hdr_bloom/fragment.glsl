#version 330 core

// render scene

in VS_OUT {
    vec2 vTexCoord;
    vec3 vNormal;
    vec3 vFragPos;
} vsin;

out vec4 fragmentColor;

uniform sampler2D diffuseTexture;
uniform vec3 lightPos[4];
uniform vec3 lightColors[4];
uniform vec3 viewPos;

vec3 calcLight(vec3 normal, vec3 lightPosision, vec3 lightColor, vec3 fragPos, vec3 viewPos) {
    // ambient
    vec3 ambient = 0.0 * lightColor;
    // diffuse
    vec3 lightDir = normalize(lightPosision - fragPos);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * lightColor;

    // specular blinn phong
    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 halfDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(halfDir, normal), 0.0), 16.0);
    vec3 specular = spec * lightColor;

    // attenaution
    float distance = length(lightPosision - fragPos);
    float attenaution = 1.0 / (distance * distance);

    vec3 res = ambient + diffuse + specular;
    return res * attenaution;
}

void main() {

    vec3 color = texture(diffuseTexture, vsin.vTexCoord).rgb;
    vec3 normal = normalize(vsin.vNormal);

    vec3 res = vec3(0.0);
    for (int i = 0; i < 4; i++) {
        res += calcLight(normal, lightPos[i], lightColors[i], vsin.vFragPos, viewPos);
    }
    res *= color;

    fragmentColor = vec4(res, 1.0);
}
