#include "stdafx.h"
#include "PlayerProgressBar.h"


void PlayerProgressBar::Init()
{

}

void PlayerProgressBar::Update()
{
	this->ShowNumber();
}

void PlayerProgressBar::Render()
{
	IMAGEMANAGER->FindImage("캐릭터UI")->SetSize(Vector2(306, 93));
	IMAGEMANAGER->FindImage("캐릭터UI")->Render(0, 0, Pivot::LeftTop);

	D2DRENDERER->RenderText(140, 0, to_wstring(ShowHp), 30, D2DRenderer::DefaultBrush::White, DWRITE_TEXT_ALIGNMENT_LEADING, L"Hall Fetica");

	IMAGEMANAGER->FindImage("캐릭터WHP")->SetSize(Vector2(WHpWidth, 9));
	IMAGEMANAGER->FindImage("캐릭터WHP")->Render(111, 36, Pivot::LeftTop);

	IMAGEMANAGER->FindImage("캐릭터HP")->SetSize(Vector2(HpWidth, 9));
	IMAGEMANAGER->FindImage("캐릭터HP")->Render(111, 36, Pivot::LeftTop);

	IMAGEMANAGER->FindImage("작은가림막")->SetSize(Vector2(9, 9));
	IMAGEMANAGER->FindImage("작은가림막")->Render(108, 36, Pivot::LeftTop);
}

void PlayerProgressBar::Release()
{
}

void PlayerProgressBar::WhiteSetGauge(int currentHp, int maxHp)
{
	if (isWhite)
	{
		if (WHp != currentHp)
		{
			WHp -= 60.0f * TIMEMANAGER->getElapedTime();

			if (WHp <= currentHp)
			{
				WHp = currentHp;
			}

			this->WHpWidth = ((float)WHp / (float)maxHp) * (float)Width;
		}
		else
		{
			isWhite = false;
		}

		this->WhiteRc = RectMake(_position.x, _position.y, WHpWidth, _size.y);
	}
}

void PlayerProgressBar::SetGauge(int currentHp, int maxHp)
{
	//체력 바 계산식 : 체력 바 비율 * 프로그래스 바 이미지 길이만큼
	this->HpWidth = ((float)currentHp / (float)maxHp) * Width;
	this->SaveHp = currentHp;

	//체력 바가 0보다 같거나 낮아지면 0으로 초기화
	if (HpWidth <= 0)
	{
		HpWidth = 0;
	}

	this->_rc = RectMake(_position.x, _position.y, HpWidth, _size.y);
}

void PlayerProgressBar::ShowNumber()
{
	if (SaveHp != ShowHp)
	{
		ShowHp -= 1.f;
		if (SaveHp >= ShowHp)
		{
			ShowHp = SaveHp;
		}
	}
}

PlayerProgressBar::PlayerProgressBar(string name, Vector2 size, Vector2 pos, Pivot pivot)
	: GameObject(name, size, pos, pivot)
{
	this->HpWidth = _size.x;
	this->WHpWidth = _size.x;
	this->Width = _size.x;
	SaveHp = 555;
	ShowHp = 555;

	IMAGEMANAGER->AddImage("캐릭터HP", L"Image/UI/캐릭터HP.png");
	IMAGEMANAGER->AddImage("작은가림막", L"Image/UI/작은가림막.png");
	IMAGEMANAGER->AddImage("캐릭터UI", L"Image/UI/캐릭터UI.png");
	IMAGEMANAGER->AddImage("캐릭터WHP", L"Image/UI/캐릭터WHP.png");

	_rc = RectMake(_position.x, _position.y, HpWidth, _size.y);
}

PlayerProgressBar::~PlayerProgressBar()
{
}
