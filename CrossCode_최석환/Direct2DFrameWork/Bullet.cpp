#include "stdafx.h"
#include "Bullet.h"


void Bullet::Fire(string key, float angle, Vector2 pos, Vector2 size, Vector2 csize, float life, float speed, bool power, bool player)
{
	TagBullet bullet;									// 임시 총알 
	bullet.angle = angle;								// 총알의 각도 설정 한다.
	bullet.isFire = true;								// 총알을 발사 한다. true
	bullet.speed = speed;								// 총알의 속도 설정 한다.
	bullet.bulletpos = bullet.bulletfir = pos;			// 총알의 좌표 x,y
	bullet.range = 2000.f;								// 총알의 사정거리
	bullet.bulletsize = size;							// 총알의 크기
	bullet.bulletRect = RectMakeCenter(					// 총알의 RECT 
		bullet.bulletpos.x, bullet.bulletpos.y,
		bullet.bulletsize.x, bullet.bulletsize.y);		
	bullet.lifetime = life;								// 총알 지속시간
	bullet.bulletImage = IMAGEMANAGER->FindImage(key);	// 총알 Image
	bullet.FrameIndex = 0;								// 총알 프레임 인덱스 초기화
	bullet.FrameCount = 0.f;							// 총알 프레임 카운트 초기화
	bullet.Power = power;								// 기모은 총알 !
	bullet.player = player;
	bullet.EffectDelay = 0.f;							

	for (int i = 0; i < 2; ++i)
	{
		bullet.Circle[i].mImage = IMAGEMANAGER->FindImage("슬라임원");
		bullet.Circle[i].Alpha = 0.3f;
		bullet.Circle[i].Size = csize;
	}
	BulletList.push_back(bullet);						// 세팅된 총알을 vetor에 push_back
}

void Bullet::BulletMove()
{
	if (BulletList.size() == 0) return;
	for (int i = 0; i < BulletList.size(); ++i)
	{
		if (BulletList[i].isFire == false) continue;				// 총알의 isFire가 ture일 때 
		BulletList[i].bulletpos.x += cosf(BulletList[i].angle) *	// 총알의 각도 방향으로 총알을 이동
			BulletList[i].speed * TIMEMANAGER->getElapedTime();
		BulletList[i].bulletpos.y -= sinf(BulletList[i].angle) *
			BulletList[i].speed * TIMEMANAGER->getElapedTime();
		BulletList[i].bulletRect = RectMakeCenter(					// 총알이 이동한 좌표만큼 RECT 다시 그려준다.
			BulletList[i].bulletpos.x, BulletList[i].bulletpos.y,
			BulletList[i].bulletsize.x, BulletList[i].bulletsize.y);
		BulletList[i].FrameCount += TIMEMANAGER->getElapedTime();	// 총알 프레임 인덱스 증가를 위한 DeltaTime 증가
		BulletList[i].lifetime -= TIMEMANAGER->getElapedTime();		// 총알 지속시간 감소

		if (BulletList[i].FrameCount > 0.1f)						// FrameCount가 0.1f 보다 증가하면
		{
			BulletList[i].FrameIndex++;													// FrameIndex 증가
			if (BulletList[i].FrameIndex > BulletList[i].bulletImage->GetMaxFrameX())	// FrameIndex가 3보다 커지면
			{			
				BulletList[i].FrameIndex = 0;											// FrameIndex 다시 0으로 초기화
			}
			// FrameCount 0으로 다시 초기화 해준다.
			// 만약에 프레임 드랍이 발생하여 DeltaTime이 아주 많이 FrameCount에 가중이 된다면
			// ( 프레임 드랍이 발생했을 때 DeltaTime은 계속 진행, 코드는 읽히지 않음... ? )
			// 아주 많이 증가하였다면 ex) FrameCount 값이 0.29 ~ 이런식으로 아주 많이 증가
			// while 문을 돌면서 0.1 보다 큰경우 계속 보정해주는 것이
			// 이상적으로 0으로 다시 초기화가 된다.
			while (BulletList[i].FrameCount > 0.1f)					
				BulletList[i].FrameCount -= 0.1f;					
																	
		}
		// 총알이 사정거리를 벗어나거나
		// 총알의 지속시간이 만료되면
		// 총알을 삭제해버린다.
		if (Math::GetDistance(BulletList[i].bulletfir.x, BulletList[i].bulletfir.y,
			BulletList[i].bulletpos.x, BulletList[i].bulletpos.y) > BulletList[i].range || BulletList[i].lifetime <= 0.f)
		{
			if (BulletList[i].player)
			{
				if (BulletList[i].Power)
				{
					SOUNDMANAGER->play("원거리공격차지터짐");
				}
				else
				{
					SOUNDMANAGER->play("원거리공격터짐");
				}
			}
			else
			{
				//SOUNDMANAGER->play("터짐2");
			}
			BulletList.erase(BulletList.begin() + i);
			--i;
		}
	}
}

