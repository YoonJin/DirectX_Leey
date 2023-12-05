#pragma once
#include "RelativeScene/Scene/BaseScene.h"

class LoadingScene final : public BaseScene
{
public:
	LoadingScene(class Context* const context);
	~LoadingScene();

	void Initialize() override;
	void Update() override;
};