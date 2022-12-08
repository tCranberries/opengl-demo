#version 460 core

subroutine vec3 lightModelType();
subroutine uniform lightModelType lightModel;

layout (location = 0) out vec4 fragmentColor;

in struct Info {
    vec3 vNormal;
    vec2 vTexCoord;
// fragment pos in world space
    vec4 vFragmentPos;
} vInfo;

uniform sampler2D texture_diffuse;
uniform sampler2D texture_specular;

uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 viewerPos;

// specular shiness
const float shiness = 32.0;
// light attenuation
const float constant = 1.0;
const float linear = 0.09;
const float quadratic = 0.032;

//
subroutine (lightModelType)
vec3 calculatePointLight() {

    vec3 texDiffuseColor = texture(texture_diffuse, vInfo.vTexCoord).rgb;
    vec3 texSpecularColor = texture(texture_specular, vInfo.vTexCoord).rgb;
    // ambient
    vec3 ambient = 0.05 * texDiffuseColor;

    // diffuse
    vec3 normal = normalize(vInfo.vNormal);
    vec3 lightDir = normalize(lightPos - vec3(vInfo.vFragmentPos));
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = lightColor * diff * texDiffuseColor;

    // specular  blinn-phone
    vec3 viewerDir = normalize(viewerPos - vec3(vInfo.vFragmentPos));
    vec3 halfwayDir = normalize(lightDir + viewerDir);
    float spec = pow(max(dot(halfwayDir, normal), 0.0), shiness);
    vec3 specular = lightColor * spec * texSpecularColor;

    // attenuation
    float dis = distance(lightPos, vec3(vInfo.vFragmentPos));
    float atten = 1.0 / (constant + dis * linear + dis * dis * quadratic);

    ambient *= atten;
    diffuse *= atten;
    specular *= atten;

    return ambient + diffuse + specular;
}

subroutine (lightModelType)
vec3 calculateNonAttenLight() {

    vec3 texDiffuseColor = texture(texture_diffuse, vInfo.vTexCoord).rgb;
    vec3 texSpecularColor = texture(texture_specular, vInfo.vTexCoord).rgb;
    // ambient
    vec3 ambient = 0.05 * texDiffuseColor;

    // diffuse
    vec3 normal = normalize(vInfo.vNormal);
    vec3 lightDir = normalize(lightPos - vec3(vInfo.vFragmentPos));
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = lightColor * diff * texDiffuseColor;

    // specular  blinn-phone
    vec3 viewerDir = normalize(viewerPos - vec3(vInfo.vFragmentPos));
    vec3 halfwayDir = normalize(lightDir + viewerDir);
    float spec = pow(max(dot(halfwayDir, normal), 0.0), shiness);
    vec3 specular = lightColor * spec * texSpecularColor;

    return ambient + diffuse + specular;
}


void main() {

    vec3 res = vec3(0.0);
    res = lightModel();

    fragmentColor = vec4(res, 1.0);
}





















