#pragma once
#include "MyRect.h"

class Monster final : public MyRect
{
public:
	Monster(class Graphics* graphics, const Color& color);
	~Monster();

	void Event() override;
protected:
	void Move() override;

private:
	//Stopwatch stopwatch;
	Direction direction = Direction::Left;
};