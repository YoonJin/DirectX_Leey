#include "pch.h"
#include "TextureComponent.h"

TextureComponent::TextureComponent(Context* const context, Actor* const actor, TransformComponent* const transform)
	: IComponent(context, actor, transform)
{
}

void TextureComponent::Initialize()
{
}

void TextureComponent::Update()
{
}

void TextureComponent::Destroy()
{
}

void TextureComponent::LoadResource(const std::string& path)
{
	auto resource_manager = context->GetSubsystem<ResourceManager>();

	auto last_index = path.find_last_of("\\/");
	auto file_name  = path.substr(last_index + 1, path.length());
	last_index      = file_name.find_last_of('.');
	std::string resource_name = file_name.substr(0, last_index);

	current_texture = resource_manager->GetResourceFromName<Texture>(resource_name);

	if (!current_texture)
		current_texture = resource_manager->Load<Texture>(path);

	if (!current_texture)
		assert(false);
}
