#include "stdafx.h"
#include "Thunder.h"


// 라디안 X, 디그리 값을 넣어주자
void Thunder::Fire(Vector2 pos, Vector2 size, float angle)
{
	for (int i = 0; i < 25; ++i)
	{
		TagCol temp;
		temp.Pos = pos + Vector2(angle) * size * i;
		temp.Size = size;
		temp.Rc = RectMakeCenter(temp.Pos.x, temp.Pos.y, temp.Size.x, temp.Size.y);
		temp.Life = 400.f;
		temp.fire = true;

		ThunderList.push_back(temp);
	}
	Angle = angle;
	Alpha = 0.5f;
	Time = 0.f;
	Attack = false;
}

void Thunder::FireOff()
{
	ThunderList.clear();
	Attack = false;
}

void Thunder::Update()
{
	if (ThunderList.size() != 0)
	{
		Alpha -= TIMEMANAGER->getElapedTime() / 1.5f;
		if (Alpha <= 0.f) Alpha = 0.f;
		if (Alpha == 0.f && Attack == false)
		{
			SOUNDMANAGER->play("번개");
			Vector2 pos(ThunderList[18].Pos.x, ThunderList[18].Pos.y); Attack = true;
			EFFECT->ShowEffect("번개2", pos, Vector2(200.f, 2400.f), -90 - Angle, 0.05f);
			CAMERA->Shake(10.f, 0.7f);
		}

		if (Alpha == 0.f)
		{
			Time += TIMEMANAGER->getElapedTime();
			if (Time > 0.4f)
			{
				while (Time > 0.4f)
					Time -= 0.4f;
				this->FireOff();
				Time = 0.f;
			}
		}
	}
	
}

void Thunder::Render()
{
	if (KEYMANAGER->IsToggleKey(VK_F1))
	{
		for (int i = 0; i < ThunderList.size(); ++i)
		{
			D2DRENDERER->DrawRectangle(CAMERA->GetRelativeRect(ThunderList[i].Rc), D2DRenderer::DefaultBrush::Red);
		}
	}

	if (ThunderList.size() != 0)
	{
		D2DRENDERER->DrawLine(CAMERA->GetRelativeVector2(ThunderList[0].Pos)
			, CAMERA->GetRelativeVector2(ThunderList[24].Pos), D2D1::ColorF::Red, Alpha, 35.f);
	}
}

Thunder::Thunder()
{
	Alpha = 0.f;
	Attack = false;
	Time = 0.f;
}


Thunder::~Thunder()
{
}
