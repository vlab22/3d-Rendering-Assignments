#ifndef MGEDEMO_HPP
#define MGEDEMO_HPP

#include "mge/core/AbstractGame.hpp"
#include "mge/core/Light.hpp"
#include "mge/materials/TerrainMaterial.hpp"
#include "mge/materials/DiffuseTerrainMaterial.hpp"

class DebugHud;

/**
 * An example subclass of AbstractGame showing how we can setup a scene.
 */
class MyGame: public AbstractGame, public IKeyEventListener
{
    //PUBLIC FUNCTIONS

	public:
		MyGame();
		virtual ~MyGame();

        //override initialize so we can add a DebugHud
        virtual void initialize();

	protected:
	    //override so we can construct the actual scene
        virtual void _initializeScene();

	    //override render to render the hud as well.
	    virtual void _render();

	private:
		DebugHud* _hud;                   //hud display

        void _updateHud();

        MyGame(const MyGame&);
        MyGame& operator=(const MyGame&);

		DiffuseTerrainMaterial* terrainMaterial;

		std::vector<Texture*> diffuseTextures;

		Light* _suzzane;

		// Inherited via IKeyEventListener
		virtual void onKeyPressed(sf::Keyboard::Key keyCode) override;
};

#endif // MGEDEMO_HPP
