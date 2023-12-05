#include "pch.h"
#include "GameScene.h"
#include "RelativeScene/Object/Actor.h"
#include "RelativeScene/Object/Component/TransformComponent.h"
#include "RelativeScene/Object/Component/CameraComponent.h"
#include "RelativeScene/Object/Component/MeshRendererComponent.h"
#include "RelativeScene/Object/Component/MoveScriptComponent.h"
#include "RelativeScene/Object/Component/AIScriptComponent.h"
#include "RelativeScene/Object/Component/AnimatorComponent.h"
#include "RelativeScene/Object/Component/TextureComponent.h"
#include "Intersect/Intersect.h"

GameScene::GameScene(Context* const context) : BaseScene(context)
{
	registerObserver(context->GetSubsystem<SceneManager>());
	registerObserver(context->GetSubsystem<Graphics>());
	registerObserver(context->GetSubsystem<Renderer>());
	registerObserver(context->GetSubsystem<Timer>());


	thread Loading([&]() {
		HRESULT hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
		CHECK(hr);

		this->LoadResource();
		// 통지
		this->notifyObserver();

		CoUninitialize();
		});

	Loading.detach();

	timer = (context->GetSubsystem<Timer>());
	renderer = (context->GetSubsystem<Renderer>());

	prevTime = timer->GetDeltaTimeSEC();
	curTime = timer->GetDeltaTimeSEC();
	candies_count = 0;
}

GameScene::~GameScene()
{
	actors.clear();
}

void GameScene::Update()
{
	if (isPlayerLoading && isEnemyLoading)
	{
		if(!this->timer->IsPause())
		{
			BaseScene::Update();

			std::vector<Actor*>::iterator candy;
			for (candy = candies.begin(); candy != candies.end(); candy++)
			{
				if (Intersect::IsIntersect(players[0], *candy))
				{
					(*candy)->SetActive(false);
					temp_candies.push_back((*candy));
					candies.erase(candy);

					candies_count--;
					break;
				}
			}
		}
		
		// 일시정지
		if (candies_count == 0)
		{
			this->timer->SetPause(true);
			this->ready->SetActive(true);

			std::cout << curTime << std::endl;
			curTime += timer->GetDeltaTimeSEC();

			if (curTime > 2.f)
			{
				GameRestart();
				this->curTime = 0;
				this->timer->SetPause(false);
				this->ready->SetActive(false);
			}
		}
	}
	
}



void GameScene::FileDataRead()
{
	// 파일 입출력
	ifstream readFile;
	readFile.open("Assets/Texture/map.txt");

	if (readFile.is_open())
	{
		int heigth = 0;
		int width = 0;

		while (!readFile.eof())
		{
			char nObjNum;

			readFile.get(nObjNum);

			if (nObjNum == ' ' || nObjNum == '\n')
				continue;

			stage_map_data[heigth][width] = nObjNum - '0';
			width++;

			if (width == MAP_WIDTH)
			{
				width = 0;
				heigth++;
			}
		}
	}

	readFile.close();

	this->CreateAndSetPlayerPosition();
	this->CreateAndSetObjectPosition();
}

