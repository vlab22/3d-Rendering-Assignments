#ifndef DIFFUSETEXTUREMATERIAL_HPP
#define DIFFUSETEXTUREMATERIAL_HPP

#include "mge/materials/AbstractMaterial.hpp"
#include "GL/glew.h"

class ShaderProgram;
class Texture;

/**
 * This material is already a little bit more complicated, instead of a color we can pass in a texture,
 * all attributes and uniforms are cached and we precompute the MVP matrix passing it in as one entity.
 */
class DiffuseTextureMaterial : public AbstractMaterial
{
    public:
        DiffuseTextureMaterial(Texture* pDiffuseTexture,
            float pMaterialShiness = 30.f,
            glm::vec3 pMaterialSpecularColor = glm::vec3(1.f, 1.f, 1.f)
        );
        virtual ~DiffuseTextureMaterial ();

        virtual void render(World* pWorld, Mesh* pMesh, const glm::mat4& pModelMatrix, const glm::mat4& pViewMatrix, const glm::mat4& pProjectionMatrix) override;

        void setDiffuseTexture (Texture* pDiffuseTexture);

        void CacheLightUniform(int lightIndex);

        void CacheNumLights(int totalLights);

        float materialShiness;
        glm::vec3 materialSpecularColor;

    protected:
    private:
        static ShaderProgram* _shader;
        static void _lazyInitializeShader();

        //in this example we cache all identifiers for uniforms & attributes
        static GLint _uMVPMatrix;
        static GLint _uDiffuseTexture; 
        static GLint _uDiffuseColor;

        static GLint _uMaterialShininess;
        static GLint _uMaterialSpecularColor;

        //For lighting
        static GLint _uModelMatrix;
        static GLint _uCamPosition;
        static GLint _uNumLights;

        static GLint _aVertex ;
        static GLint _aNormal;
        static GLint _aUV ;

        Texture* _diffuseTexture;

        DiffuseTextureMaterial(const DiffuseTextureMaterial&);
        DiffuseTextureMaterial& operator=(const DiffuseTextureMaterial&);

};

#endif // DIFFUSETEXTUREMATERIAL_HPP
