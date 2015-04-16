#version 410

layout(location=0) in vec4 position;
layout(location=1) in vec2 texcoord;

uniform mat4 view_proj;

out vec2 frag_texcoord;

void main()
{
	frag_texcoord = texcoord;
	gl_Position = view_proj * position;
}