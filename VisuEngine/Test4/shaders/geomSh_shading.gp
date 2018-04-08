#version 400

layout(triangles) in;
layout(triangle_strip, max_vertices=3) out;

// input
in vec4 vertex_Color[];

// output
flat out vec4 geom_Color;

// Model view projection matrix - combination of model-to-camera, camera-to-clip
uniform mat4 MVP;

uniform vec3 EyePos;
uniform vec3 LightPos;

void main()
{
    // The three points of the triangle
    vec3 p0 = gl_in[0].gl_Position.xyz;
    vec3 p1 = gl_in[1].gl_Position.xyz;
    vec3 p2 = gl_in[2].gl_Position.xyz;

    // Center of the triangle
    vec3 p = (p0+p1+p2)/3.0;
    vec4 pMvp = MVP * vec4(p, 1.0);

    // Two edges of the triangle and the normal
    vec3 e0 = p0 - p1;
    vec3 e1 = p2 - p1;
    vec3 norm = cross(e1, e0);
    norm = normalize(norm);
    vec4 normMVP = MVP * vec4(norm, 1.0);


    vec3 LightPos2 = vec3(200., 200., 200.);
    vec3 lightDir = EyePos - p;
    lightDir = normalize(lightDir);
    float k = clamp(dot(norm, lightDir), 0., 1.);

    for (int i=0; i<3; i++)
    {
        gl_Position = MVP * gl_in[i].gl_Position;
        geom_Color = vertex_Color[i] * k;
        EmitVertex();
    }
    EndPrimitive();
}
