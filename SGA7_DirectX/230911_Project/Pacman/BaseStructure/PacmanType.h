#pragma once

#define MAP_WIDTH  28
#define MAP_HEIGHT 31

#define CELL_WIDTH 17
#define CELL_HEIGHT 17

#define WINSIZEX 479
#define WINSIZEY 527

enum class MapObject
{
	EMPTY,
	PLAYER,
	ENEMY,
	CANDY,
	WALL
};

class Coordinate
{
public:
	int x;
	int y;

	Coordinate()
	{

	}

	Coordinate(int x, int y)
	{
		this->x = x;
		this->y = y;
	}

	bool Equare(Coordinate obj)
	{
		if ((this->x == obj.x) && (this->y == obj.y))
			return true;
		else
			return false;
	}

	Coordinate operator+= (Coordinate obj)
	{
		this->x = this->x + obj.x;
		this->y = this->y + obj.y;

		return *this;
	}
};