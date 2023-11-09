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


	float distance = GetDistance(Vec2(_curPos.x, _curPos.y), destination);
	
	// �̵� ���� ���ϱ�
	Vec2 direction = destination - _curPos;
	direction = this->Normalize(direction);

	// �Ÿ� ���ϱ�
	if (distance > 1.0f)
	{
		isMoving = true; // �̵� ����

		float time = context->GetSubsystem<Timer>()->GetDeltaTimeSEC();
		Vec2 moveValue = (time * direction * _speed);

		if (moveValue.Length() > distance)
		{
			moveValue = Normalize(moveValue);
			moveValue *= distance;
		}

		transform->SetPosition(_curPos + moveValue);
	}
	else
		isMoving = false;

}

void MoveScriptComponent::Destroy()
{
}

void MoveScriptComponent::SetMoveEventData()
{
	// ���� �浹�ϴ� ��ġ��� �ǵ����� ����.
	Coordinate originPos      = this->_mapPos;
	PlayerDirection originDir = _curDir;

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

	// �̵��� ��ġ�� ���̶�� �̵���ǥ�� ������Ʈ ���� �ʴ´�.
	if (this->stage_map_data[this->_mapPos.y][this->_mapPos.x] == UINT_CONVERT_TO(MapObject::WALL))
	{
		this->_mapPos = originPos;
		this->_curDir = originDir;
	}
}

void MoveScriptComponent::SetMapData(int stage_map_data[][MAP_WIDTH])
{
	for (int i = 0; i < MAP_HEIGHT; i++)
	{
		for (int j = 0; j < MAP_WIDTH; j++)
		{
			this->stage_map_data[i][j] = stage_map_data[i][j];
		}
	}
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
