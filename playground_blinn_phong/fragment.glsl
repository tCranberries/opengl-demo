#version 330 core

in vec2 vTexCoord;
in vec3 vNormal;
in vec3 vFragmentPosition;

out vec4 fragmentColor;

uniform sampler2D texture1;

uniform vec3 cameraPosition;
uniform vec3 lightPosition;
uniform bool blinn;

void main() {

    vec3 color = texture(texture1, vTexCoord).rgb;

    // ambient
    vec3 ambient = 0.05 * color;
    // diffuse
    vec3 lightVec = normalize(lightPosition - vFragmentPosition);
    vec3 normal = normalize(vNormal);
    float diff = max(dot(lightVec, normal), 0.0);
    vec3 diffuse = diff * color;

    // specular
    vec3 viewVec = normalize(cameraPosition - vFragmentPosition);

    float spec = 0.0;
    if (blinn) {
        // blinn-phong
        // 半程向量与法向量的约束点乘
        vec3 halfVec = normalize(lightVec + viewVec);
        spec = pow(max(dot(halfVec, normal), 0.0), 32.0);
    }
    else {
        // phong
        // 观察者向量与反射向量的约束点乘
        vec3 reflectVec = normalize(reflect(-lightVec, normal));
        spec = pow(max(dot(reflectVec, viewVec), 0.0), 8.0);
    }

    vec3 specular = vec3(0.3) * spec;

    fragmentColor = vec4(ambient + diffuse + specular, 1.0);
}
