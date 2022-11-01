#version 330 core

/*
    位置向量都以 Position 结尾
    方向向量都以 Vec 结尾
    颜色向量以 Color 结尾
 */


out vec4 fragmentColor;

// 纹理坐标
in vec2 vTexCoord;
// 经过标准化的法向量
in vec3 vNormal;
in vec3 vFragmentPosition;

struct PointLight {
    // 点光源的位置向量
    vec3 lightPosition;
    // 点光源的颜色向量
    vec3 lightColor;

    // 点光源对冯氏光照模型各分量强度
    vec3 lightAmbient;
    vec3 lightDiffuse;
    vec3 lightSpecular;

    // 光照衰减系数
    float constant;
    float linear;
    float quadratic;
};

#define NR_POINT_LIGHTS 4
uniform PointLight pointLights[NR_POINT_LIGHTS];

/*
 * 平行光和聚光先不考虑光照衰减
 */
struct DirLight {
    // 平行光方向向量
    vec3 dirLightVec;

    // 平行光颜色向量
    vec3 dirLightColor;

    // 平行光对冯氏光照模型各分量强度
    vec3 dirLightAmbient;
    vec3 dirLightDiffuse;
    vec3 dirLightSpecular;
};

uniform DirLight dirLight;

struct SpotLight {
    // 聚光光源的位置向量
    vec3 spotLightPosition;

    // 聚光方向向量
    vec3 spotLightVec;

    // 聚光颜色
    vec3 spotLightColor;

    // 聚光 内切角 与 外切角 的余弦值
    float cutoff;
    float outerCutoff;

    // 聚光对冯氏光照模型各分量强度
    vec3 spotLightAmbient;
    vec3 spotLightDiffuse;
    vec3 spotLightSpecular;

    // 光照衰减系数
    float constant;
    float linear;
    float quadratic;
};

uniform SpotLight spotLight;

// 摄像机（观察者）的位置向量
uniform vec3 cameraPosition;

// diffuse map   specular map
uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;


// 计算各个光源对最终片段的颜色贡献
vec3 calcPointLight(PointLight light, vec3 normalVec, vec3 fragmentPosition, vec3 viewerPosition) {
    vec3 ambientColor = light.lightColor * light.lightAmbient * texture(texture_diffuse1, vTexCoord).rgb;

    // 片段指向光源的方向向量
    vec3 lightVec = normalize(light.lightPosition - fragmentPosition);
    float diff = max(dot(lightVec, normalize(normalVec)), 0.0);
    vec3 diffuseColor = light.lightColor * diff * light.lightDiffuse * texture(texture_diffuse1, vTexCoord).rgb;

    // 片段指向观察者的方向向量
    float shiness = 32.0;
    vec3 viewerVec = normalize(viewerPosition - fragmentPosition);
    vec3 reflectVec = normalize(reflect(normalize(-lightVec), normalize(normalVec)));
    float spec = pow(max(dot(viewerVec, reflectVec), 0.0), shiness);
    vec3 specularColor = light.lightColor * light.lightSpecular * spec * texture(texture_specular1, vTexCoord).rgb;

    // attenuation
    float distance = length(light.lightPosition - fragmentPosition);
    float attr = 1.0 / (light.constant + light.linear * distance + light.quadratic * distance * distance);

    ambientColor *= attr;
    diffuseColor *= attr;
    specularColor *= attr;

    return ambientColor + diffuseColor + specularColor;
}

vec3 calcDirLight(DirLight light, vec3 normalVec, vec3 fragmentPosition, vec3 viewerPosition) {
    vec3 ambientColor = light.dirLightColor * light.dirLightAmbient * texture(texture_diffuse1, vTexCoord).rgb;

    // 片段指向光源的方向向量
    vec3 lightVec = normalize(-light.dirLightVec);
    float diff = max(dot(lightVec, normalize(normalVec)), 0.0);
    vec3 diffuseColor = light.dirLightColor * diff * light.dirLightDiffuse * texture(texture_diffuse1, vTexCoord).rgb;

    // 片段指向观察者的方向向量
    float shiness = 64.0;
    vec3 viewerVec = normalize(viewerPosition - fragmentPosition);
    vec3 reflectVec = normalize(reflect(normalize(-lightVec), normalize(normalVec)));
    float spec = pow(max(dot(viewerVec, reflectVec), 0.0), shiness);
    vec3 specularColor = light.dirLightColor * light.dirLightSpecular * spec * texture(texture_specular1, vTexCoord).rgb;

    return ambientColor + diffuseColor + specularColor;
}

vec3 calcSpotLight(SpotLight light, vec3 normalVec, vec3 fragmentPosition, vec3 viewerPosition) {
    vec3 ambientColor = light.spotLightColor * light.spotLightAmbient * texture(texture_diffuse1, vTexCoord).rgb;

    vec3 lightVec = normalize(light.spotLightPosition - fragmentPosition);
    float diff = max(dot(lightVec, normalize(normalVec)), 0.0);
    vec3 diffuseColor = light.spotLightColor * light.spotLightDiffuse * diff * texture(texture_diffuse1, vTexCoord).rgb;

    float shiness = 32.0;
    vec3 viewerVec = normalize(viewerPosition - fragmentPosition);
    vec3 reflectVec = normalize(reflect(-lightVec, normalize(normalVec)));
    float spec = pow(max(dot(reflectVec, viewerVec), 0.0), shiness);
    vec3 specularColor = light.spotLightColor * light.spotLightSpecular * spec * texture(texture_specular1, vTexCoord).rgb;

    // cutoff outterCutoff
    float theta = dot(lightVec, -light.spotLightVec);
    float epsilon = light.cutoff - light.outerCutoff;
    float factor = clamp((theta - light.outerCutoff) / epsilon, 0.0, 1.0);

    ambientColor *= factor;
    diffuseColor *= factor;
    specularColor *= factor;

    return ambientColor + diffuseColor + specularColor;
}



void main() {

    vec3 result = vec3(0.0, 0.0, 0.0);

    result += calcDirLight(dirLight, vNormal, vFragmentPosition, cameraPosition);

    for (int i = 0; i < NR_POINT_LIGHTS; i++) {
        result += calcPointLight(pointLights[i], vNormal, vFragmentPosition, cameraPosition);
    }

    result += calcSpotLight(spotLight, vNormal, vFragmentPosition, cameraPosition);

    // result color
    fragmentColor = vec4(result, 1.0);
}




















