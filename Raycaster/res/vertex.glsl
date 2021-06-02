#version 450 core

layout(location = 0) in vec3 a_Pos;
layout(location = 1) in vec2 a_Tex;

out vec2 v_Tex;

uniform mat4 projection = mat4(1.0f);
uniform mat4 model = mat4(1.0f);

void main()
{
	gl_Position = projection * model * vec4(a_Pos, 1.0f);
	v_Tex = a_Tex;
}