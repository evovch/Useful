#version 400

layout(points) in;
layout(points, max_vertices=1) out;

// input
in vec4 vertex_Color[];

// output
out vec4 geom_Color;




void main()
{
    gl_Position = gl_in[0].gl_Position;
    geom_Color = vertex_Color[0];
    EmitVertex();
    EndPrimitive();
}
