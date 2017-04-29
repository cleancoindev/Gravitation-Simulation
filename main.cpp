#include <reindeergl/OpenGL.h>
#include <reindeergl/ObjectContainer.h>
#include <reindeergl/Objects.h>

#include "Planet.h"
#include "GravityPlane.h"
#include "Interpret.h"

#include <iostream>

using namespace std;

OpenGL gl;
unsigned int tick = 0;
float maxIterations = 128;
const float camSpeedSlow = 1;
const float camSpeed = 10;
unsigned int KEYS = 0;
bool autoScroll = false;
bool shift = false;
const float3 homePos(0, 20, 50);
shared_ptr<GravityPlane> grvty;

inline void MoveCam()
{
	gl.Position += (GLMath::RotateY(-gl.Rotation.y()) * gl.Velocity).Norm() * (shift ? camSpeedSlow : camSpeed) * gl.DeltaTime();
}

void Display()
{
	MoveCam();

	gl.DrawAll();

	if(tick / gl.FramesPerSecond() > 5)		// Print every 5 seconds
	{
		cout << gl.FramesPerSecond() << endl;
		tick = 0;
	}

	tick++;
}

void Keyboard(unsigned char key, int x, int y)
{
	shift = glutGetModifiers() & GLUT_ACTIVE_SHIFT;
	switch (key)
	{
		case 'r': case 'R':
			gl.Wireframe();
			gl.ShowCursor(!gl.IsFilling());
			break;
		case 'h': case 'H':
			gl.Position = homePos;
			break;
		case 'f': case 'F':
			gl.Fullscreen();
			break;
		case 'w': case 'W':
			if((KEYS & 1) == 0)
			{
				gl.Velocity.z() -= 1;
				KEYS += 1;
			}
			break;
		case 'a': case 'A':
			if((KEYS & 2) == 0)
			{
				gl.Velocity.x() -= 1;
				KEYS += 2;
			}
			break;
		case 's': case 'S':
			if((KEYS & 4) == 0)
			{
				gl.Velocity.z() += 1;
				KEYS += 4;
			}
			break;
		case 'd': case 'D':
			if((KEYS & 8) == 0)
			{
				gl.Velocity.x() += 1;
				KEYS += 8;
			}
			break;
		case ' ':
			if((KEYS & 16) == 0)
			{
				gl.Velocity.y() += 1;
				KEYS += 16;
			}
			break;
		case 'c': case 'C':
			if((KEYS & 32) == 0)
			{
				gl.Velocity.y() -= 1;
				KEYS += 32;
			}
			break;
		case 27:
			glutDestroyWindow(gl.WindowId());
			exit(0);
			break;
	}
	glutPostRedisplay();
}

void KeyboardUp(unsigned char key, int x, int y)
{
	switch(key)
	{
		case 'w': case 'W':
			gl.Velocity.z() = 0;
			KEYS -= 1;
			break;
		case 'a': case 'A':
			gl.Velocity.x() = 0;
			KEYS -= 2;
			break;
		case 's': case 'S':
			gl.Velocity.z() = 0;
			KEYS -= 4;
			break;
		case 'd': case 'D':
			gl.Velocity.x() = 0;
			KEYS -= 8;
			break;
		case ' ':
			gl.Velocity.y() = 0;
			KEYS -= 16;
			break;
		case 'c': case 'C':
			gl.Velocity.y() = 0;
			KEYS -= 32;
			break;
		case 'p': case 'P':
			gl.SaveScreenshot("awesome.bmp");
			break;
	}
	glutPostRedisplay();
}

void Resize(int width, int height)
{
	gl.SetCurrentWindowSize(width, height);
}

void MouseMotion(int x, int y)
{
	if(x != gl.Width() / 2 || y != gl.Height() / 2)
		glutWarpPointer(gl.Width()/2, gl.Height()/2);

	//Rotation around y axis depends on moving x-pixel coords :P (and vice versa)
	gl.Rotation.y() -= 0.0008 * float(gl.Width() / 2 - x);
	gl.Rotation.x() -= 0.00075 * float(gl.Height() / 2 - y);

	if(gl.Rotation.x() > 1.570796)
		gl.Rotation.x() = 1.570796;
	else if(gl.Rotation.x() < -1.570796)
		gl.Rotation.x() = -1.570796;
}

int main(int argc, char** argv)
{
	std::string dataFile;
	if(argc == 2)
		dataFile = string(argv[1]);
	else
	{
		cout << "Data File: ";
		getline(cin, dataFile);
	}

	gl.GLInit(argc, argv, 1200, 680, "TestGL");
	gl.ShowCursor(false);

	gl.Position = homePos;
	glutDisplayFunc(Display);
	glutIdleFunc(Display);
	glutKeyboardFunc(Keyboard);
	glutKeyboardUpFunc(KeyboardUp);
	glutPassiveMotionFunc(MouseMotion);
	glutReshapeFunc(Resize);

    grvty = make_shared<GravityPlane>("gravity plane", "./Images/newPalette.png");
    gl.AddAnObjectPtr(OBJECT3D_PTR(grvty));
    grvty->SetScale(float3(100));
    grvty->Rotate(Quaternion(float3(1, 0, 0), GLMath::PI/2));

    if(!InterpGravityFile(dataFile, grvty))
    {
		std::cerr << "InterpGravityFile(" << dataFile << ", \'GravityPlane\') failed\n";
		return -1;
	}

	gl.StartFPS();
    glutMainLoop();

    return 0;
}
