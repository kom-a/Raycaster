#version 450 core

out vec4 FragColor;

in vec2 v_Tex;

uniform sampler2D sampler;

void main()
{
	FragColor = vec4(1.0f, 0.5f, 0.3f, 1.0f);
	FragColor = texture(sampler, v_Tex);	
}