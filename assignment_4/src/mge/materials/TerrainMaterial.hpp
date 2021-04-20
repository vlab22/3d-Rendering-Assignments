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
        TerrainMaterial(Texture* pDiffuseTexture, Texture* pHeightMapTexture);
        virtual ~TerrainMaterial();

        virtual void render(World* pWorld, Mesh* pMesh, const glm::mat4& pModelMatrix, const glm::mat4& pViewMatrix, const glm::mat4& pProjectionMatrix) override;

        void setDiffuseTexture (Texture* pDiffuseTexture);
        void setHeightMapTexture(Texture* pHeightMapTexture);

        float heightFactor = 1;

    protected:
    private:
        static ShaderProgram* _shader;
        static void _lazyInitializeShader();

        //in this example we cache all identifiers for uniforms & attributes
        static GLint _uMVPMatrix;
        static GLint _uDiffuseTexture;
        static GLint _uHeightMapTexture;
        static GLint _uHeightFactor;

        static GLint _aVertex ;
        static GLint _aNormal;
        static GLint _aUV ;

        Texture* _diffuseTexture;
        Texture* _heightMapTexture;

        TerrainMaterial(const TerrainMaterial&);
        TerrainMaterial& operator=(const TerrainMaterial&);

};

#endif // TERRAINMATERIAL_HPP
