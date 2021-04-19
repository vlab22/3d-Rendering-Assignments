#ifndef LIGHT_HPP
#define LIGHT_HPP

#include "glm.hpp"
#include "mge/core/GameObject.hpp"
#include "GL/glew.h"
#include "mge/core/ShaderProgram.hpp"

/**
 * Exercise for the student: implement the Light class...
 * Imagine things like setLightType, setLightIntensity, setFalloffAngle etc, see the corresponding lectures
 */
class Light : public GameObject
{

public:
	enum class LightType { POINT, DIRECTIONAL, SPOT };

	Light(const std::string& aName = nullptr, const glm::vec3& aPosition = glm::vec3(0.0f, 2.0f, 1.0f),
		LightType pType = LightType::POINT,
		glm::vec3 pIntensities = glm::vec3(1, 1, 1), //white
		float pAttenuation = 0.2,
		float pAmbientCoefficient = 0.005f,
		float pConeAngle = 10.0f
	);
	virtual ~Light();

	//override set parent to register/deregister light...
	virtual void _setWorldRecursively(World* pWorld) override;

	static void CacheLightUniform(ShaderProgram* shaders, int lightIndex);

	void SetLightUniform(int lightIndex);

	LightType lightType;

	glm::vec3 intensities;
	float attenuation;
	float ambientCoefficient;

	float coneAngle;

	//Material ids
	static GLint _uPosition[10];
	static GLint _uIntensities[10];
	static GLint _uAttenuation[10];
	static GLint _uAmbientCoefficient[10];

	static GLint _uConeAngle[10];
	static GLint _uConeDirection[10];
};
#endif // LIGHT_HPP