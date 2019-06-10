#include "stdafx.h"
#include "SlimeScene.h"
#include "Bullet.h"
#include "Player.h"
#include "SlimeBullet.h"
#include "MiniSlime.h"
#include "Slime.h"
#include "DamageFont.h"
#include "Event.h"
#include "tile.h"
#include "AstarHw.h"
#include "Thunder.h"

void SlimeScene::Init()
{
	this->TileLoad();
	SOUNDMANAGER->stop("Title");
	// 96, 96
	//망츙아 망츙아 내 자리 내놔아아아아
	Circle[0] = IMAGEMANAGER->FindImage("원");
	Circle[1] = IMAGEMANAGER->FindImage("원");
	//
	SlimeImage[0] = IMAGEMANAGER->FindImage("큰슬라임");
	SlimeImage[1] = IMAGEMANAGER->FindImage("큰슬라임2");
	SlimeImage[2] = IMAGEMANAGER->FindImage("큰슬라임3");
	SlimeImage[3] = IMAGEMANAGER->FindImage("큰슬라임4");

	player = new Player(Vector2(735, 1930));
	slime = new Slime("보스슬라임", Vector2(64 * 23 /2, -50), Vector2(256.f, 256.f), Pivot::Center);
	OBJECTMANAGER->addObject(ObjectType::Player, player);
	OBJECTMANAGER->addObject(ObjectType::Enemy, slime);
	OBJECTMANAGER->Init();
	CAMERA->SetMapSize(Vector2(64 * 24, 64 * 31));
	CAMERA->SetTarget(player);
	Damage = new DamageFont;
	Astar = new AstarHw;
	Astar->Init(0, 0, player->GetPosition().x / 64, player->GetPosition().y / 64);
	count = 0;
	angle = 0.f;
	alpha = 0.2f;
	Size = Vector2(50, 50);
	Event = false;

	DelayTime = 0.f;
	PathIndex = 0;

	for (int i = 0; i < YNUM; ++i)
	{
		for (int j = 0; j < XNUM; ++j)
		{
			Tile[j][i].rc = RectMake(j * SIZE, i * SIZE, SIZE, SIZE);
		}
	}

	Col[0] = RectMake(0, 256, 64 * 24, 64);								// 위
	Col[1] = RectMake(-40, 256, 64, 64 * 19);							// 왼쪽
	Col[2] = RectMake(64 * 24 - 32, 256, 64, 64 * 19);					// 오른쪽
	Col[3] = RectMake(0, 64 * 23 - 32, 64 * 10 + 32, 64 * 9);			// 아래 왼쪽
	Col[4] = RectMake(64 * 13 - 32, 64 * 23 - 32, 64 * 11 + 32, 64 * 9);// 아래 오른쪽
	Col[5] = RectMake(64 * 9, 64 * 10, 64 * 5, 64 * 6);					// 중앙 가로
	Col[6] = RectMake(64 * 8, 64 * 11, 64 * 7, 64 * 4);					// 중앙 세로				

	SCENEMANAGER->SetAlpha(1.f);
	SCENEMANAGER->SetSceneState(SceneState::FadeIn);

	SOUNDMANAGER->play("보스시작");
	//PathInfo = ASTAR->getPath(0, 0, 6, 0);
}

