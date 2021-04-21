#ifndef DIFFUSETERRAINMATERIAL_HPP
#define DIFFUSETERRAINMATERIAL_HPP

#include "mge/materials/TerrainMaterial.hpp"
#include "GL/glew.h"

class ShaderProgram;
class Texture;

/**
 * This material is already a little bit more complicated, instead of a color we can pass in a texture,
 * all attributes and uniforms are cached and we precompute the MVP matrix passing it in as one entity.
 */
class DiffuseTerrainMaterial : public TerrainMaterial
{
public:
	DiffuseTerrainMaterial(Texture* pDiffuseTexture, Texture* pHeightMapTexture,
		float pMaterialShiness = 30.f,
		glm::vec3 pMaterialSpecularColor = glm::vec3(1.f, 1.f, 1.f)
	);
	virtual ~DiffuseTerrainMaterial();

	virtual void render(World* pWorld, Mesh* pMesh, const glm::mat4& pModelMatrix, const glm::mat4& pViewMatrix, const glm::mat4& pProjectionMatrix) override;

	void setSplatMapTexture(Texture* pDiffuseTexture);

	void CacheLightUniform(int lightIndex);

	void CacheNumLights(int totalLights);

	float materialShiness;
	glm::vec3 materialSpecularColor;

protected:

	//in this example we cache all identifiers for uniforms & attributes
	static GLint _uDiffuseColor;

	static GLint _uMaterialShininess;
	static GLint _uMaterialSpecularColor;

	//For lighting
	static GLint _uModelMatrix;
	static GLint _uCamPosition;
	static GLint _uNumLights;


private:
	static void _lazyInitializeShader();

	DiffuseTerrainMaterial(const DiffuseTerrainMaterial&);
	DiffuseTerrainMaterial& operator=(const DiffuseTerrainMaterial&);

};

#endif // DIFFUSETERRAINMATERIAL_HPP
