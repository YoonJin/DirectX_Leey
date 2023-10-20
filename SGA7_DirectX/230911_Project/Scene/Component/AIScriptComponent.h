#pragma once
#include "IComponent.h"

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

private:
	enum class Direction
	{
		Up,
		Right,
		Down,
		Left
	};
	
private:
	Stopwatch stopwatch;
	Direction direction = Direction::Left;
};