#include "stdafx.h"
#include "TownScene.h"
#include "Player.h"



void TownScene::Init()
{
	this->LoadMap();
	caveImage[0] = IMAGEMANAGER->FindImage("큰마을");
	caveImage[1] = IMAGEMANAGER->FindImage("큰마을2");
	caveImage[2] = IMAGEMANAGER->FindImage("큰마을3");
	caveImage[3] = IMAGEMANAGER->FindImage("큰마을4");
	caveImage[4] = IMAGEMANAGER->FindImage("큰마을5");
	caveImage[5] = IMAGEMANAGER->FindImage("큰마을6");
	caveImage[6] = IMAGEMANAGER->FindImage("큰마을7");
	caveImage[7] = IMAGEMANAGER->FindImage("큰마을8");
	caveImage[8] = IMAGEMANAGER->FindImage("큰마을9");

	player = new Player(Vector2(2500, 4500));
	player->Init();

	OBJECTMANAGER->addObject(ObjectType::Player, player);
	CAMERA->SetMapSize(Vector2(100000,100000));
	CAMERA->SetTarget(player);

	SCENEMANAGER->SetAlpha(1.f);
	SCENEMANAGER->SetSceneState(SceneState::FadeIn);
}

void TownScene::Update()
{
	OBJECTMANAGER->Update();
}

void TownScene::Render()
{
	this->TerrainRender();
	this->Terrain2Render();
	OBJECTMANAGER->Render();
	this->ObjectRender();

	if (player->GetAttackType())
	{
		IMAGEMANAGER->FindImage("근공격커서")->SetSize(Vector2(32, 32));
		IMAGEMANAGER->FindImage("근공격커서")->Render(_ptMouse.x, _ptMouse.y, Pivot::LeftTop);
	}
	else
	{
		IMAGEMANAGER->FindImage("원공격커서")->SetSize(Vector2(32, 32));
		IMAGEMANAGER->FindImage("원공격커서")->Render(_ptMouse.x, _ptMouse.y, Pivot::LeftTop);
	}
	EFFECT->Render();
}

void TownScene::Release()
{
	OBJECTMANAGER->Release();
}

void TownScene::LoadMap()
{
	HANDLE file;
	DWORD read;

	file = CreateFile(L"Townsave.map", GENERIC_READ, 0, NULL, OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL, NULL);

	ReadFile(file, Tile, sizeof(Tile), &read, NULL);
	CloseHandle(file);
}

void TownScene::RenderMap()
{
	
	
}

