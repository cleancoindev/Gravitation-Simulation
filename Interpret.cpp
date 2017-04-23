#include "GravityPlane.h"
#include "Planet.h"

#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <reindeergl/ObjectContainer.h>


std::string Vals[7] = {"label:", "mass:", "pos:", "vel:", "rad:", "tex:", "def:"};

std::string Label(""), Tex("");
float Mass, Rad;
unsigned int Def;
float3 Pos(0), Vel(0);

std::shared_ptr<Planet> planet;

bool InterpGravityFile(const std::string& fileName, std::shared_ptr<GravityPlane>& grvty)
{
	std::ifstream File(fileName.c_str(), std::ios::in);
	if(File.is_open())
	{
		std::string line = "";
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
						std::string FoundVal = "";
						if(i != 2 && i != 3)
							FoundVal = line.substr(pos + Vals[i].length(), line.find(",", pos) - (pos + Vals[i].length()));

						switch(i)
						{
							case 0:
								Label = FoundVal;
								break;
							case 1:
								Mass = atof(FoundVal.c_str());
								break;
							case 2:
								//cout << line.substr(pos + Vals[i].length() + 1, line.find(",", pos) - (pos + Vals[i].length() + 1)).c_str() << endl;
								Pos.x() = atof(line.substr(pos + Vals[i].length() + 1, line.find(",", pos) - (pos + Vals[i].length() + 1)).c_str());
								pos = line.find(",", pos);

								//cout << line.substr(pos + 2, line.find(",", pos + 1) - (pos + 2)).c_str() << endl;
								Pos.y() = atof(line.substr(pos + 2, line.find(",", pos + 1) - (pos + 2)).c_str());
								pos = line.find(",", pos + 1);

								//cout << line.substr(pos + 2, line.find(")", pos) - (pos + 2)).c_str() << endl << endl;
								Pos.z() = atof(line.substr(pos + 2, line.find(")", pos) - (pos + 2)).c_str());
								break;
							case 3:
								//cout << line.substr(pos + Vals[i].length() + 1, line.find(",", pos) - (pos + Vals[i].length() + 1)).c_str() << endl;
								Vel.x() = atof(line.substr(pos + Vals[i].length() + 1, line.find(",", pos) - (pos + Vals[i].length() + 1)).c_str());
								pos = line.find(",", pos);

								//cout << line.substr(pos + 2, line.find(",", pos + 1) - (pos + 2)).c_str() << endl;
								Vel.y() = atof(line.substr(pos + 2, line.find(",", pos + 1) - (pos + 2)).c_str());
								pos = line.find(",", pos + 1);

								//cout << line.substr(pos + 2, line.find(")", pos) - (pos + 2)).c_str() << endl << endl;
								Vel.z() = atof(line.substr(pos + 2, line.find(")", pos) - (pos + 2)).c_str());
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

				planet = std::make_shared<Planet>(Label, Tex, Pos, Vel, Mass, Def);
				grvty->GetContainer().AddObjectPtr(planet);
			}
        }
        File.close();
	}
	else
	{
		if(fileName.substr(0, 5) != "Data/")
		{
			std::string ReTry("Data/");
			ReTry += fileName;
			return InterpGravityFile(ReTry, grvty);
		}
		else
			std::cerr << "Couldn't open file " << fileName << std::endl;

		return false;
	}

	return true;
}
