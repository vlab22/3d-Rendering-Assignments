#include "glm.hpp"

#include "DiffuseTextureMaterial.hpp"
#include "mge/core/Texture.hpp"
#include "mge/core/Light.hpp"
#include "mge/core/World.hpp"
#include "mge/core/Mesh.hpp"
#include "mge/core/GameObject.hpp"
#include "mge/core/ShaderProgram.hpp"
#include "mge/config.hpp"

ShaderProgram* DiffuseTextureMaterial::_shader = NULL;

GLint DiffuseTextureMaterial::_uMVPMatrix = 0;
GLint DiffuseTextureMaterial::_uDiffuseTexture = 0;

GLint DiffuseTextureMaterial::_uMaterialShininess = 0;
GLint DiffuseTextureMaterial::_uMaterialSpecularColor = 0;

GLint DiffuseTextureMaterial::_uModelMatrix = 0;
GLint DiffuseTextureMaterial::_uCamPosition = 0;

GLint DiffuseTextureMaterial::_uNumLights = 0;

GLint DiffuseTextureMaterial::_aVertex = 0;
GLint DiffuseTextureMaterial::_aNormal = 0;
GLint DiffuseTextureMaterial::_aUV = 0;

GLint DiffuseTextureMaterial::_uDiffuseColor = 0;

DiffuseTextureMaterial::DiffuseTextureMaterial(Texture* pDiffuseTexture,
	float pMaterialShiness,
	glm::vec3 pMaterialSpecularColor) 
	: 	_diffuseTexture(pDiffuseTexture),
		materialShiness(pMaterialShiness),
		materialSpecularColor(pMaterialSpecularColor)
{
	_lazyInitializeShader();
}

DiffuseTextureMaterial::~DiffuseTextureMaterial() {}

void DiffuseTextureMaterial::_lazyInitializeShader() {
	if (!_shader) {
		_shader = new ShaderProgram();
		_shader->addShader(GL_VERTEX_SHADER, config::MGE_SHADER_PATH + "tom_dalling.vert");
		_shader->addShader(GL_FRAGMENT_SHADER, config::MGE_SHADER_PATH + "tom_dalling.frag");
		_shader->finalize();

		//cache all the uniform and attribute indexes
		_uMVPMatrix = _shader->getUniformLocation("mvpMatrix");
		_uDiffuseTexture = _shader->getUniformLocation("diffuseTexture");

		_uMaterialShininess = _shader->getUniformLocation("materialShininess");
		_uMaterialSpecularColor = _shader->getUniformLocation("materialSpecularColor");

		_uModelMatrix = _shader->getUniformLocation("modelMatrix");
		_uCamPosition = _shader->getUniformLocation("cameraPosition");

		_aVertex = _shader->getAttribLocation("vertex");
		_aNormal = _shader->getAttribLocation("normal");
		_aUV = _shader->getAttribLocation("uv");
	}
}

void DiffuseTextureMaterial::setDiffuseTexture(Texture* pDiffuseTexture) {
	_diffuseTexture = pDiffuseTexture;
}

void DiffuseTextureMaterial::CacheLightUniform(int lightIndex)
{
	Light::CacheLightUniform(_shader, lightIndex);
}

void DiffuseTextureMaterial::CacheNumLights(int totalLights) {
	_uNumLights = _shader->getUniformLocation("numLights");
}

void DiffuseTextureMaterial::render(World* pWorld, Mesh* pMesh, const glm::mat4& pModelMatrix, const glm::mat4& pViewMatrix, const glm::mat4& pProjectionMatrix) {
	if (!_diffuseTexture) return;

	_shader->use();

	//Print the number of lights in the scene and the position of the first light.
	//It is not used, but this demo is just meant to show you THAT materials can access the lights in a world
	int lightCount = pWorld->getLightCount();

	if (lightCount > 0) {
	    //std::cout << "DiffuseTextureMaterial has discovered light is at position:" << pWorld->getLightAt(0)->getLocalPosition() << std::endl;
		
		for (int i = 0; i < lightCount; i++) {
			pWorld->getLightAt(i)->SetLightUniform(i);
		}
	}

	glUniform1i(_uNumLights, lightCount);

	glUniform1f(_uMaterialShininess, materialShiness);

	glUniform3fv(_uMaterialSpecularColor, 1, glm::value_ptr(materialSpecularColor));


	glUniformMatrix4fv(_uModelMatrix, 1, GL_FALSE, glm::value_ptr(pModelMatrix));
	
	glUniform3fv(_uCamPosition, 1, glm::value_ptr(glm::column(glm::inverse(pViewMatrix), 3)));

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
