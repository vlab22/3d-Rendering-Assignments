#include <iostream>
#include <string>
#include <random>       // std::default_random_engine
#include <chrono>       // std::chrono::system_clock

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

using namespace glm;

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
	Mesh* suzannaFlatMeshS = Mesh::load(config::MGE_MODEL_PATH + "suzanna_flat.obj");
	Mesh* terrainMeshS = Mesh::load(config::MGE_MODEL_PATH + "plane_8192.obj");

	//MATERIALS

	//create some materials to display the cube, the plane and the light
	AbstractMaterial* lightMaterial = new ColorMaterial(glm::vec3(1, 1, 0));
	AbstractMaterial* runicStoneMaterial = new TextureMaterial(Texture::load(config::MGE_TEXTURE_PATH + "runicfloor.png"));
	AbstractMaterial* checkerTextureMaterial = new TextureMaterial(Texture::load(config::MGE_TEXTURE_PATH + "checker.png"));

	Texture* heightMapTexture = Texture::load(config::MGE_TEXTURE_PATH + "terrain/heightmap.png");
	Texture* splatMapTexture = Texture::load(config::MGE_TEXTURE_PATH + "terrain/splatmap.png");

	Texture* textureDiffuse1 = Texture::load(config::MGE_TEXTURE_PATH + "terrain/diffuse1.jpg");
	Texture* textureDiffuse2 = Texture::load(config::MGE_TEXTURE_PATH + "terrain/diffuse2.jpg");
	Texture* textureDiffuse3 = Texture::load(config::MGE_TEXTURE_PATH + "terrain/diffuse3.jpg");
	Texture* textureDiffuse4 = Texture::load(config::MGE_TEXTURE_PATH + "terrain/diffuse4.jpg");

	diffuseTextures.push_back(textureDiffuse1);
	diffuseTextures.push_back(textureDiffuse2);
	diffuseTextures.push_back(textureDiffuse3);
	diffuseTextures.push_back(textureDiffuse4);

	terrainMaterial = new DiffuseTerrainMaterial(splatMapTexture, heightMapTexture);
	terrainMaterial->setDiffuseTextures(textureDiffuse1, textureDiffuse2, textureDiffuse3, textureDiffuse4);

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
	terrain->scale(glm::vec3(4, 4, 4));
	terrain->setMesh(terrainMeshS);
	terrain->setMaterial(terrainMaterial);
	_world->add(terrain);

	_suzzane = new Light("suzzane_light", vec3(-3, 2, 0));
	_suzzane->scale(vec3(.1f, .1f, .1f));
	_suzzane->rotate(radians(180.f), vec3(0, 1, 0));
	_suzzane->setMesh(suzannaFlatMeshS);
	_suzzane->setMaterial(runicStoneMaterial);
	_suzzane->setBehaviour(new KeysBehaviour(15));

	_world->add(_suzzane);

	//Set Mouse Orbit Behaviour
	MouseOrbitBehaviour* mouseOrbit = new MouseOrbitBehaviour(terrain, 8, 150);
	mouseOrbit->setAngle(glm::vec2(0, 45));
	this->addMouseWheelEventListener(mouseOrbit);
	camera->setBehaviour(mouseOrbit);

	//Set Lights
	int lightCount = _world->getLightCount();
	for (int i = 0; i < lightCount; i++) {
		((DiffuseTerrainMaterial*)terrainMaterial)->CacheLightUniform(i);
	}

	((DiffuseTerrainMaterial*)terrainMaterial)->CacheNumLights(lightCount);

	this->addKeyEventListener(this);

	this->_renderer->setClearColor(0, 127, 240);
}

void MyGame::_render() {
	AbstractGame::_render();
	_updateHud();
}

void MyGame::_updateHud() {
	std::string debugInfo = "";
	debugInfo += std::string("FPS:") + std::to_string((int)_fps) + "\n";
	debugInfo += "Arrow, Q, Z: move suzzane\n";
	debugInfo += "W, S: remove/add heightFactor\n";
	debugInfo += "[, ]: remove/add shiness\n";
	debugInfo += ";, \": remove/add attenuation\n";
	debugInfo += "T: random diffuse textures\n";

	_hud->setDebugInfo(debugInfo);
	_hud->draw();
}

void MyGame::onKeyPressed(sf::Keyboard::Key keyCode)
{
	if (keyCode == sf::Keyboard::W) {
		terrainMaterial->heightFactor += 0.05;
		std::cout << "heightFactor: " << terrainMaterial->heightFactor << std::endl;
	}
	else if (keyCode == sf::Keyboard::S) {
		terrainMaterial->heightFactor -= 0.05;
		std::cout << "heightFactor: " << terrainMaterial->heightFactor << std::endl;
	}

	if (keyCode == sf::Keyboard::RBracket) {
		terrainMaterial->materialShiness += 10;
		std::cout << "shiness: " << terrainMaterial->materialShiness << std::endl;
	}
	else if (keyCode == sf::Keyboard::LBracket) {
		terrainMaterial->materialShiness -= 10;
		std::cout << "shiness: " << terrainMaterial->materialShiness << std::endl;
	}

	if (keyCode == sf::Keyboard::Quote) {
		_suzzane->attenuation += 0.01f;
		std::cout << "attenuation: " << _suzzane->attenuation << std::endl;
	}
	else if (keyCode == sf::Keyboard::SemiColon) {
		_suzzane->attenuation -= 0.01f;
		std::cout << "attenuation: " << _suzzane->attenuation << std::endl;
	}

	if (keyCode == sf::Keyboard::T) {
		// obtain a time-based seed:
		unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();

		std::shuffle(diffuseTextures.begin(), diffuseTextures.end(), std::default_random_engine(seed));
		terrainMaterial->setDiffuseTextures(diffuseTextures[0], diffuseTextures[1], diffuseTextures[2], diffuseTextures[3]);
	}
}

MyGame::~MyGame()
{
	//dtor
}
