#ifndef MOUSEORBITBEHAVIOUR_HPP
#define MOUSEORBITBEHAVIOUR_HPP

#include "AbstractBehaviour.hpp"
#include "../listeners/IMouseWheelEventListener.hpp"
#include "glm.hpp"

class MouseOrbitBehaviour : public AbstractBehaviour, public IMouseWheelEventListener
{
public:
	float distance;
	float minVerticalAngle;
	float maxVerticalAngle;
	float minZoom;
	float maxZoom;
	float zoomSpeed = 2;

	float easeTime = 0;
	float easeDuration = 1;
	float easeFrom;
	float easeTo;
	int easeDirection;
	bool easeRunning = false;

	MouseOrbitBehaviour(GameObject* pTarget, float distance = 4, float pSensitivity = 0.1, float pMinVerticalAnglefloat = -45, float pMaxVerticalAngle = 45, float pMinZoom = 1, float pMaxZoom = 100);
	virtual ~MouseOrbitBehaviour();
	virtual void update(float pStep);

	void setAngle(glm::vec2 pAngle);

	virtual void onMouseWheelMoved(int delta);


private:
	GameObject* _target;
	glm::vec2 _delta;
	glm::vec2 _lastMousePos;
	glm::vec2 _mousePos;
	float _sensitivity;
	glm::vec2 _angle;

	virtual void lookAt(GameObject* pTarget);
	void easeInZoom(float pStep);
	void easeOutZoom(float pStep);
	virtual void orbit(GameObject* pTarget, glm::vec2 pAngle, float pDistance);
};

#endif // MOUSEORBITBEHAVIOUR_HPP
