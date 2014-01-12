#version 330
varying vec2 TexCoord;
out vec4 FragColor;

uniform sampler2D textureMap;

void main(void)
{
   FragColor = texture(textureMap, TexCoord);
}