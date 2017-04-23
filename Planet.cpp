#include "Planet.h"


Planet::Planet(const std::string& label, const std::string& texturePath, const float3& pos, const float3& vel, float m, unsigned int meshComplexity) : SphereObj(label, meshComplexity, texturePath)
{
	position = pos;
	velocity = vel;
	mass = m;
	SetTickTock(VelocityTickTock);
}

void Planet::SetMass(float m)
{
	mass = m;
}

float Planet::Mass() const
{
	return mass;
}

void Planet::VelocityTickTock(std::shared_ptr<Object3D>& planet, float deltaTime)
{
	float3 v = planet->Velocity();
	//std::cout << planet->GetLabel() << " " << v.ToString() << "\n";
	planet->Translate(v * deltaTime);
}
