#version 450 core

out vec4 FragColor;

in vec2 v_Tex;

uniform sampler2D sampler;

void main()
{
	FragColor = texture(sampler, v_Tex);	
}