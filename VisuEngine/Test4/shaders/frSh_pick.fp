#version 400

// input
flat in vec4 geom_Color;

// output
out vec4 out_Color;

void main()
{
    out_Color = geom_Color;
}
