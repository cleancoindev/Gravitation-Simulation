#include <sstream>
#include <string.h>
#include <math.h>

#include "Objects.h"
#include "Interpret.h"
#include "OpenGL.h"
#include "Text.h"
#include "Vertex3fColour.h"

const float pi = 3.14159265359;
const float tau = 6.28318530718;
const float3 x(1.0f, 0.0f, 0.0f);		//unit vector for x direction
const float G = 0.5;					//Gravitational Constant
const float FOV = 50.0;					//Field Of View in degrees

float camSpeed = 50.0;
unsigned int KEYS;						//Holds bits (on/off) for some keys
bool update = true;						//Keep updating body positions?
float3* tempV;

GL gl;
PlaneObj Background;

inline void CheckKeys()
{
	gl.Position += Norm3f(Rotate(float3(0, gl.Rotation.y, 0)) * gl.LVelocity) * camSpeed * gl.GetDeltaTime();
	if(KEYS & 64)
		camSpeed *= 1.005;
	if(KEYS & 128)
		camSpeed /= 1.005;
}

void Display()
{
	glFlush();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	gl.CalcFPS();
	
	CheckKeys();	//Move our camera using the local velocity
	if(update)
	{
		for(int i = 0; i < gl.Objects.size()/2; i++)
		{
			if(gl.Objects[i]->Mass != 0)
			{
				float3 acceleration = 0;
				for(int j = 0; j < gl.Objects.size()/2; j++)
				{
					if(i != j)
					{
						float3 gDir = Norm3f(gl.Objects[j]->Position - gl.Objects[i]->Position);		//Destination - Source, direction of gravity
						acceleration += (gDir * (G * gl.Objects[j]->Mass / pow(Mag(gl.Objects[j]->Position - gl.Objects[i]->Position), 2)));
					}
				}
				//tempV[i] = gl.Objects[i]->Velocity + (acceleration * gl.GetDeltaTime());
				gl.Objects[i]->Velocity += (acceleration * gl.GetDeltaTime());
			}
		}
		
		for(int i = 0; i < gl.Objects.size()/2; i++)
			gl.Objects[i]->Translate(gl.Objects[i]->Velocity * gl.GetDeltaTime());
		
		for(int i = 0; i < gl.Objects.size()/2; i++)
		{
			//gl.Objects[i]->Velocity = tempV[i];
			//gl.Objects[i]->Translate(gl.Objects[i]->Velocity * gl.GetDeltaTime());
			for(int j = 0; j < gl.Objects.size()/2; j++)
			{
				if(i != j && Mag(gl.Objects[i]->Position - gl.Objects[j]->Position) < gl.Objects[i]->Scale.x + gl.Objects[j]->Scale.x)	//Collision check
				{
					float3 Momentum = (gl.Objects[i]->Velocity * gl.Objects[i]->Mass) + (gl.Objects[j]->Velocity * gl.Objects[j]->Mass);
					
					float3 dir = Norm3f(gl.Objects[i]->Position - gl.Objects[j]->Position);	//Vector that points from there center to ours
					gl.Objects[i]->Position = (dir * (gl.Objects[i]->Scale.x + gl.Objects[j]->Scale.x)) + gl.Objects[j]->Position; //So they just touch
					//cout << Dot(dir, Norm3f(gl.Objects[i]->Velocity)) << ", " << Dot(dir, Norm3f(gl.Objects[j]->Velocity)) << endl;
					//if(Dot(dir, Norm3f(gl.Objects[i]->Velocity)) == -1.0f && Dot(dir, Norm3f(gl.Objects[j]->Velocity)) == 1.0f)		//Head On collision
					//{
						gl.Objects[i]->Velocity = Momentum / (gl.Objects[i]->Mass + gl.Objects[j]->Mass);
						gl.Objects[j]->Velocity = gl.Objects[i]->Velocity;
					//}
					//else
						//gl.Objects[i]->Velocity -= dir * Dot(dir, gl.Objects[i]->Velocity);
				}
			}
			
			//Velocity Lines
			gl.Objects[i + (gl.Objects.size()/2)]->Position = gl.Objects[i]->Position + (Norm3f(gl.Objects[i]->Velocity) * gl.Objects[i]->Scale.x);
			gl.Objects[i + (gl.Objects.size()/2)]->Scale = gl.Objects[i]->Velocity * 0.6f;
		}
	}
	
	mat4 Projection = Perspective(FOV, float(gl.Width) / float(gl.Height), 0.2f, 3000.0f);
	mat4 View = Rotate(gl.Rotation);
	
	mat4 modelM = Translate(Background.Position - float3(gl.Position.x, gl.Position.y, -gl.Position.z)) * Rotate(Background.Rotation) * Scale(Background.Scale);
	Background.MVP = Projection * View * modelM;
	
	float planets[16*4] = {0.0f};
	for(unsigned int i = 0; i < gl.Objects.size()/2; i++)
	{
		planets[(4*i)] = gl.Objects[i]->Position.x;
		planets[(4*i)+1] = gl.Objects[i]->Position.y;
		planets[(4*i)+2] = gl.Objects[i]->Position.z;
		planets[(4*i)+3] = gl.Objects[i]->Mass;
	}
	glUseProgram(Background.GetP());
	glUniform4fv(glGetUniformLocation(Background.GetP(), "Planets"), 16, planets);
	glUniformMatrix4fv(glGetUniformLocation(Background.GetP(), "modelM"), 1, GL_FALSE, (float*)modelM.p);
	glUniform3f(glGetUniformLocation(Background.GetP(), "camPos"), gl.Position.x, gl.Position.y, -gl.Position.z);
	Background.Draw();
	
	//MUST BE TRANSLATION * ROTATION * SCALE IN THAT ORDER!!!  (but when compiled is applied in opposite order)
	for(int i = 0; i < gl.Objects.size(); i++)
	{
		mat4 Model = Translate(gl.Objects[i]->Position - float3(gl.Position.x, gl.Position.y, -gl.Position.z)) * Rotate(gl.Objects[i]->Rotation) * Scale(gl.Objects[i]->Scale);
		gl.Objects[i]->MVP = Projection * View * Model;
		gl.Objects[i]->Draw();
	}
	glFinish();
	glutSwapBuffers();
}

