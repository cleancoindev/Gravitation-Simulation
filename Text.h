#ifndef TEXT_H
#define TEXT_H

#include <math.h>
#include <iostream>

#include "ObjectsBase.h"
#include "Vertex3fColour.h"

using namespace std;

class String3D
{
protected:
	float3 Position;
	float3 Scale;
	float Rotation;
	string Text;
	vector<Object3D*> Chars;
public:
	void Changed()
	{
		for(int i = 0; i < Text.size(); i++)
		{
			Chars[i]->Scale = float3(0.625 * Scale.x, Scale.y, 0);
			Chars[i]->Position = Position;
			Chars[i]->Position.x += (Scale.x) + ((i * Scale.x * 2) * cosf(Rotation));
			Chars[i]->Position.y += (Scale.y) + ((i * Scale.x * 2) * -sinf(Rotation));
		}
	}
	
	String3D(string text)
	{
		Position = float3(-1);
		Scale = float3(0.1);
		Rotation = 0;
		Text = text;
		for(int i = 0; i < Text.size(); i++)
		{
			Chars.push_back(new PlaneObj());
			Chars[i]->SetupVertices();
			
			string c = "Chars/";
			if(Text[i] == '.')
				c += "period";
			else if(Text[i] == '$')
				c += "dollar";
			//Many More exceptions...
			else
				c += Text[i];
			c += ".png";
			Chars[i]->Texture(c.c_str());
		}
		Changed();
	}
	
	void Print()
	{
		for(int i = 0; i < Text.size(); i++)
		{
			Chars[i]->MVP = Translate(Chars[i]->Position) * Rotate(float3(0, 0, Rotation)) * ScaleMat(Scale);
			Chars[i]->Draw();
		}
	}
	
	void SetPosition(float3 v)
	{
		Position = v;
		Changed();
		return;
	}
	float3 GetPosition()
	{
		return Position;
	}
	void SetScale(float3 v)
	{
		Scale = v;
		Changed();
		return;
	}
	float3 GetScale()
	{
		return Scale;
	}
	void SetRotation(float r)
	{
		Rotation = r;
		Changed();
		return;
	}
	float GetRotation()
	{
		return Rotation;
	}
	void SetText(string str)
	{
		Text = str;
		Chars.clear();
		for(int i = 0; i < Text.size(); i++)
		{
			Chars.push_back(new PlaneObj());
			Chars[i]->SetupVertices();
			
			string c = "Chars/";
			if(Text[i] == '.')
				c += "period";
			else if(Text[i] == '$')
				c += "dollar";
			//Many More exceptions...
			else
				c += Text[i];
			c += ".png";
			Chars[i]->Texture(c.c_str());
		}
		Changed();
		return;
	}
	string GetText()
	{
		return Text;
	}
	/*String3D& operator= (string str)
	{
		Text = str;
		Changed();
		return *this;
	}*/
};

#endif