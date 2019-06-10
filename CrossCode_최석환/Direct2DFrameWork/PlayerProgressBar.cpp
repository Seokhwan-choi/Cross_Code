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
	IMAGEMANAGER->FindImage("ĳ����UI")->SetSize(Vector2(306, 93));
	IMAGEMANAGER->FindImage("ĳ����UI")->Render(0, 0, Pivot::LeftTop);

	D2DRENDERER->RenderText(140, 0, to_wstring(ShowHp), 30, D2DRenderer::DefaultBrush::White, DWRITE_TEXT_ALIGNMENT_LEADING, L"Hall Fetica");

	IMAGEMANAGER->FindImage("ĳ����WHP")->SetSize(Vector2(WHpWidth, 9));
	IMAGEMANAGER->FindImage("ĳ����WHP")->Render(111, 36, Pivot::LeftTop);

	IMAGEMANAGER->FindImage("ĳ����HP")->SetSize(Vector2(HpWidth, 9));
	IMAGEMANAGER->FindImage("ĳ����HP")->Render(111, 36, Pivot::LeftTop);

	IMAGEMANAGER->FindImage("����������")->SetSize(Vector2(9, 9));
	IMAGEMANAGER->FindImage("����������")->Render(108, 36, Pivot::LeftTop);
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
	//ü�� �� ���� : ü�� �� ���� * ���α׷��� �� �̹��� ���̸�ŭ
	this->HpWidth = ((float)currentHp / (float)maxHp) * Width;
	this->SaveHp = currentHp;

	//ü�� �ٰ� 0���� ���ų� �������� 0���� �ʱ�ȭ
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

	IMAGEMANAGER->AddImage("ĳ����HP", L"Image/UI/ĳ����HP.png");
	IMAGEMANAGER->AddImage("����������", L"Image/UI/����������.png");
	IMAGEMANAGER->AddImage("ĳ����UI", L"Image/UI/ĳ����UI.png");
	IMAGEMANAGER->AddImage("ĳ����WHP", L"Image/UI/ĳ����WHP.png");

	_rc = RectMake(_position.x, _position.y, HpWidth, _size.y);
}

PlayerProgressBar::~PlayerProgressBar()
{
}
