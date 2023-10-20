#include "pch.h"
#include "MoveScriptComponent.h"
#include "TransformComponent.h"

MoveScriptComponent::MoveScriptComponent(Context* const context, Actor* const actor, TransformComponent* const transform)
	:IComponent(context, actor, transform)
{
}

void MoveScriptComponent::Initialize()
{
}

void MoveScriptComponent::Update()
{
	auto position = transform->GetPosition();

	if (GetAsyncKeyState('W') & 0x8000) position.y++;
	else if (GetAsyncKeyState('S') & 0x8000) position.y--;
	if (GetAsyncKeyState('D') & 0x8000) position.x++;
	else if (GetAsyncKeyState('A') & 0x8000) position.x--;

	transform->SetPosition(position);
}

void MoveScriptComponent::Destroy()
{
}
