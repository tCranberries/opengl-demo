#version 460 core

out vec4 FragColor;

in vec2 vTexCoord;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedo;
uniform sampler2D ssao;

struct Light {
    vec3 Position;
    vec3 Color;

    float Linear;
    float Quadratic;
};

uniform Light light;

void main() {

    vec3 fragPos = texture(gPosition, vTexCoord).xyz;
    vec3 normal = normalize(texture(gNormal, vTexCoord).xyz);
    vec3 diffuseColor = texture(gAlbedo, vTexCoord).rgb;
    float ambientOcclusion = texture(ssao, vTexCoord).r;

    // calculte lighting
    vec3 ambient = vec3(0.3 * diffuseColor * ambientOcclusion);
    vec3 lighting = ambient;

    // viewPos is 0.0 in view space
    vec3 viewDir = normalize(-fragPos);

    // diffuse color
    vec3 lightDir = normalize(light.Position - fragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diff * light.Color;

    // specular
    vec3 halfDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(halfDir, normal), 0.0), 8.0);
    vec3 specular = light.Color * spec;

    // attenuation
    float distan = length(light.Position - fragPos);
    float attenuation = 1.0 / (1.0 + light.Linear * distan + light.Quadratic * distan * distan);

    diffuse *= attenuation;
    specular *= attenuation;

    lighting += diffuse + specular;

    FragColor = vec4(lighting, 1.0);
}






















