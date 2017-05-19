#version 330

layout(location=0) in vec3 position;
layout(location=1) in vec2 texcoord;
layout(location=2) in vec3 normal;

uniform mat4 model;
uniform mat4 vp;

out vec3 fNormal;
out vec2 fTexcoord;
out vec3 globalPosition;

void main()
{
	fNormal = mat3(model) * normal;	
	fTexcoord = texcoord;	
	globalPosition = vec3(model*vec4(position,1.0f));	//frag position after rotate, transform.....
	gl_Position=vp*model*vec4(position, 1.0);
}
