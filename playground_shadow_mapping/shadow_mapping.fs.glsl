#version 330 core

in VS_DATA {
    vec2 vTexCoord;
    vec3 vNormal;
    vec3 vFragmentPosition;
    vec4 vFragPosLightSpace;
} vsin;

out vec4 fragmentColor;

// 当出现多个纹理时，必须指定纹理的顺序，即哪个纹理绑定到 0 上？哪个绑定到 1 上
uniform sampler2D texture1;   // GL_TEXTURE0
uniform sampler2D depthMap;   // GL_TEXTURE1

uniform vec3 cameraPosition;
uniform vec3 lightPosition;

// in shadow 1.0   out of shadow 0.0
float ShadowCalculation(vec4 fragPosLightSpace, float diff) {
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;

    float currentDepth = projCoords.z;
    // bias
    // 当阴影偏移到一个夸张的值时，就会出现明显的阴影悬浮（peter panning）问题
    // 解决这个问题的一个思路就是 渲染深度贴图的时候开启正面剔除，这样，相关片段的深度贴图值就会更大，即使一个夸张的bias也不会影响到该值
    float bias = max(0.05 * (1.0 - diff), 0.005);

    // 由于深度贴图的分辨率是固定的，阴影锯齿块情况就比较严重
    // PCF percentage-closer filtering, 临近平均采样，从深度贴图中多次采样，每一次采样的纹理坐标都稍有不同。
    // 每个独立的样本可能在也可能不再阴影中。所有的次生结果接着结合在一起，进行平均化，我们就得到了柔和阴影
    float shadow = 0.0;
    vec2 texSize = 1.0 / textureSize(depthMap, 0);
    for (int x = -1; x <= 1; x++) {
        for (int y = -1; y <= 1; y++) {
            float closestDepth = texture(depthMap, projCoords.xy + vec2(x, y) * texSize).r;
            shadow += currentDepth > closestDepth + bias ? 1.0 : 0.0;
        }
    }
    shadow /= 9.0;

    // Keep the shadow at 0.0 when outside the far_plane region of the light's frustum.
    // 也就是说在光的视锥里面的东西才有阴影，超出的没有阴影处理
    if (projCoords.z > 1.0) {
        shadow = 0.0;
    }

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

    float shadow = ShadowCalculation(vsin.vFragPosLightSpace, diff);

    shadow = min(shadow, 0.75);
    vec3 res = (ambient + (1.0 - shadow) * (diffuse + specular)) * color;

    fragmentColor = vec4(res, 1.0);
}
























