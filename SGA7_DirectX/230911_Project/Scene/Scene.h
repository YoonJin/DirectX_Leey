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
	// �Ѹ� ���� �Լ�
	/////////////////////////////////////////////////
	void FileDataRead();
	void CreateAndSetPlayerPosition();
	void CreateAndSetObjectPosition();

	//////////////////////////////////////////////////
	// ���� �ý���(�÷ο�) ���� �Լ�
	/////////////////////////////////////////////////
	void LoadResource();

private:
	//////////////////////////////////////////////////
	// ���� �ý���(�÷ο�) ���� ������
	/////////////////////////////////////////////////
	bool isLoadingComplete = false;

	class Context* context   = nullptr;
	class Renderer* renderer = nullptr;

	bool is_update = true;

	//////////////////////////////////////////////////
	// �Ѹ� ������
	/////////////////////////////////////////////////
	int stage_map_data[MAP_HEIGHT][MAP_WIDTH];

	std::vector<class Actor*> candies;
	std::vector<class Actor*> players;
	std::vector<class Actor*> enemies;
	std::vector<class Actor*> walls;

	std::vector<std::shared_ptr<class Actor>> actors;
};