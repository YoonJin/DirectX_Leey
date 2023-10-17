#pragma once
#include "IComponent.h"

class MeshRendererComponent final : public IComponent
{
public:
	MeshRendererComponent
	(
		class Actor* const actor,
		class TransformComponent* const transform
	);
	~MeshRendererComponent() = default;

	void Initialize() override;
	void Update()     override;
	void Destroy()    override;
};