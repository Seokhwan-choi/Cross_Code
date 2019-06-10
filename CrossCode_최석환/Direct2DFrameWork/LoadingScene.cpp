#include "stdafx.h"
#include "LoadingScene.h"
#include "Tileheader.h"
#include "testScene.h"

void LoadingScene::Init()
{
	//▼로딩클래스 뉴할당 해주고 초기화
	_loading = new Loading;
	_loading->Init();

	NextScene = "";
	FrameIndex = 0;
	FrameCount = 0.f;

	BackGroundImg = IMAGEMANAGER->AddImage("레아배경", L"Image/Loading/레아배경.png");
	LoadingImg = IMAGEMANAGER->AddFrameImage("로딩중", L"Image/Loading/로딩중.png", 48, 1);
	
	BackGroundAlpha = 0.01f;

	//▼이미지 사운드 씬 로딩
	this->LoadingImage();
	this->LoadingSound();
	this->LoadScenes();

	SCENEMANAGER->SetSceneState(SceneState::FadeIn);
}

void LoadingScene::Release()
{
	//로딩클래스 뉴할당 해준적 있으니 해제
	_loading->Release();
	SAFE_DELETE(_loading);
}

//▼업데이트 한번에 하나의 아이템 로딩함. 로딩서 움직이는 이미지 추가해줘도 되고, 로딩씬서 해줘도 되고.
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
	_loading->Update(); //로딩클래스 애들 업데이트

	if (KEYMANAGER->IsOnceKeyDown(VK_LBUTTON))
	{
		EFFECT->ShowEffect("터치2", _ptMouse, Vector2(50, 50), 0.f);
	}

	BackGroundAlpha = _loading->GetCurrentGauge() / (float)_loading->GetLoadItem().size();

	if (_loading->LoadingDone()) //이프 조건문을 확인하는 동시에 그 아래 있는 함수들까지 실행됨
	{
		EFFECT->Init();
		// ▼로딩이 끝나고 이동할 씬
		// 슬라임보스, 마을, 타이틀, 맵툴, 로딩씬, 에이스타
		SCENEMANAGER->SetAlpha(0.f);
		SCENEMANAGER->SetSceneState(SceneState::FadeOut);
		SCENEMANAGER->LoadScene("슬라임보스");						
	}
}

//▼퍼센테이지 화면에 그려줌. 
void LoadingScene::Render()
{
	//float num = _loading->GetLoadItem().size();
	//float cur = _loading->GetCurrentGauge();
	//per = cur / num * 100;
	//
	//std::wstring str = std::to_wstring(per);
	//str += L" %";
	//D2DRENDERER->RenderText(WINSIZEX/2, WINSIZEY/2, (std::wstring)str, str.size(), D2DRenderer::DefaultBrush::White);

	IMAGEMANAGER->FindImage("레아배경")->SetAlpha(BackGroundAlpha);
	IMAGEMANAGER->FindImage("레아배경")->SetSize(Vector2(WINSIZEX, WINSIZEY));
	IMAGEMANAGER->FindImage("레아배경")->Render(WINSIZEX / 2, WINSIZEY / 2);

	IMAGEMANAGER->FindImage("로딩중")->SetSize(Vector2(96, 96));
	IMAGEMANAGER->FindImage("로딩중")->FrameRender(WINSIZEX - 100, WINSIZEY - 100, FrameIndex, 0);

	EFFECT->Render();

	IMAGEMANAGER->FindImage("커서")->SetSize(Vector2(32, 32));
	IMAGEMANAGER->FindImage("커서")->Render(_ptMouse.x, _ptMouse.y, Pivot::LeftTop);

}

