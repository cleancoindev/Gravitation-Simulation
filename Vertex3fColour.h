#ifndef VERTEX3FCOLOUR_H
#define VERTEX3FCOLOUR_H

#include <math.h>
#include <iostream>

class Quaternion;

class float3
{
public:
	float x, y, z;
	float3(float xx, float yy, float zz)
	{
		x = xx;
		y = yy;
		z = zz;
	}
	float3(float i)
	{
		x = i;
		y = i;
		z = i;
	}
	float3()
	{
		x = 0;
		y = 0;
		z = 0;
	}
	
	float3 CompDiv(float3 Src)
	{
		return float3(x / Src.x, y / Src.y, z / Src.z);
	}
	
	float3 CompMult(float3 Src)
	{
		return float3(x * Src.x, y * Src.y, z * Src.z);
	}
	
	float3& operator= (const float3 Src)
	{
		x = Src.x;
		y = Src.y;
		z = Src.z;
		return *this;
	}
	
	float3& operator= (const float Src)
	{
		x = Src;
		y = Src;
		z = Src;
		return *this;
	}
	bool operator== (const float3 cmp)
	{
		if(x == cmp.x && y == cmp.y && z == cmp.z)
			return true;
		else
			return false;
	}
	float3& operator+= (const float3 Src)
	{
		x += Src.x;
		y += Src.y;
		z += Src.z;
		return *this;
	}
	
	float3& operator-= (const float3 Src)
	{
		x -= Src.x;
		y -= Src.y;
		z -= Src.z;
		return *this;
	}
	
	float3 operator+ (float3 Src)
	{
		return float3(x + Src.x, y + Src.y, z + Src.z);
	}
	
	float3 operator- (float3 Src)
	{
		return float3(x - Src.x, y - Src.y, z - Src.z);
	}
	
	float3 operator- ()
	{
		return float3(-x, -y, -z);
	}
	
	float3 operator/ (float Num)
	{
		return float3(x / Num, y / Num, z / Num);
	}
	float3 operator* (float Num)
	{
		return float3(x * Num, y * Num, z * Num);
	}
	void Print()
	{
		std::cout << x << ", " << y << ", " << z << std::endl;
	}
};

float3 Norm3f(float3 num)
{
	float mag = (num.x*num.x) + (num.y*num.y) + (num.z*num.z);
	if(fabs(mag) > 0.00001f && fabs(mag - 1.0f) > 0.00001f) 
	{
		mag = sqrt(mag);
		num.x /= mag;
		num.y /= mag;
		num.z /= mag;
	}
	return num;
}

float3 Cross(float3 a, float3 b)
{
	return float3(a.y*b.z - a.z*b.y, a.z*b.x - a.x*b.z, a.x*b.y - a.y*b.x);
}

float Dot(float3 a, float3 b)
{
	return float(a.x*b.x + a.y*b.y + a.z*b.z);
}

float Mag(float3 num)
{
	float mag = (num.x*num.x) + (num.y*num.y) + (num.z*num.z);
	if(fabs(mag) > 0.00001f && fabs(mag - 1.0f) > 0.00001f) 
	{
		mag = sqrt(mag);
	}
	else
		return 0.0f;
	
	return mag;
}

class Quaternion
{
public:
	float x, y, z, w;
	float magnitude;
	
	Quaternion()
	{
		x = 0;
		y = 0;
		z = 0;
		w = 0;
		magnitude = 0;
	}
	
	Quaternion(float xx, float yy, float zz, float ww)
	{
		x = xx;
		y = yy;
		z = zz;
		w = ww;
		magnitude = sqrt((x*x) + (y*y) + (z*z) + (w*w));
	}
	
	Quaternion(float i)
	{
		x = i;
		y = i;
		z = i;
		w = i;
		magnitude = sqrt((i*i) * 4);
	}
	
	Quaternion(float3 num, float ww)
	{
		x = num.x;
		y = num.y;
		z = num.z;
		w = ww;
		magnitude = sqrt((x*x) + (y*y) + (z*z) + (w*w));
	}
	
	Quaternion(const Quaternion& q)
	{
		x = q.x;
		y = q.y;
		z = q.z;
		w = q.w;
		magnitude = q.magnitude;
	}
	
	void Normalize()
	{
		magnitude = (x*x) + (y*y) + (z*z) + (w*w);
		if(fabs(magnitude) > 0.00001f && fabs(magnitude - 1.0f) > 0.00001f) 
		{
			magnitude = sqrt(magnitude);
			x = x / magnitude;
			y = y / magnitude;
			z = z / magnitude;
			w = w / magnitude;
		}
	}
	
	Quaternion GetConj()
	{
		return Quaternion(-x, -y, -z, w);
	}
	
