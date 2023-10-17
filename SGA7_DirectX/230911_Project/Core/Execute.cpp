#include "pch.h"
#include "Execute.h"
#include "Graphics.h"
#include "Scene/Actor.h"
#include "Scene/Component/TransformComponent.h"
#include "Scene/Component/CameraComponent.h"
#include "Scene/Component/MeshRendererComponent.h"

Execute::Execute()
{
	Graphics::Get().CreateDeviceAndSwapChain();
	Graphics::Get().CreateRenderTargetView
	(static_cast<uint>(Settings::Get().GetWidth()),
		static_cast<uint>(Settings::Get().GetHeight()));

	pipeline = std::make_shared<D3D11_Pipeline>(&Graphics::Get());

	auto camera = CreateActor();
	camera->AddComponent<CameraComponent>();
	camera->SetName("MainCamera");

	auto player = CreateActor();
	player->AddComponent<MeshRendererComponent>();
	player->GetComponent<TransformComponent>()->SetSclae(Vec3(100.0f, 100.0f, 1.0f));
	player->GetComponent<TransformComponent>()->SetPosition(Vec3(100.0f, 0.0f, 0.0f));
	player->SetName("Player");

	auto monster = CreateActor();
	monster->AddComponent<MeshRendererComponent>();
	monster->GetComponent<TransformComponent>()->SetSclae(Vec3(100.0f, 100.0f, 1.0f));
	monster->GetComponent<TransformComponent>()->SetPosition(Vec3(-100.0f, 0.0f, 0.0f));
	monster->SetName("Monster");
}

Execute::~Execute()
{
	
}

void Execute::Update()
{
	
}

void Execute::Render()
{
	
}

auto Execute::CreateActor(const bool& is_active) -> const std::shared_ptr<class Actor>
{
	return std::shared_ptr<class Actor>();
}

void Execute::AddActor(const std::shared_ptr<class Actor>& actor)
{
}
