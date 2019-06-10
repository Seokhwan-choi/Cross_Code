#include "stdafx.h"
#include "DamageFont.h"

// ===============================================
// ############## 데미지 폰트 합계 ################
// ===============================================

void DamageFont::Show(Vector2 Pos, int damage, float size, bool show, COLORREF color)
{
	this->show = show;
	FontTag temp;
	temp.isFire = true;
	temp.Alpha = 1.f;
	temp.Damage = damage;
	temp.Size = size;
	temp.Speed = 150.f;
	temp.Color = color;
	temp.Pos = Pos;

	FontList.push_back(temp);

	if (!isDamage)
	{
		this->Pos = Pos;
	}
	SumDamage += damage;
	DamageTime = 0.f;
	isDamage = true;
}

void DamageFont::DamageSum()
{
	if (isDamage)
	{
		if (SumDamage != ShowDamage)
		{
			ShowDamage += 1;

			if (SumDamage <= ShowDamage)
			{
				ShowDamage = SumDamage;
			}
		}
	}
	else 
	{
		SumDamage = 0;
		DamageTime = 0.f;
	}
}

void DamageFont::FontMove()
{
	Vector2 MoveValue = Vector2(0, -1);
	for (int i = 0; i < FontList.size(); ++i)
	{
		if (FontList[i].isFire == false) continue;
		FontList[i].Pos += MoveValue.Normalize() * FontList[i].Speed * TIMEMANAGER->getElapedTime() / 2.f;
		FontList[i].Alpha -= TIMEMANAGER->getElapedTime();
	}

	if (isDamage)
	{
		Alpha += TIMEMANAGER->getElapedTime();
		DamageTime += TIMEMANAGER->getElapedTime();
		if (DamageTime > 0.25f)
		{
			while (DamageTime > 0.25f)
				DamageTime -= 0.25f;
			isDamage = false;
			SumDamage = 0;
		}
	}
	else 
	{
		Alpha -= TIMEMANAGER->getElapedTime();
		if (Alpha <= 0)
		{
			Alpha = 0.f;
			ShowDamage = 0;
		}
	}

	for (int i = 0; i < FontList.size(); ++i)
	{
		if (FontList[i].isFire == true) continue;
		FontList.erase(FontList.begin() + i--);
	}
}

void DamageFont::Init()
{
}

void DamageFont::Update()
{
	this->FontMove();
	this->DamageSum();
}

void DamageFont::Render()
{
	for (int i = 0; i < FontList.size(); ++i)
	{
		Vector2 TempPos = CAMERA->GetRelativeVector2(FontList[i].Pos);
		if (FontList[i].isFire == false) continue;
		//D2DRENDERER->RenderText(TempPos.x, TempPos.y,
		//	to_wstring(FontList[i].Damage),
		//	RGB(255,255,255), FontList[i].Alpha, sizeof(to_wstring(FontList[i].Damage)),
		//	DWRITE_TEXT_ALIGNMENT_CENTER, L"Hall Fetica");
		D2DRENDERER->RenderTextField(TempPos.x - 100, TempPos.y - 50,
			to_wstring(FontList[i].Damage), FontList[i].Color, FontList[i].Size,
			200, 100, FontList[i].Alpha, DWRITE_TEXT_ALIGNMENT_CENTER, L"Hall Fetica");
	}
	if (show)
	{
		Vector2 TempPos = CAMERA->GetRelativeVector2(Pos);
		D2DRENDERER->RenderText(TempPos.x, TempPos.y, to_wstring(ShowDamage),
			RGB(255, 255, 255), Alpha, 55, DWRITE_TEXT_ALIGNMENT_CENTER, L"Hall Fetica Upper");
	}
	//D2DRENDERER->RenderTextField(TempPos.x - 100, TempPos.y - 50, 
	//	to_wstring(ShowDamage), RGB(255, 255, 255), Alpha, sizeof(to_wstring(ShowDamage)),
	//	200, 100, DWRITE_TEXT_ALIGNMENT_CENTER, L"Hall Fetica Upper");

}

void DamageFont::Release()
{
}

DamageFont::DamageFont()
{
	SumDamage = 0;
	ShowDamage = 0;
	DamageTime = 0.f;
	Alpha = 0.f;
	isDamage = false;
}