	float3 ToF3()
	{
		return float3(x, y, z);
	}
	
	void Print()
	{
		std::cout << x << ", " << y << ", " << z << ", " << w << std::endl;
		return;
	}
	
	Quaternion operator* (Quaternion Src)
	{
		return Quaternion  ((w*Src.x) + (x*Src.w) + (y*Src.z) - (z*Src.y),\
							(w*Src.y) - (x*Src.z) + (y*Src.w) + (z*Src.x),\
							(w*Src.z) + (x*Src.y) - (y*Src.x) + (z*Src.w),\
							(w*Src.w) - (x*Src.x) - (y*Src.y) - (z*Src.z));
	}
	
	Quaternion& operator+= (const float3 Src)
	{
		x += Src.x;
		y += Src.y;
		z += Src.z;
		magnitude = sqrt(x*x + y*y + z*z + w*w);
		return *this;
	}
	
	Quaternion& operator-= (const float3 Src)
	{
		x -= Src.x;
		y -= Src.y;
		z -= Src.z;
		magnitude = sqrt(x*x + y*y + z*z + w*w);
		return *this;
	}
	
	Quaternion& operator= (const Quaternion Src)
	{
		x = Src.x;
		y = Src.y;
		z = Src.z;
		w = Src.w;
		magnitude = Src.magnitude;
		return *this;
	}
	
	Quaternion& operator= (const float3 Src)
	{
		x = Src.x;
		y = Src.y;
		z = Src.z;
		w = 0;
		magnitude = sqrt((x*x) + (y*y) + (z*z));
		return *this;
	}
	
	Quaternion& operator= (const float Src)
	{
		x = Src;
		y = Src;
		z = Src;
		w = Src;
		magnitude = sqrt((x*x) + (y*y) + (z*z) + (w*w));
		return *this;
	}
};

//REMEMBER MOTHER FUCKING COLUMN MAJOR OPENGL!!!!!!!!
class mat4
{
public:
	float p[4][4];
	
	mat4()
	{
		for(int col = 0; col < 4; col++)
			for(int row = 0; row < 4; row++)
				p[col][row] = 0.0;
	}
	
	mat4(float num)
	{
		for(int col = 0; col < 4; col++)
			for(int row = 0; row < 4; row++)
				p[col][row] = num;
	}
	
	mat4(Quaternion q)
	{
		float c = cosf(q.w);
		float s = sinf(q.w);
	
		p[0][0] = c + ((q.x * q.x) * (1 - c));
		p[0][1] = (q.y * q.x) * (1 - c) + (q.z * s);
		p[0][2] = (q.z * q.x) * (1 - c) - (q.y * s);
		p[0][3] = 0.0f;
		
		p[1][0] = (q.x * q.y) * (1 - c) - (q.z * s);
		p[1][1] = c + ((q.y * q.y) * (1 - c));
		p[1][2] = (q.z * q.y) * (1 - c) + (q.x * s);
		p[1][3] = 0.0f;
		
		p[2][0] = (q.x * q.z) * (1 - c) + (q.y * s);
		p[2][1] = (q.y * q.z) * (1 - c) - (q.x * s);
		p[2][2] = c + ((q.z * q.z) * (1 - c));
		p[2][3] = 0.0f;
		
		p[3][0] = 0.0f;
		p[3][1] = 0.0f;
		p[3][2] = 0.0f;
		p[3][3] = 1.0f;
	}
	
	mat4(float* m)
	{
		for(int col = 0; col < 4; col++)
			for(int row = 0; row < 4; row++)
				p[col][row] = m[(4*col) + row];
	}
	
	mat4(const mat4& A)
	{
		for(int col = 0; col < 4; col++)
			for(int row = 0; row < 4; row++)
				p[col][row] = A.p[col][row];
	}
	
	mat4 operator* (mat4 B)
	{
		mat4 C;
		for(int col = 0; col < 4; col++)
			for(int row = 0; row < 4; row++)
				C.p[col][row] = ((p[0][row] * B.p[col][0]) + (p[1][row] * B.p[col][1]) + (p[2][row] * B.p[col][2]) + (p[3][row] * B.p[col][3]));
		
		return C;
	}
	
	Quaternion operator* (Quaternion q)
	{
		Quaternion r(0.0f);
		r.x = ((p[0][0] * q.x) + (p[0][1] * q.y) + (p[0][2] * q.z) + (p[0][3] * q.w));
		r.y = ((p[1][0] * q.x) + (p[1][1] * q.y) + (p[1][2] * q.z) + (p[1][3] * q.w));
		r.z = ((p[2][0] * q.x) + (p[2][1] * q.y) + (p[2][2] * q.z) + (p[2][3] * q.w));
		r.w = ((p[3][0] * q.x) + (p[3][1] * q.y) + (p[3][2] * q.z) + (p[3][3] * q.w));
		return r;
	}
	
