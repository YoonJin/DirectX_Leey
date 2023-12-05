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
	if (isinitialize)
	{
		float distance = 0.0;
		_curPos = transform->GetPosition();

		// ��ǥ ��ǥ ���� 
		this->SetDestPos();

		// �̵� �߿� A*�� ��ΰ� �ٲ�� �����ٸ� ������ ���� �� �����Ƿ� ������ �̵��� ���� �� A*�� �ٽ� ����� �Ѵ�.
		if (!isSearchMove)
		{
			// Aster �ʱ�ȭ
			this->InitAster();
		}

		// ����� ��ġ�� ���� ������
		if (_points.size() != 0)
		{
			// �������� �쿬�� �������� ����
			if (!isSearchMove)
			{
				_pos.x = _points.front().x;
				_pos.y = _points.front().y;
				_points.pop_front();

				// TODO : 1. ������ ���� ��ǥ ���, 2. �̵� ���� ���, 3. ���� Normalize

				float PosX = (this->_pos.x * CELL_WIDTH) - (Settings::Get().GetWidth() / 2) + (CELL_WIDTH / 2);
				float PosY = ((30 - this->_pos.y) * CELL_HEIGHT) - (Settings::Get().GetHeight() / 2) + (CELL_HEIGHT / 2);
				destination = Vec2(PosX, PosY);


				// �̵� ���� ���ϱ�
				direction = destination - Vec2(_curPos.x, _curPos.y);
				// ���� normalize
				direction = this->Normalize(direction);


			}
		}
		// 4. �Ÿ����
		distance = GetDistance(Vec2(_curPos.x, _curPos.y), destination);
		

		if (direction.x < 1 && direction.x > -1)
		{
			if (direction.y < 0)
			{
				_curDir = PlayerDirection::PlayerDown;
			}
			else if (direction.y > 0)
			{
				_curDir = PlayerDirection::PlayerUp;
			}
		}
		else
			_curDir = PlayerDirection::PlayerLeft;


		
		// �Ʒ����� �̵�
		if (distance > 0.5f)
		{
			isSearchMove = true;
			
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
			isSearchMove = false;

		if (this->confirmPos.Equare(this->_pos))
			this->isDefaultMove = false;
	}
}

void AIScriptComponent::Destroy()
{
}

float AIScriptComponent::GetDistance(Vec2 curPos, Vec2 targetPos)
{
	float dx = targetPos.x - curPos.x;
	float dy = targetPos.y - curPos.y;

	return sqrt(dx *dx + dy * dy);
}

Vec2 AIScriptComponent::Normalize(Vec2 direction)
{
	float length = sqrt((direction.x * direction.x) + (direction.y * direction.y));

	if (length != 0)
	{
		direction.x /= length;
		direction.y /= length;
	}

	return direction;
}

void AIScriptComponent::SetMapData(int stage_map_data[][MAP_WIDTH])
{
	for (int i = 0; i < MAP_HEIGHT; i++)
	{
		for (int j = 0; j < MAP_WIDTH; j++)
		{
			this->stage_map_data[i][j] = stage_map_data[i][j];
		}
	}
}

void AIScriptComponent::SetDestPos()
{
	switch (_state)
	{
	case EnemyState::MOVE:
		if (!isDefaultMove)
		{
			isDefaultMove = true;
			int width = MAP_WIDTH - 1;
			int height = MAP_HEIGHT - 1;
			confirmPos = Coordinate(MyMath::Random(0, width), MyMath::Random(0, height));

			while (1)
			{
				if (this->stage_map_data[confirmPos.y][confirmPos.x] == UINT_CONVERT_TO(MapObject::WALL))
				{
					confirmPos = Coordinate(MyMath::Random(0, width), MyMath::Random(0, height));
				}
				else
					break;
			}
		}
		break;
	case EnemyState::SEARCH:
		// TODO : ���� �߰�
		break;
	}
}

void AIScriptComponent::InitAster()
{
	if (!confirmPos.Equare(dest))
	{
		// ��ǥ���� ����
		dest = confirmPos;

		//���� �̵� ��� �ʱ�ȭ
		while (!_points.empty())
		{
			_points.pop_front();
		}
		_points.clear();

		// ��� ������ ����ִٸ� �ʱ�ȭ
		while (!pq.empty())
		{
			pq.pop();
		}

		for (int i = 0; i < MAP_HEIGHT; i++)
		{
			for (int j = 0; j < MAP_WIDTH; j++)
			{
				open[i][j] = INT_MAX;
				closed[i][j] = 0;
				parent[i][j] = Coordinate(0, 0);
			}
		}

		// ������ �߰� (������ ���� ����)
		open[_pos.y][_pos.x] = 0; // ������ �̴ϱ� G�� 0�̰� h����
		PQNode pqnode;
		pqnode.F = 0;
		pqnode.G = 0;
		pqnode.X = _pos.x;
		pqnode.Y = _pos.y;
		pq.push(pqnode);
		parent[_pos.y][_pos.x] = Coordinate(_pos.x, _pos.y);

		Aster();
	}
}

void AIScriptComponent::Aster()
{
	while (pq.size() > 0)
	{
		// ���� ���� �ĺ��� ã�´�.
		PQNode node = pq.front();
		pq.pop();

		// ������ ��ǥ�� ���� ��θ� ã�Ƽ� �� ���� ��η� ���ؼ� �̹� �湮 �Ȱ�� ��ŵ 
		if (closed[node.Y][node.X])
			continue;
		
		//�ƴ϶�� �湮
		closed[node.Y][node.X] = true;

		// ������ ���� �˻�
		if (node.Y == dest.y && node.X == dest.x)
			break;

		int size = (sizeof(deltaY)) / sizeof(int);
		for (int i = 0; i < size; i++)
		{
			int nextX = node.X + deltaX[i];
			int nextY = node.Y + deltaY[i];

			// ��ȿ ������ ������� ��ŵ
			if (nextY < 0 || nextY >= MAP_HEIGHT || nextX < 0 || nextX >= MAP_WIDTH)
				continue;

			// ������ ������ ���� ������ ��ŵ
			if (this->stage_map_data[nextY][nextX] == UINT_CONVERT_TO(MapObject::WALL))
				continue;

			// �̹� �湮�� ���̸� ��ŵ
			if (closed[nextY][nextX])
				continue;

			// ��� ��� ���
			int g = node.G + cost[i];
			// h���� ���� ��ǥ���� next������ �Ÿ��� �ǹ��Ѵ�.
			int h = (dest.y - nextY) + (dest.x - nextX);

			// �ٸ� ��ο��� �� ���� ���� �̹� ã������ ��ŵ
			if (open[nextY][nextX] < g + h)
				continue;

			// ���� ����
			open[nextY][nextX] = g + h;
			PQNode pqNode2;
			pqNode2.F = g + h;
			pqNode2.G = g;
			pqNode2.X = nextX;
			pqNode2.Y = nextY;
			pq.push(pqNode2);
			parent[nextY][nextX] = Coordinate(node.X, node.Y);
		}
	}
	CalcPathFromParent(dest);
}

void AIScriptComponent::CalcPathFromParent(Coordinate dest)
{
	int x = dest.x;
	int y = dest.y;

	while (parent[y][x].y != y || parent[y][x].x != x)
	{
		_points.push_back(Coordinate(x, y));

		// �θ� ���Ѵ�.
		Vec2 pos(parent[y][x].x, parent[y][x].y);
		x = pos.x;
		y = pos.y;
	}

	// �Ųٷ� �̵��ؾ� �ϹǷ� ��ǥ ����� ������ �ش�.
	_points.reverse();
}
