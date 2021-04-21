#include "glm.hpp"

#include "DiffuseTerrainMaterial.hpp"
#include "mge/core/Texture.hpp"
#include "mge/core/Light.hpp"
#include "mge/core/World.hpp"
#include "mge/core/Mesh.hpp"
#include "mge/core/GameObject.hpp"
#include "mge/core/ShaderProgram.hpp"
#include "mge/config.hpp"

GLint DiffuseTerrainMaterial::_uMaterialShininess = 0;
GLint DiffuseTerrainMaterial::_uMaterialSpecularColor = 0;

GLint DiffuseTerrainMaterial::_uModelMatrix = 0;
GLint DiffuseTerrainMaterial::_uCamPosition = 0;

GLint DiffuseTerrainMaterial::_uNumLights = 0;


GLint DiffuseTerrainMaterial::_uDiffuseColor = 0;

DiffuseTerrainMaterial::DiffuseTerrainMaterial(Texture* pDiffuseTexture, Texture* pHeightMapTexture,
	float pMaterialShiness,
	glm::vec3 pMaterialSpecularColor) 
	: 	TerrainMaterial(pDiffuseTexture, pHeightMapTexture, false),
		materialShiness(pMaterialShiness),
		materialSpecularColor(pMaterialSpecularColor)
{
	_lazyInitializeShader();
}

DiffuseTerrainMaterial::~DiffuseTerrainMaterial() {}

void DiffuseTerrainMaterial::_lazyInitializeShader() {
	if (!_shader) {
		_shader = new ShaderProgram();
		_shader->addShader(GL_VERTEX_SHADER, config::MGE_SHADER_PATH + "tom_dalling_terrain.vert");
		_shader->addShader(GL_FRAGMENT_SHADER, config::MGE_SHADER_PATH + "tom_dalling_terrain.frag");
		_shader->finalize();

		TerrainMaterial::_initializeShaderIds();

		_uMaterialShininess = _shader->getUniformLocation("materialShininess");
		_uMaterialSpecularColor = _shader->getUniformLocation("materialSpecularColor");

		_uModelMatrix = _shader->getUniformLocation("modelMatrix");
		_uCamPosition = _shader->getUniformLocation("cameraPosition");
	}
}

void DiffuseTerrainMaterial::setSplatMapTexture(Texture* pDiffuseTexture) {
	_diffuse1Texture = pDiffuseTexture;
}

void DiffuseTerrainMaterial::CacheLightUniform(int lightIndex)
{
	Light::CacheLightUniform(_shader, lightIndex);
}

void DiffuseTerrainMaterial::CacheNumLights(int totalLights) {
	_uNumLights = _shader->getUniformLocation("numLights");
}

void DiffuseTerrainMaterial::render(World* pWorld, Mesh* pMesh, const glm::mat4& pModelMatrix, const glm::mat4& pViewMatrix, const glm::mat4& pProjectionMatrix) {

	_shader->use();

	//Print the number of lights in the scene and the position of the first light.
	//It is not used, but this demo is just meant to show you THAT materials can access the lights in a world
	int lightCount = pWorld->getLightCount();

	if (lightCount > 0) {
	    //std::cout << "DiffuseTerrainMaterial has discovered light is at position:" << pWorld->getLightAt(0)->getLocalPosition() << std::endl;
		
		for (int i = 0; i < lightCount; i++) {
			pWorld->getLightAt(i)->SetLightUniform(i);
		}
	}

	glUniform1i(_uNumLights, lightCount);

	glUniform1f(_uMaterialShininess, materialShiness);

	glUniform3fv(_uMaterialSpecularColor, 1, glm::value_ptr(materialSpecularColor));


	glUniformMatrix4fv(_uModelMatrix, 1, GL_FALSE, glm::value_ptr(pModelMatrix));
	
	glUniform3fv(_uCamPosition, 1, glm::value_ptr(glm::column(glm::inverse(pViewMatrix), 3)));

	TerrainMaterial::render(pWorld, pMesh, pModelMatrix, pViewMatrix, pProjectionMatrix);

	////setup texture slot 0
	//glActiveTexture(GL_TEXTURE0);
	////bind the texture to the current active slot
	//glBindTexture(GL_TEXTURE_2D, _diffuse1Texture->getId());
	////tell the shader the texture slot for the diffuse texture is slot 0
	//glUniform1i(_uTextureSplatMap, 0);

	////pass in a precalculate mvp matrix (see texture material for the opposite)
	//glm::mat4 mvpMatrix = pProjectionMatrix * pViewMatrix * pModelMatrix;
	//glUniformMatrix4fv(_uMVPMatrix, 1, GL_FALSE, glm::value_ptr(mvpMatrix));

	////now inform mesh of where to stream its data
	//pMesh->streamToOpenGL(_aVertex, _aNormal, _aUV);
}

//GLenum glCheckError_(const char* file, int line)
//{
//	GLenum errorCode;
//	while ((errorCode = glGetError()) != GL_NO_ERROR)
//	{
//		std::string error;
//		switch (errorCode)
//		{
//		case GL_INVALID_ENUM:                  error = "INVALID_ENUM"; break;
//		case GL_INVALID_VALUE:                 error = "INVALID_VALUE"; break;
//		case GL_INVALID_OPERATION:             error = "INVALID_OPERATION"; break;
//		case GL_STACK_OVERFLOW:                error = "STACK_OVERFLOW"; break;
//		case GL_STACK_UNDERFLOW:               error = "STACK_UNDERFLOW"; break;
//		case GL_OUT_OF_MEMORY:                 error = "OUT_OF_MEMORY"; break;
//		case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION"; break;
//		}
//		std::cout << error << " | " << file << " (" << line << ")" << std::endl;
//	}
//	return errorCode;
//}
//#define glCheckError() glCheckError_(__FILE__, __LINE__) 
