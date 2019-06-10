#include "stdafx.h"
#include "LoadingScene.h"
#include "Tileheader.h"
#include "testScene.h"

void LoadingScene::Init()
{
	//��ε�Ŭ���� ���Ҵ� ���ְ� �ʱ�ȭ
	_loading = new Loading;
	_loading->Init();

	NextScene = "";
	FrameIndex = 0;
	FrameCount = 0.f;

	BackGroundImg = IMAGEMANAGER->AddImage("���ƹ��", L"Image/Loading/���ƹ��.png");
	LoadingImg = IMAGEMANAGER->AddFrameImage("�ε���", L"Image/Loading/�ε���.png", 48, 1);
	
	BackGroundAlpha = 0.01f;

	//���̹��� ���� �� �ε�
	this->LoadingImage();
	this->LoadingSound();
	this->LoadScenes();

	SCENEMANAGER->SetSceneState(SceneState::FadeIn);
}

void LoadingScene::Release()
{
	//�ε�Ŭ���� ���Ҵ� ������ ������ ����
	_loading->Release();
	SAFE_DELETE(_loading);
}

//�������Ʈ �ѹ��� �ϳ��� ������ �ε���. �ε��� �����̴� �̹��� �߰����൵ �ǰ�, �ε����� ���൵ �ǰ�.
void LoadingScene::Update()
{
	SOUNDMANAGER->FadeOut();
	FrameCount += TIMEMANAGER->getElapedTime();
	if (FrameCount > 0.1f)
	{
		FrameIndex++;
		while (FrameCount > 0.1f)
			FrameCount -= 0.1f;
		if (FrameIndex > 47)
		{
			FrameIndex = 0;
		}
	}
	_loading->Update(); //�ε�Ŭ���� �ֵ� ������Ʈ

	if (KEYMANAGER->IsOnceKeyDown(VK_LBUTTON))
	{
		EFFECT->ShowEffect("��ġ2", _ptMouse, Vector2(50, 50), 0.f);
	}

	BackGroundAlpha = _loading->GetCurrentGauge() / (float)_loading->GetLoadItem().size();

	if (_loading->LoadingDone()) //���� ���ǹ��� Ȯ���ϴ� ���ÿ� �� �Ʒ� �ִ� �Լ������ �����
	{
		EFFECT->Init();
		// ��ε��� ������ �̵��� ��
		// �����Ӻ���, ����, Ÿ��Ʋ, ����, �ε���, ���̽�Ÿ
		SCENEMANAGER->SetAlpha(0.f);
		SCENEMANAGER->SetSceneState(SceneState::FadeOut);
		SCENEMANAGER->LoadScene("�����Ӻ���");						
	}
}

//���ۼ������� ȭ�鿡 �׷���. 
void LoadingScene::Render()
{
	//float num = _loading->GetLoadItem().size();
	//float cur = _loading->GetCurrentGauge();
	//per = cur / num * 100;
	//
	//std::wstring str = std::to_wstring(per);
	//str += L" %";
	//D2DRENDERER->RenderText(WINSIZEX/2, WINSIZEY/2, (std::wstring)str, str.size(), D2DRenderer::DefaultBrush::White);

	IMAGEMANAGER->FindImage("���ƹ��")->SetAlpha(BackGroundAlpha);
	IMAGEMANAGER->FindImage("���ƹ��")->SetSize(Vector2(WINSIZEX, WINSIZEY));
	IMAGEMANAGER->FindImage("���ƹ��")->Render(WINSIZEX / 2, WINSIZEY / 2);

	IMAGEMANAGER->FindImage("�ε���")->SetSize(Vector2(96, 96));
	IMAGEMANAGER->FindImage("�ε���")->FrameRender(WINSIZEX - 100, WINSIZEY - 100, FrameIndex, 0);

	EFFECT->Render();

	IMAGEMANAGER->FindImage("Ŀ��")->SetSize(Vector2(32, 32));
	IMAGEMANAGER->FindImage("Ŀ��")->Render(_ptMouse.x, _ptMouse.y, Pivot::LeftTop);

}

