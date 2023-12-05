#include "pch.h"
#include "Texture.h"

Texture::Texture(Context* const context) : IResource(context, ResourceType::Texture2D)
{
}

Texture::~Texture()
{
}

bool Texture::SaveToFile(const std::string& path)
{
	return true;
}

bool Texture::LoadFromFile(const std::string& path)
{
	std::wstring real_path;
	real_path.append(path.begin(), path.end());

	origin_texture = std::make_shared<D3D11_Texture>(context->GetSubsystem<Graphics>());
	origin_texture->Create(real_path);

	return true;
}
