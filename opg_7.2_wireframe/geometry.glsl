#version 460 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

out vec3 gNormal;
out vec3 gPosition;
out vec2 gTexCoord;
noperspective out vec3 gEdgeDistance;

in vec3 vNormal[];
// view space fragment position
in vec3 vFragPos[];
in vec2 vTexCoord[];

uniform mat4 viewportMatrix;

void main() {

    // transform each vertex to viewport space
    vec2 p0 = vec2(viewportMatrix * (gl_in[0].gl_Position / gl_in[0].gl_Position.w));
    vec2 p1 = vec2(viewportMatrix * (gl_in[1].gl_Position / gl_in[1].gl_Position.w));
    vec2 p2 = vec2(viewportMatrix * (gl_in[2].gl_Position / gl_in[2].gl_Position.w));

    float a = length(p1 - p2);
    float b = length(p0 - p1);
    float c = length(p0 - p2);

    float alpha = acos((b * b + c * c - a * a) / (2 * b * c));
    float beta = acos((a * a + c * c - b * b) / (2 * a * c));

    float ha = c * sin(beta);
    float hb = c * sin(alpha);
    float hc = b * sin(alpha);

    gEdgeDistance = vec3(ha, 0.0, 0.0);
    gNormal = vNormal[0];
    gPosition = vFragPos[0];
    gTexCoord = vTexCoord[0];
    gl_Position = gl_in[0].gl_Position;
    EmitVertex();

    gEdgeDistance = vec3(0.0, hb, 0.0);
    gNormal = vNormal[1];
    gPosition = vFragPos[1];
    gTexCoord = vTexCoord[1];
    gl_Position = gl_in[1].gl_Position;
    EmitVertex();

    gEdgeDistance = vec3(0.0, 0.0, hc);
    gNormal = vNormal[2];
    gPosition = vFragPos[2];
    gTexCoord = vTexCoord[2];
    gl_Position = gl_in[2].gl_Position;
    EmitVertex();

    EndPrimitive();
}




