	float3 operator* (float3 f)
	{
		float3 r(0.0f);
		r.x = ((p[0][0] * f.x) + (p[1][0] * f.y) + (p[2][0] * f.z));
		r.y = ((p[0][1] * f.x) + (p[1][1] * f.y) + (p[2][1] * f.z));
		r.z = ((p[0][2] * f.x) + (p[1][2] * f.y) + (p[2][2] * f.z));
		return r;
	}
	
	mat4& operator= (const mat4 Src)
	{
		for(int col = 0; col < 4; col++)
			for(int row = 0; row < 4; row++)
				p[col][row] = Src.p[col][row];
			
		return *this;	
	}
	
	bool operator== (const mat4 Src)
	{
		for(int col = 0; col < 4; col++)
			for(int row = 0; row < 4; row++)
				if(p[col][row] != Src.p[col][row])
					return false;
					
		return true;
	}
};

mat4 Translate(float3 n)
{
	mat4 result(0.0);
	result.p[0][0] = 1.0;
	result.p[1][1] = 1.0;
	result.p[2][2] = 1.0;
	result.p[3][3] = 1.0;
	
	result.p[3][0] = n.x;
	result.p[3][1] = n.y;
	result.p[3][2] = n.z;
				
	return result;
}

mat4 Rotate(float3 theta)
{
	mat4 rotX;
	mat4 rotY;
	mat4 rotZ;
	
	for(int col = 0; col < 4; col++)
	{
		for(int row = 0; row < 4; row++)
		{
			if(((col*4) + row) % 5 == 0)
			{
				rotX.p[col][row] = 1;
				rotY.p[col][row] = 1;
				rotZ.p[col][row] = 1;
			}
			else
			{
				rotX.p[col][row] = 0;
				rotY.p[col][row] = 0;
				rotZ.p[col][row] = 0;
			}
		}
	}
	
	rotX.p[1][1] = cosf(theta.x);
	rotX.p[2][1] = sinf(theta.x);
	rotX.p[1][2] = -sinf(theta.x);
	rotX.p[2][2] = cos(theta.x);
	
	rotY.p[0][0] = cos(theta.y);
	rotY.p[2][0] = -sinf(theta.y);
	rotY.p[0][2] = sinf(theta.y);
	rotY.p[2][2] = cosf(theta.y);
	
	rotZ.p[0][0] = cosf(theta.z);
	rotZ.p[1][0] = sinf(theta.z);
	rotZ.p[0][1] = -sinf(theta.z);
	rotZ.p[1][1] = cosf(theta.z);
	
	return (rotX * rotY * rotZ);
}

mat4 Scale(float3 s)
{
	mat4 m(0.0);
	m.p[0][0] = s.x;
	m.p[1][1] = s.y;
	m.p[2][2] = s.z;
	m.p[3][3] = 1;
	return m;
}
mat4 Scale(float x, float y, float z)
{
	mat4 m(0.0);
	m.p[0][0] = x;
	m.p[1][1] = y;
	m.p[2][2] = z;
	m.p[3][3] = 1;
	return m;
}
mat4 ScaleMat(float3 v)
{
	return Scale(v);
}

mat4 Perspective(float fov, float ratio, float nearZ, float farZ)
{
	float scale = tanf(fov * 0.5 * (3.1415926535 / 180)) * nearZ;
	float rightX = ratio * scale, leftX = -rightX;
	float topY = scale, bottomY = -topY;
	
	mat4 proj(0.0f);
	proj.p[0][0] = 2 * nearZ / (rightX - leftX);
	proj.p[1][1] = 2 * nearZ / (topY - bottomY);
	proj.p[2][0] = (rightX + leftX) / (rightX - leftX);
	proj.p[2][1] = (topY + bottomY) / (topY - bottomY);
	proj.p[2][2] = -(farZ + nearZ) / (farZ - nearZ);
	proj.p[2][3] = -1;
	proj.p[3][2] = -2 * farZ * nearZ / (farZ - nearZ);
	return proj;
}

mat4 Identity()
{
	mat4 ident(0.0);
	for(int col = 0; col < 4; col++)
	{
		for(int row = 0; row < 4; row++)
		{
			if((col*4 + row) % 5 == 0)
				ident.p[col][row] = 1;
		}
	}
	return ident;
}

