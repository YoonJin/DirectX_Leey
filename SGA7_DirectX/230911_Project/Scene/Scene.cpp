#include "pch.h"
#include "Scene.h"
#include "Scene/Actor.h"
#include "Scene/Component/TransformComponent.h"
#include "Scene/Component/CameraComponent.h"
#include "Scene/Component/MeshRendererComponent.h"
#include "Scene/Component/MoveScriptComponent.h"
#include "Scene/Component/AIScriptComponent.h"
#include "Scene/Component/AnimatorComponent.h"
#include "Scene/Component/TextureComponent.h"

Scene::Scene(Context* const context)
	: context(context)
{
	renderer = context->GetSubsystem<Renderer>();

	LoadResource();
}

Scene::~Scene()
{
	actors.clear();
}

void Scene::Update()
{
	for (const auto& actor : actors)
		actor->Update();

	if (is_update)
	{
		renderer->UpdateRenderables(this);
		is_update = false;
	}

	for (auto iter = candies.begin(); iter != candies.end(); iter++)
	{
		if (Intersect::IsIntersect(players[0], *iter))
		{
			(*iter)->SetActive(false);
			candies.erase(iter);
			break;
		}
	}
}

auto Scene::CreateActor(const bool& is_active) -> const std::shared_ptr<class Actor>
{
	auto actor = std::make_shared<Actor>(context);
	actor->SetActive(is_active);
	AddActor(actor);

	return actor;
}

void Scene::AddActor(const std::shared_ptr<class Actor>& actor)
{
	actors.push_back(actor);
}

void Scene::FileDataRead()
{
	// 파일 입출력
	ifstream readFile;
	readFile.open("Assets/Texture/Map.txt");

	if (readFile.is_open())
	{
		int i = 0, j = 0;

		while (!readFile.eof())
		{
			char nObjNum;

			readFile.get(nObjNum);

			if (nObjNum == ' ' || nObjNum == '\n')
				continue;

			stage_map_data[i][j] = nObjNum - '0';
			j++;

			if (j == MAP_WIDTH)
			{
				j = 0;
				i++;
			}
		}
	}

	readFile.close();

	this->CreateAndSetPlayerPosition();
	this->CreateAndSetObjectPosition();
}

void Scene::CreateAndSetPlayerPosition()
{
	for (int i = 0; i < MAP_HEIGHT; i++)
	{
		for (int j = 0; j < MAP_WIDTH; j++)
		{
			if (stage_map_data[i][j] == UINT_CONVERT_TO(MapObject::PLAYER))
			{
				float posX = (j * CELL_WIDTH) - (Settings::Get().GetWidth() / 2) + (CELL_WIDTH / 2);
				float posY = ((30 - i) * CELL_HEIGHT) - (Settings::Get().GetHeight() / 2) + (CELL_HEIGHT / 2);

				auto player = CreateActor();
				player->SetName("Player");
				player->GetComponent<TransformComponent>()->SetPosition(Vec3(posX, posY, 0.0f));
				player->GetComponent<TransformComponent>()->SetScale(Vec3(0.5f, 0.5f, 0.5f));
				player->AddComponent<MeshRendererComponent>();
				player->GetComponent<MeshRendererComponent>()->SetStandardMesh(MeshType::Quad);
				player->GetComponent<MeshRendererComponent>()->SetStandardMaterial(L"Assets/Animation.hlsl");
				player->AddComponent<MoveScriptComponent>();
				player->GetComponent<MoveScriptComponent>()->_mapPos = Coordinate(j, i);
				player->GetComponent<MoveScriptComponent>()->SetMapData(this->stage_map_data);
				auto animator = player->AddComponent<AnimatorComponent>();
				animator->AddAnimation("Assets/Animation/Player.xml");
				animator->SetAnimationMode(AnimationMode::Play);
				animator->SetCurrentAnimation("PlayerMove");

				players.push_back(player.get());
			}
		}
	}

}

