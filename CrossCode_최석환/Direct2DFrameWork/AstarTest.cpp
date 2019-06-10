#include "stdafx.h"
#include "AstarTest.h"
#include "Player.h"
#include "MiniSlime.h"
#include "AstarHw.h"

void AstarTest::Init()
{
	Delay = 0.5f; Index = 0;

	player = new Player(Vector2(735, 1930));
	player->Init();
	OBJECTMANAGER->addObject(ObjectType::Player, player);

	for (int i = 0; i < 4; ++i)
	{
		slime[i] = new MiniSlime("테스트슬라임", Vector2(48.f, 48.f), Vector2(250 + 50 * i, 500), Pivot::Center);
		slime[i]->Init();
		OBJECTMANAGER->addObject(ObjectType::Enemy, slime[i]);
	}

	for (int i = 4; i < 8; ++i)
	{
		slime[i] = new MiniSlime("테스트슬라임", Vector2(48.f, 48.f), Vector2(600 + 50 * i, 500), Pivot::Center);
		slime[i]->Init();
		OBJECTMANAGER->addObject(ObjectType::Enemy, slime[i]);
	}
	

	CAMERA->SetTarget(player);

	Astar = new AstarHw;
	Astar->Init();
	//	(slime->GetPosition().x + CAMERA->GetCameraRect().left) / 64,
	//	(slime->GetPosition().y + CAMERA->GetCameraRect().top) / 64,
	//	(player->GetPosition().x + CAMERA->GetCameraRect().left) / 64,
	//	(player->GetPosition().y + CAMERA->GetCameraRect().top) / 64);

	SCENEMANAGER->SetAlpha(1.f);
	SCENEMANAGER->SetSceneState(SceneState::FadeIn);
}

void AstarTest::Update()
{
	Delay += TIMEMANAGER->getElapedTime();
	if (Delay > 0.35f)
	{
		while (Delay > 0.35f)
			Delay -= 0.35f;

			slime[Index]->SetPath(
				Astar->Update(
				(slime[Index]->GetPosition().x) / 64,
				(slime[Index]->GetPosition().y) / 64,
				(player->GetPosition().x) / 64,
				(player->GetPosition().y) / 64));
			Index++;
			if (Index == 8) Index = 0;
	}

	//Astar->Update(
	//	(slime->GetPosition().x + CAMERA->GetCameraRect().left) / 64,
	//	(slime->GetPosition().y + CAMERA->GetCameraRect().top) / 64,
	//	(player->GetPosition().x + CAMERA->GetCameraRect().left) / 64,
	//	(player->GetPosition().y + CAMERA->GetCameraRect().top) / 64);

	
	OBJECTMANAGER->Update();
}

void AstarTest::Render()
{
	Astar->Render();
	OBJECTMANAGER->Render();
}

void AstarTest::Release()
{
	OBJECTMANAGER->Release();
}
