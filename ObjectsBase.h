#ifndef OBJECTBASE_H
#define OBJECTBASE_H

#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glut.h>
#include <iostream>
#include <vector>
#include <math.h>
#include <SOIL/SOIL.h>

#include "Objects.h"
#include "Shaders.h"
#include "Vertex3fColour.h"

#define MEMBER_OFFSET(s,m) ((char *)NULL + (offsetof(s,m)))

class Object3D
{
protected:
	Vertex3fColour* vertices;
	GLuint* indices;
	float* mapcoords;
	bool VertChange, UseTex;
	string TexFile;
	
	GLuint VAO, verticesVBO, indicesVBO, textureVBO, tex;
	unsigned int verts, inds, maps;
	int imgWidth, imgHeight;
	
	GLenum mode;
	GLuint p, f, v;
	
public:
	float3 Velocity;
	float3 Position;
	float3 Rotation;
	float3 Scale;
	float Mass;
	
	mat4 MVP;
	
	Object3D()
	{
		Position = 0.0;
		Rotation = 0.0;
		Scale = 1.0;
		verticesVBO = 0;
		indicesVBO = 0;
		UseTex = false;
		
		MVP = Identity();
	}
	
	Object3D(const Object3D& Src)
	{
		verts = Src.GetVerts();
		for(int i = 0; i < verts; i++)
			vertices[i].Pos = Src.GetVertPos(i);
		inds = Src.GetInds();
		for(int i = 0; i < inds; i++)
			indices[i] = Src.GetIndice(i);
			
		VertChange = true;
		UseTex = true;
		TexFile = Src.GetTexFile();
		
		mode = Src.GetMode();
		p = Src.GetP();
		f = Src.GetF();
		v = Src.GetV();
		
		Velocity = Src.Velocity;
		Position = Src.Position;
		Rotation = Src.Rotation;
		Scale = Src.Scale;
		Mass = Src.Mass;
		MVP = Src.MVP;
	}
	
	Object3D(int Verts, int Inds, int Mode)
	{
		Position = float3(0);
		Scale = 1.0;
		verts = Verts;
		inds = Inds;
		mode = (GLenum)Mode;
		verticesVBO = 0;
		indicesVBO = 0;
		UseTex = false;
		
		vertices = new Vertex3fColour[verts];
		indices = new GLuint[inds];
	}
	
	void SetupVertices()
	{
		// Create Vertex Array Object
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);
		
		//Create Vertex Buffer Object
		glGenBuffers(1, &verticesVBO);
		glGenBuffers(1, &indicesVBO);
		
		glBindBuffer(GL_ARRAY_BUFFER, verticesVBO);
		glBufferData(GL_ARRAY_BUFFER, verts * sizeof(Vertex3fColour) , vertices, GL_STATIC_DRAW);
		
