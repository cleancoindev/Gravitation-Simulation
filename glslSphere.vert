#version 330
layout(location = 0) in vec3 position;

varying vec2 TexCoord;

//Stays constant for whole mesh
uniform mat4 MVP;			//ModelView Projection matrix
float pi = 3.14159265358;

void main()
{
	gl_Position = MVP * vec4(position, 1);
	vec2 newPos;
	newPos.x = 0.5 + -(atan(position.z, position.x)/(2 * pi));
	newPos.y = 0.5 - asin(position.y)/pi;
	
	TexCoord = newPos;
}