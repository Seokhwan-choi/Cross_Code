#include "stdafx.h"
#include "Bullet.h"


void Bullet::Fire(string key, float angle, Vector2 pos, Vector2 size, Vector2 csize, float life, float speed, bool power, bool player)
{
	TagBullet bullet;									// �ӽ� �Ѿ� 
	bullet.angle = angle;								// �Ѿ��� ���� ���� �Ѵ�.
	bullet.isFire = true;								// �Ѿ��� �߻� �Ѵ�. true
	bullet.speed = speed;								// �Ѿ��� �ӵ� ���� �Ѵ�.
	bullet.bulletpos = bullet.bulletfir = pos;			// �Ѿ��� ��ǥ x,y
	bullet.range = 2000.f;								// �Ѿ��� �����Ÿ�
	bullet.bulletsize = size;							// �Ѿ��� ũ��
	bullet.bulletRect = RectMakeCenter(					// �Ѿ��� RECT 
		bullet.bulletpos.x, bullet.bulletpos.y,
		bullet.bulletsize.x, bullet.bulletsize.y);		
	bullet.lifetime = life;								// �Ѿ� ���ӽð�
	bullet.bulletImage = IMAGEMANAGER->FindImage(key);	// �Ѿ� Image
	bullet.FrameIndex = 0;								// �Ѿ� ������ �ε��� �ʱ�ȭ
	bullet.FrameCount = 0.f;							// �Ѿ� ������ ī��Ʈ �ʱ�ȭ
	bullet.Power = power;								// ����� �Ѿ� !
	bullet.player = player;
	bullet.EffectDelay = 0.f;							

	for (int i = 0; i < 2; ++i)
	{
		bullet.Circle[i].mImage = IMAGEMANAGER->FindImage("�����ӿ�");
		bullet.Circle[i].Alpha = 0.3f;
		bullet.Circle[i].Size = csize;
	}
	BulletList.push_back(bullet);						// ���õ� �Ѿ��� vetor�� push_back
}

void Bullet::BulletMove()
{
	if (BulletList.size() == 0) return;
	for (int i = 0; i < BulletList.size(); ++i)
	{
		if (BulletList[i].isFire == false) continue;				// �Ѿ��� isFire�� ture�� �� 
		BulletList[i].bulletpos.x += cosf(BulletList[i].angle) *	// �Ѿ��� ���� �������� �Ѿ��� �̵�
			BulletList[i].speed * TIMEMANAGER->getElapedTime();
		BulletList[i].bulletpos.y -= sinf(BulletList[i].angle) *
			BulletList[i].speed * TIMEMANAGER->getElapedTime();
		BulletList[i].bulletRect = RectMakeCenter(					// �Ѿ��� �̵��� ��ǥ��ŭ RECT �ٽ� �׷��ش�.
			BulletList[i].bulletpos.x, BulletList[i].bulletpos.y,
			BulletList[i].bulletsize.x, BulletList[i].bulletsize.y);
		BulletList[i].FrameCount += TIMEMANAGER->getElapedTime();	// �Ѿ� ������ �ε��� ������ ���� DeltaTime ����
		BulletList[i].lifetime -= TIMEMANAGER->getElapedTime();		// �Ѿ� ���ӽð� ����

		if (BulletList[i].FrameCount > 0.1f)						// FrameCount�� 0.1f ���� �����ϸ�
		{
			BulletList[i].FrameIndex++;													// FrameIndex ����
			if (BulletList[i].FrameIndex > BulletList[i].bulletImage->GetMaxFrameX())	// FrameIndex�� 3���� Ŀ����
			{			
				BulletList[i].FrameIndex = 0;											// FrameIndex �ٽ� 0���� �ʱ�ȭ
			}
			// FrameCount 0���� �ٽ� �ʱ�ȭ ���ش�.
			// ���࿡ ������ ����� �߻��Ͽ� DeltaTime�� ���� ���� FrameCount�� ������ �ȴٸ�
			// ( ������ ����� �߻����� �� DeltaTime�� ��� ����, �ڵ�� ������ ����... ? )
			// ���� ���� �����Ͽ��ٸ� ex) FrameCount ���� 0.29 ~ �̷������� ���� ���� ����
			// while ���� ���鼭 0.1 ���� ū��� ��� �������ִ� ����
			// �̻������� 0���� �ٽ� �ʱ�ȭ�� �ȴ�.
			while (BulletList[i].FrameCount > 0.1f)					
				BulletList[i].FrameCount -= 0.1f;					
																	
		}
		// �Ѿ��� �����Ÿ��� ����ų�
		// �Ѿ��� ���ӽð��� ����Ǹ�
		// �Ѿ��� �����ع�����.
		if (Math::GetDistance(BulletList[i].bulletfir.x, BulletList[i].bulletfir.y,
			BulletList[i].bulletpos.x, BulletList[i].bulletpos.y) > BulletList[i].range || BulletList[i].lifetime <= 0.f)
		{
			if (BulletList[i].player)
			{
				if (BulletList[i].Power)
				{
					SOUNDMANAGER->play("���Ÿ�������������");
				}
				else
				{
					SOUNDMANAGER->play("���Ÿ���������");
				}
			}
			else
			{
				//SOUNDMANAGER->play("����2");
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
					SOUNDMANAGER->play("���Ÿ�������������");
				}
				else
				{
					SOUNDMANAGER->play("���Ÿ���������");
				}
			}
			else
			{
				//SOUNDMANAGER->play("����2");
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
