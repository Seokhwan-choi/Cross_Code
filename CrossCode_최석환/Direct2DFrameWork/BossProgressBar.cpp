#include "stdafx.h"
#include "BossProgressBar.h"

BossProgressBar::BossProgressBar(string name, Vector2 size, Vector2 pos, Pivot pivot)
	: GameObject(name, size, pos, pivot)
{
	BarImage = IMAGEMANAGER->FindImage("보스프로그래스바");

	Middle[0] = IMAGEMANAGER->FindImage("BossBarWhite");
	Middle[1] = IMAGEMANAGER->FindImage("BossBarRed");

	IMAGEMANAGER->AddImage("가림막", L"Image/Enemy/ProgressBar/가림막.png");

	for (int i = 0; i < 3; ++i)
	{
		Line[i] = IMAGEMANAGER->FindImage("BossBarLine");
	}

	Width = 1072;
	
	MaxHp = 5000.f;
	WhiteHp = 5000.f;

	HpWidth = 1;				// 1072 or 0
	WhiteHpWidth = HpWidth;

	HpHeight = 35;
	WhiteHpHeight = 35;
	isShow = false;
	Alpha = 0.f;

	this->WhiteRc = RectMake(_position.x, _position.y, WhiteHpWidth, 35);
	this->_rc = RectMake(_position.x, _position.y, HpWidth, 35);
}


BossProgressBar::~BossProgressBar()
{

}


void BossProgressBar::Init()
{

}

void BossProgressBar::Update()
{
	if (isShow)
	{
		Alpha += TIMEMANAGER->getElapedTime();
		if (Alpha > 1.f)
		{
			Alpha = 1.f;
		}
	}
}

void BossProgressBar::Render()
{
	if (isShow)
	{
		BarImage->SetSize(Vector2(1200, HpHeight));
		BarImage->Render(_rc.left, _rc.top, Pivot::LeftTop);

		Middle[0]->SetAlpha(Alpha);
		Middle[0]->SetSize(Vector2(WhiteHpWidth, 35));
		Middle[0]->Render(_rc.left + 119, _rc.top, Pivot::LeftTop);

		Middle[1]->SetAlpha(Alpha);
		Middle[1]->SetSize(Vector2(HpWidth, 35));
		Middle[1]->Render(WhiteRc.left + 119, WhiteRc.top, Pivot::LeftTop);

		IMAGEMANAGER->FindImage("가림막")->SetSize(Vector2(49,35));
		IMAGEMANAGER->FindImage("가림막")->Render(_rc.left + 119, _rc.top, Pivot::LeftTop);

		Line[0]->SetAlpha(Alpha);
		Line[0]->SetSize(Vector2(86, 29));
		Line[0]->Render(375, _rc.top + 4, Pivot::LeftTop);

		Line[1]->SetAlpha(Alpha);
		Line[1]->SetSize(Vector2(86, 29));
		Line[1]->Render(650, _rc.top + 4, Pivot::LeftTop);

		Line[2]->SetAlpha(Alpha);
		Line[2]->SetSize(Vector2(86, 29));
		Line[2]->Render(925, _rc.top + 4, Pivot::LeftTop);
	}

	//D2DRENDERER->DrawRectangle(_rc, D2DRenderer::DefaultBrush::Green);
	//D2DRENDERER->DrawRectangle(WhiteRc, D2DRenderer::DefaultBrush::Green);
}

void BossProgressBar::Release()
{

}

bool BossProgressBar::BarDone(int maxhp)
{
	if (maxhp != HpWidth)
	{
		HpWidth += 360.f * TIMEMANAGER->getElapedTime();
		
		if (HpWidth > 1072)
		{
			HpWidth = 1072;
			WhiteHpWidth = 1072;
		}

		this->WhiteRc = RectMake(_position.x, _position.y, WhiteHpWidth, WhiteHpHeight);
		this->_rc = RectMake(_position.x, _position.y, HpWidth, WhiteHpHeight);
		return false;
	}
	else
	{
		return true;
	}
}

void BossProgressBar::WhiteSetGauge(int currentHp, int maxHp)
{
	if (isWhite)
	{
		if (WhiteHp != currentHp)
		{
			WhiteHp -= 240.f * TIMEMANAGER->getElapedTime();

			if (WhiteHp <= currentHp)
			{
				WhiteHp = currentHp;
			}

			if (WhiteHp <= 0)
			{
				WhiteHp = 0;
			}

			this->WhiteHpWidth = ((float)WhiteHp / (float)maxHp) * (float)1072;
		}
		else
		{
			isWhite = false;
		}

		this->WhiteRc = RectMake(_position.x, _position.y, WhiteHpWidth, 35);
	}
}

void BossProgressBar::SetGauge(int currentHp, int maxHp)
{
	if (!isWhite && !EVENT->Eventing())
	{
		//체력 바 계산식 : 체력 바 비율 * 프로그래스 바 이미지 길이만큼
		this->HpWidth = ((float)currentHp / (float)maxHp) * 1072;

		//체력 바가 0보다 같거나 낮아지면 0으로 초기화
		if (HpWidth <= 0)
		{
			HpWidth = 0;
		}

		this->_rc = RectMake(_position.x, _position.y, HpWidth, 40);
	}
}
