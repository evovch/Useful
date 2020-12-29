#version 400
#extension GL_ARB_separate_shader_objects : enable

layout (location = 0) in vec3 in_Position;
layout (location = 1) in vec3 in_Color;

layout (location = 0) out vec3 vert_color;

void main() {
	gl_Position = vec4(in_Position, 1.0);
	vert_color = in_Color;
}