void GameScene::CreateAndSetPlayerPosition()
{
	for (int i = 0; i < MAP_HEIGHT; i++)
	{
		for (int j = 0; j < MAP_WIDTH; j++)
		{
			if (stage_map_data[i][j] == UINT_CONVERT_TO(MapObject::PLAYER))
			{

				float posX = (j * CELL_WIDTH - (Settings::Get().GetWidth() / 2) + (CELL_WIDTH / 2));
				float posY = ((30 - i) * CELL_HEIGHT - (Settings::Get().GetHeight() / 2) + (CELL_HEIGHT/2));

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
	isPlayerLoading = true;

}

void GameScene::CreateAndSetObjectPosition()
{
	for (int i = 0; i < MAP_HEIGHT; i++)
	{
		for (int j = 0; j < MAP_WIDTH; j++)
		{			
			if (stage_map_data[i][j] == UINT_CONVERT_TO(MapObject::ENEMY))
			{
				float posX = (j * CELL_WIDTH - (Settings::Get().GetWidth() / 2) + (CELL_WIDTH / 2));
				float posY = ((30 - i) * CELL_HEIGHT - (Settings::Get().GetHeight() / 2) + (CELL_HEIGHT / 2));

				auto monster = CreateActor();
				monster->SetName("Monster");
				monster->GetComponent<TransformComponent>()->SetScale(Vec3(1.2f, 1.2f, 0.3f));
				monster->GetComponent<TransformComponent>()->SetPosition(Vec3(posX, posY, 0.0f));
				monster->AddComponent<MeshRendererComponent>();
				monster->GetComponent<MeshRendererComponent>()->SetStandardMesh(MeshType::Quad);
				monster->GetComponent<MeshRendererComponent>()->SetStandardMaterial(L"Assets/Animation.hlsl");
				monster->AddComponent<AIScriptComponent>().get()->_pos = Coordinate(j, i);
				monster->GetComponent<AIScriptComponent>().get()->SetMapData(this->stage_map_data);
				monster->GetComponent<AIScriptComponent>().get()->isinitialize = true;

				auto animator = monster->AddComponent<AnimatorComponent>();
				animator->AddAnimation("Assets/Animation/Enemy.xml");
				animator->SetAnimationMode(AnimationMode::Play);
				animator->SetCurrentAnimation("EnemyMove");

				enemies.push_back(monster.get());
			}

			else if (stage_map_data[i][j] == UINT_CONVERT_TO(MapObject::CANDY))
			{
				float posX = (j * CELL_WIDTH - (Settings::Get().GetWidth() / 2) + (CELL_WIDTH / 2));
				float posY = ((30 - i) * CELL_HEIGHT - (Settings::Get().GetHeight() / 2) + (CELL_HEIGHT / 2));

				auto candy = CreateActor();
				candy->SetName("Candy");
				candy->GetComponent<TransformComponent>()->SetPosition(Vec3(posX, posY, 0.3f));
				candy->GetComponent<TransformComponent>()->SetScale(Vec3(14.0f, 14.0f, 1.0f));
				candy->AddComponent<MeshRendererComponent>();
				candy->GetComponent<MeshRendererComponent>()->SetStandardMesh(MeshType::Quad);
				candy->GetComponent<MeshRendererComponent>()->SetStandardMaterial(L"Assets/Texture.hlsl");
				auto texture = candy->AddComponent<TextureComponent>();
				texture->LoadResource("Assets/Texture/Candy.png");
				candies.push_back(candy.get());
			}
		}
	}

	candies_count = candies.size();
	isEnemyLoading = true;
}

void GameScene::LoadResource()
{
	auto map = CreateActor();
	map->SetName("Map");
	map->GetComponent<TransformComponent>()->SetPosition(Vec3(0.f, 0.f, 1.f));
	map->GetComponent<TransformComponent>()->SetScale(Vec3(479.f, 527.f, 100.f));
	map->AddComponent<MeshRendererComponent>();
	map->GetComponent<MeshRendererComponent>()->SetStandardMesh(MeshType::Quad);
	map->GetComponent<MeshRendererComponent>()->SetStandardMaterial(L"Assets/Texture.hlsl");
	auto texture = map->AddComponent<TextureComponent>();
	texture->LoadResource("Assets/Texture/background.png");

	auto ready = CreateActor();
	ready->SetName("Ready");
	ready->GetComponent<TransformComponent>()->SetPosition(Vec3(0.f, 0.f, 0.0f));
	ready->GetComponent<TransformComponent>()->SetScale(Vec3(65.f, 16.f, 1.00f));
	ready->AddComponent<MeshRendererComponent>();
	ready->GetComponent<MeshRendererComponent>()->SetStandardMesh(MeshType::Quad);
	ready->GetComponent<MeshRendererComponent>()->SetStandardMaterial(L"Assets/Texture.hlsl");
	auto texture2 = ready->AddComponent<TextureComponent>();
	texture2->LoadResource("Assets/Texture/Ready.png");
	this->ready = ready.get();
	this->ready->SetActive(false);

	FileDataRead();
}

void GameScene::GameRestart()
{
	for (int i = 0; i < MAP_HEIGHT; i++)
	{
		for (int j = 0; j < MAP_WIDTH; j++)
		{
			if (stage_map_data[i][j] == UINT_CONVERT_TO(MapObject::PLAYER))
			{

				float posX = (j * CELL_WIDTH - (Settings::Get().GetWidth() / 2) + (CELL_WIDTH / 2));
				float posY = ((30 - i) * CELL_HEIGHT - (Settings::Get().GetHeight() / 2) + (CELL_HEIGHT / 2));

				players[0]->SetName("Player");
				players[0]->GetComponent<TransformComponent>()->SetPosition(Vec3(posX, posY, 0.0f));
				players[0]->GetComponent<TransformComponent>()->SetScale(Vec3(0.5f, 0.5f, 0.5f));
				players[0]->GetComponent<MeshRendererComponent>()->SetStandardMesh(MeshType::Quad);
				players[0]->GetComponent<MeshRendererComponent>()->SetStandardMaterial(L"Assets/Animation.hlsl");
				players[0]->GetComponent<MoveScriptComponent>()->_mapPos = Coordinate(j, i);
				players[0]->GetComponent<MoveScriptComponent>()->SetMapData(this->stage_map_data);
				auto animator = players[0]->GetComponent<AnimatorComponent>();
				animator->SetAnimationMode(AnimationMode::Play);
				animator->SetCurrentAnimation("PlayerMove");
			}
		}
	}

	int k = 0;
	for (int i = 0; i < MAP_HEIGHT; i++)
	{
		for (int j = 0; j < MAP_WIDTH; j++)
		{
			if (stage_map_data[i][j] == UINT_CONVERT_TO(MapObject::ENEMY))
			{
				float posX = (j * CELL_WIDTH - (Settings::Get().GetWidth() / 2) + (CELL_WIDTH / 2));
				float posY = ((30 - i) * CELL_HEIGHT - (Settings::Get().GetHeight() / 2) + (CELL_HEIGHT / 2));

				enemies[k]->GetComponent<TransformComponent>()->SetPosition(Vec3(posX, posY, 0.0f));
				enemies[k]->GetComponent<TransformComponent>()->SetScale(Vec3(1.2f, 1.2f, 0.3f));
				enemies[k]->GetComponent<TransformComponent>()->SetPosition(Vec3(posX, posY, 0.0f));
				enemies[k]->GetComponent<MeshRendererComponent>()->SetStandardMesh(MeshType::Quad);
				enemies[k]->GetComponent<MeshRendererComponent>()->SetStandardMaterial(L"Assets/Animation.hlsl");
				enemies[k]->RemoveComponent<AIScriptComponent>();
				enemies[k]->AddComponent<AIScriptComponent>();
				enemies[k]->GetComponent<AIScriptComponent>().get()->_pos = Coordinate(j, i);
				enemies[k]->GetComponent<AIScriptComponent>().get()->SetMapData(this->stage_map_data);
				enemies[k]->GetComponent<AIScriptComponent>().get()->isinitialize = true;

				auto animator = enemies[k]->GetComponent<AnimatorComponent>();
				animator->SetAnimationMode(AnimationMode::Play);
				animator->SetCurrentAnimation("EnemyMove");
				k++;
			}
		}

		std::vector<Actor*>::iterator candy;
		for (candy = temp_candies.begin(); candy != temp_candies.end(); candy++)
		{
			(*candy)->SetActive(true);
		}

		candies = temp_candies;
		candies_count = temp_candies.size();
	}
}