void SlimeScene::Update()
{
	DelayTime += TIMEMANAGER->getElapedTime();
	OBJECTMANAGER->Update();
	UIMANAGER->Update();
	this->TileCol();
	this->PlayerBulletAttack();
	this->PlayerCloseAttack();
	this->PlayerHit();
	Damage->Update();

	if (DelayTime > 0.65f)
	{
		while (DelayTime > 0.65f)
			DelayTime -= 0.65f;
		if (!EVENT->Eventing())
		{
			if (slime->GetSlimeList()[PathIndex]->GetisActive() &&
				slime->GetSlimeList()[PathIndex]->GetisChasor())
			{
				if (player->GetisActive())
				{
					slime->GetSlimeList()[PathIndex]->SetPath(
						Astar->Update(
							slime->GetSlimeList()[PathIndex]->GetPosition().x / 64,
							slime->GetSlimeList()[PathIndex]->GetPosition().y / 64,
							player->GetPosition().x / 64,
							player->GetPosition().y / 64));
				}
			}
		}
		PathIndex++;
		if (PathIndex > slime->GetSlimeList().size() - 1) PathIndex = 0;
	}
	
	if (player->GetPosition().y < 1450)
	{
		this->ShowEvent();
	}

	if (KEYMANAGER->IsOnceKeyDown(VK_ESCAPE))
	{
		SCENEMANAGER->SetAlpha(0.f);
		SCENEMANAGER->SetSceneState(SceneState::FadeOut);
		SCENEMANAGER->LoadScene("타이틀");
	}
	
	//for (int i = 0; i < PathInfo.size(); ++i)
	//{
	//	cout << PathInfo[i]->getIdX() << ", " << PathInfo[i]->getIdY() << endl;
	//}
}

void SlimeScene::Render()
{
	//for (int i = 0; i < YNUM; ++i)
	//{
	//	for (int j = 0; j < XNUM; ++j)
	//	{
	//		D2DRENDERER->DrawRectangle(CAMERA->GetRelativeRect(Tile[j][i].rc), D2DRenderer::DefaultBrush::Red);
	//	}
	//}
	this->TileTerrain();
	EFFECT->Render2();
	OBJECTMANAGER->Render();
	this->TileObject();

	if (_isDebug)
	{
		for (int i = 0; i < 7; ++i)
		{
			D2DRENDERER->DrawRectangle(CAMERA->GetRelativeRect(Col[i]), D2DRenderer::DefaultBrush::Red);
		}
	}
	Damage->Render();
	EFFECT->Render();

	if (!EVENT->Eventing()) UIMANAGER->Render();
	UIMANAGER->Render2();
	

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
}

void SlimeScene::Release()
{
	OBJECTMANAGER->Release();
	UIMANAGER->Release();
}

void SlimeScene::TileLoad()
{
	HANDLE file;
	DWORD read;

	file = CreateFile(L"Boss1save.map", GENERIC_READ, 0, NULL, OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL, NULL);

	ReadFile(file, Tile, sizeof(Tile), &read, NULL);
	CloseHandle(file);


	//for (int i = 0; i < 100; ++i)
	//{
	//	for (int j = 0; j < 100; ++j)
	//	{
	//		if (Tile[j][i].Objectx >= 0 || Tile[j][i].Terrain2x >= 0)
	//		{
	//			_Tile[j][i] = new tile;
	//			_Tile[j][i]->Init(j,i);
	//			_Tile[j][i]->setAttribute("Wall");
	//		}
	//	}
	//}
}

