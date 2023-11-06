#pragma once

class Scene final
{
public:
	Scene(class Context* const context);
	~Scene();

	void Update();

	auto CreateActor(const bool& is_active = true)->const std::shared_ptr<class Actor>;
	void AddActor(const std::shared_ptr<class Actor>& actor);

	auto GetActors() const -> const std::vector<std::shared_ptr<class Actor>>& { return actors; }

	//////////////////////////////////////////////////
	// 팩맨 관련 함수
	/////////////////////////////////////////////////
	void FileDataRead();
	void CreateAndSetPlayerPosition();
	void CreateAndSetObjectPosition();

	//////////////////////////////////////////////////
	// 게임 시스템(플로우) 관련 함수
	/////////////////////////////////////////////////
	void LoadResource();

private:
	//////////////////////////////////////////////////
	// 게임 시스템(플로우) 관련 데이터
	/////////////////////////////////////////////////
	bool isLoadingComplete = false;

	class Context* context   = nullptr;
	class Renderer* renderer = nullptr;

	bool is_update = true;

	//////////////////////////////////////////////////
	// 팩맨 데이터
	/////////////////////////////////////////////////
	int stage_map_data[MAP_HEIGHT][MAP_WIDTH];

	std::vector<class Actor*> candies;
	std::vector<class Actor*> players;
	std::vector<class Actor*> enemies;
	std::vector<class Actor*> walls;

	std::vector<std::shared_ptr<class Actor>> actors;
};