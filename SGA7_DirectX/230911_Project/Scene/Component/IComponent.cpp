#include "pch.h"
#include "IComponent.h"

IComponent::IComponent(Actor* const actor, TransformComponent* const transform)
{
}

template <typename T>
constexpr ComponentType IComponent::DeduceComponentType()
{
	return ComponentType::Unknown;
}

#define REGISTER_COMPONENT_TYPE(T, enum_type) template<> ComponentType IComponent::DeduceComponentType<T>() { return enum_type; } 
REGISTER_COMPONENT_TYPE(TransformComponent, ComponentType::Transform)
