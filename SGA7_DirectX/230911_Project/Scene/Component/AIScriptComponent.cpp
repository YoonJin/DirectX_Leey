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
		// ��ǥ ��ǥ ����
		this->SetDestPos();

		// �̵� �߿� A*�� ��ΰ� �ٲ�� �����ٸ� ������ ���� �� �����Ƿ�
		// ������ �̵��� ���� �� A*�� �ٽ� ����� �Ѵ�.
		if (!isSearchMove)
		{
			// Astar �ʱ�ȭ
			this->InitAstar();
		}

		// ����� ��ġ�� ���� ������.
		if (_points.size() != 0)
		{
			// �������� ������ �������� ����.
			if (!isSearchMove)
			{
				_pos.x = _points.front().x;
				_pos.y = _points.front().y;
				_points.pop_front();

				// TODO : �����е��� ����!!
				// 1. ������ ���� ��ǥ ���
				// 2. �̵� ���� ���
				// 3. ���� Normalize
			}
		}

		// 4. �Ÿ� ���
		// �Ʒ����� �̵�
		/*if (distance > 0.5f)
		{
			isSearchMove = true;

			transform->SetPosition("�����");
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
		// TODO : ����
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
