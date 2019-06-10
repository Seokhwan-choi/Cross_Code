#include "stdafx.h"
#include "ProgressBar.h"
#include "MiniSlime.h"


ProgressBar::ProgressBar(string name, Vector2 pos, Vector2 size, Pivot pivot)
	: GameObject(name, pos, size, pivot)
{
	//���� ���α׷��� �� �̹���
	BarImage = IMAGEMANAGER->FindImage("���α׷�����");
	RedImage = IMAGEMANAGER->FindImage("���α׷�������");
	YellowImage = IMAGEMANAGER->FindImage("���α׷������");
	WhiteImage = IMAGEMANAGER->FindImage("���α׷����Ͼ�");

	MaxHp = 0.f;				// �ִ� ü��
	HpWidth = 0.f;				// ����, ��� ���α���
	HpHeight = 0.f;				// ����, ��� ���α���

	WhiteHpWidth = 0.f;			// �Ͼ� ���α���
	WhiteHpHeight = 0.f;		// �Ͼ� ���α���
	WhiteHp = 0.f;				// �Ͼ� ü�� 
	isWhite = false;			// �Ͼ� ����
	isYellow = false;			// ������� ������
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
		//ü�� �� ���� : ü�� �� ���� * ���α׷��� �� �̹��� ���̸�ŭ
		this->HpWidth = ((float)currentHp / (float)maxHp) * Width;

		//ü�� �ٰ� 0���� ���ų� �������� 0���� �ʱ�ȭ
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
