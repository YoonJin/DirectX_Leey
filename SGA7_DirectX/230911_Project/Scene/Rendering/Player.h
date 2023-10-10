#pragma once
#include "MyRect.h"

class Player : public MyRect
{
public:
	Player(class Graphics* graphics, const Color& color);
	~Player();

	void Event() override;
protected:
	void Move() override;
};