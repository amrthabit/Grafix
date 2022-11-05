#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec3 normal;

out vec3 v_Normal;

uniform mat4 u_MVP;

void main()
{
	gl_Position = u_MVP * position;
	v_Normal = normal;
};

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec3 v_Normal;

void main()
{
	float c = dot(v_Normal, vec3(0.8, 0.7, 0.6));
	color = vec4(c, c, c, 1.0);
}