void Resize(int width, int height)
{
	gl.Width = width; 
	gl.Height = height; 
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, width, height);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glutPostRedisplay();
	gl.CalcFPS();
}

void Keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
		case 'r': case 'R':
			if(gl.filling)
			{
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			}
			else
			{
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			}
			gl.filling = !gl.filling;
			break;
		case 'f': case 'F':
			(gl.fullscreen)? glutReshapeWindow(500, 500) : glutFullScreen();
			gl.Width = glutGet(GLUT_WINDOW_WIDTH);
			gl.Height = glutGet(GLUT_WINDOW_HEIGHT);
			gl.fullscreen = !gl.fullscreen;
			glutWarpPointer(gl.Width/2, gl.Height/2);
			break;
		case 'p':case 'P':
			update = !update;
			break;
		case '=':case '+': 		//But we mean the plus sign!
			if(!(KEYS & 64))
				KEYS += 64;
			break;
		case '-':case '_':
			if(!(KEYS & 128))
				KEYS += 128;
			break;
		case 27:
			exit(0);
			break;
	
		case 'w':case 'W':
			if(!(KEYS & 1))
			{
				gl.LVelocity.z += 1;
				KEYS += 1;
			}
			break;
		case 'a':case 'A':
			if(!(KEYS & 2))
			{
				gl.LVelocity.x -= 1;
				KEYS += 2;
			}
			break;
		case 's':case 'S':
			if(!(KEYS & 4))
			{
				gl.LVelocity.z -= 1;
				KEYS += 4;
			}
			break;
		case 'd':case 'D':
			if(!(KEYS & 8))
			{
				gl.LVelocity.x += 1;
				KEYS += 8;
			}
			break;
		case ' ':
			if(!(KEYS & 16))
			{
				gl.LVelocity.y += 1;
				KEYS += 16;
			}
			break;
		case 'c':case 'C':
			if(!(KEYS & 32))
			{
				gl.LVelocity.y -= 1;
				KEYS += 32;
			}
			break;
	}
}

void KeyboardUp(unsigned char key, int x, int y)
{
	switch(key)
	{
		case 'w':case 'W':
			gl.LVelocity.z -= 1;
			KEYS -= 1;
			break;
		case 'a':case 'A':
			gl.LVelocity.x += 1;
			KEYS -= 2;
			break;
		case 's':case 'S':
			gl.LVelocity.z += 1;
			KEYS -= 4;
			break;
		case 'd':case 'D':
			gl.LVelocity.x -= 1;
			KEYS -= 8;
			break;
		case ' ':
			gl.LVelocity.y-= 1;
			KEYS -= 16;
			break;
		case 'c':case 'C':
			gl.LVelocity.y += 1;
			KEYS -= 32;
			break;
		case '=':case '+':
			KEYS -= 64;
			break;
		case '-':case '_':
			KEYS -= 128;
			break;
	}
}

