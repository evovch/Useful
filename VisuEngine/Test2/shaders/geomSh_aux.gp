#version 400

layout(triangles) in;
layout(triangle_strip, max_vertices=3) out;

in vec4 vertex_Color[];    // in 4,
out vec3 geom_Color;  // out 3. I don't know why :-(





// Model view projection matrix - combination of model-to-camera, camera-to-clip
uniform mat4 MVP;

void main()
{
	for (int i=0; i<3; i++)
	{
		geom_Color = vertex_Color[i].xyz;
		gl_Position = MVP * gl_in[i].gl_Position;
		EmitVertex();
	}
	EndPrimitive();
}
