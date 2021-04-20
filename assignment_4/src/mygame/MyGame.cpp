#include <iostream>
#include <string>

#include "glm.hpp"

#include "mge/core/Renderer.hpp"

#include "mge/core/Mesh.hpp"
#include "mge/core/World.hpp"
#include "mge/core/Texture.hpp"
#include "mge/core/Light.hpp"
#include "mge/core/Camera.hpp"
#include "mge/core/GameObject.hpp"

#include "mge/materials/AbstractMaterial.hpp"
#include "mge/materials/ColorMaterial.hpp"
#include "mge/materials/TextureMaterial.hpp"
#include "mge/materials/TerrainMaterial.hpp"

#include "mge/behaviours/RotatingBehaviour.hpp"
#include "mge/behaviours/KeysBehaviour.hpp"

#include "mge/util/DebugHud.hpp"

#include "mge/config.hpp"
#include "mygame/MyGame.hpp"
#include "mge/behaviours/MouseOrbitBehaviour.hpp"

//construct the game class into _window, _renderer and hud (other parts are initialized by build)
MyGame::MyGame() :AbstractGame(), _hud(0)
{
}

void MyGame::initialize() {
	//setup the core part
	AbstractGame::initialize();

	//setup the custom part so we can display some text
	std::cout << "Initializing HUD" << std::endl;
	_hud = new DebugHud(_window);
	std::cout << "HUD initialized." << std::endl << std::endl;
}

//build the game _world
void MyGame::_initializeScene()
{
	//MESHES

	//load a bunch of meshes we will be using throughout this demo
	//each mesh only has to be loaded once, but can be used multiple times:
	//F is flat shaded, S is smooth shaded (normals aligned or not), check the models folder!
	Mesh* planeMeshDefault = Mesh::load(config::MGE_MODEL_PATH + "plane.obj");
	Mesh* cubeMeshF = Mesh::load(config::MGE_MODEL_PATH + "cube_flat.obj");
	Mesh* sphereMeshS = Mesh::load(config::MGE_MODEL_PATH + "sphere_smooth.obj");

	Mesh* terrainMeshS = Mesh::load(config::MGE_MODEL_PATH + "plane_8192.obj");

	//MATERIALS

	//create some materials to display the cube, the plane and the light
	AbstractMaterial* lightMaterial = new ColorMaterial(glm::vec3(1, 1, 0));
	AbstractMaterial* runicStoneMaterial = new TextureMaterial(Texture::load(config::MGE_TEXTURE_PATH + "runicfloor.png"));
	AbstractMaterial* checkerTextureMaterial = new TextureMaterial(Texture::load(config::MGE_TEXTURE_PATH + "checker.png"));

	Texture* heightMapTexture = Texture::load(config::MGE_TEXTURE_PATH + "terrain/heightmap.png");
	Texture* terrain0Texture = Texture::load(config::MGE_TEXTURE_PATH + "terrain/diffuse1.jpg");

	terrainMaterial = new TerrainMaterial(terrain0Texture, heightMapTexture);

	//SCENE SETUP

   //add camera first (it will be updated last)
	Camera* camera = new Camera("camera", glm::vec3(0, 6, 7));
	camera->rotate(glm::radians(-40.0f), glm::vec3(1, 0, 0));
	_world->add(camera);
	_world->setMainCamera(camera);

	GameObject* terrain2 = new GameObject("terrain2", glm::vec3(0, 0, 0));
	terrain2->setMesh(terrainMeshS);
	terrain2->setMaterial(checkerTextureMaterial);
	//_world->add(terrain2);

	GameObject* terrain = new GameObject("terrain", glm::vec3(0, 0, 0));
	terrain->scale(glm::vec3(4,4,4));
	terrain->setMesh(terrainMeshS);
	terrain->setMaterial(terrainMaterial);
	_world->add(terrain);

	//add the floor
	//GameObject* plane = new GameObject("plane", glm::vec3(0, 0, 0));
	//plane->scale(glm::vec3(5, 5, 5));
	//plane->setMesh(planeMeshDefault);
	//plane->setMaterial(terrainMaterial);
	//_world->add(plane);

	//Set Mouse Orbit Behaviour
	MouseOrbitBehaviour* mouseOrbit = new MouseOrbitBehaviour(terrain, 8, 150);
	mouseOrbit->setAngle(glm::vec2(0, 45));
	this->addMouseWheelEventListener(mouseOrbit);
	camera->setBehaviour(mouseOrbit);

	this->addKeyEventListener(this);

	this->_renderer->setClearColor(0,127,240);
}

void MyGame::_render() {
	AbstractGame::_render();
	_updateHud();
}

void MyGame::_updateHud() {
	std::string debugInfo = "";
	debugInfo += std::string("FPS:") + std::to_string((int)_fps) + "\n";

	_hud->setDebugInfo(debugInfo);
	_hud->draw();
}

void MyGame::onKeyPressed(sf::Keyboard::Key keyCode)
{
	if (keyCode == sf::Keyboard::W) {
		terrainMaterial->heightFactor += 0.05;
	}
	else if (keyCode == sf::Keyboard::S) {
		terrainMaterial->heightFactor -= 0.05;
	}
}

MyGame::~MyGame()
{
	//dtor
}
