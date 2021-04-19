#include <iostream>
#include <string>

#include "glm.hpp"

#include "../mge/core/Renderer.hpp"

#include "../mge/core/Mesh.hpp"
#include "../mge/core/World.hpp"
#include "../mge/core/Texture.hpp"
#include "../mge/core/Light.hpp"
#include "../mge/core/Camera.hpp"
#include "../mge/core/GameObject.hpp"

#include "../mge/materials/AbstractMaterial.hpp"
#include "../mge/materials/ColorMaterial.hpp"
#include "../mge/materials/TextureMaterial.hpp"
#include "../mge/materials/WobbleTextureMaterial.hpp"

#include "../mge/behaviours/RotatingBehaviour.hpp"
#include "../mge/behaviours/KeysBehaviour.hpp"

#include "../mge/util/DebugHud.hpp"

#include "../mge/config.hpp"
#include "MGEDemo.hpp"
#include "../mge/behaviours/MouseOrbitBehaviour.hpp"


//construct the game class into _window, _renderer and hud (other parts are initialized by build)
MGEDemo::MGEDemo() :AbstractGame(), _hud(0)
{
}

void MGEDemo::initialize() {
	//setup the core part
	AbstractGame::initialize();

	//setup the custom part so we can display some text
	std::cout << "MyGame Initializing HUD" << std::endl;
	_hud = new DebugHud(_window);
	std::cout << "MyGame HUD initialized." << std::endl << std::endl;
}

