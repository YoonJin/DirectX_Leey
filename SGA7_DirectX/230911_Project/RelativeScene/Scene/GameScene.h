#pragma once
#include "BaseScene.h"

class GameScene final : public BaseScene
{
public:
	GameScene(class Context* const context);
	~GameScene();

	void Update() override;

	//////////////////////////////////////////////////////////
	// �Ѹ� ���� �Լ�
	//////////////////////////////////////
	void FileDataRead();
	void CreateAndSetPlayerPosition();
	void CreateAndSetObjectPosition();
	
	//////////////////////////////////////////////////////////
	// ���� �ý���(�÷ο�) ���� �Լ�
	//////////////////////////////////////
	void LoadResource();

	void GameRestart();
private:

	//////////////////////////////////////////////////////////
	// ���� �ý���(�÷ο�) ���� �Լ�
	//////////////////////////////////////
	bool isLoadingComplete = false;

	//////////////////////////////////////////////////////////
	// �Ѹ� ������
	//////////////////////////////////////
	int stage_map_data[MAP_HEIGHT][MAP_WIDTH];

	std::vector<class Actor*> candies;
	std::vector<class Actor*> temp_candies;
	std::vector<class Actor*> players;
	std::vector<class Actor*> enemies;
	std::vector<class Actor*> walls;
	
	// Ÿ�Ժ��� ���� ó�����ֱ� ���� �и�
	std::vector<std::shared_ptr<class Actor>> actors;


	bool isPlayerLoading = false;
	bool isEnemyLoading = false;

	Timer* timer;
	Actor* ready;
	Renderer* renderer;

	float prevTime;
	float curTime;

public:
	int candies_count = 0;
};
