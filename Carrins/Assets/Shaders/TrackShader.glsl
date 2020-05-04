#shader vertex
#version 410 core

layout(location=0)in vec2 a_TexCoord;

out vec2 v_TexCoord;

uniform mat4 u_ViewProjection;

uniform sampler2D u_HSampler;

void main()
{
	v_TexCoord=a_TexCoord;
	vec4 height=texture(u_HSampler,a_TexCoord);
	
	gl_Position=u_ViewProjection*vec4((a_TexCoord.x-.5)*10,height.r,(-a_TexCoord.y+.5)*10,1.);
}

#shader fragment
#version 410 core

layout(location=0)out vec4 color;

in vec2 v_TexCoord;

uniform sampler2D u_NSampler;// TODO

void main()
{
	color=vec4(v_TexCoord,1.,1.);
}