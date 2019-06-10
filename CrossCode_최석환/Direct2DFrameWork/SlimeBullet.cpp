#include "stdafx.h"
#include "SlimeBullet.h"
#include "Bullet.h"


void SlimeBullet::Fire(Vector2 pos, Vector2 dirvector, float speed)
{
	BulletImage = IMAGEMANAGER->FindImage("큰웨이브");
	Pos = pos;
	DirVector = dirvector;
	Speed = speed;
	isActive = true;
	isFire = true;
	ActiveTime = 0;
	FrameIndex = 0;
	FrameCount = 0.f;
}

void SlimeBullet::Move()
{
	ActiveTime += TIMEMANAGER->getElapedTime();
	if (ActiveTime >= 1.f)
	{
		isActive = false;
	}
	else {
		Pos += DirVector * Speed * TIMEMANAGER->getElapedTime();
	}
}

void SlimeBullet::Update()
{
	if (isFire)
	{
		FrameCount += TIMEMANAGER->getElapedTime();
		if (FrameCount > 0.05f)
		{
			FrameIndex++;
			while (FrameCount > 0.05f)
				FrameCount -= 0.05f;
			if (isActive)
			{
				if (FrameIndex > 5)
				{
					FrameIndex = 0;
				}
			}
			if (FrameIndex == 11)
			{
				CAMERA->Shake(15.5f, 0.9f);
				SOUNDMANAGER->play("에너지폭파");
				for (int i = 0; i < 32; ++i)
				{
					EBullet->Fire("작은웨이브", (Math::PI / 8.f) * (float)i, Pos, Vector2(64.f, 64.f), Vector2(48.f,48.f), 1.2f, 300.f);
				}
			}
		}
		this->Move();
	}
	EBullet->Update();
}

void SlimeBullet::Render()
{
	if (isFire)
	{
		Vector2 TempPos = CAMERA->GetRelativeVector2(Pos);
		BulletImage->SetSize(Vector2(64.f, 64.f));
		BulletImage->FrameRender(TempPos.x, TempPos.y, FrameIndex, 0);
		if (FrameIndex > 11)
			isFire = false;
	}
	EBullet->Render();
}

SlimeBullet::SlimeBullet()
{
	EBullet = new Bullet;
	isActive = false;
	isFire = false;
}

SlimeBullet::~SlimeBullet()
{
}
