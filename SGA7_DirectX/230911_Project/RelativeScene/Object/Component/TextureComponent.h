#pragma once

#include "IComponent.h"

class TextureComponent : public IComponent
{
public:
	TextureComponent(
		class Context* const context,
		class Actor* const actor,
		class TransformComponent* const transform
	);
	~TextureComponent() = default;

	void Initialize() override;
	void Update() override;
	void Destroy() override;

	void LoadResource(const std::string& path);
	const std::shared_ptr<Texture>& GetCurrentTexture() { return current_texture; }


private:
	std::shared_ptr<class Texture> current_texture;
};