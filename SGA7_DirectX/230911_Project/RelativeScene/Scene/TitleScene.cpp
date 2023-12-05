#include "pch.h"
#include "TitleScene.h"
#include "RelativeScene/Object/Actor.h"
#include "RelativeScene/Object/Component/TransformComponent.h"
#include "RelativeScene/Object/Component/CameraComponent.h"
#include "RelativeScene/Object/Component/MeshRendererComponent.h"
#include "RelativeScene/Object/Component/MoveScriptComponent.h"
#include "RelativeScene/Object/Component/AIScriptComponent.h"
#include "RelativeScene/Object/Component/AnimatorComponent.h"
#include "RelativeScene/Object/Component/TextureComponent.h"

TitleScene::TitleScene(Context* const context)
	: BaseScene(context)
{
	auto map = CreateActor();
	map->SetName("Map");
	map->GetComponent<TransformComponent>()->SetPosition(Vec3(0.f, 0.f, 1.f));
	map->GetComponent<TransformComponent>()->SetScale(Vec3(479.f, 527.f, 100.f));
	map->AddComponent<MeshRendererComponent>();
	map->GetComponent<MeshRendererComponent>()->SetStandardMesh(MeshType::Quad);
	map->GetComponent<MeshRendererComponent>()->SetStandardMaterial(L"Assets/Texture.hlsl");
	auto texture = map->AddComponent<TextureComponent>();
	texture->LoadResource("Assets/title.jpg");
}

TitleScene::~TitleScene()
{
}

void TitleScene::Initialize()
{
	BaseScene::Initialize();
}

void TitleScene::Update()
{
	BaseScene::Update();

	if (GetAsyncKeyState(VK_SPACE) & 0x8000)
	{
		context->GetSubsystem<SceneManager>()->ChangeScene(SceneType::TitleScene);
	}
}
