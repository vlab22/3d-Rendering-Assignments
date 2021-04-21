#include "mge/core/Light.hpp"
#include "mge/core/World.hpp"
#include "mge/core/ShaderProgram.hpp"
#include <SFML/System/String.hpp>

GLint Light::_uPosition[10] = {};
GLint Light::_uIntensities[10] = {};
GLint Light::_uAttenuation[10] = {};
GLint Light::_uAmbientCoefficient[10] = {};

GLint Light::_uConeAngle[10] = {};
GLint Light::_uConeDirection[10] = {};

Light::Light(const std::string& pName, const glm::vec3& pPosition,
	LightType pType,
	glm::vec3 pIntensities, float pAttenuation, float pAmbientCoefficient,
	float pConeAngle) 
	: GameObject(pName, pPosition),
	lightType(pType),
	intensities(pIntensities),
	attenuation(pAttenuation),
	ambientCoefficient(pAmbientCoefficient),
	coneAngle(pConeAngle)
{}

Light::~Light() {
}

//Override setWorldRecursively to make sure we are registered
//to the world we are a part of.
void Light::_setWorldRecursively(World* pWorld) {

	//store the previous and new world before and after storing the given world
	World* previousWorld = _world;
	GameObject::_setWorldRecursively(pWorld);
	World* newWorld = _world;

	//check whether we need to register or unregister
	if (previousWorld != nullptr) previousWorld->unregisterLight(this);
	if (newWorld != nullptr) newWorld->registerLight(this);
}

/// <summary>
/// 
/// </summary>
/// <param name="shaders"></param>
/// <param name="lightIndex"></param>
void Light::CacheLightUniform(ShaderProgram* shaders, int lightIndex) {

	sf::String path("allLights[" + std::to_string(lightIndex) + "].");

	_uPosition[lightIndex] = shaders->getUniformLocation(path + "position");
	_uIntensities[lightIndex] = shaders->getUniformLocation(path + "intensities");
	_uAttenuation[lightIndex] = shaders->getUniformLocation(path + "attenuation");
	_uAmbientCoefficient[lightIndex] = shaders->getUniformLocation(path + "ambientCoefficient");
	_uConeAngle[lightIndex] = shaders->getUniformLocation(path + "coneAngle");
	_uConeDirection[lightIndex] = shaders->getUniformLocation(path + "coneDirection");

}

/// <summary>
/// must be enclosed by a shader->use()
/// </summary>
void Light::SetLightUniform(int lightIndex) {

	glUniform4fv(_uPosition[lightIndex], 1, glm::value_ptr(glm::vec4(this->getWorldPosition(), lightType == LightType::DIRECTIONAL ? 0 : 1)));
	glUniform3fv(_uIntensities[lightIndex], 1, glm::value_ptr(intensities));
	glUniform1f(_uAttenuation[lightIndex], attenuation);
	glUniform1f(_uAmbientCoefficient[lightIndex], ambientCoefficient);

	glUniform1f(_uConeAngle[lightIndex], lightType == LightType::SPOT ? coneAngle : 180.0f);
	glUniform3fv(_uConeDirection[lightIndex], 1, glm::value_ptr( glm::column(this->getWorldTransform(), 2)));
}


