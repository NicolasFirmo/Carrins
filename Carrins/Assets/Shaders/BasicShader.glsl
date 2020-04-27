#shader vertex
#version 410 core

layout(location = 0) in vec4 a_Position;
layout(location = 1) in vec4 a_Color;

out vec4 v_Color;

void main()
{
	v_Color = a_Color;
	gl_Position = a_Position;
}

#shader fragment
#version 410 core

layout(location = 0) out vec4 color;

in vec4 v_Color;

void main()
{
	color = v_Color;
}