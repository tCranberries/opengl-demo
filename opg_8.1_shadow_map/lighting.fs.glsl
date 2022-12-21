#version 460 core

layout (location = 0) out vec4 fragmentColor;

in struct Info {
    vec3 vNormal;
    vec2 vTexCoord;
    vec3 vFragWorldSpacePos;
    vec4 vFragLightSpacePos;
} info;


// calculate lighting in world space
uniform vec3 lightWorldSpacePos;
uniform vec3 lightColor;
uniform vec3 viewerWorldSpacePos;

uniform sampler2D texture_diffuse1;
uniform sampler2D shadowMap;


const float shinness = 32.0;


float calcShadow(float diff) {
    vec3 curFragCoordInLight = info.vFragLightSpacePos.xyz / info.vFragLightSpacePos.w;
    curFragCoordInLight = curFragCoordInLight * 0.5 + 0.5;
    float curDepth = curFragCoordInLight.z;
    float clostDepth = texture(shadowMap, curFragCoordInLight.xy).r;
    float bias = max(0.005 * (1.0 - diff), 0.0005);

    float shadow = 0.0;
    vec2 texSize = 1.0 / textureSize(shadowMap, 0).xy;
    for (int i = -1; i < 2; i++) {
        for (int j = -1; j < 2; j++) {
            float clostDepth = texture(shadowMap, curFragCoordInLight.xy + vec2(i, j) * texSize).r;
            shadow += curDepth > clostDepth + bias ? 0.8 : 0.0;
        }
    }
    shadow /= 9.0;

    if (curFragCoordInLight.z > 1.0) {
        shadow = 0.0;
    }

    return shadow;
}


void main() {
    vec3 res = vec3(0.0);
    vec3 normal = normalize(info.vNormal);
    vec3 lightDir = normalize(lightWorldSpacePos - info.vFragWorldSpacePos);
    vec3 viewDir = normalize(viewerWorldSpacePos - info.vFragWorldSpacePos);
    vec3 diffuseColor = texture(texture_diffuse1, info.vTexCoord).rgb;

    // ambient
    vec3 ambient = 0.2 * diffuseColor;
    // diffuse
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * diffuseColor;

    // specular
    vec3 halfDir = normalize(viewDir + lightDir);
    float spec = pow(max(dot(halfDir, normal), 0.0), shinness);
    vec3 specular = spec * vec3(0.3);
    //
    float shadow = calcShadow(diff);

    res = (ambient + (diffuse + specular) * (1 - shadow)) * lightColor;

    // gamma correction
//    res = pow(res, vec3(1.0 / 2.2));

    fragmentColor = vec4(res, 1.0);
}














