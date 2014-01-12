#ifndef INTERPRET_H
#define INTERPRET_H

#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>

#include "OpenGL.h"
#include "Vertex3fColour.h"

using namespace std;

string Vals[7] = {"type:", "mass:", "pos:", "vel:", "rad:", "tex:", "def:"};

string Type(""), Tex("");
float Mass, Rad;
unsigned int Def;
float3 Pos(0), Vel(0);

Object3D* Obj;

bool Interp(string fileName, GL* gl)
{
	ifstream File(fileName.c_str(), ios::in);
	if(File.is_open())
	{
		string line = "";
		while(!File.eof())
        {
            getline(File, line);
            if(line[0] != '#' && line.length() != 0)
            {
                for(int i = 0; i < 7; i++)
				{
					int pos = line.find(Vals[i]);
					if(pos != -1)
					{
						string FoundVal = "";
						if(i != 2 && i != 3)
							FoundVal = line.substr(pos + Vals[i].length(), line.find(",", pos) - (pos + Vals[i].length()));
						
						switch(i)
						{
							case 0:
								Type = FoundVal;
								break;
							case 1:
								Mass = atof(FoundVal.c_str());
								break;
							case 2:
								//cout << line.substr(pos + Vals[i].length() + 1, line.find(",", pos) - (pos + Vals[i].length() + 1)).c_str() << endl;
								Pos.x = atof(line.substr(pos + Vals[i].length() + 1, line.find(",", pos) - (pos + Vals[i].length() + 1)).c_str());
								pos = line.find(",", pos);
								
								//cout << line.substr(pos + 2, line.find(",", pos + 1) - (pos + 2)).c_str() << endl;
								Pos.y = atof(line.substr(pos + 2, line.find(",", pos + 1) - (pos + 2)).c_str());
								pos = line.find(",", pos + 1);
								
								//cout << line.substr(pos + 2, line.find(")", pos) - (pos + 2)).c_str() << endl << endl;
								Pos.z = atof(line.substr(pos + 2, line.find(")", pos) - (pos + 2)).c_str());
								break;
							case 3:
								//cout << line.substr(pos + Vals[i].length() + 1, line.find(",", pos) - (pos + Vals[i].length() + 1)).c_str() << endl;
								Vel.x = atof(line.substr(pos + Vals[i].length() + 1, line.find(",", pos) - (pos + Vals[i].length() + 1)).c_str());
								pos = line.find(",", pos);
								
								//cout << line.substr(pos + 2, line.find(",", pos + 1) - (pos + 2)).c_str() << endl;
								Vel.y = atof(line.substr(pos + 2, line.find(",", pos + 1) - (pos + 2)).c_str());
								pos = line.find(",", pos + 1);
								
								//cout << line.substr(pos + 2, line.find(")", pos) - (pos + 2)).c_str() << endl << endl;
								Vel.z = atof(line.substr(pos + 2, line.find(")", pos) - (pos + 2)).c_str());
								break;
							case 4:
								Rad = atof(FoundVal.c_str());
								break;
							case 5:
								Tex = FoundVal;
								break;
							case 6:
								Def = atoi(FoundVal.c_str());
								break;
						}
					}
				}
				
				if(Type == "sphere")
				{
					Obj = gl->AddSphere(Def, Rad);
				}
				else
					return false;
				
				Obj->SetupVertices();
				Obj->Mass = Mass;
				Obj->SetPosition(Pos);
				Obj->Texture(Tex.c_str(), "glslSphere.vert");
				Obj->Velocity = Vel;
			}
        }
        File.close();
	}
	else
	{
		if(fileName.substr(0, 5) != "Data/")
		{
			string ReTry("Data/");
			ReTry += fileName;
			return Interp(ReTry, gl);
		}
		else
			cout << "Couldn't open file " << fileName << endl;
		
		return false;
	}
	
	return true;
}
#endif