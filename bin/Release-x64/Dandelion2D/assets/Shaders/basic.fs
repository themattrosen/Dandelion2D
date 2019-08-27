#version 440 core

out vec4 FragColor;

uniform vec4 ourColor;

void main()
{
	FragColor = ourColor;
	//FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);
}
