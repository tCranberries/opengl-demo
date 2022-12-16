#version 460 core

out vec4 fragmentColor;

in vec3 gNormal;
in vec3 gPosition;
in vec2 gTexCoord;
noperspective in vec3 gEdgeDistance;


// light Position
uniform vec3 lightPosition;
uniform vec3 lightColor;

// line width and line color
uniform float lineWidth;
uniform vec3 lineColor;

//
uniform vec3 viewerPosition;

uniform sampler2D texture_diffuse1;

uniform bool wireframe;

vec3 calBlinnPhone() {
    vec3 texColor = texture(texture_diffuse1, gTexCoord).rgb;
    vec3 normal = normalize(gNormal);
    vec3 lightDir = normalize(lightPosition - gPosition);
    vec3 viewDir = normalize(viewerPosition - gPosition);

    // ambient
    vec3 ambient = vec3(0.08) * texColor * lightColor;
    // diffuse
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * texColor * lightColor;

    // no sepcular
    // no attenuation

    return ambient + diffuse;
}



void main() {

    vec3 color = calBlinnPhone();

    if (wireframe) {
        float fractor = 0.0;

        float d = min(min(gEdgeDistance.x, gEdgeDistance.y), gEdgeDistance.z);
        if (d < lineWidth - 1.0) {
            fractor = 1.0;
        }
        else if (d > lineWidth + 1.0) {
            fractor = 0.0;
        }
        else {
            float tmp = d - (lineWidth - 1.0);
            fractor = exp2(-2.0 * tmp * tmp);
        }

        color = mix(color, lineColor, fractor);

    }

    fragmentColor = vec4(color, 1.0);
}

























