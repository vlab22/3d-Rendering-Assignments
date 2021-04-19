#include "glm.hpp"

#include "WobbleTextureMaterial.hpp"
#include "mge/core/Texture.hpp"
#include "mge/core/Light.hpp"
#include "mge/core/World.hpp"
#include "mge/core/Mesh.hpp"
#include "mge/core/GameObject.hpp"
#include "mge/core/ShaderProgram.hpp"
#include "mge/config.hpp"
#include <mge/core/AbstractGame.hpp>

ShaderProgram* WobbleTextureMaterial::_shader = NULL;

GLint WobbleTextureMaterial::_uMVPMatrix = 0;
GLint WobbleTextureMaterial::_uDiffuseTexture = 0;

GLint WobbleTextureMaterial::_aVertex = 0;
GLint WobbleTextureMaterial::_aNormal = 0;
GLint WobbleTextureMaterial::_aUV = 0;

GLfloat WobbleTextureMaterial::_uStrength = 0;
GLfloat WobbleTextureMaterial::_uTime = 0;

WobbleTextureMaterial::WobbleTextureMaterial(Texture* pDiffuseTexture) :_diffuseTexture(pDiffuseTexture) {
	_strength = 1;
	_lazyInitializeShader();
}

WobbleTextureMaterial::~WobbleTextureMaterial() {}

void WobbleTextureMaterial::_lazyInitializeShader() {
	if (!_shader) {
		_shader = new ShaderProgram();
		_shader->addShader(GL_VERTEX_SHADER, config::MGE_SHADER_PATH + "wobble_texture.vert");
		_shader->addShader(GL_FRAGMENT_SHADER, config::MGE_SHADER_PATH + "wobble_texture.frag");
		_shader->finalize();

		//cache all the uniform and attribute indexes
		_uMVPMatrix = _shader->getUniformLocation("mvpMatrix");
		_uDiffuseTexture = _shader->getUniformLocation("diffuseTexture");

		_aVertex = _shader->getAttribLocation("vertex");
		_aNormal = _shader->getAttribLocation("normal");
		_aUV = _shader->getAttribLocation("uv");

		_uStrength = _shader->getUniformLocation("strength");
		_uTime = _shader->getUniformLocation("time");
	}
}

void WobbleTextureMaterial::setDiffuseTexture(Texture* pDiffuseTexture) {
	_diffuseTexture = pDiffuseTexture;
}

void WobbleTextureMaterial::setStrength(float strength)
{
	_strength = strength;
}

void WobbleTextureMaterial::render(World* pWorld, Mesh* pMesh, const glm::mat4& pModelMatrix, const glm::mat4& pViewMatrix, const glm::mat4& pProjectionMatrix) {
	if (!_diffuseTexture) return;

	_shader->use();

	//Print the number of lights in the scene and the position of the first light.
	//It is not used, but this demo is just meant to show you THAT materials can access the lights in a world
	//if (pWorld->getLightCount() > 0) {
	//    std::cout << "WobbleTextureMaterial has discovered light is at position:" << pWorld->getLightAt(0)->getLocalPosition() << std::endl;
	//}

	float t = AbstractGame::timeInSeconds;
	glUniform1f(_uTime, t);

	glUniform1f(_uStrength, _strength);

	//std::cout << "t: " << t << std::endl;

	//setup texture slot 0
	glActiveTexture(GL_TEXTURE0);
	//bind the texture to the current active slot
	glBindTexture(GL_TEXTURE_2D, _diffuseTexture->getId());
	//tell the shader the texture slot for the diffuse texture is slot 0
	glUniform1i(_uDiffuseTexture, 0);

	//pass in a precalculate mvp matrix (see texture material for the opposite)
	glm::mat4 mvpMatrix = pProjectionMatrix * pViewMatrix * pModelMatrix;
	glUniformMatrix4fv(_uMVPMatrix, 1, GL_FALSE, glm::value_ptr(mvpMatrix));

	//now inform mesh of where to stream its data
	pMesh->streamToOpenGL(_aVertex, _aNormal, _aUV);
}

void WobbleTextureMaterial::onKeyPressed(sf::Keyboard::Key keyCode)
{
	std::cout << "key: " << keyCode << std::endl;

	if (keyCode == sf::Keyboard::W)
		_strength++;
	else if (keyCode == sf::Keyboard::S)
		_strength--;

	std::cout << "WobbleTextureMaterial strength: " << _strength << std::endl; 
}