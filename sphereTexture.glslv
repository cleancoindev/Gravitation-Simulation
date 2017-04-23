#version 420
layout(location=0) in vec3 inPosition;
out vec3 exchPosition;

uniform mat4 MVP;	// Model-View-Projection matrix

void main(void)
{
	gl_Position = MVP * vec4(inPosition, 1);
	exchPosition = inPosition;
}
