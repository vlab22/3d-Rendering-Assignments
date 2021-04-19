#ifndef WOBBLETEXTUREMATERIAL_HPP
#define WOBBLETEXTUREMATERIAL_HPP

#include "mge/materials/AbstractMaterial.hpp"
#include "../listeners/IKeyEventListener.hpp"
#include <SFML/Window/Keyboard.hpp>
#include "GL/glew.h"

class ShaderProgram;
class Texture;

/**
 * This material is already a little bit more complicated, instead of a color we can pass in a texture,
 * all attributes and uniforms are cached and we precompute the MVP matrix passing it in as one entity.
 */
class WobbleTextureMaterial : public AbstractMaterial, public IKeyEventListener
{
    public:
        WobbleTextureMaterial(Texture* pDiffuseTexture);
        virtual ~WobbleTextureMaterial();

        virtual void render(World* pWorld, Mesh* pMesh, const glm::mat4& pModelMatrix, const glm::mat4& pViewMatrix, const glm::mat4& pProjectionMatrix) override;

        void onKeyPressed(sf::Keyboard::Key keyCode);

        void setDiffuseTexture (Texture* pDiffuseTexture);

        void setStrength(float strength);

    protected:
    private:
        static ShaderProgram* _shader;
        static void _lazyInitializeShader();

        //in this example we cache all identifiers for uniforms & attributes
        static GLint _uMVPMatrix;
        static GLint _uDiffuseTexture;

        static GLint _aVertex ;
        static GLint _aNormal;
        static GLint _aUV ;

        static GLfloat _uStrength;
        static GLfloat _uTime;

        Texture* _diffuseTexture;

        float _strength;

        WobbleTextureMaterial(const WobbleTextureMaterial&);
        WobbleTextureMaterial& operator=(const WobbleTextureMaterial&);
};

#endif // TEXTUREMATERIAL_HPP
