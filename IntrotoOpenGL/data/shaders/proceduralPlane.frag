#version 410

in vec2 frag_texcoord;

out vec4 out_colour;
uniform sampler2D perlin_texture;

void main()
{
	out_colour = texture(perlin_texture, frag_texcoord).rrrr;
	out_colour.a = 1;
}