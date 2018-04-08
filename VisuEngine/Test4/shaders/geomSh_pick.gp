#version 400

layout(triangles) in;
layout(triangle_strip, max_vertices=3) out;

// input
in vec4 vertex_Color[];

// output
flat out vec4 geom_Color;

// Model view projection matrix - combination of model-to-camera, camera-to-clip
uniform mat4 MVP;

void main()
{
    for (int i=0; i<3; i++)
    {
        gl_Position = MVP * gl_in[i].gl_Position;
        geom_Color = vertex_Color[i];
        EmitVertex();
    }
    EndPrimitive();
}