void TownScene::TerrainRender()
{
	for (int i = 0; i < YNUM; ++i) {
		for (int j = 0; j < XNUM; ++j) {
			if (CAMERA->GetCameraRect().left < Tile[j][i].rc.right &&
				CAMERA->GetCameraRect().right > Tile[j][i].rc.left &&
				CAMERA->GetCameraRect().top < Tile[j][i].rc.bottom &&
				CAMERA->GetCameraRect().bottom > Tile[j][i].rc.top)
			{
				if (Tile[j][i].show == true)
				{

						switch (Tile[j][i].TerrainNum)
						{
						case PageNum::One:
							caveImage[0]->SetSize(Vector2(64, 64) * CAMERA->GetZoom());
							caveImage[0]->FrameRender(
								CAMERA->GetRelativeRect(Tile[j][i].rc).left,
								CAMERA->GetRelativeRect(Tile[j][i].rc).top,
								Tile[j][i].Terrainx, Tile[j][i].Terrainy, Pivot::LeftTop);// , CAMERA->GetZoom());
							break;
						case PageNum::Two:
							caveImage[1]->SetSize(Vector2(64, 64) * CAMERA->GetZoom());
							caveImage[1]->FrameRender(
								CAMERA->GetRelativeRect(Tile[j][i].rc).left,
								CAMERA->GetRelativeRect(Tile[j][i].rc).top,
								Tile[j][i].Terrainx, Tile[j][i].Terrainy, Pivot::LeftTop);// , CAMERA->GetZoom());
							break;
						case PageNum::Three:
							caveImage[2]->SetSize(Vector2(64, 64) * CAMERA->GetZoom());
							caveImage[2]->FrameRender(
								CAMERA->GetRelativeRect(Tile[j][i].rc).left,
								CAMERA->GetRelativeRect(Tile[j][i].rc).top,
								Tile[j][i].Terrainx, Tile[j][i].Terrainy, Pivot::LeftTop);// , CAMERA->GetZoom());
							break;
						case PageNum::Four:
							caveImage[3]->SetSize(Vector2(64, 64) * CAMERA->GetZoom());
							caveImage[3]->FrameRender(
								CAMERA->GetRelativeRect(Tile[j][i].rc).left,
								CAMERA->GetRelativeRect(Tile[j][i].rc).top,
								Tile[j][i].Terrainx, Tile[j][i].Terrainy, Pivot::LeftTop);// , CAMERA->GetZoom());
							break;
						case PageNum::Five:
							caveImage[4]->SetSize(Vector2(64, 64) * CAMERA->GetZoom());
							caveImage[4]->FrameRender(
								CAMERA->GetRelativeRect(Tile[j][i].rc).left,
								CAMERA->GetRelativeRect(Tile[j][i].rc).top,
								Tile[j][i].Terrainx, Tile[j][i].Terrainy, Pivot::LeftTop);// , CAMERA->GetZoom());
							break;
						case PageNum::Six:
							caveImage[5]->SetSize(Vector2(64, 64) * CAMERA->GetZoom());
							caveImage[5]->FrameRender(
								CAMERA->GetRelativeRect(Tile[j][i].rc).left,
								CAMERA->GetRelativeRect(Tile[j][i].rc).top,
								Tile[j][i].Terrainx, Tile[j][i].Terrainy, Pivot::LeftTop);// , CAMERA->GetZoom());
							break;
						case PageNum::Seven:
							caveImage[6]->SetSize(Vector2(64, 64) * CAMERA->GetZoom());
							caveImage[6]->FrameRender(
								CAMERA->GetRelativeRect(Tile[j][i].rc).left,
								CAMERA->GetRelativeRect(Tile[j][i].rc).top,
								Tile[j][i].Terrainx, Tile[j][i].Terrainy, Pivot::LeftTop);// , CAMERA->GetZoom());
							break;
						case PageNum::Eight:
							caveImage[7]->SetSize(Vector2(64, 64) * CAMERA->GetZoom());
							caveImage[7]->FrameRender(
								CAMERA->GetRelativeRect(Tile[j][i].rc).left,
								CAMERA->GetRelativeRect(Tile[j][i].rc).top,
								Tile[j][i].Terrainx, Tile[j][i].Terrainy, Pivot::LeftTop);// , CAMERA->GetZoom());
							break;
						case PageNum::Nine:
							caveImage[8]->SetSize(Vector2(64, 64) * CAMERA->GetZoom());
							caveImage[8]->FrameRender(
								CAMERA->GetRelativeRect(Tile[j][i].rc).left,
								CAMERA->GetRelativeRect(Tile[j][i].rc).top,
								Tile[j][i].Terrainx, Tile[j][i].Terrainy, Pivot::LeftTop);// , CAMERA->GetZoom());
							break;
						}
					
				}
			}
		}
	}
}

