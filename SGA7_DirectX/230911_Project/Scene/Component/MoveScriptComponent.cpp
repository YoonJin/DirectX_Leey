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
	_curPos = position;

	if (GetAsyncKeyState('W') & 0x8000)
	{
		_curPos.y += _speed;
		_curDir = PlayerDirection::PlayerUp;
	}
	else if (GetAsyncKeyState('S') & 0x8000)
	{
		_curPos.y -= _speed;
		_curDir = PlayerDirection::PlayerDown;
	}
	if (GetAsyncKeyState('D') & 0x8000)
	{
		_curPos.x += _speed;
		_curDir = PlayerDirection::PlayerRight;
	}
	else if (GetAsyncKeyState('A') & 0x8000)
	{
		_curPos.x -= _speed;
		_curDir = PlayerDirection::PlayerLeft;
	}

	position = _curPos;
	transform->SetPosition(position);
}

void MoveScriptComponent::Destroy()
{
}
