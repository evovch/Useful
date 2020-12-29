#version 400
#extension GL_ARB_separate_shader_objects : enable

layout (location = 0) in vec3 vert_color;

layout (location = 0) out vec4 out_Color;

void main() {
	out_Color = vec4(vert_color, 1.0);
}
