#include "MouseOrbitBehaviour.hpp"
#include <SFML/Window/Mouse.hpp>
#include "glm.hpp"
#include <iostream>
#include "SFML/System/Clock.hpp"
#include "PennerEasing/Quad.h"
#include "mge/core/GameObject.hpp"

using std::cout;
using std::endl;

MouseOrbitBehaviour::MouseOrbitBehaviour(GameObject* pTarget, float pDistance, float pSensitivity, float pMinVerticalAnglefloat, float pMaxVerticalAngle, float pMinZoom, float pMaxZoom)
	:
	_target(pTarget),
	distance(pDistance),
	_sensitivity(pSensitivity),
	minVerticalAngle(pMinVerticalAnglefloat),
	maxVerticalAngle(pMaxVerticalAngle),
	minZoom(pMinZoom),
	maxZoom(pMaxZoom)
{

}

MouseOrbitBehaviour::~MouseOrbitBehaviour()
{
}


void MouseOrbitBehaviour::update(float pStep)
{
	_lastMousePos = _mousePos;
	_mousePos = glm::vec2(sf::Mouse::getPosition().x, sf::Mouse::getPosition().y);
	_delta = _mousePos - _lastMousePos;
	float deltaLen2 = glm::length2(_delta);

	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		if (deltaLen2 > 0) {

			_angle += glm::normalize(_delta) * _sensitivity * pStep;
			_angle.y = glm::clamp(_angle.y, minVerticalAngle, maxVerticalAngle);
		}
	}

	orbit(_target, _angle, distance);

	lookAt(_target);

	if (easeRunning) {

		if (easeDirection > 0) {
			easeOutZoom(pStep);
		}
		else {
			easeInZoom(pStep);
		}

		if (easeTime > easeDuration || easeTime < 0) {
			easeRunning = false;
		}
	}
}

void MouseOrbitBehaviour::onMouseWheelMoved(int delta)
{
	cout << "onMouseWheelMoved: " << delta << endl;

	easeFrom = distance;
	easeTo = glm::clamp(distance + delta * zoomSpeed, minZoom, maxZoom);

	if (easeFrom > easeTo) {
		float temp = easeFrom;
		easeFrom = easeTo;
		easeTo = temp;
	}

	easeDirection = delta;
	easeTime = (easeDirection > 0) ? 0 : easeDuration;
	easeRunning = true;
}

void MouseOrbitBehaviour::orbit(GameObject* pTarget, glm::vec2 pAngle, float pDistance) {
	float x = glm::cos(glm::radians(pAngle.x));
	float z = glm::sin(glm::radians(pAngle.x));

	float y = glm::sin(glm::radians(pAngle.y));

	glm::vec3 pos = glm::normalize(glm::vec3(x, y, z)) * pDistance;

	_owner->setLocalPosition(pTarget->getWorldPosition() + pos);
}

void MouseOrbitBehaviour::lookAt(GameObject* pTarget)
{
	glm::vec3 targetPos = pTarget->getWorldPosition();
	glm::vec3 camPos = _owner->getWorldPosition();
	glm::mat4 lookMat = glm::lookAt(camPos, targetPos, glm::vec3(0, 1, 0));

	_owner->setTransform(glm::inverse(lookMat));
}

void MouseOrbitBehaviour::easeInZoom(float pStep) {
	float ease = Quad::easeIn(easeTime, 0, 1, easeDuration);
	distance = easeFrom + ease * (easeTo - easeFrom);
	easeTime += easeDirection * pStep;
}

void MouseOrbitBehaviour::easeOutZoom(float pStep) {
	float ease = Quad::easeOut(easeTime, 0, 1, easeDuration);
	distance = easeFrom + ease * (easeTo - easeFrom);
	easeTime += easeDirection * pStep;
}