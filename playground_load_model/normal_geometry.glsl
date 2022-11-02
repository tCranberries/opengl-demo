#version 330 core

layout (triangles) in;
layout (line_strip, max_vertices = 6) out;

// in block   geometry shader inputs must be arrays
in VS_OUT {
    vec3 vNormal;
} gs_in[];

uniform mat4 projection;
const float magnitude = 5.2;

void generateLine(int index) {
    gl_Position = projection * (gl_in[index].gl_Position);
    EmitVertex();
    gl_Position = projection * (gl_in[index].gl_Position + vec4(gs_in[index].vNormal, 0.0) * magnitude);
    EmitVertex();
    EndPrimitive();
}


void main() {

    for (int i = 0; i < 3; i++) {
        generateLine(i);
    }
}
