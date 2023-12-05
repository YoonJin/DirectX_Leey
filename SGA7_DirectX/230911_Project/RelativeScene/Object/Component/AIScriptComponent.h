#pragma once
#include "IComponent.h"

struct PQNode
{
	// ���� �ű��
	// F = G + H
	// F = ���� ���� (���� ���� ����, ��ο� ���� �޶���)
	// G = ���������� �ش� ��ǥ���� �̵��ϴµ� ��� ��� (���� ���� ���� ��ο� ���� �޶���)
	// H = �������κ��� �󸶳� ����� �������� ������ ����(���� ���� ���� ������ ��)

public:
	int F;
	int G;
	// F�� G�� �˾Ƶ� H�� ���� �� ������ ����
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

	bool isinitialize = false;

	int stage_map_data[MAP_HEIGHT][MAP_WIDTH];

	class TransformComponent* transform;
	
	void SetMapData(int stage_map_data[][MAP_WIDTH]);

	PlayerDirection GetCurrentDirection() { return _curDir; }
	PlayerDirection _curDir = PlayerDirection::PlayerLeft;
private:
	Vec3 _curPos;

	Vec2 direction;
	Vec2 destination;



	// A* ���� ���� ���
	std::queue<PQNode> pq;

	// ��, ����, �Ʒ�,  �� ����
	int deltaX[4] = { 0 , 1 , 0 , -1 }; 
	int deltaY[4] = { -1, 0, 1, 0 };
	int cost[4] = { 10, 10, 10, 10 };

	// (y, x) �̹� �湮�ߴ��� ���� (�湮 = close ����)
	bool closed[MAP_HEIGHT][MAP_WIDTH] = { 0, };

	// ��������� ( y, x ) ���µ��� ������� ������ �ִܰŸ�
	// (y, x) ���� ���� �ѹ��̶� �߰� �߾����� ���ΰ� �ɼ��� �ִ�. (�ѹ��̶� ���� �Ǿ� �ִ��� ����
	// �߰�(����)�� �ȵǾ��ٸ� int32 maxvalue�� ������ �Ǿ� ���� ���̴�.
	int open[MAP_HEIGHT][MAP_WIDTH];

	// �Ÿ��� ���� node���� ���� �θ�����
	Coordinate parent[MAP_HEIGHT][MAP_WIDTH];

	std::list<Coordinate> _points;

	// ���� ��ǥ ������Ʈ �ϱ����� Ȯ�� ��ǥ
	Coordinate confirmPos = Coordinate(0, 0);
	// �̵��� ���� ��ǥ ��ǥ
	Coordinate dest = Coordinate(0, 0);

	bool isDefaultMove = false;
	bool isSearchMove = false;

	float _speed = 30.0f;

	void SetDestPos();
	// Aster �����͸� �ʱ�ȭ
	void InitAster();
	// Aster ��� ���
	void Aster();
	// ��� ������ ������� ���� ��� ����
	void CalcPathFromParent(Coordinate dest);
};