void TownScene::Terrain2Render()
{
	for (int i = 0; i < YNUM; ++i) {
		for (int j = 0; j < XNUM; ++j) {
			if (CAMERA->GetCameraRect().left < Tile[j][i].rc.right &&
				CAMERA->GetCameraRect().right > Tile[j][i].rc.left &&
				CAMERA->GetCameraRect().top < Tile[j][i].rc.bottom &&
				CAMERA->GetCameraRect().bottom > Tile[j][i].rc.top)
			{
				if (Tile[j][i].show == true)
				{
					switch (Tile[j][i].Terrain2Num)
					{
					case PageNum::One:
						caveImage[0]->SetSize(Vector2(64, 64) * CAMERA->GetZoom());
						caveImage[0]->FrameRender(
							CAMERA->GetRelativeRect(Tile[j][i].rc).left,
							CAMERA->GetRelativeRect(Tile[j][i].rc).top,
							Tile[j][i].Terrain2x, Tile[j][i].Terrain2y, Pivot::LeftTop);// , CAMERA->GetZoom());
						break;
					case PageNum::Two:
						caveImage[1]->SetSize(Vector2(64, 64) * CAMERA->GetZoom());
						caveImage[1]->FrameRender(
							CAMERA->GetRelativeRect(Tile[j][i].rc).left,
							CAMERA->GetRelativeRect(Tile[j][i].rc).top,
							Tile[j][i].Terrain2x, Tile[j][i].Terrain2y, Pivot::LeftTop);// , CAMERA->GetZoom());
						break;
					case PageNum::Three:
						caveImage[2]->SetSize(Vector2(64, 64) * CAMERA->GetZoom());
						caveImage[2]->FrameRender(
							CAMERA->GetRelativeRect(Tile[j][i].rc).left,
							CAMERA->GetRelativeRect(Tile[j][i].rc).top,
							Tile[j][i].Terrain2x, Tile[j][i].Terrain2y, Pivot::LeftTop);// , CAMERA->GetZoom());
						break;
					case PageNum::Four:
						caveImage[3]->SetSize(Vector2(64, 64) * CAMERA->GetZoom());
						caveImage[3]->FrameRender(
							CAMERA->GetRelativeRect(Tile[j][i].rc).left,
							CAMERA->GetRelativeRect(Tile[j][i].rc).top,
							Tile[j][i].Terrain2x, Tile[j][i].Terrain2y, Pivot::LeftTop);// , CAMERA->GetZoom());
						break;
					case PageNum::Five:
						caveImage[4]->SetSize(Vector2(64, 64) * CAMERA->GetZoom());
						caveImage[4]->FrameRender(
							CAMERA->GetRelativeRect(Tile[j][i].rc).left,
							CAMERA->GetRelativeRect(Tile[j][i].rc).top,
							Tile[j][i].Terrain2x, Tile[j][i].Terrain2y, Pivot::LeftTop);// , CAMERA->GetZoom());
						break;
					case PageNum::Six:
						caveImage[5]->SetSize(Vector2(64, 64) * CAMERA->GetZoom());
						caveImage[5]->FrameRender(
							CAMERA->GetRelativeRect(Tile[j][i].rc).left,
							CAMERA->GetRelativeRect(Tile[j][i].rc).top,
							Tile[j][i].Terrain2x, Tile[j][i].Terrain2y, Pivot::LeftTop);// , CAMERA->GetZoom());
						break;
					case PageNum::Seven:
						caveImage[6]->SetSize(Vector2(64, 64) * CAMERA->GetZoom());
						caveImage[6]->FrameRender(
							CAMERA->GetRelativeRect(Tile[j][i].rc).left,
							CAMERA->GetRelativeRect(Tile[j][i].rc).top,
							Tile[j][i].Terrain2x, Tile[j][i].Terrain2y, Pivot::LeftTop);// , CAMERA->GetZoom());
						break;
					case PageNum::Eight:
						caveImage[7]->SetSize(Vector2(64, 64) * CAMERA->GetZoom());
						caveImage[7]->FrameRender(
							CAMERA->GetRelativeRect(Tile[j][i].rc).left,
							CAMERA->GetRelativeRect(Tile[j][i].rc).top,
							Tile[j][i].Terrain2x, Tile[j][i].Terrain2y, Pivot::LeftTop);// , CAMERA->GetZoom());
						break;
					case PageNum::Nine:
						caveImage[8]->SetSize(Vector2(64, 64) * CAMERA->GetZoom());
						caveImage[8]->FrameRender(
							CAMERA->GetRelativeRect(Tile[j][i].rc).left,
							CAMERA->GetRelativeRect(Tile[j][i].rc).top,
							Tile[j][i].Terrain2x, Tile[j][i].Terrain2y, Pivot::LeftTop);// , CAMERA->GetZoom());
						break;
					}
				}
			}
		}
	}
}

