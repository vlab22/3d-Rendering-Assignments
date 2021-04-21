#include "glm.hpp"

#include "TerrainMaterial.hpp"
#include "mge/core/Texture.hpp"
#include "mge/core/Light.hpp"
#include "mge/core/World.hpp"
#include "mge/core/Mesh.hpp"
#include "mge/core/GameObject.hpp"
#include "mge/core/ShaderProgram.hpp"
#include "mge/config.hpp"

ShaderProgram* TerrainMaterial::_shader = NULL;

GLint TerrainMaterial::_uMVPMatrix = 0;
GLint TerrainMaterial::_uTextureSplatMap = 0;
GLint TerrainMaterial::_uHeightMapTexture = 0;
GLint TerrainMaterial::_uHeightFactor = 0;

GLint TerrainMaterial::_uTextureDiffuse1 = 0;
GLint TerrainMaterial::_uTextureDiffuse2 = 0;
GLint TerrainMaterial::_uTextureDiffuse3 = 0;
GLint TerrainMaterial::_uTextureDiffuse4 = 0;

GLint TerrainMaterial::_aVertex = 0;
GLint TerrainMaterial::_aNormal = 0;
GLint TerrainMaterial::_aUV = 0;

TerrainMaterial::TerrainMaterial(Texture* pTextureSplatMap, Texture* pHeightMapTexture, bool initShader)
	:_splatMapTexture(pTextureSplatMap), _heightMapTexture(pHeightMapTexture) {
	
	_diffuse1Texture = nullptr;
	_diffuse2Texture = nullptr;
	_diffuse3Texture = nullptr;
	_diffuse4Texture = nullptr;

	if (initShader)
		_lazyInitializeShader();
}

TerrainMaterial::~TerrainMaterial() {}

void TerrainMaterial::_lazyInitializeShader() {
	if (!_shader) {
		_shader = new ShaderProgram();
		_shader->addShader(GL_VERTEX_SHADER, config::MGE_SHADER_PATH + "terrain.vert");
		_shader->addShader(GL_FRAGMENT_SHADER, config::MGE_SHADER_PATH + "terrain.frag");
		_shader->finalize();

		_initializeShaderIds();
	}
}

void TerrainMaterial::_initializeShaderIds() {
	//cache all the uniform and attribute indexes
	_uMVPMatrix = _shader->getUniformLocation("mvpMatrix");
	_uTextureSplatMap = _shader->getUniformLocation("textureSplatMap");
	_uHeightMapTexture = _shader->getUniformLocation("heightMapTexture");
	_uHeightFactor = _shader->getUniformLocation("heightFactor");

	_uTextureDiffuse1 = _shader->getUniformLocation("textureDiffuse1");
	_uTextureDiffuse2 = _shader->getUniformLocation("textureDiffuse2");
	_uTextureDiffuse3 = _shader->getUniformLocation("textureDiffuse3");
	_uTextureDiffuse4 = _shader->getUniformLocation("textureDiffuse4");

	_aVertex = _shader->getAttribLocation("vertex");
	_aNormal = _shader->getAttribLocation("normal");
	_aUV = _shader->getAttribLocation("uv");
}

void TerrainMaterial::setSplatMapTexture(Texture* pSplatMapTexture) {
	_splatMapTexture = pSplatMapTexture;
}

void TerrainMaterial::setHeightMapTexture(Texture* pHeightMapTexture)
{
	_heightMapTexture = pHeightMapTexture;
}

void TerrainMaterial::setDiffuseTextures(Texture* pDiffuseTexture1, Texture* pDiffuseTexture2, Texture* pDiffuseTexture3, Texture* pDiffuseTexture4)
{
	_diffuse1Texture = pDiffuseTexture1;
	_diffuse2Texture = pDiffuseTexture2;
	_diffuse3Texture = pDiffuseTexture3;
	_diffuse4Texture = pDiffuseTexture4;
}

void TerrainMaterial::render(World* pWorld, Mesh* pMesh, const glm::mat4& pModelMatrix, const glm::mat4& pViewMatrix, const glm::mat4& pProjectionMatrix) {

	_shader->use();

	//Print the number of lights in the scene and the position of the first light.
	//It is not used, but this demo is just meant to show you THAT materials can access the lights in a world
	//if (pWorld->getLightCount() > 0) {
	//    std::cout << "TerrainMaterial has discovered light is at position:" << pWorld->getLightAt(0)->getLocalPosition() << std::endl;
	//}

	//Set heightfactor
	glUniform1f(_uHeightFactor, heightFactor);

	if (_heightMapTexture != nullptr)
	{
		//setup texture slot 0
		glActiveTexture(GL_TEXTURE0);
		//bind the texture to the current active slot
		glBindTexture(GL_TEXTURE_2D, _heightMapTexture->getId());
		//tell the shader the texture slot for the diffuse texture is slot 0
		glUniform1i(_uHeightMapTexture, 0);
	}

	if (_splatMapTexture != nullptr) {
		//setup texture slot 0
		glActiveTexture(GL_TEXTURE1);
		//bind the texture to the current active slot
		glBindTexture(GL_TEXTURE_2D, _splatMapTexture->getId());
		//tell the shader the texture slot for the diffuse texture is slot 0
		glUniform1i(_uTextureSplatMap, 1);
	}

	if (_diffuse1Texture != nullptr) {
		//setup texture slot 0
		glActiveTexture(GL_TEXTURE2);
		//bind the texture to the current active slot
		glBindTexture(GL_TEXTURE_2D, _diffuse1Texture->getId());
		//tell the shader the texture slot for the diffuse texture is slot 0
		glUniform1i(_uTextureDiffuse1, 2);
	}

	if (_diffuse2Texture != nullptr) {
		//setup texture slot 0
		glActiveTexture(GL_TEXTURE3);
		//bind the texture to the current active slot
		glBindTexture(GL_TEXTURE_2D, _diffuse2Texture->getId());
		//tell the shader the texture slot for the diffuse texture is slot 0
		glUniform1i(_uTextureDiffuse2, 3);
	}

	if (_diffuse3Texture != nullptr) {
		//setup texture slot 0
		glActiveTexture(GL_TEXTURE4);
		//bind the texture to the current active slot
		glBindTexture(GL_TEXTURE_2D, _diffuse3Texture->getId());
		//tell the shader the texture slot for the diffuse texture is slot 0
		glUniform1i(_uTextureDiffuse3, 4);
	}

	if (_diffuse4Texture != nullptr) {
		//setup texture slot 0
		glActiveTexture(GL_TEXTURE5);
		//bind the texture to the current active slot
		glBindTexture(GL_TEXTURE_2D, _diffuse4Texture->getId());
		//tell the shader the texture slot for the diffuse texture is slot 0
		glUniform1i(_uTextureDiffuse4, 5);
	}
	

	//pass in a precalculate mvp matrix (see texture material for the opposite)
	glm::mat4 mvpMatrix = pProjectionMatrix * pViewMatrix * pModelMatrix;
	glUniformMatrix4fv(_uMVPMatrix, 1, GL_FALSE, glm::value_ptr(mvpMatrix));

	//now inform mesh of where to stream its data
	pMesh->streamToOpenGL(_aVertex, _aNormal, _aUV);
}
