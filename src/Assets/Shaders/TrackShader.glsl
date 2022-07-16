#shader vertex
#version 410 core

layout(location=0)in vec2 a_TexCoord;

out vec2 v_TexCoord;

uniform mat4 u_ViewProjection;

uniform sampler2D u_HSampler;

void main()
{
	vec4 heightSample=texture(u_HSampler,a_TexCoord);
	float height=heightSample.r;
	
	v_TexCoord=a_TexCoord;
	gl_Position=u_ViewProjection*vec4((a_TexCoord.x-.5)*8,height,(-a_TexCoord.y+.5)*8,1.);
}

#shader fragment
#version 410 core

layout(location=0)out vec4 color;

in vec2 v_TexCoord;

uniform sampler2D u_NSampler;

uniform vec4 u_MaterialColor=vec4(0.2, 0.5412, 0.0941, 1.0);
uniform vec4 u_DiffuseLightColor=vec4(1.0, 1.0, 1.0, 1.0);
uniform vec4 u_AmbientLightColor=vec4(0.4588, 0.2941, 0.1412, 1.0);

void main()
{
	vec4 normalSample=texture(u_NSampler,v_TexCoord);
	vec3 normal=vec3(normalSample.x*2-1,normalSample.y*2-1,normalSample.z);
	
	vec4 u_DiffuseLight=u_DiffuseLightColor*clamp(dot(normal,vec3(.2673,.5345,.8018)),0.,1.);
	
	color=(u_DiffuseLight+u_AmbientLightColor)*u_MaterialColor;
}