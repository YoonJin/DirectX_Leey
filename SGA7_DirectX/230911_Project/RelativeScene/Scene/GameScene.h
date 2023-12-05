#pragma once
#include "BaseScene.h"

class GameScene final : public BaseScene
{
public:
	GameScene(class Context* const context);
	~GameScene();

	void Update() override;

	//////////////////////////////////////////////////////////
	// 팩맨 관련 함수
	//////////////////////////////////////
	void FileDataRead();
	void CreateAndSetPlayerPosition();
	void CreateAndSetObjectPosition();
	
	//////////////////////////////////////////////////////////
	// 게임 시스템(플로우) 관련 함수
	//////////////////////////////////////
	void LoadResource();

	void GameRestart();
private:

	//////////////////////////////////////////////////////////
	// 게임 시스템(플로우) 관련 함수
	//////////////////////////////////////
	bool isLoadingComplete = false;

	//////////////////////////////////////////////////////////
	// 팩맨 데이터
	//////////////////////////////////////
	int stage_map_data[MAP_HEIGHT][MAP_WIDTH];

	std::vector<class Actor*> candies;
	std::vector<class Actor*> temp_candies;
	std::vector<class Actor*> players;
	std::vector<class Actor*> enemies;
	std::vector<class Actor*> walls;
	
	// 타입별로 따로 처리해주기 위해 분리
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
