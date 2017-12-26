#version 400

layout(lines) in;
layout(line_strip, max_vertices=2) out;

// input
in vec4 vertex_Color[];

// output
flat out vec4 geom_Color;

void main()
{
    for (int i=0; i<2; i++)
    {
        gl_Position = gl_in[i].gl_Position;
        geom_Color = vertex_Color[i];
        EmitVertex();
    }
    EndPrimitive();
}
