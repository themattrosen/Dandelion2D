#version 440 core

layout(location = 0) in vec3 aPos;

uniform mat4 transform;
uniform mat4 view;
uniform mat4 perspective;

void main()
{
	gl_Position = perspective * view * transform * vec4(aPos, 1.0);
	//gl_Position = vec4(aPos, 1.0);
}