void Bullet::Update()
{
	this->BulletMove();
	this->UpdateCircle();
	for (int i = 0; i < BulletList.size(); ++i)
	{
		if (BulletList[i].isFire == false)
		{
			if (BulletList[i].player)
			{
				if (BulletList[i].Power)
				{
					SOUNDMANAGER->play("원거리공격차지터짐");
				}
				else
				{
					SOUNDMANAGER->play("원거리공격터짐");
				}
			}
			else
			{
				//SOUNDMANAGER->play("터짐2");
			}
			BulletList.erase(BulletList.begin() + i--);
		}
		
	}
}

void Bullet::Render()
{
	if (BulletList.size() == 0) return;
	for (int i = 0; i < BulletList.size(); ++i)
	{
		Vector2 TempPos = CAMERA->GetRelativeVector2(BulletList[i].bulletpos);
		if (BulletList[i].isFire == false) continue;
		//D2DRENDERER->DrawRectangle(BulletList[i].bulletRect,
		//	D2DRenderer::DefaultBrush::Yellow);
		float Degree = (BulletList[i].angle) * 180.f / Math::PI;
		if (BulletList[i].Power)
		{
			BulletList[i].bulletImage->SetAngle(-Degree);
			BulletList[i].bulletImage->SetSize(BulletList[i].bulletsize);
			BulletList[i].bulletImage->FrameRender(TempPos.x, TempPos.y,
				BulletList[i].FrameIndex, 0, Pivot::Center);
		}
		else {
			BulletList[i].bulletImage->SetAngle(-Degree);
			BulletList[i].bulletImage->SetSize(BulletList[i].bulletsize);
			BulletList[i].bulletImage->FrameRender(TempPos.x, TempPos.y,
				BulletList[i].FrameIndex, 0, Pivot::Center);
		}
	}
	this->RenderCircle();
}

void Bullet::RenderCircle()
{
	for (int i = 0; i < BulletList.size(); ++i)
	{
		Vector2 TempPos = CAMERA->GetRelativeVector2(BulletList[i].bulletpos);
		if (BulletList[i].EffectDelay > 50)
		{
			for (int j = 0; j < 2; ++j)
			{
				BulletList[i].Circle[j].mImage->SetAlpha(BulletList[i].Circle[j].Alpha);
				BulletList[i].Circle[j].mImage->SetSize(BulletList[i].Circle[j].Size);
				BulletList[i].Circle[j].mImage->FrameRender((int)TempPos.x, (int)TempPos.y, 0, 0);
			}
		}
		else
		{
			BulletList[i].Circle[0].mImage->SetAlpha(BulletList[i].Circle[0].Alpha);
			BulletList[i].Circle[0].mImage->SetSize(BulletList[i].Circle[0].Size);
			BulletList[i].Circle[0].mImage->FrameRender((int)TempPos.x, (int)TempPos.y, 0, 0);
		}
	}
}

void Bullet::UpdateCircle()
{
	for (int i = 0; i < BulletList.size(); ++i)
	{
		if (BulletList[i].isFire == false) continue;
		BulletList[i].EffectDelay += TIMEMANAGER->getElapedTime();
		if (BulletList[i].EffectDelay > 50)
		{
			for (int j = 0; j < 2; ++j)
			{
				BulletList[i].Circle[j].Size.x = BulletList[i].Circle[j].Size.x + 5.f;
				BulletList[i].Circle[j].Size.y = BulletList[i].Circle[j].Size.y + 5.f;
				BulletList[i].Circle[j].Alpha -= 0.008f;
				if (BulletList[i].Circle[j].Alpha <= 0.f)
				{
					BulletList[i].Circle[j].Alpha = 0.2f;
					BulletList[i].Circle[j].Size = Vector2(100.f, 100.f);
				}
			}
		}
		else
		{
			BulletList[i].Circle[0].Size.x = BulletList[i].Circle[0].Size.x + 5.f;
			BulletList[i].Circle[0].Size.y = BulletList[i].Circle[0].Size.y + 5.f;
			BulletList[i].Circle[0].Alpha -= 0.008f;
			if (BulletList[i].Circle[0].Alpha <= 0.f)
			{
				BulletList[i].Circle[0].Alpha = 0.2f;
				BulletList[i].Circle[0].Size = Vector2(100.f, 100.f);
			}
		}
	}
}

Bullet::Bullet()
{
	BulletList = vector<TagBullet>();
}

void Bullet::Release()
{
	for (int i = 0; i < BulletList.size(); ++i)
	{
		BulletList.erase(BulletList.begin() + i);
		--i;
	}
	BulletList.clear();
}
