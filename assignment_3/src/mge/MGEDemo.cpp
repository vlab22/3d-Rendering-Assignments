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
#include "mge/materials/DiffuseTextureMaterial.hpp"

#include "mge/behaviours/RotatingBehaviour.hpp"
#include "mge/behaviours/KeysBehaviour.hpp"
#include "mge/behaviours/MouseOrbitBehaviour.hpp"

#include "mge/util/DebugHud.hpp"

#include "mge/config.hpp"
#include "mge/MGEDemo.hpp"

using namespace glm;

//construct the game class into _window, _renderer and hud (other parts are initialized by build)
MGEDemo::MGEDemo() :AbstractGame(), _hud(0)
{
}

void MGEDemo::initialize() {
	//setup the core part
	AbstractGame::initialize();

	//setup the custom part so we can display some text
	std::cout << "Initializing HUD" << std::endl;
	_hud = new DebugHud(_window);
	std::cout << "HUD initialized." << std::endl << std::endl;
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
	Mesh* suzannaFlatMeshS = Mesh::load(config::MGE_MODEL_PATH + "suzanna_flat.obj");


	//MATERIALS

	//create some materials to display the cube, the plane and the light
	AbstractMaterial* lightMaterial = new ColorMaterial(vec3(1, 1, 0));
	AbstractMaterial* runicStoneLightedMaterial = new DiffuseTextureMaterial(Texture::load(config::MGE_TEXTURE_PATH + "runicfloor.png"));
	AbstractMaterial* landLightedMaterial = new DiffuseTextureMaterial(Texture::load(config::MGE_TEXTURE_PATH + "land.jpg"));
	AbstractMaterial* bricksMaterial = new TextureMaterial(Texture::load(config::MGE_TEXTURE_PATH + "bricks.jpg"));


	//SCENE SETUP

   //add camera first (it will be updated last)
	Camera* camera = new Camera("camera", vec3(0, 6, 7));
	camera->rotate(radians(-40.0f), vec3(1, 0, 0));
	_world->add(camera);
	_world->setMainCamera(camera);

	//add the floor
	GameObject* plane = new GameObject("plane", vec3(0, 0, 0));
	plane->scale(vec3(5, 5, 5));
	plane->setMesh(planeMeshDefault);
	plane->setMaterial(landLightedMaterial);
	_world->add(plane);

	//add the wall behind
	GameObject* wallB = new GameObject("plane", vec3(0, 0, -2.5));
	wallB->scale(vec3(5, 5, 5));
	wallB->rotate(radians(90.f), vec3(1, 0, 0));
	wallB->setMesh(planeMeshDefault);
	wallB->setMaterial(landLightedMaterial);
	_world->add(wallB);

	//add a spinning sphere
	GameObject* sphere = new GameObject("sphere", vec3(0, 0, 0));
	sphere->scale(vec3(2.5, 2.5, 2.5));
	sphere->setMesh(sphereMeshS);
	sphere->setMaterial(runicStoneLightedMaterial);
	sphere->setBehaviour(new RotatingBehaviour());
	_world->add(sphere);

	//add a light. Note that the light does ABSOLUTELY ZIP! NADA ! NOTHING !
	//It's here as a place holder to get you started.
	//Note how the texture material is able to detect the number of lights in the scene
	//even though it doesn't implement any lighting yet!

	Light* light = new Light("light", vec3(3, 2, 0));
	light->scale(vec3(0.1f, 0.1f, 0.1f));
	light->setMesh(cubeMeshF);
	light->setMaterial(bricksMaterial);
	light->setBehaviour(new KeysBehaviour(15));

	light->intensities = vec3(0, 1, 0);

	_world->add(light);

	_suzzane = new Light("light2", vec3(-3, 2, 0));
	_suzzane->scale(vec3(.1f, .1f, .1f));
	_suzzane->rotate(radians(180.f), vec3(0, 1, 0));
	_suzzane->setMesh(suzannaFlatMeshS);
	_suzzane->setMaterial(bricksMaterial);
	_suzzane->setBehaviour(new KeysBehaviour(15));

	_suzzane->lightType = Light::LightType::SPOT;

	_world->add(_suzzane);

	//Set Camera
	//Set Mouse Orbit Behaviour
	MouseOrbitBehaviour* mouseOrbit = new MouseOrbitBehaviour(_suzzane, 8, 150);
	this->addMouseWheelEventListener(mouseOrbit);
	camera->setBehaviour(mouseOrbit);

	int lightCount = _world->getLightCount();
	for (int i = 0; i < lightCount; i++) {
		((DiffuseTextureMaterial*)runicStoneLightedMaterial)->CacheLightUniform(i);
		((DiffuseTextureMaterial*)landLightedMaterial)->CacheLightUniform(i);
	}

	((DiffuseTextureMaterial*)runicStoneLightedMaterial)->CacheNumLights(lightCount);
	((DiffuseTextureMaterial*)landLightedMaterial)->CacheNumLights(lightCount);

	//Add key event
	this->addKeyEventListener(this);
}

void MGEDemo::_render() {
	AbstractGame::_render();
	_updateHud();
}

void MGEDemo::_updateHud() {
	std::string debugInfo = "";
	debugInfo += std::string("FPS:") + std::to_string((int)_fps) + "\n";
	debugInfo += "Arrows to move | Q, Z up and down \n";
	debugInfo += "D toggle suzzane light type(0=Point, 1=Directional, 2=Spot): "+ std::to_string(((int)_suzzane->lightType)) + " \n";
	debugInfo += "W,S add/sub suzzane spot cone: " + std::to_string(_suzzane->coneAngle) + " \n";
	debugInfo += "A, change all lights attenuation: "+ std::to_string(_world->getLightAt(0)->attenuation) +" \n";
	debugInfo += "Mouse Click Hold to orbit suzanne light \n";



	_hud->setDebugInfo(debugInfo);
	_hud->draw();
}

void MGEDemo::onKeyPressed(sf::Keyboard::Key keyCode)
{
	if (keyCode == sf::Keyboard::D) {
		int lType = ((int)_suzzane->lightType + 1) % 3;
		_suzzane->lightType = (Light::LightType) lType;
	}
	else if (keyCode == sf::Keyboard::W) {
		_suzzane->coneAngle = glm::clamp(_suzzane->coneAngle + 5, 10.f, 180.f);
	}
	else if (keyCode == sf::Keyboard::S) {
		_suzzane->coneAngle = glm::clamp(_suzzane->coneAngle - 5, 10.f, 180.f);

	}
	else if (keyCode == sf::Keyboard::A) {
		int lightCount = _world->getLightCount();
		for (int i = 0; i < lightCount; i++) {
			Light* light = _world->getLightAt(i);
			light->attenuation = fmodf(light->attenuation + 0.025f, 1.0f);
		}
	}
}

MGEDemo::~MGEDemo()
{
	//dtor
}
