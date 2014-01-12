#version 330
layout(location = 0) in vec3 position;

//Stays constant for whole mesh
uniform mat4 MVP;			//Model View Projection matrix
uniform mat4 modelM;		//Model matrix

varying vec4 worldCoord;

void main()
{
	gl_Position = MVP * vec4(position, 1);
	worldCoord = modelM * vec4(position, 1);
}