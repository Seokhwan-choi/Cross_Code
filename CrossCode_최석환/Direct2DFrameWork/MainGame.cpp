#include "stdafx.h"
#include "MainGame.h"
#include "LoadingScene.h"
#include "MapToolScene.h"
#include "SlimeScene.h"
#include "TitleScene.h"
#include "TownScene.h"
#include "AstarTest.h"


MainGame::MainGame()
{

}


MainGame::~MainGame()
{
}

void MainGame::Init()
{
	SOUNDMANAGER->Init();
	SCENEMANAGER->AddScene("�ε���", new LoadingScene);
	SCENEMANAGER->AddScene("����", new MapToolScene);
	SCENEMANAGER->AddScene("�����Ӻ���", new SlimeScene);
	SCENEMANAGER->AddScene("Ÿ��Ʋ", new TitleScene);
	SCENEMANAGER->AddScene("����", new TownScene);
	SCENEMANAGER->AddScene("���̽�Ÿ", new AstarTest);
	SCENEMANAGER->LoadScene("Ÿ��Ʋ");
}

void MainGame::Release()
{
	//�̱���� ����������
	ImageManager::Delete();
	D2DRenderer::Delete();
	KeyManager::Delete();
	SceneManager::Delete();
	SoundManager::Delete();
	CameraManager::Delete();
	ObjectManager::Delete();
	TimeManager::Delete();
	RandomFunction::Delete();
	EffectManager::Delete();
	UiManager::Delete();
	EventManager::Delete();
}

void MainGame::Update()
{
	if (KEYMANAGER->IsOnceKeyDown(VK_F1))
		_isDebug = !_isDebug;
	SCENEMANAGER->Update();
	SOUNDMANAGER->Update();
	CAMERA->Update();
	EFFECT->Update();
	EVENT->Update();
}

void MainGame::Render()
{
	D2DRENDERER->BeginRender();
	/********************************************************/
	//���⼭ �׸��ÿ�
	float mousex = _ptMouse.x + CAMERA->GetCameraRect().left;
	float mousey = _ptMouse.y + CAMERA->GetCameraRect().top;
	
	//D2DRENDERER->DrawRectangle(RectMakeCenter(WINSIZEX / 2, WINSIZEY / 2, 100, 100), D2DRenderer::DefaultBrush::Red);
	SCENEMANAGER->Render();
	TIMEMANAGER->Render();

	if (_isDebug)
	{
		D2DRENDERER->RenderText(_ptMouse.x, _ptMouse.y + 20, L"X : " + to_wstring((int)mousex) + L", Y :" + to_wstring((int)mousey), 20);
		D2DRENDERER->RenderText(_ptMouse.x, _ptMouse.y, L"X : " + to_wstring((int)mousex / 64) + L", Y :" + to_wstring((int)mousey / 64), 20);
	}

	/********************************************************/
	D2DRENDERER->EndRender();
}