//▼원래 사용하던 이미지 로딩방법
void LoadingScene::LoadingImage()
{
	//▼프레임, 일반 이미지 로딩 예시
	//_loading->LoadFrameImage("NPC-상인", L"image/NPC/NPC-상인 할아버지_12장.bmp",12, 1);
	//_loading->LoadImg("이펙트-말풍선", L"image/NPC/이펙트 - 하이.bmp");

	//▼테스트 이미지 한장
	_loading->LoadImg("testImg", L"IMAGE/testImg.png");

	// ====================================================================================
	// ############################## ProgressBar 이미지 ##################################
	// ====================================================================================
	_loading->LoadImg("프로그래스바", L"Image/Enemy/ProgressBar/ProgressBar.png");
	_loading->LoadImg("프로그래스빨강", L"Image/Enemy/ProgressBar/ProgressRed.png");
	_loading->LoadImg("프로그래스노랑", L"Image/Enemy/ProgressBar/ProgressYellow.png");
	_loading->LoadImg("프로그래스하양", L"Image/Enemy/ProgressBar/ProgressWhite.png");

	_loading->LoadImg("보스프로그래스바", L"Image/Enemy/ProgressBar/BossBar.png");
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
	// ############################## Tile 이미지 ####################################
	// ===============================================================================
	_loading->LoadFrameImage("큰마을", L"Image/MapTool/큰마을.png", SAMPLENUMX, SAMPLENUMY);
	_loading->LoadFrameImage("큰마을2", L"Image/MapTool/큰마을2.png", SAMPLENUMX, SAMPLENUMY);
	_loading->LoadFrameImage("큰마을3", L"Image/MapTool/큰마을3.png", SAMPLENUMX, SAMPLENUMY);
	_loading->LoadFrameImage("큰마을4", L"Image/MapTool/큰마을4.png", SAMPLENUMX, SAMPLENUMY);
	_loading->LoadFrameImage("큰마을5", L"Image/MapTool/큰마을5.png", SAMPLENUMX, SAMPLENUMY);
	_loading->LoadFrameImage("큰마을6", L"Image/MapTool/큰마을6.png", SAMPLENUMX, SAMPLENUMY);
	_loading->LoadFrameImage("큰마을7", L"Image/MapTool/큰마을7.png", SAMPLENUMX, SAMPLENUMY);
	_loading->LoadFrameImage("큰마을8", L"Image/MapTool/큰마을8.png", SAMPLENUMX, SAMPLENUMY);
	_loading->LoadFrameImage("큰마을9", L"Image/MapTool/큰마을9.png", SAMPLENUMX, SAMPLENUMY);
	_loading->LoadFrameImage("마을", L"Image/MapTool/마을.png", SAMPLENUMX, SAMPLENUMY);
	_loading->LoadFrameImage("마을2", L"Image/MapTool/마을2.png", SAMPLENUMX, SAMPLENUMY);
	_loading->LoadFrameImage("마을3", L"Image/MapTool/마을3.png", SAMPLENUMX, SAMPLENUMY);
	_loading->LoadFrameImage("마을4", L"Image/MapTool/마을4.png", SAMPLENUMX, SAMPLENUMY);
	_loading->LoadFrameImage("마을5", L"Image/MapTool/마을5.png", SAMPLENUMX, SAMPLENUMY);
	_loading->LoadFrameImage("마을6", L"Image/MapTool/마을6.png", SAMPLENUMX, SAMPLENUMY);
	_loading->LoadFrameImage("마을7", L"Image/MapTool/마을7.png", SAMPLENUMX, SAMPLENUMY);
	_loading->LoadFrameImage("마을8", L"Image/MapTool/마을8.png", SAMPLENUMX, SAMPLENUMY);
	_loading->LoadFrameImage("마을9", L"Image/MapTool/마을9.png", SAMPLENUMX, SAMPLENUMY);

	_loading->LoadFrameImage("큰슬라임", L"Image/MapTool/큰슬라임맵.png", SAMPLENUMX, SAMPLENUMY);
	_loading->LoadFrameImage("큰슬라임2", L"Image/MapTool/큰슬라임맵2.png", SAMPLENUMX, SAMPLENUMY);
	_loading->LoadFrameImage("큰슬라임3", L"Image/MapTool/큰슬라임맵3.png", SAMPLENUMX, SAMPLENUMY);
	_loading->LoadFrameImage("큰슬라임4", L"Image/MapTool/큰슬라임맵4.png", SAMPLENUMX, SAMPLENUMY);
	_loading->LoadFrameImage("슬라임", L"Image/MapTool/슬라임맵.png", SAMPLENUMX, SAMPLENUMY);
	_loading->LoadFrameImage("슬라임2", L"Image/MapTool/슬라임맵2.png", SAMPLENUMX, SAMPLENUMY);
	_loading->LoadFrameImage("슬라임3", L"Image/MapTool/슬라임맵3.png", SAMPLENUMX, SAMPLENUMY);
	_loading->LoadFrameImage("슬라임4", L"Image/MapTool/슬라임맵4.png", SAMPLENUMX, SAMPLENUMY);

	_loading->LoadFrameImage("큰얼음", L"Image/MapTool/큰얼음맵.png", SAMPLENUMX, SAMPLENUMY);
	_loading->LoadFrameImage("큰얼음2", L"Image/MapTool/큰얼음맵2.png", SAMPLENUMX, SAMPLENUMY);
	_loading->LoadFrameImage("큰얼음3", L"Image/MapTool/큰얼음맵3.png", SAMPLENUMX, SAMPLENUMY);
	_loading->LoadFrameImage("큰얼음4", L"Image/MapTool/큰얼음맵4.png", SAMPLENUMX, SAMPLENUMY);
	_loading->LoadFrameImage("큰얼음5", L"Image/MapTool/큰얼음맵5.png", SAMPLENUMX, SAMPLENUMY);
	_loading->LoadFrameImage("큰얼음6", L"Image/MapTool/큰얼음맵6.png", SAMPLENUMX, SAMPLENUMY);
	_loading->LoadFrameImage("얼음", L"Image/MapTool/얼음맵.png", SAMPLENUMX, SAMPLENUMY);
	_loading->LoadFrameImage("얼음2", L"Image/MapTool/얼음맵2.png", SAMPLENUMX, SAMPLENUMY);
	_loading->LoadFrameImage("얼음3", L"Image/MapTool/얼음맵3.png", SAMPLENUMX, SAMPLENUMY);
	_loading->LoadFrameImage("얼음4", L"Image/MapTool/얼음맵4.png", SAMPLENUMX, SAMPLENUMY);
	_loading->LoadFrameImage("얼음5", L"Image/MapTool/얼음맵5.png", SAMPLENUMX, SAMPLENUMY);
	_loading->LoadFrameImage("얼음6", L"Image/MapTool/얼음맵6.png", SAMPLENUMX, SAMPLENUMY);

	// ===============================================================================
	// ############################## Effect 이미지 ##################################
	// ===============================================================================
	_loading->LoadFrameImage("터치", L"Image/MapTool/터치.png", 22, 1);									// 마우스 터치
	_loading->LoadFrameImage("작은웨이브", L"Image/Enemy/Slime/작은웨이브.png", 12, 1);					// 6번 부터 터지는거
	_loading->LoadFrameImage("큰웨이브", L"Image/Enemy/Slime/큰웨이브.png", 13, 1);						// 6번 부터 터지는거
	_loading->LoadFrameImage("근접공격1", L"Image/Effect/AttackEffect_7.png", 7, 1);
	_loading->LoadFrameImage("근접공격2", L"Image/Effect/AttackEffect2_7.png", 7, 1);
	_loading->LoadFrameImage("근접공격3", L"Image/Effect/AttackEffect3.png", 4, 1);

	_loading->LoadFrameImage("맞음", L"Image/Effect/맞음_8.png", 8, 1);									// 0 ~ 4 까지만 쓸것
	_loading->LoadFrameImage("맞음2", L"Image/Effect/맞음2.png", 5, 1);

	_loading->LoadFrameImage("먼지2", L"Image/Effect/먼지_7.png", 7, 1);
	_loading->LoadFrameImage("먼지", L"Image/Effect/먼지_6.png", 6, 1);
	_loading->LoadFrameImage("작은전기1", L"Image/Effect/작은전기1.png", 4, 1);
	_loading->LoadFrameImage("작은전기2", L"Image/Effect/작은전기2.png", 4, 1);
	_loading->LoadFrameImage("큰전기1", L"Image/Effect/큰전기1.png", 4, 1);
	_loading->LoadFrameImage("큰전기2", L"Image/Effect/큰전기2.png", 4, 1);
	_loading->LoadFrameImage("작은폭파", L"Image/Effect/작은폭파_8.png", 8, 1);
	_loading->LoadFrameImage("큰폭파", L"Image/Effect/큰폭파_10.png", 10, 1);
	_loading->LoadFrameImage("죽음잔상", L"Image/Effect/죽음잔상_12.png", 12, 1);
	_loading->LoadFrameImage("충전", L"Image/Effect/충전.png", 10, 1);
	_loading->LoadFrameImage("파편", L"Image/Effect/파편.png", 4, 1);
	_loading->LoadFrameImage("폭발", L"Image/Effect/폭발.png", 7, 1);
	_loading->LoadFrameImage("번개", L"Image/Effect/번개.png", 9, 1);
	_loading->LoadFrameImage("번개2", L"Image/Effect/번개2.png", 9, 1);

	_loading->LoadImg("공격범위", L"Image/Effect/공격범위.png");
	_loading->LoadImg("원공격커서", L"Image/Effect/원거리공격커서.png");
	_loading->LoadImg("근공격커서", L"Image/Effect/근거리공격커서.png");
	// ===============================================================================
	// ############################### 큰 슬라임 #####################################
	// ===============================================================================
	_loading->LoadFrameImage("슬라임PageOneIdle", L"Image/Enemy/Slime/슬라임Page1.png", 6, 1);
	_loading->LoadFrameImage("슬라임PageTwoIdle", L"Image/Enemy/Slime/슬라임Page2.png", 6, 1);
	_loading->LoadFrameImage("슬라임PageThreeIdle", L"Image/Enemy/Slime/슬라임Page3.png", 6, 1);
	_loading->LoadFrameImage("슬라임PageFinalIdle", L"Image/Enemy/Slime/슬라임Page4.png", 6, 1);
	_loading->LoadFrameImage("슬라임PageOneJump", L"Image/Enemy/Slime/슬라임Page1점프.png", 4, 1);
	_loading->LoadFrameImage("슬라임PageTwoJump", L"Image/Enemy/Slime/슬라임Page2점프.png", 4, 1);
	_loading->LoadFrameImage("슬라임PageThreeJump", L"Image/Enemy/Slime/슬라임Page3점프.png", 4, 1);
	_loading->LoadFrameImage("슬라임PageFinalJump", L"Image/Enemy/Slime/슬라임Page4점프.png", 4, 1);
	_loading->LoadFrameImage("슬라임Core", L"Image/Enemy/Slime/슬라임핵.png", 7, 1);
	_loading->LoadImg("슬라임그림자", L"Image/Enemy/Slime/shadow.png");

	// ===============================================================================
	// ############################### 작은 슬라임 ####################################
	// ===============================================================================
	_loading->LoadFrameImage("슬라임Idle", L"Image/Enemy/Slime/작은슬라임Idle.png", 3, 1);
	_loading->LoadFrameImage("슬라임Jump", L"Image/Enemy/Slime/작은슬라임Jump.png", 4, 1);
	_loading->LoadFrameImage("슬라임Left", L"Image/Enemy/Slime/작은슬라임Left.png", 3, 1);
	_loading->LoadFrameImage("슬라임LeftUp", L"Image/Enemy/Slime/작은슬라임LeftUp.png", 3, 1);
	_loading->LoadFrameImage("슬라임LeftDown", L"Image/Enemy/Slime/작은슬라임LeftDown.png", 3, 1);
	_loading->LoadFrameImage("슬라임Right", L"Image/Enemy/Slime/작은슬라임Right.png", 3, 1);
	_loading->LoadFrameImage("슬라임RightUp", L"Image/Enemy/Slime/작은슬라임RightUp.png", 3, 1);
	_loading->LoadFrameImage("슬라임RightDown", L"Image/Enemy/Slime/작은슬라임RightDown.png", 3, 1);
	_loading->LoadFrameImage("슬라임Hold", L"Image/Enemy/Slime/작은슬라임Hold.png", 2, 1);
	_loading->LoadFrameImage("슬라임원", L"Image/wh360.png", 2, 1);
	_loading->LoadImg("슬라임Hit", L"Image/Enemy/Slime/작은슬라임Hit.png");

	// =================================================================================
	// ############################### 플레이어 #########################################
	// =================================================================================
	// =================================================================================
	// ########################## 캐릭터 총알 Image ####################################
	// =================================================================================
	_loading->LoadFrameImage("Ball", L"Image/Player/Ball.png", 4, 1);
	_loading->LoadFrameImage("Bullet", L"Image/Player/Bullet.png", 4, 1);
	// ======================================================================================== 
	// ######################## 캐릭터 Left 동작 ##### 캐릭터 Left 동작 #########################
	// ======================================================================================== 
	_loading->LoadFrameImage("LeftAttack3", L"Image/Player/Left/LeftAttack3.png", 10, 1);				// 피니쉬 근접 공격
	_loading->LoadFrameImage("LeftMove", L"Image/Player/Left/LeftMove.png", 6, 1);						// 이동
	_loading->LoadFrameImage("LeftAttack", L"Image/Player/Left/LeftAttack.png", 6, 1);					// 근접공격 2
	_loading->LoadFrameImage("LeftAttack2", L"Image/Player/Left/LeftAttack2.png", 6, 1);				// 근접공격 1
	_loading->LoadFrameImage("LeftThrow", L"Image/Player/Left/LeftThrow.png", 6, 1);					// 원거리공격 1
	_loading->LoadFrameImage("LeftThrow2", L"Image/Player/Left/LeftThrow2.png", 6, 1);					// 원거리공격 2
	_loading->LoadFrameImage("LeftHit", L"Image/Player/Left/LeftHit.png", 2, 1);						// 맞음
	_loading->LoadFrameImage("LeftHold", L"Image/Player/Left/LeftHold.png", 4, 1);						// 원거리공격 준비
	_loading->LoadFrameImage("LeftStop", L"Image/Player/Left/LeftStop.png", 3, 1);						// 정지
	_loading->LoadFrameImage("LeftHoldMove", L"Image/Player/Left/LeftHoldMove.png", 3, 1);				// 원거리공격 준비 중 이동
	_loading->LoadImg("LeftIdle", L"Image/Player/Left/LeftIdle.png");									// 기본

	_loading->LoadFrameImage("Rolling", L"Image/Player/Rolling.png", 8, 1);								// 롤링.. ! ( 대쉬 )

	// ========================================================================================
	// ######################## 캐릭터 Right 동작 ##### 캐릭터 Right 동작 #########################
	// ========================================================================================
	_loading->LoadFrameImage("RightAttack3", L"Image/Player/Right/RightAttack3.png", 10, 1);				// 피니쉬 근접 공격
	_loading->LoadFrameImage("RightMove", L"Image/Player/Right/RightMove.png", 6, 1);					// 이동
	_loading->LoadFrameImage("RightAttack", L"Image/Player/Right/RightAttack.png", 6, 1);				// 근접공격 2
	_loading->LoadFrameImage("RightAttack2", L"Image/Player/Right/RightAttack2.png", 6, 1);				// 근접공격 1
	_loading->LoadFrameImage("RightThrow", L"Image/Player/Right/RightThrow.png", 6, 1);					// 원거리공격 1
	_loading->LoadFrameImage("RightThrow2", L"Image/Player/Right/RightThrow2.png", 6, 1);				// 원거리공격 2
	_loading->LoadFrameImage("RightHit", L"Image/Player/Right/RightHit.png", 2, 1);						// 맞음
	_loading->LoadFrameImage("RightHold", L"Image/Player/Right/RightHold.png", 4, 1);					// 원거리공격 준비
	_loading->LoadFrameImage("RightStop", L"Image/Player/Right/RightStop.png", 3, 1);					// 정지
	_loading->LoadFrameImage("RightHoldMove", L"Image/Player/Right/RightHoldMove.png", 3, 1);			// 원거리공격 준비 중 이동
	_loading->LoadImg("RightIdle", L"Image/Player/Right/RightIdle.png");								// 기본

	// ========================================================================================
	// ######################## 캐릭터 Up 동작 ##### 캐릭터 Up 동작 #############################
	// ========================================================================================
	_loading->LoadFrameImage("UpAttack3", L"Image/Player/Up/UpAttack3.png", 10, 1);						// 피니쉬 근접 공격
	_loading->LoadFrameImage("UpMove", L"Image/Player/Up/UpMove.png", 6, 1);								// 이동
	_loading->LoadFrameImage("UpAttack", L"Image/Player/Up/UpAttack.png", 6, 1);							// 근접공격 2
	_loading->LoadFrameImage("UpAttack2", L"Image/Player/Up/UpAttack2.png", 6, 1);						// 근접공격 1
	_loading->LoadFrameImage("UpThrow", L"Image/Player/Up/UpThrow.png", 5, 1);							// 원거리공격 1
	_loading->LoadFrameImage("UpThrow2", L"Image/Player/Up/UpThrow2.png", 5, 1);							// 원거리공격 2
	_loading->LoadFrameImage("UpHit", L"Image/Player/Up/UpHit.png", 2, 1);								// 맞음
	_loading->LoadFrameImage("UpHold", L"Image/Player/Up/UpHold.png", 4, 1);								// 원거리공격 준비
	_loading->LoadFrameImage("UpStop", L"Image/Player/Up/UpStop.png", 3, 1);								// 정지
	_loading->LoadFrameImage("UpHoldMove", L"Image/Player/Up/UpHoldMove.png", 3, 1);						// 원거리공격 준비 중 이동
	_loading->LoadImg("UpIdle", L"Image/Player/Up/UpIdle.png");										// 기본

	// ==========================================================================================
	// ######################## 캐릭터 Down 동작 ##### 캐릭터 Down 동작 ###########################
	// ==========================================================================================
	_loading->LoadFrameImage("DownAttack3", L"Image/Player/Down/DownAttack3.png", 10, 1);				// 피니쉬 근접 공격
	_loading->LoadFrameImage("DownMove", L"Image/Player/Down/DownMove.png", 6, 1);						// 이동
	_loading->LoadFrameImage("DownAttack", L"Image/Player/Down/DownAttack.png", 6, 1);					// 근접공격 2
	_loading->LoadFrameImage("DownAttack2", L"Image/Player/Down/DownAttack2.png", 6, 1);					// 근접공격 1
	_loading->LoadFrameImage("DownThrow", L"Image/Player/Down/DownThrow.png", 6, 1);						// 원거리공격 1
	_loading->LoadFrameImage("DownThrow2", L"Image/Player/Down/DownThrow2.png", 6, 1);					// 원거리공격 2
	_loading->LoadFrameImage("DownHit", L"Image/Player/Down/DownHit.png", 2, 1);							// 맞음
	_loading->LoadFrameImage("DownHold", L"Image/Player/Down/DownHold.png", 4, 1);						// 원거리공격 준비
	_loading->LoadFrameImage("DownStop", L"Image/Player/Down/DownStop.png", 3, 1);						// 정지
	_loading->LoadFrameImage("DownHoldMove", L"Image/Player/Down/DownHoldMove.png", 3, 1);				// 원거리공격 준비 중 이동
	_loading->LoadImg("DownIdle", L"Image/Player/Down/DownIdle.png");									// 기본

	// ==============================================================================================
	// ######################## 캐릭터 Left_Up 동작 ##### 캐릭터 Left_Up 동작 #########################
	// ==============================================================================================
	_loading->LoadFrameImage("LeftUpAttack3", L"Image/Player/LeftUp/LeftUpAttack3.png", 10, 1);			// 피니쉬 근접 공격
	_loading->LoadFrameImage("LeftUpMove", L"Image/Player/LeftUp/LeftUpMove.png", 6, 1);					// 이동
	_loading->LoadFrameImage("LeftUpAttack", L"Image/Player/LeftUp/LeftUpAttack.png", 6, 1);				// 근접공격 2
	_loading->LoadFrameImage("LeftUpAttack2", L"Image/Player/LeftUp/LeftUpAttack2.png", 6, 1);			// 근접공격 1
	_loading->LoadFrameImage("LeftUpThrow", L"Image/Player/LeftUp/LeftUpThrow.png", 6, 1);				// 원거리공격 1
	_loading->LoadFrameImage("LeftUpThrow2", L"Image/Player/LeftUp/LeftUpThrow2.png", 5, 1);				// 원거리공격 2
	_loading->LoadFrameImage("LeftUpHit", L"Image/Player/LeftUp/LeftUpHit.png", 2, 1);					// 맞음
	_loading->LoadFrameImage("LeftUpHold", L"Image/Player/LeftUp/LeftUpHold.png", 4, 1);					// 원거리공격 준비
	_loading->LoadFrameImage("LeftUpStop", L"Image/Player/LeftUp/LeftUpStop.png", 3, 1);					// 정지
	_loading->LoadFrameImage("LeftUpHoldMove", L"Image/Player/LeftUp/LeftUpHoldMove.png", 3, 1);			// 원거리공격 준비 중 이동
	_loading->LoadImg("LeftUpIdle", L"Image/Player/LeftUp/LeftUpIdle.png");							// 기본

	// ================================================================================================== 
	// ######################## 캐릭터 Left_Down 동작 ##### 캐릭터 Left_Down 동작 #########################
	// ==================================================================================================
	_loading->LoadFrameImage("LeftDownAttack3", L"Image/Player/LeftDown/LeftDownAttack3.png", 10, 1);	// 피니쉬 근접 공격
	_loading->LoadFrameImage("LeftDownMove", L"Image/Player/LeftDown/LeftDownMove.png", 6, 1);			// 이동
	_loading->LoadFrameImage("LeftDownAttack", L"Image/Player/LeftDown/LeftDownAttack.png", 6, 1);		// 근접공격 2
	_loading->LoadFrameImage("LeftDownAttack2", L"Image/Player/LeftDown/LeftDownAttack2.png", 6, 1);		// 근접공격 1
	_loading->LoadFrameImage("LeftDownThrow", L"Image/Player/LeftDown/LeftDownThrow.png", 6, 1);			// 원거리공격 1
	_loading->LoadFrameImage("LeftDownThrow2", L"Image/Player/LeftDown/LeftDownThrow2.png", 6, 1);		// 원거리공격 2
	_loading->LoadFrameImage("LeftDownHit", L"Image/Player/LeftDown/LeftDownHit.png", 2, 1);				// 맞음
	_loading->LoadFrameImage("LeftDownHold", L"Image/Player/LeftDown/LeftDownHold.png", 4, 1);			// 원거리공격 준비
	_loading->LoadFrameImage("LeftDownStop", L"Image/Player/LeftDown/LeftDownStop.png", 3, 1);			// 정지
	_loading->LoadFrameImage("LeftDownHoldMove", L"Image/Player/LeftDown/LeftDownHoldMove.png", 3, 1);	// 원거리공격 준비 중 이동
	_loading->LoadImg("LeftDownIdle", L"Image/Player/LeftDown/LeftDownIdle.png");						// 기본

	// ================================================================================================ 
	// ######################## 캐릭터 Right_Up 동작 ##### 캐릭터 Right_Up 동작 #########################
	// ================================================================================================
	_loading->LoadFrameImage("RightUpAttack3", L"Image/Player/RightUp/RightUpAttack3.png", 10, 1);		// 피니쉬 근접 공격
	_loading->LoadFrameImage("RightUpMove", L"Image/Player/RightUp/RightUpMove.png", 6, 1);				// 이동
	_loading->LoadFrameImage("RightUpAttack", L"Image/Player/RightUp/RightUpAttack.png", 6, 1);			// 근접공격 2
	_loading->LoadFrameImage("RightUpAttack2", L"Image/Player/RightUp/RightUpAttack2.png", 6, 1);		// 근접공격 1
	_loading->LoadFrameImage("RightUpThrow", L"Image/Player/RightUp/RightUpThrow.png", 5, 1);			// 원거리공격 1
	_loading->LoadFrameImage("RightUpThrow2", L"Image/Player/RightUp/RightUpThrow2.png", 6, 1);			// 원거리공격 2
	_loading->LoadFrameImage("RightUpHit", L"Image/Player/RightUp/RightUpHit.png", 2, 1);				// 맞음
	_loading->LoadFrameImage("RightUpHold", L"Image/Player/RightUp/RightUpHold.png", 4, 1);				// 원거리공격 준비
	_loading->LoadFrameImage("RightUpStop", L"Image/Player/RightUp/RightUpStop.png", 3, 1);				// 정지
	_loading->LoadFrameImage("RightUpHoldMove", L"Image/Player/RightUp/RightUpHoldMove.png", 3, 1);		// 원거리공격 준비 중 이동
	_loading->LoadImg("RightUpIdle", L"Image/Player/RightUp/RightUpIdle.png");							// 기본

	// =====================================================================`===============================
	// ######################## 캐릭터 Right_Down 동작 ##### 캐릭터 Right_Down 동작 #########################
	// ====================================================================================================
	_loading->LoadFrameImage("RightDownAttack3", L"Image/Player/RightDown/RightDownAttack3.png", 10, 1);	// 피니쉬 근접 공격
	_loading->LoadFrameImage("RightDownMove", L"Image/Player/RightDown/RightDownMove.png", 6, 1);		// 이동
	_loading->LoadFrameImage("RightDownAttack", L"Image/Player/RightDown/RightDownAttack.png", 6, 1);	// 근접공격 2
	_loading->LoadFrameImage("RightDownAttack2", L"Image/Player/RightDown/RightDownAttack2.png", 6, 1);	// 근접공격 1
	_loading->LoadFrameImage("RightDownThrow", L"Image/Player/RightDown/RightDownThrow.png", 6, 1);		// 원거리공격 1
	_loading->LoadFrameImage("RightDownThrow2", L"Image/Player/RightDown/RightDownThrow2.png", 6, 1);	// 원거리공격 2
	_loading->LoadFrameImage("RightDownHit", L"Image/Player/RightDown/RightDownHit.png", 2, 1);			// 맞음
	_loading->LoadFrameImage("RightDownHold", L"Image/Player/RightDown/RightDownHold.png", 4, 1);		// 원거리공격 준비
	_loading->LoadFrameImage("RightDownStop", L"Image/Player/RightDown/RightDownStop.png", 3, 1);		// 정지
	_loading->LoadFrameImage("RightDownHoldMove", L"Image/Player/RightDown/RightDownHoldMove.png", 3, 1);// 원거리공격 준비 중 이동
	_loading->LoadImg("RightDownIdle", L"Image/Player/RightDown/RightDownIdle.png");					// 기본


}

