#version 460 core

out vec4 FragmentColor;

in struct Info {
    vec3 Normal;
    vec2 TexCoord;
    vec3 FragmentWorldPos;
} info;

// material parameters
// 一般来说，albedo 反射率贴图和 ambient occlusion 环境光遮蔽贴图是在 sRGB 空间中的，进行光照计算前需要把他们转换到线性空间中
// 而 金属度贴图和粗糙度贴图已经是存放在RGB 空间中的
uniform sampler2D albedoMap;
uniform sampler2D normalMap;
uniform sampler2D metallicMap;
uniform sampler2D roughnessMap;
uniform sampler2D aoMap;

uniform vec3 viewerPos;

// lights
uniform vec3 lightPositions[4];
uniform vec3 lightColors[4];

const float PI = 3.1415926;

// easy trick to get tangent normal to world space to keep code easy
vec3 getNormalFormMap() {
    // [0, 1] -> [-1, 1]
    vec3 tagentNormal = texture(normalMap, info.TexCoord).xyz * 2.0 - 1.0;
    vec3 q1 = dFdx(info.FragmentWorldPos);
    vec3 q2 = dFdy(info.FragmentWorldPos);
    vec2 st1 = dFdx(info.TexCoord);
    vec2 st2 = dFdy(info.TexCoord);

    vec3 N = normalize(info.Normal);
    vec3 T = normalize(q1 * st2.t - q2 * st1.t);
    vec3 B = -normalize(cross(N, T));
    mat3 TBN = mat3(T, B, N);

    return normalize(TBN * tagentNormal);
}


// D
float DistributionGGX(vec3 N, vec3 H, float roughness) {
    float alpha2 = pow(roughness, 4);
    float NdotH2 = pow(max(dot(N, H), 0.0), 2);

    return alpha2 / (PI * pow(NdotH2 * (alpha2 - 1) + 1, 2));
}

// G
float GeometrySchlickGGX(float NdotV, float roughness) {
    float k = pow(roughness + 1, 2) / 8.0;
    return NdotV / (NdotV * (1.0 - k) + k);
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness) {
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}


// F
vec3 fresnelSchlick(float cosTheta, vec3 F0) {
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

void main() {

    vec3 albedo = pow(texture(albedoMap, info.TexCoord).rgb, vec3(2.2));
    float metallic = texture(metallicMap, info.TexCoord).r;
    float roughness = texture(roughnessMap, info.TexCoord).r;
    float ao = texture(aoMap, info.TexCoord).r;

    vec3 N = normalize(info.Normal);
    vec3 V = normalize(viewerPos - info.FragmentWorldPos);

    // caclulate reflectance at normal incidence
    // if dia-electric use F0 of 0.04 and if a metal, use the albedo color as F0
    // metallic workflow
    vec3 F0 = vec3(0.04);
    F0 = mix(F0, albedo, metallic);

    // reflectance equation
    vec3 Lo = vec3(0.0);
    for (int i = 0; i < 4; i++) {
        // calculate per-light radiance
        vec3 L = normalize(lightPositions[i] - info.FragmentWorldPos);
        vec3 H = normalize(V + L);
        float distance = length(lightPositions[i] - info.FragmentWorldPos);
        float attenuation = 1.0 / (distance * distance);
        vec3 radians = lightColors[i] * attenuation;

        // Cook-Torrance BRDF
        float NDF = DistributionGGX(N, H, roughness);
        float G = GeometrySmith(N, V, L, roughness);
        vec3 F = fresnelSchlick(clamp(dot(H, V), 0.0, 1.0), F0);

        vec3 numerator = NDF * F * G;
        float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.001;
        vec3 specular = numerator / denominator;

        vec3 Ks = F;
        vec3 Kd = vec3(1.0) - Ks;
        // pure metals has no diffuse light
        Kd *= 1.0 - metallic;

        float NdotL = max(dot(N, L), 0.0);
        Lo += (Kd * albedo / PI + specular) * radians * NdotL;
    }

    // ambient lighting -> envirment lighting
    vec3 ambient = vec3(0.03) * albedo * ao;

    vec3 color = ambient + Lo;

    // HDR tonemapping
    color = color / (color + vec3(1.0));
    // gamma correct
    color = pow(color, vec3(1.0 / 2.2));

    FragmentColor = vec4(color, 1.0);

}
























