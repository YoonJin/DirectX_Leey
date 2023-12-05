#pragma once
#include "RelativeScene/Scene/BaseScene.h"

class TitleScene final : public BaseScene
{
public:
	TitleScene(class Context* const context);
	~TitleScene();

	void Initialize() override;
	void Update() override;

};