//▼원래 사용하던 로딩방법
void LoadingScene::LoadingSound()
{
	//▼백그라운드 사운드 로딩 예시
	//_loading->LoadSound("시네마틱", "SOUND/Intro_Cinematic.ogg", true, true);

	//▼효과음 로딩 예시
	//_loading->LoadSound("구출", "SOUND/villager_save.ogg", false, false);



	//▼테스트
	_loading->LoadSound("testSound", "SOUND/testSound.mp3", true, true);


	// =======================================================================
	// ############## Effect 효과음 ###########################################
	// =======================================================================
	_loading->LoadSound("근접공격1", "SOUND/근접공격1.ogg", false, false);
	_loading->LoadSound("근접공격2", "SOUND/근접공격2.ogg", false, false);
	_loading->LoadSound("대쉬", "SOUND/대쉬.ogg", false, false);
	_loading->LoadSound("맞음", "SOUND/맞음.ogg", false, false);
	_loading->LoadSound("원거리공격", "SOUND/원거리공격.ogg", false, false);
	_loading->LoadSound("원거리공격차지", "SOUND/원거리공격차지.ogg", false, false);
	_loading->LoadSound("원거리공격터짐", "SOUND/원거리공격터짐.ogg", false, false);
	_loading->LoadSound("원거리공격차지터짐", "SOUND/원거리공격차지터짐.ogg", false, false);

	_loading->LoadSound("모이기", "SOUND/모이기.ogg", false, false);
	_loading->LoadSound("번개", "SOUND/번개.ogg", false, false);
	_loading->LoadSound("보스펑", "SOUND/보스펑.ogg", false, false);
	_loading->LoadSound("비명", "SOUND/비명.ogg", false, false);
	_loading->LoadSound("슬라임맞음", "SOUND/슬라임맞음.ogg", false, false);
	
	_loading->LoadSound("에너지탄", "SOUND/에너지탄.ogg", false, false);

	_loading->LoadSound("슬라임죽음", "SOUND/슬라임죽음.ogg", false, false);
	_loading->LoadSound("작은슬라임공격", "SOUND/작은슬라임공격.ogg", false, false);
	_loading->LoadSound("작은슬라임점프", "SOUND/작은슬라임점프.ogg", false, false);
	_loading->LoadSound("찡", "SOUND/찡.ogg", false, false);
	_loading->LoadSound("차징", "SOUND/차징.ogg", false, false);
	_loading->LoadSound("탱", "SOUND/탱.ogg", false, false);
	_loading->LoadSound("퐁퐁", "SOUND/퐁퐁.ogg", false, false);
	_loading->LoadSound("터짐", "SOUND/터짐.ogg", false, false);
	_loading->LoadSound("터짐2", "SOUND/터짐2.ogg", false, false);
	_loading->LoadSound("에너지폭파", "SOUND/에너지폭파.ogg", false, false);
	_loading->LoadSound("죽음", "SOUND/죽음.ogg", false, false);

	_loading->LoadSound("흩어짐", "SOUND/흩어짐.ogg", false, false);
	_loading->LoadSound("보스슬라임", "SOUND/보스슬라임.ogg", false, false);
	_loading->LoadSound("보스시작", "SOUND/보스시작.ogg", true, true);

	// ===================================================================
	// ############## BGM 브금 ###########################################
	// ===================================================================
	_loading->LoadSound("배틀", "SOUND/배틀.ogg", true, true);
	

}

//▼로딩방법
void LoadingScene::LoadScenes()
{
	_loading->LoadScene("testScene",new testScene);
}
