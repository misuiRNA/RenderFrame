#version 330 core

layout (points) in;
in VERTEX_ATTRIBUT {
    vec3 color;
} gs_in[];

layout (triangle_strip, max_vertices = 5) out;
out vec3 pColor;

void build_house(vec4 position)
{
    pColor = gs_in[0].color;
    gl_Position = position + vec4(-0.2, -0.2, 0.0, 0.0);
    EmitVertex();   
    gl_Position = position + vec4( 0.2, -0.2, 0.0, 0.0);
    EmitVertex();
    gl_Position = position + vec4(-0.2,  0.2, 0.0, 0.0);
    EmitVertex();
    gl_Position = position + vec4( 0.2,  0.2, 0.0, 0.0);
    EmitVertex();
    pColor = vec3(1.0, 1.0, 1.0);
    gl_Position = position + vec4( 0.0,  0.4, 0.0, 0.0);
    EmitVertex();
    EndPrimitive();
}

void main() {
    build_house(gl_in[0].gl_Position);
}