//����� ����ϴ� �̹��� �ε����
void LoadingScene::LoadingImage()
{
	//��������, �Ϲ� �̹��� �ε� ����
	//_loading->LoadFrameImage("NPC-����", L"image/NPC/NPC-���� �Ҿƹ���_12��.bmp",12, 1);
	//_loading->LoadImg("����Ʈ-��ǳ��", L"image/NPC/����Ʈ - ����.bmp");

	//���׽�Ʈ �̹��� ����
	_loading->LoadImg("testImg", L"IMAGE/testImg.png");

	// ====================================================================================
	// ############################## ProgressBar �̹��� ##################################
	// ====================================================================================
	_loading->LoadImg("���α׷�����", L"Image/Enemy/ProgressBar/ProgressBar.png");
	_loading->LoadImg("���α׷�������", L"Image/Enemy/ProgressBar/ProgressRed.png");
	_loading->LoadImg("���α׷������", L"Image/Enemy/ProgressBar/ProgressYellow.png");
	_loading->LoadImg("���α׷����Ͼ�", L"Image/Enemy/ProgressBar/ProgressWhite.png");

	_loading->LoadImg("�������α׷�����", L"Image/Enemy/ProgressBar/BossBar.png");
	_loading->LoadImg("BossBarRed", L"Image/Enemy/ProgressBar/BossBarRed.png");
	_loading->LoadImg("BossBarLine", L"Image/Enemy/ProgressBar/BossBarLine.png");
	_loading->LoadImg("BossBarWhite", L"Image/Enemy/ProgressBar/BossBarWhite.png");


	//_loading->LoadFrameImage("BossBarRed_soonDie", L"Image/Enemy/ProgressBar/BossBarRed_soonDie.png", 5, 1);
	//_loading->LoadImg("BossBarRed_first", L"Image/Enemy/ProgressBar/BossBarRed_first.png");
	//_loading->LoadImg("BossBarRed_second", L"Image/Enemy/ProgressBar/BossBarRed_second.png");
	//_loading->LoadImg("BossBarRed_third", L"Image/Enemy/ProgressBar/BossBarRed_third.png");

	//_loading->LoadFrameImage("BossBarWhite_soonDIe", L"Image/Enemy/ProgressBar/BossBarWhite_soonDIe.png", 5, 1);
	//_loading->LoadImg("BossBarWhite_first", L"Image/Enemy/ProgressBar/BossBarWhite_first.png");
	//_loading->LoadImg("BossBarWhite_second", L"Image/Enemy/ProgressBar/BossBarWhite_second.png");
	//_loading->LoadImg("BossBarWhite_third", L"Image/Enemy/ProgressBar/BossBarWhite_third.png");

	//_loading->LoadImg("BossBarFirst", L"Image/Enemy/ProgressBar/BossBarFirst.png");
	//_loading->LoadImg("BossBarLast", L"Image/Enemy/ProgressBar/BossBarLast.png");
	
	

	
	// ===============================================================================
	// ############################## Tile �̹��� ####################################
	// ===============================================================================
	_loading->LoadFrameImage("ū����", L"Image/MapTool/ū����.png", SAMPLENUMX, SAMPLENUMY);
	_loading->LoadFrameImage("ū����2", L"Image/MapTool/ū����2.png", SAMPLENUMX, SAMPLENUMY);
	_loading->LoadFrameImage("ū����3", L"Image/MapTool/ū����3.png", SAMPLENUMX, SAMPLENUMY);
	_loading->LoadFrameImage("ū����4", L"Image/MapTool/ū����4.png", SAMPLENUMX, SAMPLENUMY);
	_loading->LoadFrameImage("ū����5", L"Image/MapTool/ū����5.png", SAMPLENUMX, SAMPLENUMY);
	_loading->LoadFrameImage("ū����6", L"Image/MapTool/ū����6.png", SAMPLENUMX, SAMPLENUMY);
	_loading->LoadFrameImage("ū����7", L"Image/MapTool/ū����7.png", SAMPLENUMX, SAMPLENUMY);
	_loading->LoadFrameImage("ū����8", L"Image/MapTool/ū����8.png", SAMPLENUMX, SAMPLENUMY);
	_loading->LoadFrameImage("ū����9", L"Image/MapTool/ū����9.png", SAMPLENUMX, SAMPLENUMY);
	_loading->LoadFrameImage("����", L"Image/MapTool/����.png", SAMPLENUMX, SAMPLENUMY);
	_loading->LoadFrameImage("����2", L"Image/MapTool/����2.png", SAMPLENUMX, SAMPLENUMY);
	_loading->LoadFrameImage("����3", L"Image/MapTool/����3.png", SAMPLENUMX, SAMPLENUMY);
	_loading->LoadFrameImage("����4", L"Image/MapTool/����4.png", SAMPLENUMX, SAMPLENUMY);
	_loading->LoadFrameImage("����5", L"Image/MapTool/����5.png", SAMPLENUMX, SAMPLENUMY);
	_loading->LoadFrameImage("����6", L"Image/MapTool/����6.png", SAMPLENUMX, SAMPLENUMY);
	_loading->LoadFrameImage("����7", L"Image/MapTool/����7.png", SAMPLENUMX, SAMPLENUMY);
	_loading->LoadFrameImage("����8", L"Image/MapTool/����8.png", SAMPLENUMX, SAMPLENUMY);
	_loading->LoadFrameImage("����9", L"Image/MapTool/����9.png", SAMPLENUMX, SAMPLENUMY);

	_loading->LoadFrameImage("ū������", L"Image/MapTool/ū�����Ӹ�.png", SAMPLENUMX, SAMPLENUMY);
	_loading->LoadFrameImage("ū������2", L"Image/MapTool/ū�����Ӹ�2.png", SAMPLENUMX, SAMPLENUMY);
	_loading->LoadFrameImage("ū������3", L"Image/MapTool/ū�����Ӹ�3.png", SAMPLENUMX, SAMPLENUMY);
	_loading->LoadFrameImage("ū������4", L"Image/MapTool/ū�����Ӹ�4.png", SAMPLENUMX, SAMPLENUMY);
	_loading->LoadFrameImage("������", L"Image/MapTool/�����Ӹ�.png", SAMPLENUMX, SAMPLENUMY);
	_loading->LoadFrameImage("������2", L"Image/MapTool/�����Ӹ�2.png", SAMPLENUMX, SAMPLENUMY);
	_loading->LoadFrameImage("������3", L"Image/MapTool/�����Ӹ�3.png", SAMPLENUMX, SAMPLENUMY);
	_loading->LoadFrameImage("������4", L"Image/MapTool/�����Ӹ�4.png", SAMPLENUMX, SAMPLENUMY);

	_loading->LoadFrameImage("ū����", L"Image/MapTool/ū������.png", SAMPLENUMX, SAMPLENUMY);
	_loading->LoadFrameImage("ū����2", L"Image/MapTool/ū������2.png", SAMPLENUMX, SAMPLENUMY);
	_loading->LoadFrameImage("ū����3", L"Image/MapTool/ū������3.png", SAMPLENUMX, SAMPLENUMY);
	_loading->LoadFrameImage("ū����4", L"Image/MapTool/ū������4.png", SAMPLENUMX, SAMPLENUMY);
	_loading->LoadFrameImage("ū����5", L"Image/MapTool/ū������5.png", SAMPLENUMX, SAMPLENUMY);
	_loading->LoadFrameImage("ū����6", L"Image/MapTool/ū������6.png", SAMPLENUMX, SAMPLENUMY);
	_loading->LoadFrameImage("����", L"Image/MapTool/������.png", SAMPLENUMX, SAMPLENUMY);
	_loading->LoadFrameImage("����2", L"Image/MapTool/������2.png", SAMPLENUMX, SAMPLENUMY);
	_loading->LoadFrameImage("����3", L"Image/MapTool/������3.png", SAMPLENUMX, SAMPLENUMY);
	_loading->LoadFrameImage("����4", L"Image/MapTool/������4.png", SAMPLENUMX, SAMPLENUMY);
	_loading->LoadFrameImage("����5", L"Image/MapTool/������5.png", SAMPLENUMX, SAMPLENUMY);
	_loading->LoadFrameImage("����6", L"Image/MapTool/������6.png", SAMPLENUMX, SAMPLENUMY);

	// ===============================================================================
	// ############################## Effect �̹��� ##################################
	// ===============================================================================
	_loading->LoadFrameImage("��ġ", L"Image/MapTool/��ġ.png", 22, 1);									// ���콺 ��ġ
	_loading->LoadFrameImage("�������̺�", L"Image/Enemy/Slime/�������̺�.png", 12, 1);					// 6�� ���� �����°�
	_loading->LoadFrameImage("ū���̺�", L"Image/Enemy/Slime/ū���̺�.png", 13, 1);						// 6�� ���� �����°�
	_loading->LoadFrameImage("��������1", L"Image/Effect/AttackEffect_7.png", 7, 1);
	_loading->LoadFrameImage("��������2", L"Image/Effect/AttackEffect2_7.png", 7, 1);
	_loading->LoadFrameImage("��������3", L"Image/Effect/AttackEffect3.png", 4, 1);

	_loading->LoadFrameImage("����", L"Image/Effect/����_8.png", 8, 1);									// 0 ~ 4 ������ ����
	_loading->LoadFrameImage("����2", L"Image/Effect/����2.png", 5, 1);

	_loading->LoadFrameImage("����2", L"Image/Effect/����_7.png", 7, 1);
	_loading->LoadFrameImage("����", L"Image/Effect/����_6.png", 6, 1);
	_loading->LoadFrameImage("��������1", L"Image/Effect/��������1.png", 4, 1);
	_loading->LoadFrameImage("��������2", L"Image/Effect/��������2.png", 4, 1);
	_loading->LoadFrameImage("ū����1", L"Image/Effect/ū����1.png", 4, 1);
	_loading->LoadFrameImage("ū����2", L"Image/Effect/ū����2.png", 4, 1);
	_loading->LoadFrameImage("��������", L"Image/Effect/��������_8.png", 8, 1);
	_loading->LoadFrameImage("ū����", L"Image/Effect/ū����_10.png", 10, 1);
	_loading->LoadFrameImage("�����ܻ�", L"Image/Effect/�����ܻ�_12.png", 12, 1);
	_loading->LoadFrameImage("����", L"Image/Effect/����.png", 10, 1);
	_loading->LoadFrameImage("����", L"Image/Effect/����.png", 4, 1);
	_loading->LoadFrameImage("����", L"Image/Effect/����.png", 7, 1);
	_loading->LoadFrameImage("����", L"Image/Effect/����.png", 9, 1);
	_loading->LoadFrameImage("����2", L"Image/Effect/����2.png", 9, 1);

	_loading->LoadImg("���ݹ���", L"Image/Effect/���ݹ���.png");
	_loading->LoadImg("������Ŀ��", L"Image/Effect/���Ÿ�����Ŀ��.png");
	_loading->LoadImg("�ٰ���Ŀ��", L"Image/Effect/�ٰŸ�����Ŀ��.png");
	// ===============================================================================
	// ############################### ū ������ #####################################
	// ===============================================================================
	_loading->LoadFrameImage("������PageOneIdle", L"Image/Enemy/Slime/������Page1.png", 6, 1);
	_loading->LoadFrameImage("������PageTwoIdle", L"Image/Enemy/Slime/������Page2.png", 6, 1);
	_loading->LoadFrameImage("������PageThreeIdle", L"Image/Enemy/Slime/������Page3.png", 6, 1);
	_loading->LoadFrameImage("������PageFinalIdle", L"Image/Enemy/Slime/������Page4.png", 6, 1);
	_loading->LoadFrameImage("������PageOneJump", L"Image/Enemy/Slime/������Page1����.png", 4, 1);
	_loading->LoadFrameImage("������PageTwoJump", L"Image/Enemy/Slime/������Page2����.png", 4, 1);
	_loading->LoadFrameImage("������PageThreeJump", L"Image/Enemy/Slime/������Page3����.png", 4, 1);
	_loading->LoadFrameImage("������PageFinalJump", L"Image/Enemy/Slime/������Page4����.png", 4, 1);
	_loading->LoadFrameImage("������Core", L"Image/Enemy/Slime/��������.png", 7, 1);
	_loading->LoadImg("�����ӱ׸���", L"Image/Enemy/Slime/shadow.png");

	// ===============================================================================
	// ############################### ���� ������ ####################################
	// ===============================================================================
	_loading->LoadFrameImage("������Idle", L"Image/Enemy/Slime/����������Idle.png", 3, 1);
	_loading->LoadFrameImage("������Jump", L"Image/Enemy/Slime/����������Jump.png", 4, 1);
	_loading->LoadFrameImage("������Left", L"Image/Enemy/Slime/����������Left.png", 3, 1);
	_loading->LoadFrameImage("������LeftUp", L"Image/Enemy/Slime/����������LeftUp.png", 3, 1);
	_loading->LoadFrameImage("������LeftDown", L"Image/Enemy/Slime/����������LeftDown.png", 3, 1);
	_loading->LoadFrameImage("������Right", L"Image/Enemy/Slime/����������Right.png", 3, 1);
	_loading->LoadFrameImage("������RightUp", L"Image/Enemy/Slime/����������RightUp.png", 3, 1);
	_loading->LoadFrameImage("������RightDown", L"Image/Enemy/Slime/����������RightDown.png", 3, 1);
	_loading->LoadFrameImage("������Hold", L"Image/Enemy/Slime/����������Hold.png", 2, 1);
	_loading->LoadFrameImage("�����ӿ�", L"Image/wh360.png", 2, 1);
	_loading->LoadImg("������Hit", L"Image/Enemy/Slime/����������Hit.png");

	// =================================================================================
	// ############################### �÷��̾� #########################################
	// =================================================================================
	// =================================================================================
	// ########################## ĳ���� �Ѿ� Image ####################################
	// =================================================================================
	_loading->LoadFrameImage("Ball", L"Image/Player/Ball.png", 4, 1);
	_loading->LoadFrameImage("Bullet", L"Image/Player/Bullet.png", 4, 1);
	// ======================================================================================== 
	// ######################## ĳ���� Left ���� ##### ĳ���� Left ���� #########################
	// ======================================================================================== 
	_loading->LoadFrameImage("LeftAttack3", L"Image/Player/Left/LeftAttack3.png", 10, 1);				// �ǴϽ� ���� ����
	_loading->LoadFrameImage("LeftMove", L"Image/Player/Left/LeftMove.png", 6, 1);						// �̵�
	_loading->LoadFrameImage("LeftAttack", L"Image/Player/Left/LeftAttack.png", 6, 1);					// �������� 2
	_loading->LoadFrameImage("LeftAttack2", L"Image/Player/Left/LeftAttack2.png", 6, 1);				// �������� 1
	_loading->LoadFrameImage("LeftThrow", L"Image/Player/Left/LeftThrow.png", 6, 1);					// ���Ÿ����� 1
	_loading->LoadFrameImage("LeftThrow2", L"Image/Player/Left/LeftThrow2.png", 6, 1);					// ���Ÿ����� 2
	_loading->LoadFrameImage("LeftHit", L"Image/Player/Left/LeftHit.png", 2, 1);						// ����
	_loading->LoadFrameImage("LeftHold", L"Image/Player/Left/LeftHold.png", 4, 1);						// ���Ÿ����� �غ�
	_loading->LoadFrameImage("LeftStop", L"Image/Player/Left/LeftStop.png", 3, 1);						// ����
	_loading->LoadFrameImage("LeftHoldMove", L"Image/Player/Left/LeftHoldMove.png", 3, 1);				// ���Ÿ����� �غ� �� �̵�
	_loading->LoadImg("LeftIdle", L"Image/Player/Left/LeftIdle.png");									// �⺻

	_loading->LoadFrameImage("Rolling", L"Image/Player/Rolling.png", 8, 1);								// �Ѹ�.. ! ( �뽬 )

	// ========================================================================================
	// ######################## ĳ���� Right ���� ##### ĳ���� Right ���� #########################
	// ========================================================================================
	_loading->LoadFrameImage("RightAttack3", L"Image/Player/Right/RightAttack3.png", 10, 1);				// �ǴϽ� ���� ����
	_loading->LoadFrameImage("RightMove", L"Image/Player/Right/RightMove.png", 6, 1);					// �̵�
	_loading->LoadFrameImage("RightAttack", L"Image/Player/Right/RightAttack.png", 6, 1);				// �������� 2
	_loading->LoadFrameImage("RightAttack2", L"Image/Player/Right/RightAttack2.png", 6, 1);				// �������� 1
	_loading->LoadFrameImage("RightThrow", L"Image/Player/Right/RightThrow.png", 6, 1);					// ���Ÿ����� 1
	_loading->LoadFrameImage("RightThrow2", L"Image/Player/Right/RightThrow2.png", 6, 1);				// ���Ÿ����� 2
	_loading->LoadFrameImage("RightHit", L"Image/Player/Right/RightHit.png", 2, 1);						// ����
	_loading->LoadFrameImage("RightHold", L"Image/Player/Right/RightHold.png", 4, 1);					// ���Ÿ����� �غ�
	_loading->LoadFrameImage("RightStop", L"Image/Player/Right/RightStop.png", 3, 1);					// ����
	_loading->LoadFrameImage("RightHoldMove", L"Image/Player/Right/RightHoldMove.png", 3, 1);			// ���Ÿ����� �غ� �� �̵�
	_loading->LoadImg("RightIdle", L"Image/Player/Right/RightIdle.png");								// �⺻

	// ========================================================================================
	// ######################## ĳ���� Up ���� ##### ĳ���� Up ���� #############################
	// ========================================================================================
	_loading->LoadFrameImage("UpAttack3", L"Image/Player/Up/UpAttack3.png", 10, 1);						// �ǴϽ� ���� ����
	_loading->LoadFrameImage("UpMove", L"Image/Player/Up/UpMove.png", 6, 1);								// �̵�
	_loading->LoadFrameImage("UpAttack", L"Image/Player/Up/UpAttack.png", 6, 1);							// �������� 2
	_loading->LoadFrameImage("UpAttack2", L"Image/Player/Up/UpAttack2.png", 6, 1);						// �������� 1
	_loading->LoadFrameImage("UpThrow", L"Image/Player/Up/UpThrow.png", 5, 1);							// ���Ÿ����� 1
	_loading->LoadFrameImage("UpThrow2", L"Image/Player/Up/UpThrow2.png", 5, 1);							// ���Ÿ����� 2
	_loading->LoadFrameImage("UpHit", L"Image/Player/Up/UpHit.png", 2, 1);								// ����
	_loading->LoadFrameImage("UpHold", L"Image/Player/Up/UpHold.png", 4, 1);								// ���Ÿ����� �غ�
	_loading->LoadFrameImage("UpStop", L"Image/Player/Up/UpStop.png", 3, 1);								// ����
	_loading->LoadFrameImage("UpHoldMove", L"Image/Player/Up/UpHoldMove.png", 3, 1);						// ���Ÿ����� �غ� �� �̵�
	_loading->LoadImg("UpIdle", L"Image/Player/Up/UpIdle.png");										// �⺻

	// ==========================================================================================
	// ######################## ĳ���� Down ���� ##### ĳ���� Down ���� ###########################
	// ==========================================================================================
	_loading->LoadFrameImage("DownAttack3", L"Image/Player/Down/DownAttack3.png", 10, 1);				// �ǴϽ� ���� ����
	_loading->LoadFrameImage("DownMove", L"Image/Player/Down/DownMove.png", 6, 1);						// �̵�
	_loading->LoadFrameImage("DownAttack", L"Image/Player/Down/DownAttack.png", 6, 1);					// �������� 2
	_loading->LoadFrameImage("DownAttack2", L"Image/Player/Down/DownAttack2.png", 6, 1);					// �������� 1
	_loading->LoadFrameImage("DownThrow", L"Image/Player/Down/DownThrow.png", 6, 1);						// ���Ÿ����� 1
	_loading->LoadFrameImage("DownThrow2", L"Image/Player/Down/DownThrow2.png", 6, 1);					// ���Ÿ����� 2
	_loading->LoadFrameImage("DownHit", L"Image/Player/Down/DownHit.png", 2, 1);							// ����
	_loading->LoadFrameImage("DownHold", L"Image/Player/Down/DownHold.png", 4, 1);						// ���Ÿ����� �غ�
	_loading->LoadFrameImage("DownStop", L"Image/Player/Down/DownStop.png", 3, 1);						// ����
	_loading->LoadFrameImage("DownHoldMove", L"Image/Player/Down/DownHoldMove.png", 3, 1);				// ���Ÿ����� �غ� �� �̵�
	_loading->LoadImg("DownIdle", L"Image/Player/Down/DownIdle.png");									// �⺻

	// ==============================================================================================
	// ######################## ĳ���� Left_Up ���� ##### ĳ���� Left_Up ���� #########################
	// ==============================================================================================
	_loading->LoadFrameImage("LeftUpAttack3", L"Image/Player/LeftUp/LeftUpAttack3.png", 10, 1);			// �ǴϽ� ���� ����
	_loading->LoadFrameImage("LeftUpMove", L"Image/Player/LeftUp/LeftUpMove.png", 6, 1);					// �̵�
	_loading->LoadFrameImage("LeftUpAttack", L"Image/Player/LeftUp/LeftUpAttack.png", 6, 1);				// �������� 2
	_loading->LoadFrameImage("LeftUpAttack2", L"Image/Player/LeftUp/LeftUpAttack2.png", 6, 1);			// �������� 1
	_loading->LoadFrameImage("LeftUpThrow", L"Image/Player/LeftUp/LeftUpThrow.png", 6, 1);				// ���Ÿ����� 1
	_loading->LoadFrameImage("LeftUpThrow2", L"Image/Player/LeftUp/LeftUpThrow2.png", 5, 1);				// ���Ÿ����� 2
	_loading->LoadFrameImage("LeftUpHit", L"Image/Player/LeftUp/LeftUpHit.png", 2, 1);					// ����
	_loading->LoadFrameImage("LeftUpHold", L"Image/Player/LeftUp/LeftUpHold.png", 4, 1);					// ���Ÿ����� �غ�
	_loading->LoadFrameImage("LeftUpStop", L"Image/Player/LeftUp/LeftUpStop.png", 3, 1);					// ����
	_loading->LoadFrameImage("LeftUpHoldMove", L"Image/Player/LeftUp/LeftUpHoldMove.png", 3, 1);			// ���Ÿ����� �غ� �� �̵�
	_loading->LoadImg("LeftUpIdle", L"Image/Player/LeftUp/LeftUpIdle.png");							// �⺻

	// ================================================================================================== 
	// ######################## ĳ���� Left_Down ���� ##### ĳ���� Left_Down ���� #########################
	// ==================================================================================================
	_loading->LoadFrameImage("LeftDownAttack3", L"Image/Player/LeftDown/LeftDownAttack3.png", 10, 1);	// �ǴϽ� ���� ����
	_loading->LoadFrameImage("LeftDownMove", L"Image/Player/LeftDown/LeftDownMove.png", 6, 1);			// �̵�
	_loading->LoadFrameImage("LeftDownAttack", L"Image/Player/LeftDown/LeftDownAttack.png", 6, 1);		// �������� 2
	_loading->LoadFrameImage("LeftDownAttack2", L"Image/Player/LeftDown/LeftDownAttack2.png", 6, 1);		// �������� 1
	_loading->LoadFrameImage("LeftDownThrow", L"Image/Player/LeftDown/LeftDownThrow.png", 6, 1);			// ���Ÿ����� 1
	_loading->LoadFrameImage("LeftDownThrow2", L"Image/Player/LeftDown/LeftDownThrow2.png", 6, 1);		// ���Ÿ����� 2
	_loading->LoadFrameImage("LeftDownHit", L"Image/Player/LeftDown/LeftDownHit.png", 2, 1);				// ����
	_loading->LoadFrameImage("LeftDownHold", L"Image/Player/LeftDown/LeftDownHold.png", 4, 1);			// ���Ÿ����� �غ�
	_loading->LoadFrameImage("LeftDownStop", L"Image/Player/LeftDown/LeftDownStop.png", 3, 1);			// ����
	_loading->LoadFrameImage("LeftDownHoldMove", L"Image/Player/LeftDown/LeftDownHoldMove.png", 3, 1);	// ���Ÿ����� �غ� �� �̵�
	_loading->LoadImg("LeftDownIdle", L"Image/Player/LeftDown/LeftDownIdle.png");						// �⺻

	// ================================================================================================ 
	// ######################## ĳ���� Right_Up ���� ##### ĳ���� Right_Up ���� #########################
	// ================================================================================================
	_loading->LoadFrameImage("RightUpAttack3", L"Image/Player/RightUp/RightUpAttack3.png", 10, 1);		// �ǴϽ� ���� ����
	_loading->LoadFrameImage("RightUpMove", L"Image/Player/RightUp/RightUpMove.png", 6, 1);				// �̵�
	_loading->LoadFrameImage("RightUpAttack", L"Image/Player/RightUp/RightUpAttack.png", 6, 1);			// �������� 2
	_loading->LoadFrameImage("RightUpAttack2", L"Image/Player/RightUp/RightUpAttack2.png", 6, 1);		// �������� 1
	_loading->LoadFrameImage("RightUpThrow", L"Image/Player/RightUp/RightUpThrow.png", 5, 1);			// ���Ÿ����� 1
	_loading->LoadFrameImage("RightUpThrow2", L"Image/Player/RightUp/RightUpThrow2.png", 6, 1);			// ���Ÿ����� 2
	_loading->LoadFrameImage("RightUpHit", L"Image/Player/RightUp/RightUpHit.png", 2, 1);				// ����
	_loading->LoadFrameImage("RightUpHold", L"Image/Player/RightUp/RightUpHold.png", 4, 1);				// ���Ÿ����� �غ�
	_loading->LoadFrameImage("RightUpStop", L"Image/Player/RightUp/RightUpStop.png", 3, 1);				// ����
	_loading->LoadFrameImage("RightUpHoldMove", L"Image/Player/RightUp/RightUpHoldMove.png", 3, 1);		// ���Ÿ����� �غ� �� �̵�
	_loading->LoadImg("RightUpIdle", L"Image/Player/RightUp/RightUpIdle.png");							// �⺻

	// =====================================================================`===============================
	// ######################## ĳ���� Right_Down ���� ##### ĳ���� Right_Down ���� #########################
	// ====================================================================================================
	_loading->LoadFrameImage("RightDownAttack3", L"Image/Player/RightDown/RightDownAttack3.png", 10, 1);	// �ǴϽ� ���� ����
	_loading->LoadFrameImage("RightDownMove", L"Image/Player/RightDown/RightDownMove.png", 6, 1);		// �̵�
	_loading->LoadFrameImage("RightDownAttack", L"Image/Player/RightDown/RightDownAttack.png", 6, 1);	// �������� 2
	_loading->LoadFrameImage("RightDownAttack2", L"Image/Player/RightDown/RightDownAttack2.png", 6, 1);	// �������� 1
	_loading->LoadFrameImage("RightDownThrow", L"Image/Player/RightDown/RightDownThrow.png", 6, 1);		// ���Ÿ����� 1
	_loading->LoadFrameImage("RightDownThrow2", L"Image/Player/RightDown/RightDownThrow2.png", 6, 1);	// ���Ÿ����� 2
	_loading->LoadFrameImage("RightDownHit", L"Image/Player/RightDown/RightDownHit.png", 2, 1);			// ����
	_loading->LoadFrameImage("RightDownHold", L"Image/Player/RightDown/RightDownHold.png", 4, 1);		// ���Ÿ����� �غ�
	_loading->LoadFrameImage("RightDownStop", L"Image/Player/RightDown/RightDownStop.png", 3, 1);		// ����
	_loading->LoadFrameImage("RightDownHoldMove", L"Image/Player/RightDown/RightDownHoldMove.png", 3, 1);// ���Ÿ����� �غ� �� �̵�
	_loading->LoadImg("RightDownIdle", L"Image/Player/RightDown/RightDownIdle.png");					// �⺻


}

