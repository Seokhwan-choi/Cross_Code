#include "stdafx.h"
#include "ProgressBar.h"
#include "MiniSlime.h"


ProgressBar::ProgressBar(string name, Vector2 pos, Vector2 size, Pivot pivot)
	: GameObject(name, pos, size, pivot)
{
	//보스 프로그래스 바 이미지
	BarImage = IMAGEMANAGER->FindImage("프로그래스바");
	RedImage = IMAGEMANAGER->FindImage("프로그래스빨강");
	YellowImage = IMAGEMANAGER->FindImage("프로그래스노랑");
	WhiteImage = IMAGEMANAGER->FindImage("프로그래스하양");

	MaxHp = 0.f;				// 최대 체력
	HpWidth = 0.f;				// 빨강, 노랑 가로길이
	HpHeight = 0.f;				// 빨강, 노랑 세로길이

	WhiteHpWidth = 0.f;			// 하양 가로길이
	WhiteHpHeight = 0.f;		// 하양 세로길이
	WhiteHp = 0.f;				// 하양 체력 
	isWhite = false;			// 하양 시작
	isYellow = false;			// 노랑으로 쓸꺼야
	isShow = true;
}


ProgressBar::~ProgressBar()
{
}

void ProgressBar::Init()
{
}

void ProgressBar::Init(float maxhp, float hpwidth, float hpheight, bool isyellow)
{
	MaxHp = maxhp;
	WhiteHp = maxhp;
	Width = hpwidth;
	HpWidth = hpwidth;
	HpHeight = hpheight;
	WhiteHpWidth = HpWidth;
	WhiteHpHeight = HpHeight;
	isYellow = isyellow;

	_rc = RectMake(_position.x, _position.y, HpWidth, HpHeight);
	WhiteRc = RectMake(_position.x, _position.y, WhiteHpWidth, WhiteHpHeight);
}

void ProgressBar::Release()
{

}

void ProgressBar::Update()
{

}

void ProgressBar::Render()
{
	if (isShow)
	{
		BarImage->SetSize(Vector2(Width, HpHeight) * CAMERA->GetZoom());
		BarImage->Render(CAMERA->GetRelativeRect(_rc).left, CAMERA->GetRelativeRect(_rc).top, Pivot::LeftTop);

		WhiteImage->SetSize(Vector2(WhiteHpWidth, WhiteHpHeight) * CAMERA->GetZoom());
		WhiteImage->Render(CAMERA->GetRelativeRect(WhiteRc).left, CAMERA->GetRelativeRect(WhiteRc).top, Pivot::LeftTop);

		if (!isYellow)
		{
			RedImage->SetSize(Vector2(HpWidth, HpHeight) * CAMERA->GetZoom());
			RedImage->SetAlpha(0.7f);
			RedImage->Render(CAMERA->GetRelativeRect(_rc).left, CAMERA->GetRelativeRect(_rc).top, Pivot::LeftTop);
		}
		else
		{
			YellowImage->SetSize(Vector2(HpWidth, HpHeight) * CAMERA->GetZoom());
			YellowImage->SetAlpha(0.7f);
			YellowImage->Render(CAMERA->GetRelativeRect(_rc).left, CAMERA->GetRelativeRect(_rc).top, Pivot::LeftTop);
		}
	}
	
}

void ProgressBar::WhiteSetGauge(int currentHp, int maxHp)
{
	if (isWhite)
	{
		if (WhiteHp != currentHp)
		{
			WhiteHp -= 60.0f * TIMEMANAGER->getElapedTime();

			if (WhiteHp <= currentHp)
			{
				WhiteHp = currentHp;
			}

			this->WhiteHpWidth = ((float)WhiteHp / (float)maxHp) * (float)Width;
		}
		else 
		{
			isWhite = false;
		}

		this->WhiteRc = RectMake(_position.x, _position.y, WhiteHpWidth, WhiteHpHeight);
	}
}

void ProgressBar::SetGauge(int currentHp, int maxHp)
{
	if (!isWhite)
	{
		//체력 바 계산식 : 체력 바 비율 * 프로그래스 바 이미지 길이만큼
		this->HpWidth = ((float)currentHp / (float)maxHp) * Width;

		//체력 바가 0보다 같거나 낮아지면 0으로 초기화
		if (HpWidth <= 0)
		{
			HpWidth = 0;
		}

		this->_rc = RectMake(_position.x, _position.y, HpWidth, HpHeight);
	}
}

void ProgressBar::SetPosition(Vector2 pos)
{
	_position = pos;
	_rc = RectMake(_position.x, _position.y, HpWidth, HpHeight);
	WhiteRc = RectMake(_position.x, _position.y, WhiteHpWidth, WhiteHpHeight);
}
