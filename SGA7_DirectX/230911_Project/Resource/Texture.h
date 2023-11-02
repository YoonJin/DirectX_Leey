#pragma once
#include "IResource.h"

class Texture final : public IResource
{
public:
	Texture(class Context* const context);
	~Texture();

	bool SaveToFile(const std::string& path) override;
	bool LoadFromFile(const std::string& path) override;

	const D3D11_Texture* GetTexture() { return origin_texture.get(); }

private:
	std::shared_ptr<class D3D11_Texture> origin_texture;
};