#include "pch.h"
#include "AIScriptComponent.h"
#include "TransformComponent.h"

AIScriptComponent::AIScriptComponent(Context* const context, Actor* const actor, TransformComponent* const transform)
	: IComponent(context, actor, transform)
{
	this->transform = transform;
}

void AIScriptComponent::Initialize()
{
}

void AIScriptComponent::Update()
{
	if (isInitialize)
	{
		// 목표 좌표 설정
		this->SetDestPos();

		// 이동 중에 A*로 경로가 바뀌어 버린다면 문제가 생길 수 있으므로
		// 무조건 이동이 끝난 후 A*로 다시 계산을 한다.
		if (!isSearchMove)
		{
			// Astar 초기화
			this->InitAstar();
		}

		// 계산한 위치에 따라 움직임.
		if (_points.size() != 0)
		{
			// 지속적인 유연한 움직임을 위함.
			if (!isSearchMove)
			{
				_pos.x = _points.front().x;
				_pos.y = _points.front().y;
				_points.pop_front();

				// TODO : 여러분들이 직접!!
				// 1. 목적지 실제 좌표 계산
				// 2. 이동 방향 계산
				// 3. 방향 Normalize
			}
		}

		// 4. 거리 계산
		// 아래에서 이동
		/*if (distance > 0.5f)
		{
			isSearchMove = true;

			transform->SetPosition("블라블라");
		}
		else
		 isSearchMove = false;
		*/

		if (this->confirmPos.Equare(this->_pos))
			this->isDefaultMove = false;
	}
}

void AIScriptComponent::Destroy()
{
}

float AIScriptComponent::GetDistance(Vec2 curPos, Vec2 targetPos)
{
	return 0.0f;
}

Vec2 AIScriptComponent::Normalize(Vec2 direction)
{
	return Vec2();
}

void AIScriptComponent::SetDestPos()
{
	switch (_state)
	{
	case EnemyState::MOVE:
		if (!isDefaultMove)
		{
			isDefaultMove = true;
			int width  = MAP_WIDTH  - 1;
			int height = MAP_HEIGHT - 1;
			confirmPos = Coordinate(MyMath::Random(0, width), MyMath::Random(0, height));

			while (1)
			{
				if (this->stage_map_data[confirmPos.y][confirmPos.x] ==
					UINT_CONVERT_TO(MapObject::WALL))
				{
					confirmPos = Coordinate(MyMath::Random(0, width), MyMath::Random(0, height));
				}
				else
					break;
			}
		}
		break;
	case EnemyState::SEARCH:
		// TODO : 추후
		break;
	}
}

void AIScriptComponent::InitAstar()
{
}

void AIScriptComponent::Astar()
{
}

void AIScriptComponent::CalcPathFromParent(Coordinate dest)
{
}
