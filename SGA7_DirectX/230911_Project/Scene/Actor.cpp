#include "pch.h"
#include "Actor.h"

Actor::Actor()
{
}

Actor::~Actor()
{
}

void Actor::Initialize()
{
}

void Actor::Update()
{
}

void Actor::Destroy()
{
}

auto Actor::AddComponent(const ComponentType& type) -> const std::shared_ptr<IComponent>
{
	return std::shared_ptr<IComponent>();
}

bool Actor::HasComponent(const ComponentType& type)
{
	return false;
}
