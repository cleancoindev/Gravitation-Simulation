#version 330
layout(location = 0) in vec3 position;

varying vec2 TexCoord;

//Stays constant for whole mesh
uniform mat4 MVP;			//ModelView Projection matrix

void main()
{
	gl_Position = MVP * vec4(position, 1);
	TexCoord = vec2(0.5 * position.x + 0.5, -0.5 * position.y + 0.5);
}