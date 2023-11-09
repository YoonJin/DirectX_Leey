#pragma once
#include "IComponent.h"

class MoveScriptComponent final : public IComponent
{
public:
	MoveScriptComponent
	(
		class Context* const context,
		class Actor* const actor,
		class TransformComponent* const transform
	);
	~MoveScriptComponent() = default;

	void Initialize() override;
	void Update()     override;
	void Destroy()    override;

	void SetMoveEventData();
	void SetMapData(int stage_map_data[][MAP_WIDTH]);

	float GetDistance(Vec2 curPos, Vec2 targetPos);
	Vec2  Normalize(Vec2 direction);

	PlayerDirection GetCurrentDirection() { return _curDir; }

	bool isMoving = false;
	float _speed  = 30.f;
	
	Vec3  _curPos;
	Coordinate _mapPos;

	PlayerDirection _curDir = PlayerDirection::PlayerLeft;

	int stage_map_data[MAP_HEIGHT][MAP_WIDTH];
};