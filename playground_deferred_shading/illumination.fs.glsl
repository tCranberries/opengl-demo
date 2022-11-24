#version 330 core

out vec4 fragmentColor;

in vec2 vTexCoord;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedo;

struct Light {
    vec3 Position;
    vec3 Color;

    float linear;
    float quadratic;
};

const int NR_LIGHTS = 32;
uniform Light lights[NR_LIGHTS];
uniform vec3 viewPosition;

void main() {
    // retrieve data from g buffer
    vec3 fragPosition = texture(gPosition, vTexCoord).rgb;
    vec3 normal = texture(gNormal, vTexCoord).rgb;
    vec3 diffuse = texture(gAlbedo, vTexCoord).rgb;

    vec3 res = vec3(0.0);
    vec3 ambient = diffuse * 0.2;
    res += ambient;

//    vec3 viewDirection = vec3(viewPosition - fragPosition);
    for (int i = 0; i < NR_LIGHTS; i++) {
        //
        vec3 lightDirection = vec3(lights[i].Position - fragPosition);
        float diff = max(dot(lightDirection, normal), 0.0);
        vec3 tmpDiffuse = diff * diffuse * lights[i].Color;

        // attenuation
        float distance = length(lights[i].Position - fragPosition);
        float attenuation = 1.0 / (1.0 + distance * lights[i].linear + distance * distance * lights[i].quadratic);

        tmpDiffuse *= attenuation;
        res += tmpDiffuse;
    }

    fragmentColor = vec4(res, 1.0);

}


















