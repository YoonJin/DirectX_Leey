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

private:
	class Context* context   = nullptr;
	class Renderer* renderer = nullptr;

	bool is_update = true;

	//////////////////////////////////////////////////
	// 팩맨 데이터
	/////////////////////////////////////////////////
	int stage_map_data[MAP_HEIGHT][MAP_WIDTH];

	std::vector<std::shared_ptr<class Actor*>> candies;
	std::vector<std::shared_ptr<class Actor*>> players;
	std::vector<std::shared_ptr<class Actor*>> enemies;
	std::vector<std::shared_ptr<class Actor*>> walls;

	std::vector<std::shared_ptr<class Actor>> actors;
};