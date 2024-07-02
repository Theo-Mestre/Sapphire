#type vertex
#version 330 core

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec4 a_color;

uniform mat4 u_viewProjection;
uniform mat4 u_transform;

out vec3 v_position;

void main()
{
	v_position = a_position;
	gl_Position = u_viewProjection * u_transform * vec4(a_position, 1.0);
}

#type fragment
#version 330 core

layout(location = 0) out vec4 color;

uniform float u_time;
in vec3 v_position;

void main()
{
	float colorModifier = cos(u_time);
	color = vec4(v_position * 0.5 + (0.5 + colorModifier * 0.5), 1.0);
}