		// Copy the index data to the VBO
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesVBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, inds * sizeof(GLuint), indices, GL_STATIC_DRAW);
		
		VertChange = false;

		int i;
		if((i = glGetError()) != 0 && i != 1281)
		{
			cout << "OpenGL::SetupVertices() Error: " << i << endl;
			if(TexFile != "")
				cout << TexFile << endl;
		}
	}
	
	void Draw()
	{
		glBindVertexArray(VAO);
		glUseProgram(p);
		if(VertChange)
		{
			glBindBuffer(GL_ARRAY_BUFFER, verticesVBO);
			glBufferData(GL_ARRAY_BUFFER, verts * sizeof(Vertex3fColour) , vertices, GL_STATIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			VertChange = false;
		}
		
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_COLOR_ARRAY);
		
		if(UseTex)
		{
			// Bind our texture in Texture Unit 0
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, tex);
		}
		
		glUniformMatrix4fv(glGetUniformLocation(p, "MVP"), 1, GL_FALSE, (float*)MVP.p); //ModelView Projection matrix
		
		// Bind the vertices's VBO
		glBindBuffer(GL_ARRAY_BUFFER, verticesVBO);
		glVertexPointer(3, GL_FLOAT, sizeof(Vertex3fColour), MEMBER_OFFSET(Vertex3fColour, Pos));
		glColorPointer(3, GL_FLOAT, sizeof(Vertex3fColour), MEMBER_OFFSET(Vertex3fColour, Col));
		
		// Bind the indices's VBO
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesVBO);
		glDrawElements(mode, inds, GL_UNSIGNED_INT, 0);
		
		// Unbind buffers so client-side vertex arrays still work.
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
		
		if(UseTex)
			glBindTexture(GL_TEXTURE_2D, 0);
		
		// Disable the client side arrays again.
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_COLOR_ARRAY);
		
		int i;
		if((i = glGetError()) != 0 && i != 1281)
			cout << "OpenGL::Draw() Error: " << i << endl;
	}
	
	void SetPosition(float3 newPos)
	{
		Position = newPos;
	}
	void Translate(float3 Vector)
	{
		Position += Vector;
	}
	void SetScale(float3 newScale)
	{
		Scale = newScale;
	}
	
	void Texture(const char* ImgName, char* vertName = "glsl.vert", char* fragName = "glsl.frag")
	{
		//Shader Time!
		SetShaders(&p ,&f, &v, vertName, fragName);
		GLint texAttrib = glGetAttribLocation(p, "texcoord");
		glEnableVertexAttribArray(texAttrib);
		glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), 0);
		
		TexFile = ImgName;
		glGenTextures(1, &tex);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, tex);
		
		unsigned char* img = SOIL_load_image(ImgName, &imgWidth, &imgHeight, 0, SOIL_LOAD_RGBA);
		if(img == 0)
		{
			cout << "SOIL loading image error: " << SOIL_last_result() << endl;
			return;
		}
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imgWidth, imgHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, img);
		SOIL_free_image_data(img);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glGenerateMipmap(GL_TEXTURE_2D);
		
		glUniform1i(glGetUniformLocation(p, "textureMap"), 0);
		glBindTexture(GL_TEXTURE_2D, 0);
		UseTex = true;
	}
	
	void SetVertices(Vertex3fColour* vertSrc)
	{
		for(unsigned int i = 0; i < verts; i++)
		{
			vertices[i].Pos = vertSrc[i].Pos;
			vertices[i].Col = vertSrc[i].Col;
		}
		VertChange = true;
	}
	void SetIndices(GLuint* indSrc)
	{
		for(unsigned int i = 0; i < inds; i++)
			indices[i] = indSrc[i];
	}
	void SetMapCoords(float* mapSrc)
	{
		for(unsigned int i = 0; i < maps; i++)
			mapcoords[i] = mapSrc[i];
	}
	
	Object3D& operator= (const Vertex3fColour* vertSrc)
	{
		int i;
		for(i = 0; i < verts; i++)
		{
			vertices[i].Pos = vertSrc[i].Pos;
			vertices[i].Col = vertSrc[i].Col;
		}
		VertChange = true;
		return *this;
	}
	Object3D& operator= (const Object3D Src)
	{
		verts = Src.GetVerts();
		for(int i = 0; i < verts; i++)
			vertices[i].Pos = Src.GetVertPos(i);
		inds = Src.GetInds();
		for(int i = 0; i < inds; i++)
			indices[i] = Src.GetIndice(i);
			
		VertChange = true;
		UseTex = true;
		TexFile = Src.GetTexFile();
		
		/*GLuint VAO, verticesVBO, indicesVBO, textureVBO, tex;
		unsigned int verts, inds, maps;
		int imgWidth, imgHeight;*/
		
		mode = Src.GetMode();
		p = Src.GetP();
		f = Src.GetF();
		v = Src.GetV();
		
		Velocity = Src.Velocity;
		Position = Src.Position;
		Rotation = Src.Rotation;
		Scale = Src.Scale;
		Mass = Src.Mass;
		MVP = Src.MVP;
		return *this;
	}
	
	void PrintVerts()
	{
		for(int i = 0; i < verts; i++)
			cout << " " << 	vertices[i].Pos.x << " " << vertices[i].Pos.y << " " << vertices[i].Pos.z << " " << \
							vertices[i].Col.x << " " << vertices[i].Col.y << " " << vertices[i].Col.z << endl;
	}
	void PrintInds()
	{
		for(int i = 0; i < inds; i++)
		{
			cout << " " << indices[i] << " ";
			if((i+1) % 3 == 0)
				cout << endl;
		}
	}
	
	unsigned int GetP() const
	{
		return p;
	}
	unsigned int GetF() const
	{
		return f;
	}
	unsigned int GetV() const
	{
		return v;
	}
	const char* GetTexFile() const
	{
		return TexFile.c_str();
	}
	unsigned int GetVerts() const
	{
		return verts;
	}
	unsigned int GetInds() const
	{
		return inds;
	}
	GLenum GetMode() const
	{
		return mode;
	}
	float3 GetVertPos(unsigned int i) const
	{
		return vertices[i].Pos;
	}
	unsigned int GetIndice(unsigned int i) const
	{
		return indices[i];
	}
	
	~Object3D()
	{
		glDetachShader(p, v);
		glDetachShader(p, f);
		glDeleteProgram(p);
		
		delete[] vertices;
		delete[] indices;
	}
};
#endif