#pragma once
#include "IComponent.h"

class TextureComponent : public IComponent
{
public:
	TextureComponent
	(
		class Context* const context,
		class Actor*   const actor,
		class TransformComponent* const transform
	);
	~TextureComponent() = default;

	// TODO : �Լ� �߰� �ʿ�
private:
	std::shared_ptr<class Texture> current_texture;
};
