#include "stdafx.h"
#include "TitleScene.h"


void TitleScene::Init()
{
	SOUNDMANAGER->addSound("��", "SOUND/��.ogg", false, false);
	SOUNDMANAGER->addSound("Title", "SOUND/Title.ogg", true, true);
	SOUNDMANAGER->addSound("����", "SOUND/����.ogg", false, false);
	SOUNDMANAGER->addSound("ȣ��", "SOUND/ȣ��.ogg", false, false);

	Line = IMAGEMANAGER->AddImage("�Ͼ����", L"Image/Title/Title_Line.png");				// WINSIZEX , 81
	WhiteRect[0] = IMAGEMANAGER->AddImage("�Ͼ縶����", L"Image/Title/Title_White.png");		// 125, 125
	WhiteRect[1] = IMAGEMANAGER->FindImage("�Ͼ縶����");
	BlueRect = IMAGEMANAGER->AddImage("�Ķ�������", L"Image/Title/Title_Blue.png");


	Logo[0] = IMAGEMANAGER->AddImage("�ΰ�", L"Image/Title/Title_Logo.png");					// 491, 255
	Logo[1] = IMAGEMANAGER->AddImage("�ΰ���", L"Image/Title/Title_LogoBack.png");			// 691, 236
	Start = IMAGEMANAGER->AddImage("���۱���", L"Image/Title/Title_Start.png");

	LogoPos = Vector2(WINSIZEX / 2, WINSIZEY / 2);
	LogoAlpha = 0.f;		
	LogoEnd = false; 
	hovor = false;
	hovorSound = false;
	hovorTime = 0.f;
	blue = false;

	Cloud[0] = IMAGEMANAGER->AddImage("����1", L"Image/Title/Title_Cloud1.png");
	Cloud[1] = IMAGEMANAGER->AddImage("����2", L"Image/Title/Title_Cloud2.png"); // 624
	Sky = IMAGEMANAGER->AddImage("�ϴ�", L"Image/Title/Title_Sky.png");
	Ground = IMAGEMANAGER->AddImage("�ٴ�", L"Image/Title/Title_Ground.png");
	Planet = IMAGEMANAGER->AddImage("�༺", L"Image/Title/Title_Planet.png");
	Black = IMAGEMANAGER->AddImage("����", L"Image/Title/Black.png");

	Bridge = IMAGEMANAGER->AddImage("����", L"Image/Title/Title_Bridge.png");
	BridgePos = Vector2(WINSIZEX/2, WINSIZEY + 125 + 260  + 200);
	Lea = IMAGEMANAGER->AddFrameImage("���ΰ�", L"Image/Title/Title_Lea.png", 6, 1);
	LeaPos = Vector2(WINSIZEX/2 + 350, WINSIZEY + 195 + 200);
	LeaEnd = false;


	IMAGEMANAGER->AddImage("���ӽ���", L"Image/Title/���ӽ���.png");
	IMAGEMANAGER->AddImage("����", L"Image/Title/����.png");
	IMAGEMANAGER->AddImage("��������", L"Image/Title/��������.png");

	FrameCount = 0.f;
	FrameIndex = 0;

	BlackAlpha = 1.f;
	
	LineSize = Vector2(WINSIZEX, 81);
	LineEnd = false;

	BlueRectSize = Vector2(175, 175);
	BlueEnd = false;
	BlueAlpha = 1.f;


	StartRect = RectMakeCenter(WINSIZEX / 2, WINSIZEY / 2 - 25, 200, 65);
	AlphaTime = 0.f;
	StartAlpha = 1.f;
	AlphaChange = false;
	isTouch = false;

	ButtonDelay = 0.f;
	Button[0].Image = IMAGEMANAGER->AddFrameImage("���۹�ư", L"Image/MapTool/ū��ư2.png", 20, 1);
	Button[1].Image = IMAGEMANAGER->FindImage("���۹�ư");
	Button[2].Image = IMAGEMANAGER->FindImage("���۹�ư");

	for (int i = 0; i < 3; ++i)
	{
		Button[i].Alpha = 1.f;
		Button[i].AlphaChange = false;
		Button[i].FrameCount = 0.f;
		Button[i].FrameIndex = 0;
		Button[i].Pos = Vector2(-150, WINSIZEY / 2 + 80 * i);
		Button[i].Size = Vector2(250, 50);
		Button[i].Rc = RectMakeCenter(Button[i].Pos.x, Button[i].Pos.y,
			Button[i].Size.x, Button[i].Size.y);
	}

	Cursor = IMAGEMANAGER->AddImage("Ŀ��", L"Image/MapTool/Ŀ��.png");
	IMAGEMANAGER->AddFrameImage("��ġ2", L"Image/MapTool/��ġ2.png", 21, 1);
	ShowCursor(false);

	SCENEMANAGER->SetAlpha(0.f);
	SCENEMANAGER->SetSceneState(SceneState::None);
}

