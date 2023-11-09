#pragma once
#include "IComponent.h"


struct PQNode
{
	// 점수 매기기
	// F = G + H
	// F = 최종 점수 (작을 수록 좋음. 경로에 따라 달라짐)
	// G = 시작점에서 해당 좌표까지 이동하는데 드는 비용(작을 수록 좋음. 경로에 따라 달라짐)
	// H = 목적지로부터 얼마나 가까운 곳인지를 따지는 점수(작을 수록 좋음. 고정인 값)
public:
	int F;
	int G;
	// F랑 G만 알아도 H는 구할 수 있으니 생략
	int Y;
	int X;
};

enum class EnemyState
{
	MOVE = 0,
	SEARCH
};


class AIScriptComponent final : public IComponent
{
public:
	AIScriptComponent
	(
		class Context* const context,
		class Actor* const actor,
		class TransformComponent* const transform
	);
	~AIScriptComponent() = default;

	void Initialize() override;
	void Update()     override;
	void Destroy()    override;

	float GetDistance(Vec2 curPos, Vec2 targetPos);
	Vec2 Normalize(Vec2 direction);

	Coordinate _pos;

	EnemyState _state = EnemyState::MOVE;

	bool isInitialize = false;

	int stage_map_data[MAP_HEIGHT][MAP_WIDTH];

	class TransformComponent* transform;
private:
	Vec2 direction;
	Vec2 destination;

	// A* 계산용 개별 노드
	std::queue<PQNode> pq;

	// 위, 오, 아래, 왼
	int deltaX[4] = { 0, 1, 0, -1};
	int deltaY[4] = { -1, 0, 1, 0};
	int cost[4] = {10, 10, 10, 10};

	// (y, x) 이미 방문 했는지 여부 (방문 = close 상태)
	bool closed[MAP_HEIGHT][MAP_WIDTH] = { 0, };

	// 출발지에서 (y, x) 가는데에 현재까지 업뎃된 최단거리
	// (y, x) 가는 길을 한번이라도 발견 했었는지 여부가 될 수도 있다.(한번이라도 예약 되어 있는지 여부_
	// 발견(예약)이 안되었다면 Int32.MaxValue로 저장이 되어 있을것.
	int open[MAP_HEIGHT][MAP_WIDTH];

	// 거리가 계산된 node들의 개별 부모정보
	Coordinate parent[MAP_HEIGHT][MAP_WIDTH];

	std::list<Coordinate> _points;

	// 최종 목표 업데이트 하기전의 확인 좌표
	Coordinate confirmPos = Coordinate(0, 0);
	// 이동할 최종 목표 좌표
	Coordinate dest = Coordinate(0, 0);

	bool isDefaultMove = false;
	bool isSearchMove = false;

	float _speed = 30.0f;

	void SetDestPos();
	// Astar 데이터들 초기화
	void InitAstar();
	// Astar 경로 계산
	void Astar();
	// 계산 데이터 기반으로 최종 경로 세팅
	void CalcPathFromParent(Coordinate dest);
};