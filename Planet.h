#include <reindeergl/Objects.h>

class Planet : public SphereObj
{
public:
	Planet(const std::string& label, const std::string& texturePath, const float3& position, const float3& velocity, float mass = 1, unsigned int meshComplexity = 5);

	void SetMass(float mass);
	float Mass() const;

private:
	float mass;

	static void VelocityTickTock(std::shared_ptr<Object3D>& planet, float deltaTime);
};
