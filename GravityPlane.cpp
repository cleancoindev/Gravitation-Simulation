#include "GravityPlane.h"
#include "Planet.h"

#include <reindeergl/ObjectContainer.h>
#include <reindeergl/GLMath.h>


GravityPlane::GravityPlane(const std::string& label, const std::string& texturePath) : PlaneObj(label, "")
{
	SetShaders("gravityPlane.glslv", "gravityPlane.glslf", texturePath);
	glUniform1f(glGetUniformLocation(programId, "G"), gravitationalConst);

	SetTickTock(UpdateContainedPlanets);
}

void GravityPlane::Draw(const mat4& projectionViewMatrix)
{
	if(!initialized)
    {
		throw std::runtime_error("Object not yet initialized, but Draw() was invoked");
    }

    if(programId)
    {
		glUseProgram(programId);
    }

    glBindVertexArray(vaoId);
    if(textureId)
    {
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, textureId);
    }

    mat4 mvp = projectionViewMatrix * modelMatrix;
    glUniformMatrix4fv(glGetUniformLocation(programId, "MVP"), 1, GL_FALSE, (float*)&mvp.p[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(programId, "modelM"), 1, GL_FALSE, (float*)&modelMatrix.p[0][0]);

    float planets[4 * 16];		// Max Planet size of 16
    float planetRadii[16];
    unsigned int size = container.Size();
    unsigned int i = 0;
    float maxGFS = 0;
    for(LabelObjectPair lop : container)
    {
    	std::shared_ptr<Planet> planetPtr = std::dynamic_pointer_cast<Planet>(lop.second);
    	planets[4*i + 0] = planetPtr->Position().x();
    	planets[4*i + 1] = planetPtr->Position().y();
    	planets[4*i + 2] = planetPtr->Position().z();
    	planets[4*i + 3] = planetPtr->Mass();
		planetRadii[i] = planetPtr->GetScale().x();

		maxGFS = std::max(gravitationalConst * planetPtr->Mass() / (planetPtr->GetScale().x() * planetPtr->GetScale().x()), maxGFS);

    	if(++i >= 16)
    	{
    		size = 16;
    		break;
    	}
    }

	glUniform4fv(glGetUniformLocation(programId, "Planets"), 16, planets);
	glUniform1i(glGetUniformLocation(programId, "PlanetsLen"), size);
	glUniform1fv(glGetUniformLocation(programId, "PlanetRadii"), 16, planetRadii);
	glUniform1f(glGetUniformLocation(programId, "MaxGFS"), maxGFS);

    glDrawArrays(mode, 0, verts);

    glBindTexture(GL_TEXTURE_2D, 0);
    glBindVertexArray(0);

    // Now draw all sub-contained objects (using orignal projectionView matrix because we dont want to move planets with gravity plane)
    for(ConstLabelObjectPair objectPair : container)
    {
		const std::shared_ptr<Object3D> object = objectPair.second;
		object->Draw(projectionViewMatrix);
    }
}

void GravityPlane::UpdateContainedPlanets(std::shared_ptr<Object3D>& grvtyPlane, float deltaTime)
{
	for(LabelObjectPair outerLop : grvtyPlane->GetContainer())
	{
		std::shared_ptr<Planet> object = std::dynamic_pointer_cast<Planet>(outerLop.second);
		float3 acceleration(0);

		for(LabelObjectPair innerLop : grvtyPlane->GetContainer())
		{
			std::shared_ptr<Planet> object2 = std::dynamic_pointer_cast<Planet>(innerLop.second);

			if(object2->GetLabel() != object->GetLabel())
			{
				float3 r = object2->Position() - object->Position();
				acceleration += r.Norm() / GLMath::Dot(r, r) * object2->Mass();
			}
		}
		acceleration *= gravitationalConst;

		float3 v = object->Velocity();
		object->SetVelocity(v + acceleration * deltaTime);
		object->GetTickTock()(outerLop.second, deltaTime);
	}
}
