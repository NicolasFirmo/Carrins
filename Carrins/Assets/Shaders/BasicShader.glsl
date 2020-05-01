#shader vertex
#version 410 core

layout(location = 0) in vec4 a_Position;
layout(location = 1) in vec3 a_Normal;
//layout(location = 2) in vec4 a_Color;

//out vec4 v_Color;
out vec3 v_Normal;

uniform mat4 u_ViewProjection;
uniform mat4 u_ModelTransform;

void main()
{
	//v_Color = a_Color;
	v_Normal = mat3(u_ModelTransform) * a_Normal;

	gl_Position = u_ViewProjection * u_ModelTransform * a_Position;
}

#shader fragment
#version 410 core

layout(location = 0) out vec4 color;

//in vec4 v_Color;
in vec3 v_Normal;

uniform vec4 u_DifuseColor = vec4(1.0, 0.2, 1.0, 1.0);
uniform vec4 u_AmbientColor = vec4(0.2, 0.1, 0.0, 1.0);

void main()
{
	color = u_DifuseColor * (clamp(dot(v_Normal, vec3(0.2673, 0.5345, 0.8018)), 0.0, 1.0) + u_AmbientColor);
}