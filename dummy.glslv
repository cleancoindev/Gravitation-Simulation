#version 400
layout(location=0) in vec3 inPosition;

uniform mat4 MVP;	// Model-View-Projection matrix

void main(void)
{
        gl_Position = MVP * vec4(inPosition, 1);
}
