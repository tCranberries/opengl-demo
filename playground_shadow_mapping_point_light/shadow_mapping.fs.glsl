#version 330 core

in VS_DATA {
    vec2 vTexCoord;
    vec3 vNormal;
    vec3 vFragmentPosition;
} vsin;

out vec4 fragmentColor;

// 当出现多个纹理时，必须指定纹理的顺序，即哪个纹理绑定到 0 上？哪个绑定到 1 上
uniform sampler2D texture1;   // GL_TEXTURE0
uniform samplerCube depthMap;   // GL_TEXTURE1

uniform vec3 cameraPosition;
uniform vec3 lightPosition;
uniform float farPlane;


const int samples = 20;
// array of offset direction for sampling
vec3 gridSamplingDisk[samples] = vec3[]
(
vec3(1, 1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1, 1,  1),
vec3(1, 1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1, 1, -1),
vec3(1, 1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1, 1,  0),
vec3(1, 0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1, 0, -1),
vec3(0, 1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0, 1, -1)
);


// in shadow 1.0   out of shadow 0.0
float ShadowCalculation(vec3 vFragmentPosition, float diff) {
    vec3 lightToFrag = vFragmentPosition - lightPosition;
    float currentDepth = length(lightToFrag);
    // bias
    // 当阴影偏移到一个夸张的值时，就会出现明显的阴影悬浮（peter panning）问题
    // 解决这个问题的一个思路就是 渲染深度贴图的时候开启正面剔除，这样，相关片段的深度贴图值就会更大，即使一个夸张的bias也不会影响到该值
    float bias = max(0.15 * (1.0 - diff), 0.1);
    float shadow = 0.0;
    float viewDistance = length(vFragmentPosition - cameraPosition);
    float diskRadius = (1.0 + viewDistance / farPlane) / 25.0;
    for (int i = 0; i < samples; i++) {
        float closestDepth = texture(depthMap, lightToFrag + gridSamplingDisk[i] * diskRadius).r;
        closestDepth *= farPlane;
        if (currentDepth > closestDepth + bias) {
            shadow += 1.0;
        }
    }
    shadow /= float(samples);

    return shadow;
}


void main() {

    // light color
    vec3 lightColor = vec3(0.3);
    // texture color
    vec3 color = texture(texture1, vsin.vTexCoord).rgb;
    vec3 normal = normalize(vsin.vNormal);
    // ambient
    vec3 ambient = 0.3 * lightColor;

    // diffuse
    vec3 lightDir = normalize(lightPosition - vsin.vFragmentPosition);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * lightColor;

    // specular
    vec3 viewDir = normalize(cameraPosition - vsin.vFragmentPosition);
    vec3 halfDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(halfDir, normal), 0.0), 64.0);
    vec3 specular = spec * lightColor;

    float shadow = ShadowCalculation(vsin.vFragmentPosition, diff);

    shadow = min(shadow, 0.75);
    vec3 res = (ambient + (1.0 - shadow) * (diffuse + specular)) * color;

    fragmentColor = vec4(res, 1.0);
}
























