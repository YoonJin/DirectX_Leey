#include "pch.h"
#include "IComponent.h"
#include "TransformComponent.h"
#include "CameraComponent.h"
#include "MeshRendererComponent.h"

IComponent::IComponent(Actor* const actor, TransformComponent* const transform)
	: actor(actor),
	  transform(transform)
{

}

template <typename T>
constexpr ComponentType IComponent::DeduceComponentType()
{
	return ComponentType::Unknown;
}

#define REGISTER_COMPONENT_TYPE(T, enum_type) template<> ComponentType IComponent::DeduceComponentType<T>() { return enum_type; } 
REGISTER_COMPONENT_TYPE(TransformComponent, ComponentType::Transform)
REGISTER_COMPONENT_TYPE(CameraComponent, ComponentType::Camera)
REGISTER_COMPONENT_TYPE(MeshRendererComponent, ComponentType::MeshRenderer)
