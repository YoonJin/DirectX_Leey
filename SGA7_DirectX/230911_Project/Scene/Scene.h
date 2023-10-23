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

private:
	class Context* context   = nullptr;
	class Renderer* renderer = nullptr;

	std::vector<std::shared_ptr<class Actor>> actors;

	bool is_update = true;
};