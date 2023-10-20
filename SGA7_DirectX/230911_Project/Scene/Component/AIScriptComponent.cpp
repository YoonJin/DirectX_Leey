#include "pch.h"
#include "AIScriptComponent.h"
#include "TransformComponent.h"

AIScriptComponent::AIScriptComponent(Context* const context, Actor* const actor, TransformComponent* const transform)
	: IComponent(context, actor, transform)
{
}

void AIScriptComponent::Initialize()
{
}

void AIScriptComponent::Update()
{
	auto position = transform->GetPosition();

	switch (direction)
	{
	case Direction::Up:    position.y++; break;
	case Direction::Right: position.x++; break;
	case Direction::Down:  position.y--; break;
	case Direction::Left:  position.x--; break;
	}

	if (stopwatch.GetElapsedTimeSec() >= 3.0f)
	{
		auto new_dir = static_cast<Direction>(MyMath::Random(0, 3));
		if (direction == new_dir)
		{
			int dir_num = static_cast<int>(new_dir);
			dir_num = (dir_num <= 1) ? dir_num + 2 : dir_num - 2;

			direction = static_cast<Direction>(dir_num);
		}
		else
			direction = new_dir;

		stopwatch.Start();
	}

	transform->SetPosition(position);
}

void AIScriptComponent::Destroy()
{
}