void TitleScene::Update()
{
	this->FrameUpdate();
	this->LineUpdate();
	this->BlueUpdate();
	this->LogoUpdate();
	this->LeaUpdate();
	this->StartUpdate();

	this->ButtonMove();
	this->ButtonUpdate();
	this->ButtonClick();
}

void TitleScene::Render()
{
	this->TitleRender();
	this->ButtonRender();

	IMAGEMANAGER->FindImage("���ӽ���")->SetSize(Vector2(150,50));
	IMAGEMANAGER->FindImage("���ӽ���")->Render(Button[0].Pos.x, Button[0].Pos.y);

	IMAGEMANAGER->FindImage("����")->SetSize(Vector2(150, 50));
	IMAGEMANAGER->FindImage("����")->Render(Button[1].Pos.x, Button[1].Pos.y);

	IMAGEMANAGER->FindImage("��������")->SetSize(Vector2(150, 50));
	IMAGEMANAGER->FindImage("��������")->Render(Button[2].Pos.x, Button[2].Pos.y);

	Cursor->SetSize(Vector2(32, 32));
	Cursor->Render((int)_ptMouse.x, (int)_ptMouse.y, Pivot::LeftTop);
}

void TitleScene::Release()
{
}

void TitleScene::FrameUpdate()
{
	FrameCount += TIMEMANAGER->getElapedTime();
	if (FrameCount > 0.2f)
	{
		while (FrameCount > 0.2f)
			FrameCount -= 0.2f;
		FrameIndex++;
		if (FrameIndex > 5)
			FrameIndex = 0;
	}

	// ��ư ������ ������Ʈ
	for (int i = 0; i < 3; ++i)
	{
		if (PtInRect(&Button[i].Rc, _ptMouse.ToPoint()))
		{
			Button[i].FrameCount += TIMEMANAGER->getElapedTime();
			while (Button[i].FrameCount > 0.1f)
				Button[i].FrameCount -= 0.1f;
			Button[i].FrameIndex++;
			if (Button[i].FrameIndex > 19)
				Button[i].FrameIndex--;

			if (Button[i].FrameIndex == 19)
			{
				if (AlphaChange)
				{
					Button[i].Alpha -= TIMEMANAGER->getElapedTime() / 3.f;
				}
				else
				{
					Button[i].Alpha += TIMEMANAGER->getElapedTime() / 3.f;
				}

				if (Button[i].Alpha <= 0.85 && AlphaChange)
				{
					AlphaChange = !AlphaChange;
				}

				if (Button[i].Alpha >= 1.f && !AlphaChange)
				{
					AlphaChange = !AlphaChange;
				}
			}
		}
		else
		{
			Button[i].FrameCount += TIMEMANAGER->getElapedTime();
			while (Button[i].FrameCount > 0.1f)
				Button[i].FrameCount -= 0.1f;
			Button[i].FrameIndex--;
			if (Button[i].FrameIndex <= 0)
				Button[i].FrameIndex = 0;
		}
	}
}