void MouseButton(int button, int state, int x, int y)
{
	//Scroll Up
	if(button == 3)
	{
		Background.Position.y += 0.4;
	}
	//Scroll Down
	else if(button == 4)
	{
		Background.Position.y -= 0.4;
	}
}

void MouseMotion(int x, int y)
{
	if(x != gl.Width/2 || y != gl.Height/2)
		glutWarpPointer(gl.Width/2, gl.Height/2);
		
	//Rotation around y axis depends on moving x coords :P	
	gl.Rotation.y += 0.0015 * float(gl.Width/2 - x);
	gl.Rotation.x += 0.0015 * float(gl.Height/2 - y);
	if(gl.Rotation.x > 1.570796)
		gl.Rotation.x = 1.570796;
	else if(gl.Rotation.x < -1.570796)
		gl.Rotation.x = -1.570796;
}

int main(int argc, char** argv)
{
	string DataFile;
	if(argc == 2)
		DataFile = string(argv[1]);
	else
	{
		cout << "Data File: ";
		getline(cin, DataFile);
	}

	gl.GLInit(argc, argv, 500, 500, "Render Window", Display, Display, Resize, Keyboard, 0, MouseButton);
	glutKeyboardUpFunc(KeyboardUp);
	glutPassiveMotionFunc(MouseMotion);
	gl.Position = float3(0, 20, 0);
	gl.Rotation = float3(-0.3, 0, 0);
	
	Background.SetupVertices();
	Background.SetPosition(float3(0, -0.1, 0));
	Background.Rotation = float3(0.5*pi, 0, 0);
	Background.Texture("Images/myPalette.png", "glslBack.vert", "glslBack.frag");
	Background.Scale = float3(3000);
	
	if(!Interp(DataFile, &gl))
	{
		cout << "Interp() failed\n";
		return -1;
	}
	
	unsigned int bodies = gl.Objects.size();
	if(bodies > 16)
	{
		bodies = 16;
		cout << "Too many bodies, only using first 16\n";
	}

	float MaxGFS = 0;						//The highest gravitational field stength that should occur (without adding) to reduce extra white
	float planets[16*4] = {0.0f};			//Initialise more than needed because it needs to fill all 16 floats
	float radii[16] = {0.0f};
	tempV = new float3[gl.Objects.size()];	//Store first temp velocity
	for(unsigned int i = 0; i < bodies; i++)
	{
		planets[(4*i)] = gl.Objects[i]->Position.x;
		planets[(4*i)+1] = gl.Objects[i]->Position.y;
		planets[(4*i)+2] = gl.Objects[i]->Position.z;
		planets[(4*i)+3] = gl.Objects[i]->Mass;
		
		tempV[i] = gl.Objects[i]->Velocity;
		radii[i] = gl.Objects[i]->Scale.x;		//its a radius, so x, y, z scale's are all equal
		
		if(MaxGFS < (G * gl.Objects[i]->Mass)/(radii[i]*radii[i]));
			MaxGFS = (G * gl.Objects[i]->Mass)/(radii[i]*radii[i]);
	}
	MaxGFS *= 0.70;
	glUseProgram(Background.GetP());
	glUniform4fv(glGetUniformLocation(Background.GetP(), "Planets"), 16, planets);		//set the pos and mass
	glUniform1fv(glGetUniformLocation(Background.GetP(), "PlanetRadii"), 16, radii);		//set the radii
	glUniform1fv(glGetUniformLocation(Background.GetP(), "GravConstant"), 1, &G);
	glUniform1fv(glGetUniformLocation(Background.GetP(), "MaxGFS"), 1, &MaxGFS);

	for(unsigned int i = 0; i < bodies; i++)
	{
		Object3D* Line = gl.AddLine();
		Line->SetupVertices();
		Line->Mass = 0;
		Line->SetPosition(float3(0));
		Line->Texture(gl.Objects[i]->GetTexFile());
	}
	
	glutSetCursor(GLUT_CURSOR_NONE);
	glutWarpPointer(gl.Width/2, gl.Height/2);
	gl.StartFPS();
	glutMainLoop();
	return 0;
}
