#version 330 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

// in block   geometry shader inputs must be arrays
in VS_OUT {
    vec2 vTexCoord;
    vec3 vNormal;
    vec3 vFragmentPosition;
} gs_in[];


out vec2 vTexCoord;
out vec3 vNormal;
out vec3 vFragmentPosition;

uniform float time;

vec4 explode(vec4 position, vec3 normal) {
    float magnitude = 2.0;
    vec3 dir = normal * ((sin(time) + 1.0) / 2.0) * magnitude;
    return position + vec4(dir, 0.0);
}

vec3 GetNormal() {
    vec3 a = vec3(gl_in[0].gl_Position) - vec3(gl_in[1].gl_Position);
    vec3 b = vec3(gl_in[2].gl_Position) - vec3(gl_in[1].gl_Position);
    return normalize(cross(a, b));
}


void main() {

    vec3 normal = GetNormal();

    for (int i = 0; i < 3; i++) {
        gl_Position = explode(gl_in[i].gl_Position, normal);
        vTexCoord = gs_in[i].vTexCoord;
        vNormal = gs_in[i].vNormal;
        vFragmentPosition = gs_in[i].vFragmentPosition;
        EmitVertex();
    }

    EndPrimitive();
}
