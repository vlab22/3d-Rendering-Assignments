#pragma once
#include <SFML/Window/Keyboard.hpp>
class IKeyEventListener
{
public:
	virtual ~IKeyEventListener() {}
	virtual void onKeyPressed(sf::Keyboard::Key keyCode) = 0;
};

