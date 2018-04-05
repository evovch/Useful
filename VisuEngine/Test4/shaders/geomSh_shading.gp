#version 400

layout(triangles) in;
layout(triangle_strip, max_vertices=3) out;
//layout(line_strip, max_vertices=3) out;

// input
in vec4 vertex_Color[];

// output
out vec4 geom_Color;
out float geom_k;

// Model view projection matrix - combination of model-to-camera, camera-to-clip
uniform mat4 MVP;

uniform vec3 EyePos;
uniform vec3 LightPos;

void main()
{
    for (int i=0; i<3; i++)
    {
        gl_Position = MVP * gl_in[i].gl_Position;
        geom_Color = vertex_Color[i];
        EmitVertex();
    }
    EndPrimitive();

    // The three points of the triangle
    vec3 p0 = gl_in[0].gl_Position.xyz;
    vec3 p1 = gl_in[1].gl_Position.xyz;
    vec3 p2 = gl_in[2].gl_Position.xyz;

    // Center of the triangle
    vec3 p = (p0+p1+p2)/3.0;
    vec4 pMvp = MVP * vec4(p, 1.0);

    // Two edge of the triangle and the normal
    vec3 e0 = p0 - p1;
    vec3 e1 = p2 - p1;
    vec3 norm = cross(e1, e0);
    norm = normalize(norm);

/*
    float k = 5.0;

    // First emitted vertex - center of the triangle
    gl_Position = MVP * vec4(p+norm*k, 1.0);
    geom_Color = vec4(0.0, 1.0, 0.0, 1.0);
    EmitVertex();

    // Second emitted vertex
    gl_Position = pMvp;
    geom_Color = vec4(1.0, 0.0, 0.0, 1.0);
    EmitVertex();

    // Third emitted vertex
    gl_Position = vec4(normalize(LightPos), 1.0);
    geom_Color = vec4(0.0, 1.0, 1.0, 1.0);
    EmitVertex();

 	EndPrimitive();
*/
}