//����� ����ϴ� �ε����
void LoadingScene::LoadingSound()
{
	//���׶��� ���� �ε� ����
	//_loading->LoadSound("�ó׸�ƽ", "SOUND/Intro_Cinematic.ogg", true, true);

	//��ȿ���� �ε� ����
	//_loading->LoadSound("����", "SOUND/villager_save.ogg", false, false);



	//���׽�Ʈ
	_loading->LoadSound("testSound", "SOUND/testSound.mp3", true, true);


	// =======================================================================
	// ############## Effect ȿ���� ###########################################
	// =======================================================================
	_loading->LoadSound("��������1", "SOUND/��������1.ogg", false, false);
	_loading->LoadSound("��������2", "SOUND/��������2.ogg", false, false);
	_loading->LoadSound("�뽬", "SOUND/�뽬.ogg", false, false);
	_loading->LoadSound("����", "SOUND/����.ogg", false, false);
	_loading->LoadSound("���Ÿ�����", "SOUND/���Ÿ�����.ogg", false, false);
	_loading->LoadSound("���Ÿ���������", "SOUND/���Ÿ���������.ogg", false, false);
	_loading->LoadSound("���Ÿ���������", "SOUND/���Ÿ���������.ogg", false, false);
	_loading->LoadSound("���Ÿ�������������", "SOUND/���Ÿ�������������.ogg", false, false);

	_loading->LoadSound("���̱�", "SOUND/���̱�.ogg", false, false);
	_loading->LoadSound("����", "SOUND/����.ogg", false, false);
	_loading->LoadSound("������", "SOUND/������.ogg", false, false);
	_loading->LoadSound("���", "SOUND/���.ogg", false, false);
	_loading->LoadSound("�����Ӹ���", "SOUND/�����Ӹ���.ogg", false, false);
	
	_loading->LoadSound("������ź", "SOUND/������ź.ogg", false, false);

	_loading->LoadSound("����������", "SOUND/����������.ogg", false, false);
	_loading->LoadSound("���������Ӱ���", "SOUND/���������Ӱ���.ogg", false, false);
	_loading->LoadSound("��������������", "SOUND/��������������.ogg", false, false);
	_loading->LoadSound("��", "SOUND/��.ogg", false, false);
	_loading->LoadSound("��¡", "SOUND/��¡.ogg", false, false);
	_loading->LoadSound("��", "SOUND/��.ogg", false, false);
	_loading->LoadSound("����", "SOUND/����.ogg", false, false);
	_loading->LoadSound("����", "SOUND/����.ogg", false, false);
	_loading->LoadSound("����2", "SOUND/����2.ogg", false, false);
	_loading->LoadSound("����������", "SOUND/����������.ogg", false, false);
	_loading->LoadSound("����", "SOUND/����.ogg", false, false);

	_loading->LoadSound("�����", "SOUND/�����.ogg", false, false);
	_loading->LoadSound("����������", "SOUND/����������.ogg", false, false);
	_loading->LoadSound("��������", "SOUND/��������.ogg", true, true);

	// ===================================================================
	// ############## BGM ��� ###########################################
	// ===================================================================
	_loading->LoadSound("��Ʋ", "SOUND/��Ʋ.ogg", true, true);
	

}

//��ε����
void LoadingScene::LoadScenes()
{
	_loading->LoadScene("testScene",new testScene);
}