void TitleScene::TitleRender()
{
	Planet->SetSize(Vector2(425, 744));
	Planet->Render(0, 0, Pivot::LeftTop);

	Sky->SetAlpha(0.65f);
	Sky->SetSize(Vector2(WINSIZEX, 650));
	Sky->Render(WINSIZEX / 2, WINSIZEY / 2 - 70);

	Cloud[0]->SetSize(Vector2(WINSIZEX, 318));
	Cloud[0]->Render(WINSIZEX / 2, WINSIZEY / 2);

	Cloud[1]->SetSize(Vector2(WINSIZEX, 628));
	Cloud[1]->Render(WINSIZEX / 2, WINSIZEY / 2);

	Ground->SetSize(Vector2(WINSIZEX, 365));
	Ground->Render(WINSIZEX / 2, WINSIZEY - 180);

	Black->SetAlpha(BlackAlpha);
	Black->SetSize(Vector2(WINSIZEX, WINSIZEY));
	Black->Render(WINSIZEX / 2, WINSIZEY / 2);

	Line->SetAlpha(BlackAlpha);
	Line->SetSize(LineSize);
	Line->Render(WINSIZEX / 2, WINSIZEY / 2);

	if (LineEnd)
	{
		BlueRect->SetAlpha(BlueAlpha);
		BlueRect->SetSize(BlueRectSize);
		BlueRect->Render(WINSIZEX / 2, WINSIZEY / 2);

		Logo[0]->SetAlpha(LogoAlpha);
		Logo[0]->SetSize(Vector2(491, 255));
		Logo[0]->Render(LogoPos.x, LogoPos.y);

		Logo[1]->SetAlpha(LogoAlpha);
		Logo[1]->SetSize(Vector2(691, 236));
		Logo[1]->Render(LogoPos.x, LogoPos.y);

		Bridge->SetSize(Vector2(WINSIZEX, 142));
		Bridge->Render(BridgePos.x, BridgePos.y);

		Lea->SetSize(Vector2(395, 525));
		Lea->FrameRender(LeaPos.x, LeaPos.y, FrameIndex, 0);
	}

	if (LeaEnd && !isTouch)
	{
		Start->SetSize(Vector2(200, 65));
		Start->SetAlpha(StartAlpha);
		Start->Render(WINSIZEX / 2, WINSIZEY / 2 - 25);
	}

	if (isTouch)
	{



	}


	if (KEYMANAGER->IsToggleKey(VK_F1))
	{
		D2DRENDERER->DrawRectangle(StartRect, D2DRenderer::DefaultBrush::Red);
	}

}

void TitleScene::ButtonRender()
{
	for (int i = 0; i < 3; ++i)
	{
		Button[i].Image->SetAlpha(Button[i].Alpha);
		Button[i].Image->SetSize(Vector2(Button[i].Size.x, Button[i].Size.y));
		Button[i].Image->FrameRender(Button[i].Pos.x, Button[i].Pos.y
		,Button[i].FrameIndex, 0);
	}
}

void TitleScene::LineUpdate()
{
	if (LineSize.y <= 0 && !LineEnd)
	{
		LineSize.y = 0;
		LineEnd = true;
	}

	if (!LineEnd)
	{
		LineSize.y -= 1.5f;
	}
}

void TitleScene::BlueUpdate()
{
	if (LineEnd && !BlueEnd)
	{
		BlueAlpha -= 0.01f;
		BlueRectSize.x += 6.f;
		BlueRectSize.y += 6.f;
		LogoAlpha += 0.01f;
	}

	if (BlueAlpha <= 0.98f && !blue)
	{
		blue = true;
		SOUNDMANAGER->play("��");
	}

	if (BlueAlpha <= 0.f && !BlueEnd)
	{
		BlueAlpha = 0.f;
		BlueEnd = true;
		SOUNDMANAGER->play("Title", 0.3f);
	}
}

void TitleScene::LogoUpdate()
{
	if (BlueEnd)
	{
		LogoPos.y -= 3.5f;
		LogoAlpha += 0.01f;
		BlackAlpha -= 0.01f;
	}

	if (LogoPos.y <= 155)
	{
		LogoPos.y = 155;
		LogoEnd = true;
	}

	if (LogoAlpha >= 1.f)
	{
		LogoAlpha = 1.f;
	}

}

