#ifndef MYGL_H
#define MYGL_H

#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glut.h>
#include <iostream>
#include <vector>
#include <memory>
#include <math.h>
#include <SOIL/SOIL.h>

#include "Objects.h"
#include "Shaders.h"
#include "Vertex3fColour.h"

using namespace std;

class GL
{
private:
	GLuint p, f, v;
	unsigned int FrameCount;
	float FPS;
	unsigned int CurrentTime, PreviousTime, ElapsedTime;
	float deltaTime;

public:
	int Width, Height;
	bool filling, fullscreen;
	
	float3 Position;
	float3 Rotation;
	float3 LVelocity;
	
	vector<Object3D*> Objects;
	
	GL()
	{
		return;
	}
	
	GL(int argc, char** argv, int width, int height, char* Title, void (*display)(void), void (*idle)(void), void (*resize)(int, int), void (*keyboard)(unsigned char, int, int), void (*keyboard_s)(int, int, int), void (*mouse)(int, int, int, int))
	{
		GLInit(argc, argv, width, height, Title, display, idle, resize, keyboard, keyboard_s, mouse);
	}
	
	void GLInit(int argc, char** argv, int width, int height, char* Title, void (*display)(void), void (*idle)(void), void (*resize)(int, int), void (*keyboard)(unsigned char, int, int), void (*keyboard_s)(int, int, int), void (*mouse)(int, int, int, int))
	{
		Width = width;
		Height = height;
		filling = true;
		fullscreen = false;
		Position = 0.0f;
		Rotation = 0.0f;
		FPS = 0.0f;
		deltaTime = 0.0f;
		
		//GLUT
		glutInit(&argc, argv);
		glutInitWindowSize(width, height);
		glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
		glutCreateWindow(Title);
		if((int)display != 0)
			glutDisplayFunc(display);
		if((int)idle != 0)
			glutIdleFunc(idle);
		if((int)resize != 0)
			glutReshapeFunc(resize);
		if((int)keyboard != 0)
			glutKeyboardFunc(keyboard);
		if((int)keyboard_s != 0)
			glutSpecialFunc(keyboard_s);
		if((int)mouse != 0)
			glutMouseFunc(mouse);
		
		//GL
		glClearColor(0.0, 0.0, 0.0, 0.0);
		glShadeModel(GL_SMOOTH);
		glViewport(0, 0, width, height);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 1.0f, 1000.0f);
		glEnable(GL_DEPTH_TEST);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 
		glEnable(GL_BLEND);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		
		//GLEW
		glewExperimental = GL_TRUE;
		glewInit();
	}
	
	void CalcFPS()
	{
		FrameCount++;
		CurrentTime = glutGet(GLUT_ELAPSED_TIME);
		if(CurrentTime - PreviousTime > 200)
		{
			FPS = FrameCount * 1000.0f / (CurrentTime - PreviousTime);
			deltaTime = 1.0f / FPS;
			PreviousTime = CurrentTime;
			FrameCount = 0;
		}
	}
	
	void StartFPS()
	{
		PreviousTime = glutGet(GLUT_ELAPSED_TIME);
	}
	
	Object3D* AddObject(int Verts, int Inds, int Mode)
	{
		Objects.push_back(new Object3D(Verts, Inds, Mode));
		return Objects[Objects.size() - 1];
	}
	Object3D* AddCube()
	{
		Objects.push_back(new CubeObj());
		return Objects[Objects.size() - 1];
	}
	Object3D* AddSphere(unsigned int n, float radius)
	{
		Objects.push_back(new SphereObj(n, radius));
		return Objects[Objects.size() - 1];
	}
	Object3D* AddLine()
	{
		Objects.push_back(new LineObj());
		return Objects[Objects.size() - 1];
	}
	Object3D* AddPlane()
	{
		Objects.push_back(new PlaneObj());
		return Objects[Objects.size() - 1];
	}
	
	float GetFPS()
	{
		return FPS;
	}
	float GetDeltaTime()
	{
		return deltaTime;
	}
	
	~GL()
	{
		glDetachShader(p, v);
		glDetachShader(p, f);
		glDeleteProgram(p);
	}
};
#endif