#pragma once

class Engine final
{
public:
	Engine();
	~Engine();

	void Update();
	void Render();

private:
	class Context* context = nullptr;
};