void SlimeScene::TileTerrain()
{
	RECT temp;
	for (int i = 0; i < YNUM; ++i) {
		for (int j = 0; j < XNUM; ++j) {
			if (IntersectRect(&temp, &Tile[j][i].rc, &CAMERA->GetCameraRect()))
			{
				if (Tile[j][i].show == true)
				{
					// ======================================================================
					// ###################### 슬라임 보스 테마 ###############################
					// ======================================================================
					switch (Tile[j][i].TerrainNum)
					{
					//case PageNum::Zero:
					//	SlimeImage[0]->SetSize(Vector2(64, 64) * CAMERA->GetZoom());
					//	SlimeImage[0]->FrameRender(
					//		CAMERA->GetRelativeRect(Tile[j][i].rc).left,
					//		CAMERA->GetRelativeRect(Tile[j][i].rc).top,
					//		Tile[j][i].Terrainx, Tile[j][i].Terrainy, Pivot::LeftTop);// , CAMERA->GetZoom());
					//	break;

					case PageNum::One:
						SlimeImage[0]->SetSize(Vector2(64, 64) * CAMERA->GetZoom());
						SlimeImage[0]->FrameRender(
							CAMERA->GetRelativeRect(Tile[j][i].rc).left,
							CAMERA->GetRelativeRect(Tile[j][i].rc).top,
							Tile[j][i].Terrainx, Tile[j][i].Terrainy, Pivot::LeftTop);// , CAMERA->GetZoom());
						break;
					case PageNum::Two:
						SlimeImage[1]->SetSize(Vector2(64, 64) * CAMERA->GetZoom());
						SlimeImage[1]->FrameRender(
							CAMERA->GetRelativeRect(Tile[j][i].rc).left,
							CAMERA->GetRelativeRect(Tile[j][i].rc).top,
							Tile[j][i].Terrainx, Tile[j][i].Terrainy, Pivot::LeftTop);// , CAMERA->GetZoom());
						break;
					case PageNum::Three:
						SlimeImage[2]->SetSize(Vector2(64, 64) * CAMERA->GetZoom());
						SlimeImage[2]->FrameRender(
							CAMERA->GetRelativeRect(Tile[j][i].rc).left,
							CAMERA->GetRelativeRect(Tile[j][i].rc).top,
							Tile[j][i].Terrainx, Tile[j][i].Terrainy, Pivot::LeftTop);// , CAMERA->GetZoom());
						break;
					case PageNum::Four:
						SlimeImage[3]->SetSize(Vector2(64, 64) * CAMERA->GetZoom());
						SlimeImage[3]->FrameRender(
							CAMERA->GetRelativeRect(Tile[j][i].rc).left,
							CAMERA->GetRelativeRect(Tile[j][i].rc).top,
							Tile[j][i].Terrainx, Tile[j][i].Terrainy, Pivot::LeftTop);// , CAMERA->GetZoom());
						break;
					}

					switch (Tile[j][i].Terrain2Num)
					{
					case PageNum::One:
						SlimeImage[0]->SetSize(Vector2(64, 64) * CAMERA->GetZoom());
						SlimeImage[0]->FrameRender(
							CAMERA->GetRelativeRect(Tile[j][i].rc).left,
							CAMERA->GetRelativeRect(Tile[j][i].rc).top,
							Tile[j][i].Terrain2x, Tile[j][i].Terrain2y, Pivot::LeftTop);// , CAMERA->GetZoom());
						break;
					case PageNum::Two:
						SlimeImage[1]->SetSize(Vector2(64, 64) * CAMERA->GetZoom());
						SlimeImage[1]->FrameRender(
							CAMERA->GetRelativeRect(Tile[j][i].rc).left,
							CAMERA->GetRelativeRect(Tile[j][i].rc).top,
							Tile[j][i].Terrain2x, Tile[j][i].Terrain2y, Pivot::LeftTop);// , CAMERA->GetZoom());
						break;
					case PageNum::Three:
						SlimeImage[2]->SetSize(Vector2(64, 64) * CAMERA->GetZoom());
						SlimeImage[2]->FrameRender(
							CAMERA->GetRelativeRect(Tile[j][i].rc).left,
							CAMERA->GetRelativeRect(Tile[j][i].rc).top,
							Tile[j][i].Terrain2x, Tile[j][i].Terrain2y, Pivot::LeftTop);// , CAMERA->GetZoom());
						break;
					case PageNum::Four:
						SlimeImage[3]->SetSize(Vector2(64, 64) * CAMERA->GetZoom());
						SlimeImage[3]->FrameRender(
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

void SlimeScene::TileObject()
{
	RECT temp;
	for (int i = 0; i < YNUM; ++i) 
	{
		for (int j = 0; j < XNUM; ++j) 
		{
			if (IntersectRect(&temp, &Tile[j][i].rc, &CAMERA->GetCameraRect()))
			{
				if (Tile[j][i].show == true)
				{
					// ======================================================================
					// ###################### 슬라임 보스 테마 ###############################
					// ======================================================================

					switch (Tile[j][i].ObjectNum)
					{
					case PageNum::One:
						SlimeImage[0]->SetSize(Vector2(64, 64) * CAMERA->GetZoom());
						SlimeImage[0]->FrameRender(
							CAMERA->GetRelativeRect(Tile[j][i].rc).left,
							CAMERA->GetRelativeRect(Tile[j][i].rc).top,
							Tile[j][i].Objectx, Tile[j][i].Objecty, Pivot::LeftTop);// , CAMERA->GetZoom());
						break;
					case PageNum::Two:
						SlimeImage[1]->SetSize(Vector2(64, 64) * CAMERA->GetZoom());
						SlimeImage[1]->FrameRender(
							CAMERA->GetRelativeRect(Tile[j][i].rc).left,
							CAMERA->GetRelativeRect(Tile[j][i].rc).top,
							Tile[j][i].Objectx, Tile[j][i].Objecty, Pivot::LeftTop);// , CAMERA->GetZoom());
						break;
					case PageNum::Three:
						SlimeImage[2]->SetSize(Vector2(64, 64) * CAMERA->GetZoom());
						SlimeImage[2]->FrameRender(
							CAMERA->GetRelativeRect(Tile[j][i].rc).left,
							CAMERA->GetRelativeRect(Tile[j][i].rc).top,
							Tile[j][i].Objectx, Tile[j][i].Objecty, Pivot::LeftTop);// , CAMERA->GetZoom());
						break;
					case PageNum::Four:
						SlimeImage[3]->SetSize(Vector2(64, 64) * CAMERA->GetZoom());
						SlimeImage[3]->FrameRender(
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

void SlimeScene::TileCol()
{
	RECT temp;
	for (int i = 0; i < 7; ++i)
	{
		if (IntersectRect(&temp, &Col[i], &player->GetColRect()))
		{
			float Width = temp.right - temp.left;
			float Height = temp.bottom - temp.top;

			// ===============================================
			// ############ RECT 충돌 위 / 아래 ###############
			// ===============================================
			if (Width > Height)
			{
				if (player->GetColRect().top == temp.top)
				{
					player->SetPosition(Vector2(player->GetPosition().x,
						player->GetPosition().y + Height));
				}
				else if (player->GetColRect().bottom == temp.bottom)
				{
					player->SetPosition(Vector2(player->GetPosition().x,
						player->GetPosition().y - Height));
				}
			}
			else
			{
				if (player->GetColRect().left == temp.left)
				{
					player->SetPosition(Vector2(player->GetPosition().x + Width,
						player->GetPosition().y));
				}
				else if (player->GetColRect().right == temp.right)
				{
					player->SetPosition(Vector2(player->GetPosition().x - Width,
						player->GetPosition().y));
				}
			}
		}

		for (int j = 0; j< slime->GetSlimeList().size(); ++j)
		{
			if (!slime->GetSlimeList()[j]->GetisCol()) continue;
			if (!slime->GetSlimeList()[j]->GetisActive()) continue;
			if (IntersectRect(&temp, &Col[i], &slime->GetSlimeList()[j]->GetColRect()))
			{
				float Width = temp.right - temp.left;
				float Height = temp.bottom - temp.top;

				// ===============================================
				// ############ RECT 충돌 위 / 아래 ###############
				// ===============================================
				if (Width > Height)
				{
					if (slime->GetSlimeList()[j]->GetColRect().top == temp.top)
					{
						slime->GetSlimeList()[j]->SetPosition(Vector2(
							slime->GetSlimeList()[j]->GetPosition().x,
							slime->GetSlimeList()[j]->GetPosition().y + Height));
						slime->GetSlimeList()[j]->SetJumpPos(slime->GetSlimeList()[j]->GetPosition());
					}
					else if (slime->GetSlimeList()[j]->GetColRect().bottom == temp.bottom)
					{
						slime->GetSlimeList()[j]->SetPosition(Vector2(
							slime->GetSlimeList()[j]->GetPosition().x,
							slime->GetSlimeList()[j]->GetPosition().y - Height));
						slime->GetSlimeList()[j]->SetJumpPos(slime->GetSlimeList()[j]->GetPosition());
					}
				}
				else
				{
					if (slime->GetSlimeList()[j]->GetColRect().left == temp.left)
					{
						slime->GetSlimeList()[j]->SetPosition(Vector2(
							slime->GetSlimeList()[j]->GetPosition().x + Width,
							slime->GetSlimeList()[j]->GetPosition().y));
						slime->GetSlimeList()[j]->SetJumpPos(slime->GetSlimeList()[j]->GetPosition());
					}
					else if (slime->GetSlimeList()[j]->GetColRect().right == temp.right)
					{
						slime->GetSlimeList()[j]->SetPosition(Vector2(
							slime->GetSlimeList()[j]->GetPosition().x - Width,
							slime->GetSlimeList()[j]->GetPosition().y));
						slime->GetSlimeList()[j]->SetJumpPos(slime->GetSlimeList()[j]->GetPosition());
					}
				}
			}
		}
	}

	for (int i = 0; i < 5; ++i)
	{
		if (IntersectRect(&temp, &Col[i], &slime->GetColRect()) && EVENT->Eventing() != true)
		{
			if (slime->GetisCol()) break;
			if (slime->GetisJump()) break;
			if(!slime->GetisDash()) slime->CrushWall();
			else
			{
				if (i == 0 && !slime->GetPong())
				{
					SOUNDMANAGER->play("탱");
					slime->SetDirVector(Vector2(RND->Range(210, 330)));
					slime->SetPong(true);
					CAMERA->Shake(10.f, 0.7f);
				}
				else if (i == 1 && !slime->GetPong())
				{
					SOUNDMANAGER->play("탱");
					slime->SetDirVector(Vector2(RND->Range(-60, 60)));
					slime->SetPong(true);
					CAMERA->Shake(10.f, 0.7f);
				}
				else if (i == 2 && !slime->GetPong())
				{
					SOUNDMANAGER->play("탱");
					slime->SetDirVector(Vector2(RND->Range(120, 240)));
					slime->SetPong(true);
					CAMERA->Shake(10.f, 0.7f);
				}
				else if (i == 3 && !slime->GetPong())
				{
					SOUNDMANAGER->play("탱");
					slime->SetDirVector(Vector2(RND->Range(30, 150)));
					slime->SetPong(true);
					CAMERA->Shake(10.f, 0.7f);
				}
				else if (i == 4 && !slime->GetPong())
				{
					SOUNDMANAGER->play("탱");
					slime->SetDirVector(Vector2(RND->Range(30, 150)));
					slime->SetPong(true);
					CAMERA->Shake(10.f, 0.7f);
				}	
			}
			CAMERA->Shake(10.f, 0.9f);
			for (int i = 0; i < 8; ++i)
			{
				slime->GetEffectBullet()->Fire("작은웨이브",
					(Math::PI / 4.f) * (float)i, slime->GetJPos(), Vector2(128.f, 128.f), Vector2(100.f, 100.f), 1.3f, 150.f);
			}
			break;
		}
	}
}

void SlimeScene::PlayerBulletAttack()
{
	if (player->GetPlayerBullet()->GetBulletList().size() == 0) return;
	RECT temp;
	for (int i = 0; i < player->GetPlayerBullet()->GetBulletList().size(); ++i)
	{
		for (int j = 0; j < slime->GetSlimeList().size(); ++j)
		{
			if (!slime->GetSlimeList()[j]->GetisActive()) continue;
			if (!slime->GetSlimeList()[j]->GetisCrush()) continue;
			if (!player->GetPlayerBullet()->GetisFire(i)) continue;
			if (IntersectRect(&temp,
				&slime->GetSlimeList()[j]->GetHitRect(),		// 작은 슬라임
				&player->GetPlayerBullet()->GetBulletRect(i)))	// 플레이어 총알
			{
				float damage = RND->Range(15, 30);
				Damage->Show(Vector2(
					slime->GetSlimeList()[j]->GetHitRect().left,
					(int)(slime->GetSlimeList()[j]->GetPosition().y - slime->GetSlimeList()[j]->GetSize().y / 1.2f)), damage, 55.f);
				slime->GetSlimeList()[j]->Hit(damage);
				slime->Hit(damage);
				EFFECT->ShowEffect("맞음", Vector2(player->GetPlayerBullet()->GetBulletList()[i].bulletpos.x
					, player->GetPlayerBullet()->GetBulletList()[i].bulletpos.y), Vector2(64, 64), true, 0, 0.1f);
				player->GetPlayerBullet()->SetisFire(i, false);
			}
		}
		if (!player->GetPlayerBullet()->GetisFire(i)) continue;
		if (IntersectRect(&temp,
			&slime->GetHitRect(),								// 큰 슬라임		
			&player->GetPlayerBullet()->GetBulletRect(i))		// 플레이어 총알
			&& slime->GetisShow())		
		{
			float damage = RND->Range(15, 30);
			Damage->Show(Vector2(slime->GetHitRect().left + (int)(slime->GetSlimeSize() / 2)
				, (int)(slime->GetPosition().y - slime->GetSlimeSize() / 2.f)), damage, 65.f, true);
			slime->BreakHit();
			EFFECT->ShowEffect("맞음", Vector2(player->GetPlayerBullet()->GetBulletList()[i].bulletpos.x
				, player->GetPlayerBullet()->GetBulletList()[i].bulletpos.y), Vector2(64, 64), true, 0, 0.1f);
			player->GetPlayerBullet()->SetisFire(i, false);
		}

		for (int k = 0; k < 5; ++k)
		{
			if (IntersectRect(&temp,
				&player->GetPlayerBullet()->GetBulletRect(i),	// 플레이어 총알
				&Col[k]))										// 벽
			{
				EFFECT->ShowEffect("맞음", Vector2(player->GetPlayerBullet()->GetBulletList()[i].bulletpos.x
					, player->GetPlayerBullet()->GetBulletList()[i].bulletpos.y), Vector2(64, 64), true, 0, 0.1f);
				player->GetPlayerBullet()->SetisFire(i, false);
			}
		}
		
	}
}

void SlimeScene::PlayerCloseAttack()
{
	if (!player->GetisAttack()) return;
	RECT temp;
	for (int j = 0; j < slime->GetSlimeList().size(); ++j)
	{
		if (!slime->GetSlimeList()[j]->GetisActive()) continue;
		if (!slime->GetSlimeList()[j]->GetisCrush()) continue;
			
		if (IntersectRect(&temp,
			&slime->GetSlimeList()[j]->GetHitRect(),		// 작은 슬라임
			&player->GetAttackRect()))						// 플레이어 공격 RECT
		{
			float damage = RND->Range(15, 30);
			Damage->Show(Vector2(
				slime->GetSlimeList()[j]->GetHitRect().left,
				(int)(slime->GetSlimeList()[j]->GetPosition().y - slime->GetSlimeList()[j]->GetSize().y / 1.2f)), damage);

			EFFECT->ShowEffect("맞음", Vector2(slime->GetSlimeList()[j]->GetPosition().x, slime->GetSlimeList()[j]->GetPosition().y),
				Vector2(64, 64), true, 0, 0.1f);
			slime->GetSlimeList()[j]->Hit(damage);
			slime->Hit(damage);
			break;
		}
	}

	if (IntersectRect(&temp,
		&slime->GetHitRect(),													// 큰 슬라임		
		&player->GetAttackRect()) && slime->GetisShow() && !slime->GetisHit())	// 플레이어 공격 RECT
	{
		float damage = RND->Range(15, 30);
		Damage->Show(Vector2(slime->GetHitRect().left + (int)(slime->GetSlimeSize() / 2)
			, (int)(slime->GetPosition().y - slime->GetSlimeSize()/2.f)), damage, true);

		slime->BreakHit();

		float Height = temp.bottom - temp.top;
		float Width = temp.right - temp.left;

		if (Height > Width)
		{
			if (temp.right == player->GetAttackRect().right)
			{
				EFFECT->ShowEffect("맞음", Vector2((float)player->GetAttackRect().right,
					player->GetAttackRect().top + (Height / 2.f)), Vector2(64, 64), true, 0, 0.1f);
			}
			else if (temp.left == player->GetAttackRect().left)
			{
				EFFECT->ShowEffect("맞음", Vector2((float)player->GetAttackRect().left,
					player->GetAttackRect().top + (Height / 2.f)), Vector2(64, 64), true, 0, 0.1f);
			}
		}
		else
		{
			if (temp.bottom == player->GetAttackRect().bottom)
			{
				EFFECT->ShowEffect("맞음", Vector2(player->GetAttackRect().left + (Width / 2.f),
					(float)player->GetAttackRect().bottom), Vector2(64, 64), true, 0, 0.1f);
			}
			else if (temp.top == player->GetAttackRect().top)
			{
				EFFECT->ShowEffect("맞음", Vector2(player->GetAttackRect().left + (Width / 2.f),
					(float)player->GetAttackRect().top), Vector2(64, 64), true, 0, 0.1f);
			}
		}
	}
}

void SlimeScene::PlayerHit()
{
	if (player->GetisAlphaStart()) return;
	if (player->GetisHit()) return;
	if (slime->GetisDash()) return;

	RECT temp;
	// 작은 슬라임 RECT
	for (int i = 0; i < slime->GetSlimeList().size(); ++i)
	{
		if (!slime->GetSlimeList()[i]->GetisCrush()) continue;
		if (!slime->GetSlimeList()[i]->GetisActive()) continue;
		if (slime->GetSlimeList()[i]->GetisDying()) continue;
		if (IntersectRect(&temp, &player->GetHitRect(), &slime->GetSlimeList()[i]->GetAttackRect()))
		{
			float damage = RND->Range(5, 10);
			player->Hit(damage);
			Damage->Show(player->GetPosition(), damage, 55.f, false, RGB(0, 0, 255));
			break;
		}
	}

	// 보스 ( 슬라임 ), 충격 파 RECT
	for (int i = 0; i < slime->GetEffectBullet()->GetBulletList().size(); ++i)
	{
		if (IntersectRect(&temp, &player->GetHitRect(), &slime->GetEffectBullet()->GetBulletRect(i)))
		{
			float damage = RND->Range(20, 350);
			player->Hit(damage);
			Damage->Show(player->GetPosition(), damage, 55.f, false, RGB(0, 0, 255));
			break;
		}
	}

	// 보스 ( 슬라임 ), 발사 총알 RECT
	for (int j = 0; j < slime->GetShotBullet()->GetEBullet()->GetBulletList().size(); ++j)
	{
		if (IntersectRect(&temp, &player->GetHitRect(), &slime->GetShotBullet()->GetEBullet()->GetBulletRect(j)))
		{
			float damage = RND->Range(20, 350);
			player->Hit(damage);
			Damage->Show(player->GetPosition(), damage, 55.f, false, RGB(0, 0, 255));
			break;
		}
	}

	if (IntersectRect(&temp, &player->GetHitRect(), &slime->GetAttackRect()))
	{
		if (slime->GetisShow() && slime->GetisCrush())
		{
			float damage = RND->Range(20, 350);
			player->Hit(damage);
			Damage->Show(player->GetPosition(), damage, 55.f, false, RGB(0, 0, 255));
		}
	}

	if (!slime->GetThunder()->GetAttack()) return;
	for (int i = 0; i < slime->GetThunder()->GetThunderList().size(); ++i)
	{
		RECT temp;
		if (IntersectRect(&temp, &slime->GetThunder()->GetRect(i), &player->GetHitRect()))
		{
			float damage = RND->Range(20, 350);
			player->Hit(damage);
			Damage->Show(player->GetPosition(), damage, 55.f, false, RGB(0, 0, 255));
			slime->GetThunder()->FireOff();
			break;
		}
	}
}

void SlimeScene::ShowEvent()
{
	if (!Event)
	{
		EVENT->AddEvent(new Start);
		EVENT->AddEvent(new BossMove(slime));
		EVENT->AddEvent(new Delay(1.f));
		EVENT->AddEvent(new BossGather(slime));
		EVENT->AddEvent(new CameraZoom);
		EVENT->AddEvent(new Delay(1.5f));
		EVENT->AddEvent(new BossBar);
		EVENT->AddEvent(new CameraMove(player));
		EVENT->AddEvent(new End);
		this->Event = true;
	}
}

