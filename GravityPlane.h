#include <reindeergl/Objects.h>

const float gravitationalConst = 1;

class GravityPlane : public PlaneObj
{
public:
	GravityPlane(const std::string& label, const std::string& texturePath);
	void Draw(const mat4& projectionViewMatrix);

private:
	static void UpdateContainedPlanets(std::shared_ptr<Object3D>& grvtyPlane, float deltaTime);
};
