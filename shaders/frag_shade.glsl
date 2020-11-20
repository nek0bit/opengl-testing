#version 330 core
out vec4 FragColor;

in vec2 textures;

uniform sampler2D texturesamp;

void main()
{
	FragColor = texture(texturesamp, textures);
}