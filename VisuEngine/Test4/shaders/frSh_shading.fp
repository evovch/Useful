#version 400

// input
in vec4 geom_Color;
in float geom_k;

// output
out vec4 out_Color;

void main()
{
    out_Color = geom_Color;
}
