#version 460 core

out float FragColor;

in vec2 vTexCoord;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D texNoise;

uniform vec3 samples[64];

const int kernelSize = 64;
const float radius = 0.5;
const float bias = 0.025;

const vec2 noiseScale = vec2(1920.0 / 4.0, 1080.0 / 4.0);

uniform mat4 projection;

void main() {

    vec3 fragPos = texture(gPosition, vTexCoord).xyz;
    vec3 normal = normalize(texture(gNormal, vTexCoord).rgb);
    vec3 randomVec = normalize(texture(texNoise, vTexCoord * noiseScale).xyz);
    // create TBN change of base matrix
    vec3 tangent = normalize(randomVec - normal * dot(randomVec, normal));
    vec3 bitangent = normalize(cross(normal, tangent));
    mat3 TBN = mat3(tangent, bitangent, normal);

    float occlusion = 0.0;
    for (int i = 0; i < kernelSize; i++) {
        // from tangent to view space
        vec3 samplePos = TBN * samples[i];
        samplePos = fragPos + samplePos * radius;

        // project sample position to get position on the screen
        vec4 offset = vec4(samplePos, 1.0);
        offset = projection * offset;
        offset.xyz /= offset.w;
        offset.xyz = offset.xyz * 0.5 + 0.5;

        float sampleDepth = texture(gPosition, offset.xy).z;

        float rangCheck = smoothstep(0.0, 1.0, radius / abs(fragPos.z - sampleDepth));
        occlusion += (sampleDepth >= samplePos.z + bias ? 1.0 : 0.0) * rangCheck;
    }

    occlusion = 1.0 - (occlusion / kernelSize);
    FragColor = occlusion;
}




























