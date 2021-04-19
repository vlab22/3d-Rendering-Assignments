#pragma once
class IMouseWheelEventListener
{
public:
	virtual ~IMouseWheelEventListener() {}
	virtual void onMouseWheelMoved(int delta) = 0;
};