void TitleScene::LeaUpdate()
{
	if (LogoEnd)
	{
		BridgePos.y -= 13.5f;
		LeaPos.y -= 13.f;
	}

	if (BridgePos.y <= WINSIZEY - 71)
	{
		BridgePos.y = WINSIZEY - 71;
	}

	if (LeaPos.y <= WINSIZEY - 262)
	{
		LeaPos.y = WINSIZEY - 262;
	}

	if (BridgePos.y == WINSIZEY - 71 && LeaPos.y == WINSIZEY - 262)
	{
		LeaEnd = true;
	}
}

void TitleScene::StartUpdate()
{
	if (LeaEnd)
	{
		
		if (StartAlpha >= 1.f && !AlphaChange)
		{
			AlphaChange = !AlphaChange;
		}

		if (StartAlpha <= 0.f && AlphaChange)
		{
			AlphaChange = !AlphaChange;
		}

		if (!AlphaChange)
		{
			StartAlpha += TIMEMANAGER->getElapedTime();
		}
		else
		{
			StartAlpha -= TIMEMANAGER->getElapedTime();
		}

		if (!isTouch)
		{
			if (KEYMANAGER->IsOnceKeyDown(VK_LBUTTON))
			{
				if (PtInRect(&StartRect, _ptMouse.ToPoint()))
				{
					SOUNDMANAGER->play("����");
					isTouch = true;
				}
			}
		}
	}
}

void TitleScene::ButtonMove()
{
	if (isTouch)
	{
		ButtonDelay += TIMEMANAGER->getElapedTime();

		if (ButtonDelay > 0.25f)
		{
			Button[0].Pos.x += 15.f;
		}

		if (ButtonDelay > 0.5f)
		{
			Button[1].Pos.x += 17.f;
		}

		if (ButtonDelay > 0.75f)
		{
			Button[2].Pos.x += 19.f;
		}

		if (Button[0].Pos.x >= 180)
		{
			Button[0].Pos.x = 180;
		}

		if (Button[1].Pos.x >= 180)
		{		   
			Button[1].Pos.x = 180;
		}

		if (Button[2].Pos.x >= 180)
		{		   
			Button[2].Pos.x = 180;
		}

		for (int i = 0; i < 3; ++i)
		{
			Button[i].Rc = RectMakeCenter(Button[i].Pos.x, Button[i].Pos.y,
				Button[i].Size.x, Button[i].Size.y);
		}
	}
}

void TitleScene::ButtonUpdate()
{
}

void TitleScene::ButtonClick()
{
		if (PtInRect(&Button[0].Rc, _ptMouse.ToPoint()))
		{
			hovor = true; 
		}
		else if (PtInRect(&Button[1].Rc, _ptMouse.ToPoint()))
		{
			hovor = true;
		}
		else if (PtInRect(&Button[2].Rc, _ptMouse.ToPoint()))
		{
			hovor = true;
		}
		else {
			hovor = false; hovorSound = false;
		}

		if (hovor && !hovorSound)
		{
			hovorSound = true;
			SOUNDMANAGER->play("ȣ��");
		}


		for (int i = 0; i < 3; ++i)
		{
			if (KEYMANAGER->IsOnceKeyDown(VK_LBUTTON))
			{

				if (PtInRect(&Button[i].Rc, _ptMouse.ToPoint()))
				{
					switch (i)
					{
					case 0:
						SOUNDMANAGER->play("����");
						SOUNDMANAGER->FadeOut();
						SCENEMANAGER->LoadScene("�ε���");
						break;
					case 1:
						SOUNDMANAGER->play("����");
						SOUNDMANAGER->FadeOut();
						SCENEMANAGER->LoadScene("����");
						break;
					case 2:
						SOUNDMANAGER->play("����");
						PostQuitMessage(0);
						break;
					}
				}
				
			}
		}
}
