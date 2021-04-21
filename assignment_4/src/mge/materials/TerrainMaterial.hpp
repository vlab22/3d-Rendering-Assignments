#ifndef TERRAINMATERIAL_HPP
#define TERRAINMATERIAL_HPP

#include "mge/materials/AbstractMaterial.hpp"
#include "GL/glew.h"

class ShaderProgram;
class Texture;

/**
 * This material is already a little bit more complicated, instead of a color we can pass in a texture,
 * all attributes and uniforms are cached and we precompute the MVP matrix passing it in as one entity.
 */
class TerrainMaterial : public AbstractMaterial
{
public:
	TerrainMaterial(Texture* pTextureSplatMap, Texture* pHeightMapTexture, bool initShader = true);
	virtual ~TerrainMaterial();

	virtual void render(World* pWorld, Mesh* pMesh, const glm::mat4& pModelMatrix, const glm::mat4& pViewMatrix, const glm::mat4& pProjectionMatrix) override;

	void setSplatMapTexture(Texture* pSplatMapTexture);
	void setHeightMapTexture(Texture* pHeightMapTexture);

	void setDiffuseTextures(Texture* pDiffuseTexture1,
		Texture* pDiffuseTexture2,
		Texture* pDiffuseTexture3,
		Texture* pDiffuseTexture4);

	float heightFactor = 1;

protected:
	Texture* _heightMapTexture;
	Texture* _splatMapTexture;
	Texture* _diffuse1Texture;
	Texture* _diffuse2Texture;
	Texture* _diffuse3Texture;
	Texture* _diffuse4Texture;

	static ShaderProgram* _shader;

	//in this example we cache all identifiers for uniforms & attributes
	static GLint _uMVPMatrix;
	static GLint _uTextureSplatMap;
	static GLint _uHeightMapTexture;
	static GLint _uHeightFactor;

	static GLint _uTextureDiffuse1;
	static GLint _uTextureDiffuse2;
	static GLint _uTextureDiffuse3;
	static GLint _uTextureDiffuse4;

	static GLint _aVertex;
	static GLint _aNormal;
	static GLint _aUV;

	static void _initializeShaderIds();

private:
	static void _lazyInitializeShader();

	TerrainMaterial(const TerrainMaterial&);
	TerrainMaterial& operator=(const TerrainMaterial&);

};

#endif // TERRAINMATERIAL_HPP