//build the game _world
void MGEDemo::_initializeScene()
{
	//MESHES

	//load a bunch of meshes we will be using throughout this demo
	//each mesh only has to be loaded once, but can be used multiple times:
	//F is flat shaded, S is smooth shaded (normals aligned or not), check the models folder!
	Mesh* planeMeshDefault = Mesh::load(config::MGE_MODEL_PATH + "plane.obj");
	Mesh* cubeMeshF = Mesh::load(config::MGE_MODEL_PATH + "cube_flat.obj");
	Mesh* sphereMeshS = Mesh::load(config::MGE_MODEL_PATH + "sphere_smooth.obj");
	Mesh* skullMeshF = Mesh::load(config::MGE_MODEL_PATH + "skull-low.obj");


	//MATERIALS

	//create some materials to display the cube, the plane and the light
	AbstractMaterial* lightMaterial = new ColorMaterial(glm::vec3(1, 1, 0));
	AbstractMaterial* blueColorMaterial = new ColorMaterial(glm::vec3(0, 0, 1));
	AbstractMaterial* yellowColorMaterial = new ColorMaterial(glm::vec3(1, 0, 1));
	AbstractMaterial* grayColorMaterial = new ColorMaterial(glm::vec3(0.2, 0.2, 0.2));
	AbstractMaterial* runicStoneMaterial = new TextureMaterial(Texture::load(config::MGE_TEXTURE_PATH + "runicfloor.png"));
	AbstractMaterial* bricksMaterial = new TextureMaterial(Texture::load(config::MGE_TEXTURE_PATH + "bricks.jpg"));
	WobbleTextureMaterial* skullMaterial = new WobbleTextureMaterial(Texture::load(config::MGE_TEXTURE_PATH + "Skull-Low.png"));


	//SCENE SETUP

	//add camera first (it will be updated last)
	Camera* camera = new Camera("camera", glm::vec3(0, 6, 7));
	_world->add(camera);
	_world->setMainCamera(camera);

	//add the floor
	GameObject* plane = new GameObject("plane", glm::vec3(0, 0, 0));
	plane->scale(glm::vec3(5, 5, 5));
	plane->setMesh(planeMeshDefault);
	plane->setMaterial(runicStoneMaterial);
	_world->add(plane);

	//add a spinning sphere
	GameObject* sphere = new GameObject("sphere", glm::vec3(0, 0, 0));
	sphere->scale(glm::vec3(2.5, 2.5, 2.5));
	sphere->setMesh(sphereMeshS);
	sphere->setMaterial(runicStoneMaterial);
	sphere->setBehaviour(new RotatingBehaviour());
	//_world->add(sphere);

	//add player root
	GameObject* playerRoot = new GameObject("player_root", glm::vec3(0, 3, 0));
	playerRoot->setBehaviour(new KeysBehaviour(15));

	//add player Head
	GameObject* playerHead = new GameObject("player_skull", glm::vec3(0, 0, 0));
	playerHead->setMesh(skullMeshF);
	playerHead->setMaterial(skullMaterial);
	playerRoot->add(playerHead);

	//add player eye0
	GameObject* playerEye0 = new GameObject("player_cube_eye0", glm::vec3(0.35, -0.23, 0.72));
	playerEye0->scale(glm::vec3(0.2, 0.2, 0.2));
	playerEye0->setMesh(sphereMeshS);
	playerEye0->setMaterial(runicStoneMaterial);
	playerEye0->setBehaviour(new RotatingBehaviour());
	playerHead->add(playerEye0);

	//add player eye1
	GameObject* playerEye1 = new GameObject("player_cube_eye1", glm::vec3(-0.35, -0.23, 0.72));
	playerEye1->scale(glm::vec3(0.2, 0.2, 0.2));
	playerEye1->setMesh(sphereMeshS);
	playerEye1->setMaterial(runicStoneMaterial);
	playerEye1->setBehaviour(new RotatingBehaviour());
	playerHead->add(playerEye1);

	//add player mounth
	GameObject* playerMounth = new GameObject("player_cube_mounth", glm::vec3(0, -1.3, 0));
	playerMounth->scale(glm::vec3(0.2, 0.6, 0.2));
	playerMounth->setMesh(cubeMeshF);
	playerMounth->setMaterial(bricksMaterial);
	playerHead->add(playerMounth);

	//add player hair
	GameObject* playerHair = new GameObject("player_cube_hair", glm::vec3(0, 1.2, 0));
	playerHair->scale(glm::vec3(0.4, 0.2, 1));
	playerHair->setMesh(cubeMeshF);
	playerHair->setMaterial(grayColorMaterial);
	playerHead->add(playerHair);

	_world->add(playerRoot);

	//Origin debug
	GameObject* originDebug = new GameObject("origin_debug", glm::vec3(0));
	originDebug->scale(glm::vec3(0.2, 0.2, 1));
	originDebug->setMesh(cubeMeshF);
	originDebug->setMaterial(blueColorMaterial);
	_world->add(originDebug);


	//Set Mouse Orbit Behaviour
	MouseOrbitBehaviour* mouseOrbit = new MouseOrbitBehaviour(playerRoot, 8, 150);
	this->addMouseWheelEventListener(mouseOrbit);
	camera->setBehaviour(mouseOrbit);


	this->addKeyEventListener(skullMaterial);


	//add a light. Note that the light does ABSOLUTELY ZIP! NADA ! NOTHING !
	//It's here as a place holder to get you started.
	//Note how the texture material is able to detect the number of lights in the scene
	//even though it doesn't implement any lighting yet!

	Light* light = new Light("light", glm::vec3(0, 4, 0));
	light->scale(glm::vec3(0.1f, 0.1f, 1.f));
	light->setMesh(cubeMeshF);
	light->setMaterial(lightMaterial);
	light->setBehaviour(new KeysBehaviour(25));
	playerRoot->add(light);

}

void MGEDemo::_render() {
	AbstractGame::_render();
	_updateHud();
}

void MGEDemo::_updateHud() {
	std::string debugInfo = "";
	debugInfo += std::string("FPS:") + std::to_string((int)_fps) + "\n";

	_hud->setDebugInfo(debugInfo);
	_hud->draw();
}


MGEDemo::~MGEDemo()
{
	//dtor
}

