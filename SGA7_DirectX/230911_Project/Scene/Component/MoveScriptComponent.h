#pragma once
#include "IComponent.h"

class MoveScriptComponent final : public IComponent
{
public:
	MoveScriptComponent
	(
		class Context* const context,
		class Actor* const actor,
		class TransformComponent* const transform
	);
	~MoveScriptComponent() = default;

	void Initialize() override;
	void Update()     override;
	void Destroy()    override;
};