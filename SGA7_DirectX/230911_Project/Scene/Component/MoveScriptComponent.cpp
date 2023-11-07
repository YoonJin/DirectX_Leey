#include "pch.h"
#include "MoveScriptComponent.h"
#include "TransformComponent.h"

MoveScriptComponent::MoveScriptComponent(Context* const context, Actor* const actor, TransformComponent* const transform)
	:IComponent(context, actor, transform)
{
}

void MoveScriptComponent::Initialize()
{
}

void MoveScriptComponent::Update()
{
	if (isMoving == false)
		SetMoveEventData();

	_curPos = transform->GetPosition();

	float PosX = (this->_mapPos.x * CELL_WIDTH) -
		(Settings::Get().GetWidth() / 2) + (CELL_WIDTH / 2);
	float PosY = ((30 - this->_mapPos.y) * CELL_HEIGHT) -
		(Settings::Get().GetHeight() / 2) + (CELL_HEIGHT / 2);
	Vec2 destination = Vec2(PosX, PosY);

	// 거리 구하기
	if (this->GetDistance(Vec2(_curPos.x, _curPos.y), destination) > 1.0f)
	{
		isMoving = true; // 이동 시작

		// 이동 방향 구하기
		Vec2 direction = destination - _curPos;
		direction = this->Normalize(direction);

		transform->SetPosition(_curPos + (context->GetSubsystem<Timer>()->GetDeltaTimeSEC() * direction * this->_speed));
	}
	else
		isMoving = false;

}

void MoveScriptComponent::Destroy()
{
}

void MoveScriptComponent::SetMoveEventData()
{
	if (GetAsyncKeyState('W') & 0x8000)
	{
		this->_mapPos += Coordinate(0, -1);
		_curDir = PlayerDirection::PlayerUp;
	}
	else if (GetAsyncKeyState('S') & 0x8000)
	{
		this->_mapPos += Coordinate(0, +1);
		_curDir = PlayerDirection::PlayerDown;
	}
	else if (GetAsyncKeyState('D') & 0x8000)
	{
		this->_mapPos += Coordinate(+1, 0);
		_curDir = PlayerDirection::PlayerRight;
	}
	else if (GetAsyncKeyState('A') & 0x8000)
	{
		this->_mapPos += Coordinate(-1, 0);
		_curDir = PlayerDirection::PlayerLeft;
	}
}

void MoveScriptComponent::SetMapData(int stage_map_data[][MAP_WIDTH])
{
}

float MoveScriptComponent::GetDistance(Vec2 curPos, Vec2 targetPos)
{
	float dx = targetPos.x - curPos.x;
	float dy = targetPos.y - curPos.y;

	return sqrt(dx * dx + dy * dy);
}

Vec2 MoveScriptComponent::Normalize(Vec2 direction)
{
	float length = sqrt((direction.x * direction.x) + (direction.y * direction.y));

	if (length != 0)
	{
		direction.x /= length;
		direction.y /= length;
	}
	return direction;
}
