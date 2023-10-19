#pragma once

class Scene final
{
public:
	Scene();
	~Scene();

	void Update();
	void Render();

	auto CreateActor(const bool& is_active = true)->const std::shared_ptr<class Actor>;
	void AddActor(const std::shared_ptr<class Actor>& actor);

private:
	std::shared_ptr<class D3D11_Pipeline> pipeline;
	std::vector<std::shared_ptr<class Actor>> actors;
};