void TownScene::ObjectRender()
{
	for (int i = 0; i < YNUM; ++i) {
		for (int j = 0; j < XNUM; ++j) {
			if (CAMERA->GetCameraRect().left < Tile[j][i].rc.right &&
				CAMERA->GetCameraRect().right > Tile[j][i].rc.left &&
				CAMERA->GetCameraRect().top < Tile[j][i].rc.bottom &&
				CAMERA->GetCameraRect().bottom > Tile[j][i].rc.top)
			{
				if (Tile[j][i].show == true)
				{
					switch (Tile[j][i].ObjectNum)
					{
					case PageNum::One:
						caveImage[0]->SetSize(Vector2(64, 64) * CAMERA->GetZoom());
						caveImage[0]->FrameRender(
							CAMERA->GetRelativeRect(Tile[j][i].rc).left,
							CAMERA->GetRelativeRect(Tile[j][i].rc).top,
							Tile[j][i].Objectx, Tile[j][i].Objecty, Pivot::LeftTop);// , CAMERA->GetZoom());
						break;
					case PageNum::Two:
						caveImage[1]->SetSize(Vector2(64, 64) * CAMERA->GetZoom());
						caveImage[1]->FrameRender(
							CAMERA->GetRelativeRect(Tile[j][i].rc).left,
							CAMERA->GetRelativeRect(Tile[j][i].rc).top,
							Tile[j][i].Objectx, Tile[j][i].Objecty, Pivot::LeftTop);// , CAMERA->GetZoom());
						break;
					case PageNum::Three:
						caveImage[2]->SetSize(Vector2(64, 64) * CAMERA->GetZoom());
						caveImage[2]->FrameRender(
							CAMERA->GetRelativeRect(Tile[j][i].rc).left,
							CAMERA->GetRelativeRect(Tile[j][i].rc).top,
							Tile[j][i].Objectx, Tile[j][i].Objecty, Pivot::LeftTop);// , CAMERA->GetZoom());
						break;
					case PageNum::Four:
						caveImage[3]->SetSize(Vector2(64, 64) * CAMERA->GetZoom());
						caveImage[3]->FrameRender(
							CAMERA->GetRelativeRect(Tile[j][i].rc).left,
							CAMERA->GetRelativeRect(Tile[j][i].rc).top,
							Tile[j][i].Objectx, Tile[j][i].Objecty, Pivot::LeftTop);// , CAMERA->GetZoom());
						break;
					case PageNum::Five:
						caveImage[4]->SetSize(Vector2(64, 64) * CAMERA->GetZoom());
						caveImage[4]->FrameRender(
							CAMERA->GetRelativeRect(Tile[j][i].rc).left,
							CAMERA->GetRelativeRect(Tile[j][i].rc).top,
							Tile[j][i].Objectx, Tile[j][i].Objecty, Pivot::LeftTop);// , CAMERA->GetZoom());
						break;
					case PageNum::Six:
						caveImage[5]->SetSize(Vector2(64, 64) * CAMERA->GetZoom());
						caveImage[5]->FrameRender(
							CAMERA->GetRelativeRect(Tile[j][i].rc).left,
							CAMERA->GetRelativeRect(Tile[j][i].rc).top,
							Tile[j][i].Objectx, Tile[j][i].Objecty, Pivot::LeftTop);// , CAMERA->GetZoom());
						break;
					case PageNum::Seven:
						caveImage[6]->SetSize(Vector2(64, 64) * CAMERA->GetZoom());
						caveImage[6]->FrameRender(
							CAMERA->GetRelativeRect(Tile[j][i].rc).left,
							CAMERA->GetRelativeRect(Tile[j][i].rc).top,
							Tile[j][i].Objectx, Tile[j][i].Objecty, Pivot::LeftTop);// , CAMERA->GetZoom());
						break;
					case PageNum::Eight:
						caveImage[7]->SetSize(Vector2(64, 64) * CAMERA->GetZoom());
						caveImage[7]->FrameRender(
							CAMERA->GetRelativeRect(Tile[j][i].rc).left,
							CAMERA->GetRelativeRect(Tile[j][i].rc).top,
							Tile[j][i].Objectx, Tile[j][i].Objecty, Pivot::LeftTop);// , CAMERA->GetZoom());
						break;
					case PageNum::Nine:
						caveImage[8]->SetSize(Vector2(64, 64) * CAMERA->GetZoom());
						caveImage[8]->FrameRender(
							CAMERA->GetRelativeRect(Tile[j][i].rc).left,
							CAMERA->GetRelativeRect(Tile[j][i].rc).top,
							Tile[j][i].Objectx, Tile[j][i].Objecty, Pivot::LeftTop);// , CAMERA->GetZoom());
						break;
					}
				}
			}
		}
	}
}

TownScene::TownScene()
{
}


TownScene::~TownScene()
{
}