mat4 Inverse(mat4 M)
{
	float* m;
	m = (float*)(&M.p[0]);
    float inv[16], det;
	float invOut[16];
 
    inv[ 0] =  m[5] * m[10] * m[15] - m[5] * m[11] * m[14] - m[9] * m[6] * m[15] + m[9] * m[7] * m[14] + m[13] * m[6] * m[11] - m[13] * m[7] * m[10];
    inv[ 4] = -m[4] * m[10] * m[15] + m[4] * m[11] * m[14] + m[8] * m[6] * m[15] - m[8] * m[7] * m[14] - m[12] * m[6] * m[11] + m[12] * m[7] * m[10];
    inv[ 8] =  m[4] * m[ 9] * m[15] - m[4] * m[11] * m[13] - m[8] * m[5] * m[15] + m[8] * m[7] * m[13] + m[12] * m[5] * m[11] - m[12] * m[7] * m[ 9];
    inv[12] = -m[4] * m[ 9] * m[14] + m[4] * m[10] * m[13] + m[8] * m[5] * m[14] - m[8] * m[6] * m[13] - m[12] * m[5] * m[10] + m[12] * m[6] * m[ 9];
    inv[ 1] = -m[1] * m[10] * m[15] + m[1] * m[11] * m[14] + m[9] * m[2] * m[15] - m[9] * m[3] * m[14] - m[13] * m[2] * m[11] + m[13] * m[3] * m[10];
    inv[ 5] =  m[0] * m[10] * m[15] - m[0] * m[11] * m[14] - m[8] * m[2] * m[15] + m[8] * m[3] * m[14] + m[12] * m[2] * m[11] - m[12] * m[3] * m[10];
    inv[ 9] = -m[0] * m[ 9] * m[15] + m[0] * m[11] * m[13] + m[8] * m[1] * m[15] - m[8] * m[3] * m[13] - m[12] * m[1] * m[11] + m[12] * m[3] * m[ 9];
    inv[13] =  m[0] * m[ 9] * m[14] - m[0] * m[10] * m[13] - m[8] * m[1] * m[14] + m[8] * m[2] * m[13] + m[12] * m[1] * m[10] - m[12] * m[2] * m[ 9];
    inv[ 2] =  m[1] * m[ 6] * m[15] - m[1] * m[ 7] * m[14] - m[5] * m[2] * m[15] + m[5] * m[3] * m[14] + m[13] * m[2] * m[ 7] - m[13] * m[3] * m[ 6];
    inv[ 6] = -m[0] * m[ 6] * m[15] + m[0] * m[ 7] * m[14] + m[4] * m[2] * m[15] - m[4] * m[3] * m[14] - m[12] * m[2] * m[ 7] + m[12] * m[3] * m[ 6];
    inv[10] =  m[0] * m[ 5] * m[15] - m[0] * m[ 7] * m[13] - m[4] * m[1] * m[15] + m[4] * m[3] * m[13] + m[12] * m[1] * m[ 7] - m[12] * m[3] * m[ 5];
    inv[14] = -m[0] * m[ 5] * m[14] + m[0] * m[ 6] * m[13] + m[4] * m[1] * m[14] - m[4] * m[2] * m[13] - m[12] * m[1] * m[ 6] + m[12] * m[2] * m[ 5];
    inv[ 3] = -m[1] * m[ 6] * m[11] + m[1] * m[ 7] * m[10] + m[5] * m[2] * m[11] - m[5] * m[3] * m[10] - m[ 9] * m[2] * m[ 7] + m[ 9] * m[3] * m[ 6];
    inv[ 7] =  m[0] * m[ 6] * m[11] - m[0] * m[ 7] * m[10] - m[4] * m[2] * m[11] + m[4] * m[3] * m[10] + m[ 8] * m[2] * m[ 7] - m[ 8] * m[3] * m[ 6];
    inv[11] = -m[0] * m[ 5] * m[11] + m[0] * m[ 7] * m[ 9] + m[4] * m[1] * m[11] - m[4] * m[3] * m[ 9] - m[ 8] * m[1] * m[ 7] + m[ 8] * m[3] * m[ 5];
    inv[15] =  m[0] * m[ 5] * m[10] - m[0] * m[ 6] * m[ 9] - m[4] * m[1] * m[10] + m[4] * m[2] * m[ 9] + m[ 8] * m[1] * m[ 6] - m[ 8] * m[2] * m[ 5];
 
    det = m[0] * inv[0] + m[1] * inv[4] + m[2] * inv[8] + m[3] * inv[12];
 
    if(det == 0)
        Identity();
 
    det = 1.f / det;
 
    for(int i = 0; i < 16; i++)
        invOut[i] = inv[i] * det;
 
    return mat4(invOut);
}

struct Vertex3fColour
{
    float3 Pos;
	float3 Col;
};
#endif