void Scene::CreateAndSetObjectPosition()
{
	for (int i = 0; i < MAP_HEIGHT; i++)
	{
		for (int j = 0; j < MAP_WIDTH; j++)
		{
			if (stage_map_data[i][j] == UINT_CONVERT_TO(MapObject::ENEMY))
			{
				float posX = (j * CELL_WIDTH) - (Settings::Get().GetWidth() / 2) + (CELL_WIDTH / 2);
				float posY = ((30 - i) * CELL_HEIGHT) - (Settings::Get().GetHeight() / 2) + (CELL_HEIGHT / 2);

				auto monster = CreateActor();
				monster->SetName("Monster");
				monster->GetComponent<TransformComponent>()->SetPosition(Vec3(posX, posY, 0.3f));
				monster->GetComponent<TransformComponent>()->SetScale(Vec3(14.0f, 14.0f, 14.0f));
				monster->AddComponent<MeshRendererComponent>();
				monster->GetComponent<MeshRendererComponent>()->SetStandardMesh(MeshType::Quad);
				monster->GetComponent<MeshRendererComponent>()->SetStandardMaterial(L"Assets/Texture.hlsl");
				monster->AddComponent<AIScriptComponent>().get()->_pos = Coordinate(j, i);
				monster->GetComponent<AIScriptComponent>().get()->SetMapData(this->stage_map_data);
				monster->GetComponent<AIScriptComponent>().get()->isInitialize = true;
				auto texture_mob = monster->AddComponent<TextureComponent>();
				texture_mob->LoadResource("Assets/Texture/enemy.png");
				enemies.push_back(monster.get());
			}
			else if (stage_map_data[i][j] == UINT_CONVERT_TO(MapObject::CANDY))
			{
				float posX = (j * CELL_WIDTH) - (Settings::Get().GetWidth() / 2) + (CELL_WIDTH / 2);
				float posY = ((30 - i) * CELL_HEIGHT) - (Settings::Get().GetHeight() / 2) + (CELL_HEIGHT / 2);

				auto candy = CreateActor();
				candy->SetName("Candy");
				candy->GetComponent<TransformComponent>()->SetPosition(Vec3(posX, posY, 0.3f));
				candy->GetComponent<TransformComponent>()->SetScale(Vec3(14.0f, 14.0f, 14.0f));
				candy->AddComponent<MeshRendererComponent>();
				candy->GetComponent<MeshRendererComponent>()->SetStandardMesh(MeshType::Quad);
				candy->GetComponent<MeshRendererComponent>()->SetStandardMaterial(L"Assets/Texture.hlsl");
				auto texture = candy->AddComponent<TextureComponent>();
				texture->LoadResource("Assets/Texture/Candy.png");
				candies.push_back(candy.get());
			}
		}
	}
}

void Scene::LoadResource()
{
	auto camera = CreateActor();
	camera->AddComponent<CameraComponent>();
	camera->SetName("MainCamera");

	auto map = CreateActor();
	map->SetName("Map");
	map->GetComponent<TransformComponent>()->SetPosition(Vec3(0.f, 0.f, 1.f));
	map->GetComponent<TransformComponent>()->SetScale(Vec3(479.f, 527.f, 100.f));
	map->AddComponent<MeshRendererComponent>();
	map->GetComponent<MeshRendererComponent>()->SetStandardMesh(MeshType::Quad);
	map->GetComponent<MeshRendererComponent>()->SetStandardMaterial(L"Assets/Texture.hlsl");
	auto texture = map->AddComponent<TextureComponent>();
	texture->LoadResource("Assets/Texture/background.png");


	

	//auto monster = CreateActor();
	//monster->SetName("Monster");
	//monster->GetComponent<TransformComponent>()->SetSclae(Vec3(100.0f, 100.0f, 1.0f));
	//monster->GetComponent<TransformComponent>()->SetPosition(Vec3(-100.0f, 0.0f, 0.0f));
	//monster->AddComponent<MeshRendererComponent>();
	//monster->AddComponent<AIScriptComponent>();

	FileDataRead();
}
