#version 460 core

layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

uniform mat4 projection;

// half the width of quad
uniform float size2;

out vec2 texCoord;

void main() {

    gl_Position = projection * (vec4(-size2, -size2, 0.0, 0.0) + gl_in[0].gl_Position);
    texCoord = vec2(0.0, 0.0);
    EmitVertex();

    gl_Position = projection * (vec4(size2, -size2, 0.0, 0.0) + gl_in[0].gl_Position);
    texCoord = vec2(1.0, 0.0);
    EmitVertex();

    gl_Position = projection * (vec4(-size2, size2, 0.0, 0.0) + gl_in[0].gl_Position);
    texCoord = vec2(0.0, 1.0);
    EmitVertex();

    gl_Position = projection * (vec4(size2, size2, 0.0, 0.0) + gl_in[0].gl_Position);
    texCoord = vec2(1.0, 1.0);
    EmitVertex();

    EndPrimitive();

}
