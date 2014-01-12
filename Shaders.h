#ifndef SHADERS_H
#define SHADERS_H

#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glut.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string.h>

using namespace std;

char* Read(char* FileName)
{
	ifstream File(FileName, ifstream::binary);
	if(File.good())
	{
		File.seekg(0, File.end);
		int size = File.tellg();
		File.seekg(0, File.beg);
		
		char* buffer = new char [size+1];
		memset(buffer, 0, size + 1);
		File.read(buffer, size);
		File.close();
		
		return buffer;
	}
	cout << "Could not open file " << FileName << endl;
	exit(0);
}

void SetShaders(GLuint* p, GLuint* f, GLuint* v, char* VertShader, char* FragShader)
{
	char* vs, *fs;
	GLint Success = 0;
	GLchar ErrorLog[1024] = {0};
	
	*v = glCreateShader(GL_VERTEX_SHADER);
	*f = glCreateShader(GL_FRAGMENT_SHADER);
	
	vs = Read(VertShader);
	fs = Read(FragShader);
	
	const char* vv = vs;
	const char* ff = fs;
	
	glShaderSource(*v, 1, &vv, NULL);
	glShaderSource(*f, 1, &ff, NULL);
	
	delete[] vs;
	delete[] fs;
	
	glCompileShader(*v);
	glGetShaderiv(*v, GL_COMPILE_STATUS, &Success);
    if (!Success) {
        glGetShaderInfoLog(*v, 1024, NULL, ErrorLog);
        cout << "Error compiling vertex shader: " << ErrorLog << endl;
        exit(1);
    }
	glCompileShader(*f);
	glGetShaderiv(*f, GL_COMPILE_STATUS, &Success);
    if (!Success) {
        glGetShaderInfoLog(*f, 1024, NULL, ErrorLog);
        cout << "Error compiling fragment shader: " << ErrorLog << endl;
        exit(1);
    }
	
	*p = glCreateProgram();
	
	glAttachShader(*p, *v);
	glAttachShader(*p, *f);
	
	glLinkProgram(*p);
	glGetProgramiv(*p, GL_LINK_STATUS, &Success);
	if (Success == 0)
	{
		glGetProgramInfoLog(*p, sizeof(ErrorLog), NULL, ErrorLog);
		cout << "Error linking shader program: " << ErrorLog << endl;
        exit(1);
	}
	glValidateProgram(*p);
    glGetProgramiv(*p, GL_VALIDATE_STATUS, &Success);
    if (!Success)
	{
		glGetProgramInfoLog(*p, sizeof(ErrorLog), NULL, ErrorLog);
        cout << "Invalid shader program: " << ErrorLog << endl;
        exit(1);
    }
	
	glUseProgram(*p);
}
#endif