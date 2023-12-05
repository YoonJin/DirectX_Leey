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

		// 목표 좌표 설정 
		this->SetDestPos();

		// 이동 중에 A*로 경로가 바뀌어 버린다면 문제가 생길 수 있으므로 무조건 이동이 끝난 후 A*로 다시 계산을 한다.
		if (!isSearchMove)
		{
			// Aster 초기화
			this->InitAster();
		}

		// 계산한 위치에 따라 움직임
		if (_points.size() != 0)
		{
			// 지속적인 우연한 움직임을 위함
			if (!isSearchMove)
			{
				_pos.x = _points.front().x;
				_pos.y = _points.front().y;
				_points.pop_front();

				// TODO : 1. 목적지 실제 좌표 계산, 2. 이동 방향 계산, 3. 방향 Normalize

				float PosX = (this->_pos.x * CELL_WIDTH) - (Settings::Get().GetWidth() / 2) + (CELL_WIDTH / 2);
				float PosY = ((30 - this->_pos.y) * CELL_HEIGHT) - (Settings::Get().GetHeight() / 2) + (CELL_HEIGHT / 2);
				destination = Vec2(PosX, PosY);


				// 이동 방향 구하기
				direction = destination - Vec2(_curPos.x, _curPos.y);
				// 방향 normalize
				direction = this->Normalize(direction);


			}
		}
		// 4. 거리계산
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


		
		// 아래에서 이동
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
		// TODO : 추후 추가
		break;
	}
}

void AIScriptComponent::InitAster()
{
	if (!confirmPos.Equare(dest))
	{
		// 목표지점 세팅
		dest = confirmPos;

		//최종 이동 경로 초기화
		while (!_points.empty())
		{
			_points.pop_front();
		}
		_points.clear();

		// 노드 정보가 들어있다면 초기화
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

		// 시작점 발견 (시작점 예약 진행)
		open[_pos.y][_pos.x] = 0; // 시작점 이니까 G는 0이고 h점금
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
		// 제일 좋은 후보를 찾는다.
		PQNode node = pq.front();
		pq.pop();

		// 동일한 좌표를 여러 경로를 찾아서 더 빠른 경로로 인해서 이미 방문 된경우 스킵 
		if (closed[node.Y][node.X])
			continue;
		
		//아니라면 방문
		closed[node.Y][node.X] = true;

		// 목적지 인지 검사
		if (node.Y == dest.y && node.X == dest.x)
			break;

		int size = (sizeof(deltaY)) / sizeof(int);
		for (int i = 0; i < size; i++)
		{
			int nextX = node.X + deltaX[i];
			int nextY = node.Y + deltaY[i];

			// 유효 범위를 벗어났으면 스킵
			if (nextY < 0 || nextY >= MAP_HEIGHT || nextX < 0 || nextX >= MAP_WIDTH)
				continue;

			// 벽으로 막혀서 갈수 없으면 스킵
			if (this->stage_map_data[nextY][nextX] == UINT_CONVERT_TO(MapObject::WALL))
				continue;

			// 이미 방문한 곳이면 스킵
			if (closed[nextY][nextX])
				continue;

			// 경로 비용 계산
			int g = node.G + cost[i];
			// h값은 현재 좌표에서 next까지의 거리를 의미한다.
			int h = (dest.y - nextY) + (dest.x - nextX);

			// 다른 경로에서 더 빠른 길을 이미 찾았으면 스킵
			if (open[nextY][nextX] < g + h)
				continue;

			// 예약 진행
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

		// 부모를 비교한다.
		Vec2 pos(parent[y][x].x, parent[y][x].y);
		x = pos.x;
		y = pos.y;
	}

	// 거꾸로 이동해야 하므로 좌표 목록을 뒤집어 준다.
	_points.reverse();
}
