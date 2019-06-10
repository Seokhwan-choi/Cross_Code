#include "stdafx.h"
#include "Player.h"
#include "Bullet.h"
#include "PlayerProgressBar.h"
#include "Event.h"
#include "Particle.h"
#include "Slime.h"

void Player::Init()
{
	PlayerDir = Dir::Up;						// 캐릭터 방향
	PlayerState = STATE::IdleUp;				// 캐릭터 상태 초기화 
	Speed = 400.f;								// 캐릭터 이동속도
	isMove = false;								// 캐릭터 이동 중 초기화
	MoveValue = Vector2(0.f, 0.f);				// 캐릭터 방향 벡터 초기화
	
	AttackDir = 0.f;							// 캐릭터 공격방향
	AttackRound = 0.f;							// 캐릭터 공격범위 ( 원거리/근거리 구분 )
	AttackAngle = 0.f;							// 캐릭터 공격각도
	AttackTime = 0.f;							// 캐릭터 근접공격 콤보 시간
	AttackCount = 0;							// 캐릭터 근접공격 콤보 단계
	isAttack = false;							// 캐릭터 근접공격 중
	AttackRect = RectMakeCenter(_position.x,	// 캐릭터 근접공격 판정 RECT
		_position.y, 64, 64);	
	PowerTime = 0.f;							// 캐릭터 원거리공격 얼마나 기모았냐?
	ThrowTime = 0.f;							// 캐릭터 원거리공격 콤보 시간
	ThrowCount = 0;								// 캐릭터 원거리공격 콤보 단계
	isThrow = false;							// 캐릭터 원거리공격 중
	isHold = false;								// 캐릭터 원거리공격 조준 중
	PlayerBullet = new Bullet;					// 캐릭터 원거리공격 용 총알
	
	RollingTime = 0.f;							// 캐릭터 대쉬 시간
	RollingCount = 3;							// 캐릭터 대쉬 제한 횟수 ( 최대 3회 )
	isRoll = false;								// 캐릭터 현재 대쉬 중이냐 ?
	
	FrameIndex = 0;								// 캐릭터 애니메이션 프레임 인덱스
	FrameCount = 0.f;							// 캐릭터 애니메이션 프레임 카운트
	KeyTime = 0.f;								// 캐릭터 키 입력 제한시간

	PlayerHp = 426;								// 캐릭터 Hp
	isHit = false;								// 캐릭터 근접공격 맞음
	HitTime = 0.f;								// 캐릭터 Hit 지속 시간s
	
	AlphaStart = false;							// 캐릭터 Alpha 시작
	AlphaTime = 0.f;							// 캐릭터 Alpha 지속 시간
	isAlpha = false;							// 캐릭터 Alpha 여부
	AlphaChangeTime = 0.f;						// 캐릭터 Alpha 변경 시간
	Alpha = 1.f;								// 캐릭터 Alpha 값
	Event = false;								// 캐릭터 이벤트 중 동작 X
	AttackType = false;							// 캐릭터 공격타입 초기값 원거리 false / 근거리 true
	isDying = false;							// 캐릭터 죽어간다.
	Boom = false;

	DustDelay = 0.f;							// 캐릭터 먼지 지연시간

	this->ImageResource();						// 캐릭터 ImageResource 나중에 로딩으로 뺌

	PlayerHpBar = new PlayerProgressBar("캐릭터HP", Vector2(150, 9), Vector2(0, 50));
	UIMANAGER->addUi(ObjectType::UI, PlayerHpBar);

	PlayerParticle = new Particle;				// 캐릭터 파편

}

void Player::Update()
{
	this->Move();								// 캐릭터 이동 처리 함수
	this->Attack();								// 캐릭터 공격 처리 함수
	this->ImageFrameUpdate();					// 캐릭터 이미지 프레임 인덱스 Update()
	this->UpdateRect();							// 캐릭터 RECT Update()
	this->HitMove();							// 캐릭터 HitMove
	this->AlphaUpdate();						// 캐릭터 Alpha값 Update()
	PlayerHpBar->SetGauge(PlayerHp, 426);
	PlayerHpBar->WhiteSetGauge(PlayerHp, 426);
	PlayerHpBar->ShowNumber();
	PlayerParticle->Update();
	if (KEYMANAGER->IsOnceKeyDown('O'))
	{
		this->Hit(1);
	}

	if (PlayerHp <= 0 && !isDying)
	{
		this->Death(); PlayerHp = 0;
		((Slime*)OBJECTMANAGER->FindObject(ObjectType::Enemy, "보스슬라임"))->PlayerDeath();
	}
	if (isDying)
	{
		isHit = true; AlphaStart = false;
		Alpha -= TIMEMANAGER->getElapedTime() / 2.f;
		if (Alpha <= 0 && !Boom)
		{
			Alpha = 0.f; Boom = true;
			SOUNDMANAGER->play("죽음");
			EFFECT->ShowEffect("폭발", _position, Vector2(128, 128), true, 0, 0.1f);
			for (int i = 0; i < 10; ++i)
			{
				for (int j = 0; j < 5; ++j)
				{
					PlayerParticle->Make(_position, 125.f, -1.5f - (0.5) * i, RND->Range(i * 30.f, (i + 1) * 30.f));
				}
			}
		}

		if (Boom)
		{
			if (PlayerParticle->GetList().size() == 0)
			{
				EVENT->AddEvent(new FadeIn);
				EVENT->AddEvent(new ChangeScene("타이틀"));
				_isActive = false;
			}
		}
	}

	if (KEYMANAGER->IsOnceKeyDown(VK_F2))
	{
		PlayerHp += 350;
	}
}

void Player::Render()
{
	PlayerBullet->Render();						// 캐릭터 총알 Render
	PlayerParticle->Render();
	if (_isDebug)
	{
		//D2DRENDERER->DrawRectangle(CAMERA->GetRelativeRect(_rc), D2DRenderer::DefaultBrush::Green,3.f);
		if (isAttack)
		{
			switch (AttackCount)
			{
			case 1:
				D2DRENDERER->FillRectangle(CAMERA->GetRelativeRect(AttackRect), D2DRenderer::DefaultBrush::Yellow);
				break;
			case 2:
				D2DRENDERER->FillRectangle(CAMERA->GetRelativeRect(AttackRect), D2DRenderer::DefaultBrush::Green);
				break;
			case 3:
				D2DRENDERER->FillRectangle(CAMERA->GetRelativeRect(AttackRect), D2DRenderer::DefaultBrush::Blue);
				break;
			case 4:
				D2DRENDERER->FillRectangle(CAMERA->GetRelativeRect(AttackRect), D2DRenderer::DefaultBrush::Red);
				break;
			}
		}
	}

	this->ImageRender();			// 캐릭터 애니메이션 Render

	if (KEYMANAGER->IsToggleKey(VK_F1))
	{
		D2DRENDERER->DrawRectangle(CAMERA->GetRelativeRect(HitRect), D2DRenderer::DefaultBrush::Green);
		D2DRENDERER->DrawRectangle(CAMERA->GetRelativeRect(AttackRect), D2DRenderer::DefaultBrush::Green);
		D2DRENDERER->DrawRectangle(CAMERA->GetRelativeRect(ColRect), D2DRenderer::DefaultBrush::Green);
	}
	UIMANAGER->Render();
} 

void Player::Release()
{
	PlayerBullet->Release();
	SAFE_DELETE(PlayerBullet);
	SAFE_DELETE(PlayerParticle);
}

void Player::Death()
{
	isDying = true; PlayerState = STATE::Death;
	EVENT->AddEvent(new CameraZoom2);
	
  //EVENT->AddEvent(new ChangeScene("타이틀"));
}

void Player::Hit(int damage)
{
	PlayerHp -= damage;
	isHit = true; 
	AlphaStart = true;
	SOUNDMANAGER->play("맞음", 0.15f);
	switch (PlayerDir)
	{
	case Player::Dir::Left:
		PlayerState = STATE::LeftHit;
		break;
	case Player::Dir::Right:
		PlayerState = STATE::RightHit;
		break;
	case Player::Dir::Up:
		PlayerState = STATE::UpHit;
		break;
	case Player::Dir::Down:
		PlayerState = STATE::DownHit;
		break;
	case Player::Dir::LeftUp:
		PlayerState = STATE::LeftUpHit;
		break;
	case Player::Dir::RightUp:
		PlayerState = STATE::RightUpHit;
		break;
	case Player::Dir::LeftDown:
		PlayerState = STATE::LeftDownHit;
		break;
	case Player::Dir::RightDown:
		PlayerState = STATE::RightDownHit;
		break;
	}
	CAMERA->Shake(2.5f);
}

void Player::HitMove()
{
	if (isHit)
	{
		HitTime += TIMEMANAGER->getElapedTime();
		if (HitTime > 0.25f)
		{
			while (HitTime > 0.25f)
				HitTime -= 0.25f;
			isHit = false;
			PlayerHpBar->SetisWhite(true);
			switch (PlayerState)
			{
			case STATE::LeftHit:
				PlayerState = STATE::IdleLeft;
				break;
			case STATE::RightHit:
				PlayerState = STATE::IdleRight;
				break;
			case STATE::UpHit:
				PlayerState = STATE::IdleUp;
				break;
			case STATE::DownHit:
				PlayerState = STATE::IdleDown;
				break;
			case STATE::LeftUpHit:
				PlayerState = STATE::IdleLeftUp;
				break;
			case STATE::RightUpHit:
				PlayerState = STATE::IdleRightUp;
				break;
			case STATE::LeftDownHit:
				PlayerState = STATE::IdleLeftDown;
				break;
			case STATE::RightDownHit:
				PlayerState = STATE::IdleRightDown;
				break;
			}
			
		}
		else
		{
			switch (PlayerState)
			{
			case STATE::LeftHit:
				MoveValue = Vector2(-1.f, 0.f);
				break;
			case STATE::RightHit:
				MoveValue = Vector2(1.f, 0.f);
				break;
			case STATE::UpHit:
				MoveValue = Vector2(0.f, -1.f);
				break;
			case STATE::DownHit:
				MoveValue = Vector2(0.f, 1.f);
				break;
			case STATE::LeftUpHit:
				MoveValue = Vector2(-1.f, -1.f);
				break;
			case STATE::RightUpHit:
				MoveValue = Vector2(1.f, -1.f);
				break;
			case STATE::LeftDownHit:
				MoveValue = Vector2(-1.f, 1.f);
				break;
			case STATE::RightDownHit:
				MoveValue = Vector2(1.f, 1.f);
				break;
			}
			_position += MoveValue.Normalize() * Speed * TIMEMANAGER->getElapedTime() / 1.7f;
		}
	}
	
}

void Player::Move()
{
	RollingTime += TIMEMANAGER->getElapedTime();		// 대쉬 제한 횟수 초기화
	MoveValue = Vector2(0.f, 0.f);						// 방향 벡터 초기화
	EVENT->Eventing();
	if (KEYMANAGER->IsStayKeyDown('A') && PlayerState != STATE::Right && !isHit && EVENT->Eventing() != true)
	{
		isMove = true;										// 캐릭터 이동상태 true
		KeyTime += TIMEMANAGER->getElapedTime();			// DeltaTime 계속 더 해준다.
		MoveValue += Vector2(-1.0f, 0.f);					// 캐릭터가 이동하는 방향 벡터
		if (isHold || isThrow) Speed = 150.f;				// 원거리 공격중 이동속도 150.f
		if (isAttack) Speed = 0.f;							// 근접 공격중 이동 불가능
		if (isRoll && RollingCount > 0) Speed = 400.f * 2.5f;	// 대쉬 중 이동속도
		if (isRoll && RollingCount < 0) Speed = 150.f;			// 대쉬 모두 소모
															// 일반적인 이동속도 400.f
		if (isMove && !isHold && !isThrow && !isAttack && !isRoll ) Speed = 400.f;								

		if (KeyTime > 0.025f)							// 플레이어가 어느정도 누르고 있어야 상태변화
		{							
			if (!isThrow && !isAttack && !isRoll)		// 캐릭터가 공격 중이면 상태변화 X
				PlayerState = STATE::Left;
		}
	}
	
	if (KEYMANAGER->IsOnceKeyUp('A') && !isHit && EVENT->Eventing() != true)
	{
		// 플레이어가 키를 손에서 놓았을 때
		// 캐릭터가 대각선 이동중이라면
		// 캐릭터는 Left로 바로 상태 변화가 이루어지지 않으므로
		// 캐릭터가 이동중이던 대각선 방향으로 Stop하게 된다.
		if (!isAttack && !isThrow)
			FrameIndex = 0; KeyTime = 0.f;
		
		if (PlayerState == STATE::LeftDown) {
			PlayerState = STATE::LeftDownStop;
		}
		else if (PlayerState == STATE::LeftUp) {
			PlayerState = STATE::LeftUpStop;
		}
		else if (PlayerState == STATE::Left){
			PlayerState = STATE::LeftStop;
		}
		else if (isHold)
		{
			PlayerState = STATE::LeftHold;
		}

		// =================================================
		// ######## 캐릭터가 공격 중이라면 isMove 유지 #######
		// =================================================
		if (PlayerState != STATE::LeftAttack &&
			PlayerState != STATE::LeftUpAttack &&
			PlayerState != STATE::LeftDownAttack)
		{
			isMove = false;
		}
	}

	if (KEYMANAGER->IsStayKeyDown('D') && PlayerState != STATE::Left && !isHit && EVENT->Eventing() != true)
	{
		isMove = true;									// 캐릭터 이동상태 true
		KeyTime += TIMEMANAGER->getElapedTime();		// DeltaTime 계속 더 해준다.
		MoveValue += Vector2(1.0f, 0.f);				// 캐릭터가 이동하는 방향 벡터
		if (isHold || isThrow) Speed = 150.f;			// 원거리 공격중 이동속도 150.f
		if (isAttack) Speed = 0.f;						// 근접 공격중 이동불가능
		if (isRoll && RollingCount > 0) Speed = 400.f * 2.5f;	// 대쉬 중 이동속도
		if (isRoll && RollingCount < 0) Speed = 150.f;			// 대쉬 모두 소모
														// 일반적인 이동속도 400.f
		if (isMove && !isHold && !isThrow && !isAttack && !isRoll) Speed = 400.f;

		if (KeyTime > 0.025f) {							// 플레이어가 어느정도 누르고 있어야 상태변화
			if (!isThrow && !isAttack && !isRoll)
				PlayerState = STATE::Right;				// 캐릭터가 공격 중이면 상태변화 X
		}

	}															  

	if (KEYMANAGER->IsOnceKeyUp('D') && !isHit && EVENT->Eventing() != true)
	{
		if (!isAttack && !isThrow)
			FrameIndex = 0; KeyTime = 0.f;
		if (PlayerState == STATE::RightDown) {
			PlayerState = STATE::RightDownStop;
		}
		else if (PlayerState == STATE::RightUp) {
			PlayerState = STATE::RightUpStop;
		}
		else if (PlayerState == STATE::Right){
			PlayerState = STATE::RightStop;
		}
		else if (isHold)
		{
			PlayerState = STATE::RightHold;
		}

		// =================================================
		// ######## 캐릭터가 공격 중이라면 isMove 유지 #######
		// =================================================
		if (PlayerState != STATE::RightAttack &&
			PlayerState != STATE::RightUpAttack &&
			PlayerState != STATE::RightDownAttack)
		{
			isMove = false;
		}
	}

	if (KEYMANAGER->IsStayKeyDown('S') && PlayerState != STATE::Up && !isHit && EVENT->Eventing() != true)
	{
		isMove = true;									// 캐릭터 이동상태 true
		KeyTime += TIMEMANAGER->getElapedTime();		// DeltaTime 계속 더 해준다.
		MoveValue += Vector2(0.0f, 1.f);				// 캐릭터가 이동하는 방향 벡터
		if (isHold || isThrow) Speed = 150.f;			// 원거리 공격중 이동속도 150.f
		if (isAttack) Speed = 0.f;						// 근접 공격중 이동불가능
		if (isRoll && RollingCount > 0) Speed = 400.f * 2.5f;	// 대쉬 중 이동속도
		if (isRoll && RollingCount < 0) Speed = 150.f;			// 대쉬 모두 소모
														// 일반적인 이동속도 400.f
		if (isMove && !isHold && !isThrow && !isAttack && !isRoll) Speed = 400.f;

		if (KeyTime > 0.025f) {							// 플레이어가 어느정도 누르고 있어야 상태변화
			if (!isThrow && !isAttack && !isRoll)
				PlayerState = STATE::Down;				// 캐릭터가 공격 중이면 상태변화 X
		}				
	}

	// Key를 2가지 누르고 있는 경우 KeyTime을 0으로 초기화 계속해준다.
	if (KEYMANAGER->IsStayKeyDown('A') && KEYMANAGER->IsStayKeyDown('S') && !isHit && EVENT->Eventing() != true)
	{
		if (!isThrow && !isAttack && !isRoll)
			PlayerState = STATE::LeftDown; KeyTime = 0.f;
	}

	if (KEYMANAGER->IsStayKeyDown('D') && KEYMANAGER->IsStayKeyDown('S') && !isHit && EVENT->Eventing() != true)
	{
		if (!isThrow && !isAttack && !isRoll)
			PlayerState = STATE::RightDown; KeyTime = 0.f;
	}

	if (KEYMANAGER->IsOnceKeyUp('S') && !isHit)
	{
		if (!isAttack && !isThrow)
			FrameIndex = 0; KeyTime = 0.f;
		if (PlayerState == STATE::RightDown) {
			PlayerState = STATE::RightDownStop;
		}
		else if (PlayerState == STATE::LeftDown) {
			PlayerState = STATE::LeftDownStop;
		}
		else if (PlayerState == STATE::Down){
			PlayerState = STATE::DownStop;
		}
		else if (isHold)
		{
			PlayerState = STATE::DownHold;
		}

		// =================================================
		// ######## 캐릭터가 공격 중이라면 isMove 유지 #######
		// =================================================
		if (PlayerState != STATE::DownAttack &&
			PlayerState != STATE::LeftDownAttack &&
			PlayerState != STATE::RightDownAttack)
		{
			isMove = false;
		}
	}

	if (KEYMANAGER->IsOnceKeyUp('A') && KEYMANAGER->IsOnceKeyUp('S') && !isHit && EVENT->Eventing() != true)
	{
		isMove = false;
		PlayerState = STATE::LeftDownStop; 
	}

	if (KEYMANAGER->IsOnceKeyUp('D') && KEYMANAGER->IsOnceKeyUp('S') && !isHit && EVENT->Eventing() != true)
	{
		isMove = false;
		PlayerState = STATE::RightDownStop;
	}

	if (KEYMANAGER->IsStayKeyDown('W') && PlayerState != STATE::Down && !isHit && EVENT->Eventing() != true)
	{
		isMove = true;									// 캐릭터 이동상태 true
		KeyTime += TIMEMANAGER->getElapedTime();		// DeltaTime 계속 더 해준다.
		MoveValue += Vector2(0.0f, -1.f);				// 캐릭터가 이동하는 방향 벡터
		if (isHold || isThrow) Speed = 150.f;			// 원거리 공격중 이동속도 150.f
		if (isAttack) Speed = 0.f;						// 근접 공격중 이동불가능
		if (isRoll && RollingCount > 0) Speed = 400.f * 2.5f;	// 대쉬 중 이동속도
		if (isRoll && RollingCount < 0) Speed = 150.f;			// 대쉬 모두 소모
														// 일반적인 이동속도 400.f
		if (isMove && !isHold && !isThrow && !isAttack && !isRoll) Speed = 400.f;

		if (KeyTime > 0.025f) {							// 플레이어가 어느정도 누르고 있어야 상태변화
			if (!isThrow && !isAttack && !isRoll)
				PlayerState = STATE::Up;				// 캐릭터가 공격 중이면 상태변화 X
		}												
			
	}

	if (KEYMANAGER->IsStayKeyDown('A') && KEYMANAGER->IsStayKeyDown('W') && !isHit && EVENT->Eventing() != true)
	{
		if (!isThrow && !isAttack && !isRoll)
			PlayerState = STATE::LeftUp; KeyTime = 0.f;
	}

	if (KEYMANAGER->IsStayKeyDown('D') && KEYMANAGER->IsStayKeyDown('W') && !isHit && EVENT->Eventing() != true)
	{
		if (!isThrow && !isAttack && !isRoll)
			PlayerState = STATE::RightUp; KeyTime = 0.f;
	}

	if (KEYMANAGER->IsOnceKeyUp('W') && !isHit)
	{
		if (!isAttack && !isThrow)
			FrameIndex = 0; KeyTime = 0.f;

		if (PlayerState == STATE::RightUp) {
			PlayerState = STATE::RightUpStop;
		}
		else if (PlayerState == STATE::LeftUp) {
			PlayerState = STATE::LeftUpStop;
		}
		else if (PlayerState == STATE::Up){
			PlayerState = STATE::UpStop;
		}
		else if (isHold)
		{
			PlayerState = STATE::UpHold;
		}

		// =================================================
		// ######## 캐릭터가 공격 중이라면 isMove 유지 #######
		// =================================================
		if (PlayerState != STATE::UpAttack &&
			PlayerState != STATE::LeftUpAttack &&
			PlayerState != STATE::RightUpAttack)
		{
			isMove = false;
		}
	}

	if (KEYMANAGER->IsOnceKeyUp('A') && KEYMANAGER->IsOnceKeyUp('W') && !isHit && EVENT->Eventing() != true)
	{
		isMove = false;
		PlayerState = STATE::LeftUpStop;
	}

	if (KEYMANAGER->IsOnceKeyUp('D') && KEYMANAGER->IsOnceKeyUp('W') && !isHit && EVENT->Eventing() != true)
	{
		isMove = false;
		PlayerState = STATE::RightUpStop;
	}

	if (KEYMANAGER->IsOnceKeyDown(VK_RBUTTON) && isMove && !isRoll && !isHit && EVENT->Eventing() != true)
	{
		FrameIndex = 0; SOUNDMANAGER->play("대쉬");
		RollingTime = 0.f; isRoll = true;
		PlayerState = STATE::Rolling;
		if (RollingCount > 0)			// 플레이어 대쉬 횟수 제한 ( 최대 3번 )
		{
			RollingCount--;				// 플레이어 대쉬 횟수 감소
		}
		else {
			Speed = 150.f;				// 대쉬 횟수 모두 소모하면 느려짐!
		}
	}

	if (KEYMANAGER->IsOnceKeyUp(VK_RBUTTON) && !isHit && EVENT->Eventing() != true)
	{
		FrameIndex = 0;
		if (RollingCount > 0)
		{
			Speed = Speed / 2.f;		// 원래 이동속도로 !
		}
	}

	if (RollingTime > 0.75f)
	{
		RollingCount = 3;				// 대쉬 횟수 초기화
		isRoll = false;
	}

	_position += MoveValue.Normalize() * Speed * TIMEMANAGER->getElapedTime();
	this->DirCheck();	// 캐릭터 방향 Check						
	this->UpdateRect();
}

void Player::Attack()
{
	AttackTime += TIMEMANAGER->getElapedTime();
	ThrowTime += TIMEMANAGER->getElapedTime();
	AttackRound = Math::GetDistance(_position.x, _position.y, _ptMouse.x / CAMERA->GetZoom() + CAMERA->GetCameraRect().left,
		_ptMouse.y / CAMERA->GetZoom() + CAMERA->GetCameraRect().top);
	AttackAngle = Math::GetAngle(_position.x, _position.y, _ptMouse.x / CAMERA->GetZoom() + CAMERA->GetCameraRect().left,
		_ptMouse.y / CAMERA->GetZoom() + CAMERA->GetCameraRect().top);
	// =================================================================================
	// ############################ 캐릭터 원거리 공격 ###################################
	// =================================================================================
	if (AttackRound > AttackROUND)
	{
		AttackType = false;
		// =================================================================================
		// ############################ 캐릭터 원거리 조준 ###################################
		// =================================================================================
		if (KEYMANAGER->IsStayKeyDown(VK_LBUTTON) && !isThrow && !isHit && EVENT->Eventing() != true)
		{
			isHold = true; PowerTime += TIMEMANAGER->getElapedTime();
			// =========================================================================
			// ###################### 마우스 캐릭터의 오른/위 쪽 #########################
			// =========================================================================
			if (AttackAngle < (Math::PI / 2.f) - Math::PI / 8.f &&
				AttackAngle >(Math::PI / 8.f))
			{
				if (isMove)
				{
					PlayerState = STATE::RightUpHoldMove;
				}
				else {
					PlayerState = STATE::RightUpHold;
				}	
			}
			// =========================================================================
			// ###################### 마우스 캐릭터의 위쪽 ###############################
			// =========================================================================
			else if (AttackAngle < (Math::PI / 2.f) + Math::PI / 8.f &&				// 90 + 22.5
				AttackAngle >(Math::PI / 2.f) - Math::PI / 8.f)						// 90 - 22.5
			{
				if (isMove)
				{
					PlayerState = STATE::UpHoldMove;
				}
				else {
					PlayerState = STATE::UpHold;
				}
			}
			// =========================================================================
			// ################## 마우스 캐릭터의 왼/위 쪽 ###############################
			// =========================================================================
			else if (AttackAngle < Math::PI - (Math::PI / 8.f) &&					// 180 - 22.5
				AttackAngle >(Math::PI / 2.f) + Math::PI / 8.f)						// 90 + 22.5
			{
				if (isMove)
				{
					PlayerState = STATE::LeftUpHoldMove;
				}
				else {
					PlayerState = STATE::LeftUpHold;
				}
			}
			// =========================================================================
			// ###################### 마우스 캐릭터의 왼쪽 ##############################
			// =========================================================================
			else if (AttackAngle < Math::PI + (Math::PI / 8.f) &&					// 180 + 22.5
				AttackAngle > Math::PI - Math::PI / 8.f)							// 180 - 22.5
			{
				if (isMove)
				{
					PlayerState = STATE::LeftHoldMove;
				}
				else {
					PlayerState = STATE::LeftHold;
				}
			}
			// =========================================================================
			// ################### 마우스 캐릭터의 왼/아래 쪽 ############################
			// =========================================================================
			else if (AttackAngle < (Math::PI + Math::PI / 2) - (Math::PI / 8.f) &&	// 270 - 22.5
				AttackAngle > Math::PI + (Math::PI / 8.f))							// 180 + 22.5
			{
				if (isMove)
				{
					PlayerState = STATE::LeftDownHoldMove;
				}
				else {
					PlayerState = STATE::LeftDownHold;
				}
			}
			// =========================================================================
			// ###################### 마우스 캐릭터의 아래 쪽 ############################
			// =========================================================================
			else if (AttackAngle < (Math::PI + Math::PI / 2) + (Math::PI / 8.f) &&	// 270 + 22.5
				AttackAngle >(Math::PI + Math::PI / 2) - (Math::PI / 8.f))			// 270 - 22.5
			{
				if (isMove)
				{
					PlayerState = STATE::DownHoldMove;
				}
				else {
					PlayerState = STATE::DownHold;
				}
			}
			// =========================================================================
			// ################### 마우스 캐릭터의 오른/아래 쪽 ##########################
			// =========================================================================
			else if (AttackAngle < (Math::PI * 2.f) - (Math::PI / 8.f) &&			// 360 - 22.5
				AttackAngle >(Math::PI + Math::PI / 2) + (Math::PI / 8.f))			// 270 + 22.5
			{
				if (isMove)
				{
					PlayerState = STATE::RightDownHoldMove;
				}
				else {
					PlayerState = STATE::RightDownHold;
				}
			}
			// =========================================================================
			// ###################### 마우스 캐릭터의 오른쪽 #############################
			// =========================================================================
			else																	// 22.5
			{																		// 360 - 22.5
				if (isMove)
				{
					PlayerState = STATE::RightHoldMove;
				}
				else {
					PlayerState = STATE::RightHold;
				}
			}
			
			
		}
		if (KEYMANAGER->IsOnceKeyUp(VK_LBUTTON) && !isHit && EVENT->Eventing() != true)
		{
			if (PowerTime > 0.2f)
			{
				SOUNDMANAGER->play("원거리공격차지");
				PlayerBullet->Fire("Ball", AttackAngle, _position, Vector2(48.f, 48.f), true, true);			// 기모아 탄!
			}
			else
			{
				SOUNDMANAGER->play("원거리공격");
				PlayerBullet->Fire("Bullet", AttackAngle, _position, Vector2(24.f, 24.f), false, true);			// 그냥 탄!
			}

			PowerTime = 0.f;
			ThrowCount++; ThrowTime = 0.f; FrameIndex = 0;
			if (ThrowCount > 2) ThrowCount = 1;
			isThrow = true; isHold = false;

			// =========================================================================
			// ###################### 마우스 캐릭터의 오른/위 쪽 #########################
			// =========================================================================
			if ( AttackAngle < (Math::PI / 2.f) - Math::PI / 8.f && 
				AttackAngle > (Math::PI / 8.f) )
			{
				PlayerState = STATE::RightUpThrow;
				cout << "오른위" << endl;
			}
			// =========================================================================
			// ###################### 마우스 캐릭터의 위쪽 ###############################
			// =========================================================================
			else if (AttackAngle < (Math::PI / 2.f) + Math::PI / 8.f &&		// 90 + 22.5
				AttackAngle > (Math::PI / 2.f) - Math::PI / 8.f)				// 90 - 22.5
			{
				PlayerState = STATE::UpThrow;
				cout << "위" << endl;
			}
			// =========================================================================
			// ################## 마우스 캐릭터의 왼/위 쪽 ###############################
			// =========================================================================
			else if (AttackAngle < Math::PI - (Math::PI / 8.f) &&			// 180 - 22.5
				AttackAngle >(Math::PI / 2.f) + Math::PI / 8.f)				// 90 + 22.5
			{
				PlayerState = STATE::LeftUpThrow;
				cout << "왼위" << endl;
			}
			// =========================================================================
			// ###################### 마우스 캐릭터의 왼쪽 ##############################
			// =========================================================================
			else if (AttackAngle < Math::PI + (Math::PI / 8.f) &&			// 180 + 22.5
				AttackAngle > Math::PI - Math::PI / 8.f)					// 180 - 22.5
			{
				PlayerState = STATE::LeftThrow;
				cout << "왼" << endl;
			}
			// =========================================================================
			// ################### 마우스 캐릭터의 왼/아래 쪽 ############################
			// =========================================================================
			else if (AttackAngle < (Math::PI + Math::PI / 2) - (Math::PI / 8.f) &&	// 270 - 22.5
				AttackAngle > Math::PI + (Math::PI / 8.f))							// 180 + 22.5
			{
				PlayerState = STATE::LeftDownThrow;
				cout << "왼아래" << endl;
			}
			// =========================================================================
			// ###################### 마우스 캐릭터의 아래 쪽 ############################
			// =========================================================================
			else if (AttackAngle < (Math::PI + Math::PI / 2) + (Math::PI / 8.f) &&	// 270 + 22.5
				AttackAngle > (Math::PI + Math::PI / 2) - (Math::PI / 8.f))			// 270 - 22.5
			{
				PlayerState = STATE::DownThrow;
				cout << "아래" << endl;
			}
			// =========================================================================
			// ################### 마우스 캐릭터의 오른/아래 쪽 ##########################
			// =========================================================================
			else if (AttackAngle < (Math::PI * 2.f) - (Math::PI / 8.f) &&			// 360 - 22.5
				AttackAngle > (Math::PI + Math::PI / 2) + (Math::PI / 8.f))			// 270 + 22.5
			{
				PlayerState = STATE::RightDownThrow;
				cout << "오른아래" << endl;
			}
			// =========================================================================
			// ###################### 마우스 캐릭터의 오른쪽 #############################
			// =========================================================================
			else																	// 22.5
			{																		// 360 - 22.5
				PlayerState = STATE::RightThrow;
				cout << "오른" << endl;
			}
		}
			
	}
	// =========================================================================
	// ######################## 캐릭터의 근접 공격 ###############################
	// =========================================================================
	else {
		AttackType = true;
		if (KEYMANAGER->IsOnceKeyDown(VK_LBUTTON) && AttackCount < 4 && !isHit && EVENT->Eventing() != true)
		{
			isAttack = true; AttackTime = 0.f; 
			FrameIndex = 0; AttackCount++;
			isHold = false; isThrow = false;

			if (AttackCount == 4) SOUNDMANAGER->play("근접공격2");
			else SOUNDMANAGER->play("근접공격1");

			switch (PlayerState)
			{
			case Player::STATE::IdleLeft:
				PlayerState = STATE::LeftAttack;
				break;
			case Player::STATE::IdleRight:
				PlayerState = STATE::RightAttack;
				break;
			case Player::STATE::IdleUp:
				PlayerState = STATE::UpAttack;
				break;
			case Player::STATE::IdleDown:
				PlayerState = STATE::DownAttack;
				break;
			case Player::STATE::Left:
				PlayerState = STATE::LeftAttack;
				break;
			case Player::STATE::Right:
				PlayerState = STATE::RightAttack;
				break;
			case Player::STATE::Up:
				PlayerState = STATE::UpAttack;
				break;
			case Player::STATE::Down:
				PlayerState = STATE::DownAttack;
				break;
			case Player::STATE::LeftThrow:
				PlayerState = STATE::LeftAttack;
				break;
			case Player::STATE::RightThrow:
				PlayerState = STATE::RightAttack;
				break;
			case Player::STATE::UpThrow:
				PlayerState = STATE::UpAttack;
				break;
			case Player::STATE::DownThrow:
				PlayerState = STATE::DownAttack;
				break;
			case Player::STATE::IdleLeftUp:
				PlayerState = STATE::LeftUpAttack;
				break;
			case Player::STATE::IdleRightUp:
				PlayerState = STATE::RightUpAttack;
				break;
			case Player::STATE::IdleLeftDown:
				PlayerState = STATE::LeftDownAttack;
				break;
			case Player::STATE::IdleRightDown:
				PlayerState = STATE::RightDownAttack;
				break;
			case Player::STATE::LeftUp:
				PlayerState = STATE::LeftUpAttack;
				break;
			case Player::STATE::RightUp:
				PlayerState = STATE::RightUpAttack;
				break;
			case Player::STATE::LeftDown:
				PlayerState = STATE::LeftDownAttack;
				break;
			case Player::STATE::RightDown:
				PlayerState = STATE::RightDownAttack;
				break;
			case Player::STATE::LeftUpThrow:
				PlayerState = STATE::LeftUpAttack;
				break;
			case Player::STATE::RightUpThrow:
				PlayerState = STATE::RightUpAttack;
				break;
			case Player::STATE::LeftDownThrow:
				PlayerState = STATE::LeftDownAttack;
				break;
			case Player::STATE::RightDownThrow:
				PlayerState = STATE::RightDownAttack;
				break;
			case Player::STATE::LeftHold:
				PlayerState = STATE::LeftAttack;
				break;
			case Player::STATE::LeftHoldMove:
				PlayerState = STATE::LeftAttack;
				break;
			case Player::STATE::LeftDownHold:
				PlayerState = STATE::LeftDownAttack;
				break;
			case Player::STATE::LeftDownHoldMove:
				PlayerState = STATE::LeftDownAttack;
				break;
			case Player::STATE::DownHold:
				PlayerState = STATE::DownAttack;
				break;
			case Player::STATE::DownHoldMove:
				PlayerState = STATE::DownAttack;
				break;
			case Player::STATE::RightDownHold:
				PlayerState = STATE::RightDownAttack;
				break;
			case Player::STATE::RightDownHoldMove:
				PlayerState = STATE::RightDownAttack;
				break;
			case Player::STATE::RightHold:
				PlayerState = STATE::RightAttack;
				break;
			case Player::STATE::RightHoldMove:
				PlayerState = STATE::RightAttack;
				break;
			case Player::STATE::RightUpHold:
				PlayerState = STATE::RightUpAttack;
				break;
			case Player::STATE::RightUpHoldMove:
				PlayerState = STATE::RightUpAttack;
				break;
			case Player::STATE::UpHold:
				PlayerState = STATE::UpAttack;
				break;
			case Player::STATE::UpHoldMove:
				PlayerState = STATE::UpAttack;
				break;
			case Player::STATE::LeftUpHold:
				PlayerState = STATE::LeftUpAttack;
				break;
			case Player::STATE::LeftUpHoldMove:
				PlayerState = STATE::LeftUpAttack;
				break;
			case Player::STATE::LeftStop:
				PlayerState = STATE::LeftAttack;
				break;

			case Player::STATE::RightStop:
				PlayerState = STATE::RightAttack;
				break;

			case Player::STATE::UpStop:
				PlayerState = STATE::UpAttack;
				break;

			case Player::STATE::DownStop:
				PlayerState = STATE::DownAttack;
				break;
			case Player::STATE::LeftUpStop:
				PlayerState = STATE::LeftUpAttack;
				break;

			case Player::STATE::LeftDownStop:
				PlayerState = STATE::LeftDownAttack;
				break;

			case Player::STATE::RightUpStop:
				PlayerState = STATE::RightUpAttack;
				break;

			case Player::STATE::RightDownStop:
				PlayerState = STATE::RightDownAttack;
				break;
			}

			// 뿅
			switch (PlayerState)
			{
			case Player::STATE::LeftAttack:
				AttackDir = -90.f;
				if (AttackCount == 1 || AttackCount == 3)
				{
					EFFECT->ShowEffect("근접공격1", Vector2(_position.x - 25, _position.y), Vector2(64, 45), -90.f);
				}
				else if (AttackCount == 2)
				{
					EFFECT->ShowEffect("근접공격2", Vector2(_position.x - 25, _position.y), Vector2(64, 45), -90.f);
				}
				break;
			case Player::STATE::RightAttack:
				AttackDir = 90.f;
				if (AttackCount == 1 || AttackCount == 3)
				{
					EFFECT->ShowEffect("근접공격1", Vector2(_position.x + 25, _position.y), Vector2(64, 45), 90.f);
				}
				else if (AttackCount == 2)
				{
					EFFECT->ShowEffect("근접공격2", Vector2(_position.x + 25, _position.y), Vector2(64, 45), 90.f);
				}
				break;
			case Player::STATE::UpAttack:
				AttackDir = 0.f;
				if (AttackCount == 1 || AttackCount == 3)
				{
					EFFECT->ShowEffect("근접공격1", Vector2(_position.x, _position.y - 25), Vector2(64, 45), 0.f);
				}
				else if (AttackCount == 2)
				{
					EFFECT->ShowEffect("근접공격2", Vector2(_position.x, _position.y - 25), Vector2(64, 45), 0.f);
				}
				break;
			case Player::STATE::DownAttack:
				AttackDir = -180.f;
				if (AttackCount == 1 || AttackCount == 3)
				{
					EFFECT->ShowEffect("근접공격1", Vector2(_position.x, _position.y + 25), Vector2(64, 45), -180.f);
				}
				else if (AttackCount == 2)
				{
					EFFECT->ShowEffect("근접공격2", Vector2(_position.x, _position.y + 25), Vector2(64, 45), -180.f);
				}
				break;
			case Player::STATE::LeftUpAttack:
				AttackDir = -45.f;
				if (AttackCount == 1 || AttackCount == 3)
				{
					EFFECT->ShowEffect("근접공격1", Vector2(_position.x - 25, _position.y - 25), Vector2(64, 45), -45.f);
				}
				else if (AttackCount == 2)
				{
					EFFECT->ShowEffect("근접공격2", Vector2(_position.x - 25, _position.y - 25), Vector2(64, 45), -45.f);
				}
				break;
			case Player::STATE::RightUpAttack:
				AttackDir = 45.f;
				if (AttackCount == 1 || AttackCount == 3)
				{
					EFFECT->ShowEffect("근접공격1", Vector2(_position.x + 25, _position.y - 25), Vector2(64, 45), 45.f);
				}
				else if (AttackCount == 2)
				{
					EFFECT->ShowEffect("근접공격2", Vector2(_position.x + 25, _position.y - 25), Vector2(64, 45), 45.f);
				}
				break;
			case Player::STATE::LeftDownAttack:
				AttackDir = -135.f;
				if (AttackCount == 1 || AttackCount == 3)
				{
					EFFECT->ShowEffect("근접공격1", Vector2(_position.x - 25, _position.y + 25), Vector2(64, 45), -135.f);
				}
				else if (AttackCount == 2)
				{
					EFFECT->ShowEffect("근접공격2", Vector2(_position.x - 25, _position.y + 25), Vector2(64, 45), -135.f);
				}
				break;
			case Player::STATE::RightDownAttack:
				AttackDir = -305.f;
				if (AttackCount == 1 || AttackCount == 3)
				{
					EFFECT->ShowEffect("근접공격1", Vector2(_position.x + 25, _position.y + 25), Vector2(64, 45), -225.f);
				}
				else if (AttackCount == 2)
				{
					EFFECT->ShowEffect("근접공격2", Vector2(_position.x + 25, _position.y + 25), Vector2(64, 45), -225.f);
				}
				break;
			}
		}
	}
	if (AttackTime > 0.5f)
	{
		AttackCount = 0;
		isAttack = false;
	}

	if (ThrowTime > 0.4f)
	{
		ThrowCount = 0;
		isThrow = false;
	}

	PlayerBullet->Update();
}

void Player::ImageFrameUpdate()
{
	Vector2 renderPos = CAMERA->GetRelativeVector2(_position);
	switch (PlayerState)
	{
	case Player::STATE::Left:
		this->FrameUpdate(5);
		break;
	case Player::STATE::Right:
		this->FrameUpdate(5);
		break;
	case Player::STATE::Up:
		this->FrameUpdate(5);
		break;
	case Player::STATE::Down:
		this->FrameUpdate(5);
		break;
	case Player::STATE::LeftStop:
		FrameCount += TIMEMANAGER->getElapedTime();
		if (FrameCount > 0.1f)
		{
			FrameIndex++;
			if (FrameIndex > 2)
			{
				PlayerState = STATE::IdleLeft;
			}
			while (FrameCount > 0.1f)
				FrameCount -= 0.1f;
		}
		break;
	case Player::STATE::RightStop:
		FrameCount += TIMEMANAGER->getElapedTime();
		if (FrameCount > 0.1f)
		{
			FrameIndex++;
			if (FrameIndex > 2)
			{
				PlayerState = STATE::IdleRight;
			}
			while (FrameCount > 0.1f)
				FrameCount -= 0.1f;
		}
		break;
	case Player::STATE::UpStop:
		FrameCount += TIMEMANAGER->getElapedTime();
		if (FrameCount > 0.1f)
		{
			FrameIndex++;
			if (FrameIndex > 2)
			{
				PlayerState = STATE::IdleUp;
			}
			while (FrameCount > 0.1f)
				FrameCount -= 0.1f;
		}
		break;
	case Player::STATE::DownStop:
		FrameCount += TIMEMANAGER->getElapedTime();
		if (FrameCount > 0.1f)
		{
			FrameIndex++;
			if (FrameIndex > 2)
			{
				PlayerState = STATE::IdleDown;
			}
			while (FrameCount > 0.1f)
				FrameCount -= 0.1f;
		}
		break;
	case Player::STATE::LeftUp:
		this->FrameUpdate(5);
		break;
	case Player::STATE::RightUp:
		this->FrameUpdate(5);
		break;
	case Player::STATE::LeftDown:
		this->FrameUpdate(5);
		break;
	case Player::STATE::RightDown:
		this->FrameUpdate(5);
		break;
	case Player::STATE::LeftUpStop:
		FrameCount += TIMEMANAGER->getElapedTime();
		if (FrameCount > 0.1f)
		{
			FrameIndex++;
			if (FrameIndex > 2)
			{
				PlayerState = STATE::IdleLeftUp;
			}
			while (FrameCount > 0.1f)
				FrameCount -= 0.1f;
		}
		break;
	case Player::STATE::RightUpStop:
		FrameCount += TIMEMANAGER->getElapedTime();
		if (FrameCount > 0.1f)
		{
			FrameIndex++;
			if (FrameIndex > 2)
			{
				PlayerState = STATE::IdleRightUp;
			}
			while (FrameCount > 0.1f)
				FrameCount -= 0.1f;
		}
		break;
	case Player::STATE::LeftDownStop:
		FrameCount += TIMEMANAGER->getElapedTime();
		if (FrameCount > 0.1f)
		{
			FrameIndex++;
			if (FrameIndex > 2)
			{
				PlayerState = STATE::IdleLeftDown;
			}
			while (FrameCount > 0.1f)
				FrameCount -= 0.1f;
		}
		break;
	case Player::STATE::RightDownStop:
		FrameCount += TIMEMANAGER->getElapedTime();
		if (FrameCount > 0.1f)
		{
			FrameIndex++;
			if (FrameIndex > 2) {
				PlayerState = STATE::IdleRightDown;
			}
			while (FrameCount > 0.1f)
				FrameCount -= 0.1f;
		}
		break;
	case Player::STATE::LeftHold:
		FrameUpdate(3);
		break;
	case Player::STATE::RightHold:
		FrameUpdate(3);
		break;
	case Player::STATE::UpHold:
		FrameUpdate(3);
		break;
	case Player::STATE::DownHold:
		FrameUpdate(3);
		break;

	case STATE::LeftHoldMove:
		FrameUpdate(2, 0.25f);
		break;
	case STATE::RightHoldMove:
		FrameUpdate(2, 0.25f);
		break;
	case STATE::UpHoldMove:
		FrameUpdate(2, 0.25f);
		break;
	case STATE::DownHoldMove:
		FrameUpdate(2, 0.25f);
		break;

	case Player::STATE::LeftThrow:
		if (ThrowCount == 1)
		{
			FrameHold(5);
		}
		else if ( ThrowCount == 2)
		{
			FrameHold(5);
		}
		else {
			PlayerState = STATE::IdleLeft;
			FrameIndex = 0;
		}
		break;
	case Player::STATE::RightThrow:
		if (ThrowCount == 1)
		{
			FrameHold(5);
		}
		else if (ThrowCount == 2)
		{
			FrameHold(5);
		}
		else {
			PlayerState = STATE::IdleRight;
			FrameIndex = 0;
		}
		break;
	case Player::STATE::UpThrow:
		if (ThrowCount == 1)
		{
			FrameHold(4);
		}
		else if (ThrowCount == 2)
		{
			FrameHold(4);
		}
		else {
			PlayerState = STATE::IdleUp;
			FrameIndex = 0;
		}
		break;
	case Player::STATE::DownThrow:
		if (ThrowCount == 1)
		{
			FrameHold(5);
		}
		else if (ThrowCount == 2)
		{
			FrameHold(5);
		}
		else {
			PlayerState = STATE::IdleDown;
			FrameIndex = 0;
		}
		break;
	case Player::STATE::LeftAttack:
		if (AttackCount == 1)
		{
			FrameHoldMove(5, 250.f);
		}
		else if (AttackCount == 2)
		{
			FrameHoldMove(5, 250.f);
		}
		else if (AttackCount == 3)
		{
			FrameHoldMove(5, 250.f);
		}
		else if (AttackCount == 4) {
			FrameHoldMove2(9, 0.025f, 450.f);
		}
		else {
			PlayerState = STATE::IdleLeft;
		}
		break;
	case Player::STATE::RightAttack:
		if (AttackCount == 1)
		{
			FrameHoldMove(5, 250.f);
		}
		else if (AttackCount == 2)
		{
			FrameHoldMove(5, 250.f);
		}
		else if (AttackCount == 3)
		{
			FrameHoldMove(5, 250.f);
		}
		else if (AttackCount == 4) {
			FrameHoldMove2(9, 0.025f, 450.f);
		}
		else {
			PlayerState = STATE::IdleRight;
		}
		break;
	case Player::STATE::UpAttack:
		if (AttackCount == 1)
		{
			FrameHoldMove(5, 250.f);
		}
		else if (AttackCount == 2)
		{
			FrameHoldMove(5, 250.f);
		}
		else if (AttackCount == 3)
		{
			FrameHoldMove(5, 250.f);
		}
		else if (AttackCount == 4) {
			FrameHoldMove2(9, 0.025f, 450.f);
		}
		else {
			PlayerState = STATE::IdleUp;
		}
		break;
	case Player::STATE::DownAttack:
		if (AttackCount == 1)
		{
			FrameHoldMove(5, 250.f);
		}
		else if (AttackCount == 2)
		{
			FrameHoldMove(5, 250.f);
		}
		else if (AttackCount == 3)
		{
			FrameHoldMove(5, 250.f);
		}
		else if (AttackCount == 4) {
			FrameHoldMove2(9, 0.025f, 450.f);
		}
		else {
			PlayerState = STATE::IdleDown;
		}
		break;
	case Player::STATE::LeftUpHold:
		FrameUpdate(3);
		break;
	case Player::STATE::RightUpHold:
		FrameUpdate(3);
		break;
	case Player::STATE::LeftDownHold:
		FrameUpdate(3);
		break;
	case Player::STATE::RightDownHold:
		FrameUpdate(3);
		break;
	case STATE::LeftUpHoldMove:
		FrameUpdate(2, 0.25f);
		break;
	case STATE::RightUpHoldMove:
		FrameUpdate(2, 0.25f);
		break;
	case STATE::LeftDownHoldMove:
		FrameUpdate(2, 0.25f);
		break;
	case STATE::RightDownHoldMove:
		FrameUpdate(2, 0.25f);
		break;

	case Player::STATE::LeftUpThrow:
		if (ThrowCount == 1)
		{
			FrameHold(5);
		}
		else if (ThrowCount == 2)
		{
			FrameHold(4);
		}
		else {
			PlayerState = STATE::IdleLeftUp;
			FrameIndex = 0;
		}
		break;
	case Player::STATE::RightUpThrow:
		if (ThrowCount == 1)
		{
			FrameHold(4);
		}
		else if (ThrowCount == 2)
		{
			FrameHold(5);
		}
		else {
			PlayerState = STATE::IdleRightUp;
			FrameIndex = 0;
		}
		break;
	case Player::STATE::LeftDownThrow:
		if (ThrowCount == 1)
		{
			FrameHold(5);
		}
		else if (ThrowCount == 2)
		{
			FrameHold(5);
		}
		else {
			PlayerState = STATE::IdleLeftDown;
			FrameIndex = 0;
		}
		break;
	case Player::STATE::RightDownThrow:
		if (ThrowCount == 1)
		{
			FrameHold(5);
		}
		else if (ThrowCount == 2)
		{
			FrameHold(5);
		}
		else {
			PlayerState = STATE::IdleRightDown;
			FrameIndex = 0;
		}
		break;
	case Player::STATE::LeftUpAttack:
		if (AttackCount == 1)
		{
			FrameHoldMove(5, 250.f);
		}
		else if (AttackCount == 2)
		{
			FrameHoldMove(5, 250.f);
		}
		else if (AttackCount == 3)
		{
			FrameHoldMove(5, 250.f);
		}
		else if (AttackCount == 4) {
			FrameHoldMove2(9, 0.025f, 450.f);
		}
		else {
			PlayerState = STATE::IdleLeftUp;
		}
		break;
	case Player::STATE::RightUpAttack:
		if (AttackCount == 1)
		{
			FrameHoldMove(5, 250.f);
		}
		else if (AttackCount == 2)
		{
			FrameHoldMove(5, 250.f);
		}
		else if (AttackCount == 3)
		{
			FrameHoldMove(5, 250.f);
		}
		else if (AttackCount == 4) {
			FrameHoldMove2(9, 0.025f, 450.f);
		}
		else {
			PlayerState = STATE::IdleRightUp;
		}
		break;
	case Player::STATE::LeftDownAttack:
		if (AttackCount == 1)
		{
			FrameHoldMove(5, 250.f);
		}
		else if (AttackCount == 2)
		{
			FrameHoldMove(5, 250.f);
		}
		else if (AttackCount == 3)
		{
			FrameHoldMove(5, 250.f);
		}
		else if (AttackCount == 4) {
			FrameHoldMove2(9, 0.025f, 450.f);
		}
		else {
			PlayerState = STATE::IdleLeftDown;
		}
		break;
	case Player::STATE::RightDownAttack:
		if (AttackCount == 1)
		{
			FrameHoldMove(5, 250.f);
		}
		else if (AttackCount == 2)
		{
			FrameHoldMove(5, 250.f);
		}
		else if (AttackCount == 3)
		{
			FrameHoldMove(5, 250.f);
		}
		else if (AttackCount == 4) {
			FrameHoldMove2(9, 0.025f, 450.f);
		}
		else {
			PlayerState = STATE::IdleRightDown;
		}
		break;
	case Player::STATE::Rolling:
		FrameCount += TIMEMANAGER->getElapedTime();
		if (FrameCount > 0.025f)
		{
			FrameIndex++;
			while (FrameCount > 0.025f)
				FrameCount -= 0.025f;
			if (FrameIndex > 7)
			{
				FrameIndex--;
				isRoll = false;
				switch (PlayerDir)
				{
				case Player::Dir::Left:
					PlayerState = STATE::LeftStop;
					break;
				case Player::Dir::Right:
					PlayerState = STATE::RightStop;
					break;
				case Player::Dir::Up:
					PlayerState = STATE::UpStop;
					break;
				case Player::Dir::Down:
					PlayerState = STATE::DownStop;
					break;
				case Player::Dir::LeftUp:
					PlayerState = STATE::LeftUpStop;
					break;
				case Player::Dir::RightUp:
					PlayerState = STATE::RightUpStop;
					break;
				case Player::Dir::LeftDown:
					PlayerState = STATE::LeftDownStop;
					break;
				case Player::Dir::RightDown:
					PlayerState = STATE::RightDownStop;
					break;
				}
			}
			
		}
		
		break;
	case Player::STATE::LeftHit:
		FrameHold(2);
		break;
	case Player::STATE::RightHit:
		FrameHold(2);
		break;
	case Player::STATE::UpHit:
		FrameHold(2);
		break;
	case Player::STATE::DownHit:
		FrameHold(2);
		break;
	case Player::STATE::LeftUpHit:
		FrameHold(2);
		break;
	case Player::STATE::LeftDownHit:
		FrameHold(2);
		break;
	case Player::STATE::RightUpHit:
		FrameHold(2);
		break;
	case Player::STATE::RightDownHit:
		FrameHold(2);
		break;
	}
	
}

void Player::ImageResource()
{
	
}

void Player::ImageRender()
{
	Vector2 renderPos = CAMERA->GetRelativeVector2(_position);
	switch (PlayerState)
	{
	case Player::STATE::IdleLeft:
		IMAGEMANAGER->FindImage("LeftIdle")->SetAlpha(Alpha);
		IMAGEMANAGER->FindImage("LeftIdle")->SetSize(Vector2(64, 64) * CAMERA->GetZoom());
		IMAGEMANAGER->FindImage("LeftIdle")->Render(renderPos.x, renderPos.y);
		break;
	case Player::STATE::IdleRight:
		IMAGEMANAGER->FindImage("RightIdle")->SetAlpha(Alpha);
		IMAGEMANAGER->FindImage("RightIdle")->SetSize(Vector2(64, 64) * CAMERA->GetZoom());
		IMAGEMANAGER->FindImage("RightIdle")->Render(renderPos.x, renderPos.y);
		break;
	case Player::STATE::IdleUp:
		IMAGEMANAGER->FindImage("UpIdle")->SetAlpha(Alpha);
		IMAGEMANAGER->FindImage("UpIdle")->SetSize(Vector2(64, 64) * CAMERA->GetZoom());
		IMAGEMANAGER->FindImage("UpIdle")->Render(renderPos.x, renderPos.y);
		break;
	case Player::STATE::IdleDown:
		IMAGEMANAGER->FindImage("DownIdle")->SetAlpha(Alpha);
		IMAGEMANAGER->FindImage("DownIdle")->SetSize(Vector2(64, 64) * CAMERA->GetZoom());
		IMAGEMANAGER->FindImage("DownIdle")->Render(renderPos.x, renderPos.y);
		break;
	case Player::STATE::Left:
		IMAGEMANAGER->FindImage("LeftMove")->SetAlpha(Alpha);
		IMAGEMANAGER->FindImage("LeftMove")->SetSize(Vector2(64, 64) * CAMERA->GetZoom());
		IMAGEMANAGER->FindImage("LeftMove")->FrameRender(renderPos.x, renderPos.y, FrameIndex, 0);
		break;
	case Player::STATE::Right:
		IMAGEMANAGER->FindImage("RightMove")->SetAlpha(Alpha);
		IMAGEMANAGER->FindImage("RightMove")->SetSize(Vector2(64, 64) * CAMERA->GetZoom());
		IMAGEMANAGER->FindImage("RightMove")->FrameRender(renderPos.x, renderPos.y, FrameIndex, 0);
		break;
	case Player::STATE::Up:
		IMAGEMANAGER->FindImage("UpMove")->SetAlpha(Alpha);
		IMAGEMANAGER->FindImage("UpMove")->SetSize(Vector2(64, 64) * CAMERA->GetZoom());
		IMAGEMANAGER->FindImage("UpMove")->FrameRender(renderPos.x, renderPos.y, FrameIndex, 0);
		break;
	case Player::STATE::Down:
		IMAGEMANAGER->FindImage("DownMove")->SetAlpha(Alpha);
		IMAGEMANAGER->FindImage("DownMove")->SetSize(Vector2(64, 64) * CAMERA->GetZoom());
		IMAGEMANAGER->FindImage("DownMove")->FrameRender(renderPos.x, renderPos.y, FrameIndex, 0);
		break;
	case Player::STATE::LeftStop:
		IMAGEMANAGER->FindImage("LeftStop")->SetAlpha(Alpha);
		IMAGEMANAGER->FindImage("LeftStop")->SetSize(Vector2(64, 64) * CAMERA->GetZoom());
		IMAGEMANAGER->FindImage("LeftStop")->FrameRender(renderPos.x, renderPos.y, FrameIndex, 0);
		break;
	case Player::STATE::RightStop:
		IMAGEMANAGER->FindImage("RightStop")->SetAlpha(Alpha);
		IMAGEMANAGER->FindImage("RightStop")->SetSize(Vector2(64, 64) * CAMERA->GetZoom());
		IMAGEMANAGER->FindImage("RightStop")->FrameRender(renderPos.x, renderPos.y, FrameIndex, 0);
		break;
	case Player::STATE::UpStop:
		IMAGEMANAGER->FindImage("UpStop")->SetAlpha(Alpha);
		IMAGEMANAGER->FindImage("UpStop")->SetSize(Vector2(64, 64) * CAMERA->GetZoom());
		IMAGEMANAGER->FindImage("UpStop")->FrameRender(renderPos.x, renderPos.y, FrameIndex, 0);
		break;
	case Player::STATE::DownStop:
		IMAGEMANAGER->FindImage("DownStop")->SetAlpha(Alpha);
		IMAGEMANAGER->FindImage("DownStop")->SetSize(Vector2(64, 64) * CAMERA->GetZoom());
		IMAGEMANAGER->FindImage("DownStop")->FrameRender(renderPos.x, renderPos.y, FrameIndex, 0);
		break;
	case Player::STATE::IdleLeftUp:
		IMAGEMANAGER->FindImage("LeftUpIdle")->SetAlpha(Alpha);
		IMAGEMANAGER->FindImage("LeftUpIdle")->SetSize(Vector2(64, 64) * CAMERA->GetZoom());
		IMAGEMANAGER->FindImage("LeftUpIdle")->Render(renderPos.x, renderPos.y);
		break;
	case Player::STATE::IdleRightUp:
		IMAGEMANAGER->FindImage("RightUpIdle")->SetAlpha(Alpha);
		IMAGEMANAGER->FindImage("RightUpIdle")->SetSize(Vector2(64, 64) * CAMERA->GetZoom());
		IMAGEMANAGER->FindImage("RightUpIdle")->Render(renderPos.x, renderPos.y);
		break;
	case Player::STATE::IdleLeftDown:
		IMAGEMANAGER->FindImage("LeftDownIdle")->SetAlpha(Alpha);
		IMAGEMANAGER->FindImage("LeftDownIdle")->SetSize(Vector2(64, 64) * CAMERA->GetZoom());
		IMAGEMANAGER->FindImage("LeftDownIdle")->Render(renderPos.x, renderPos.y);
		break;
	case Player::STATE::IdleRightDown:
		IMAGEMANAGER->FindImage("RightDownIdle")->SetAlpha(Alpha);
		IMAGEMANAGER->FindImage("RightDownIdle")->SetSize(Vector2(64, 64) * CAMERA->GetZoom());
		IMAGEMANAGER->FindImage("RightDownIdle")->Render(renderPos.x, renderPos.y);
		break;
	case Player::STATE::LeftUp:
		IMAGEMANAGER->FindImage("LeftUpMove")->SetAlpha(Alpha);
		IMAGEMANAGER->FindImage("LeftUpMove")->SetSize(Vector2(64, 64) * CAMERA->GetZoom());
		IMAGEMANAGER->FindImage("LeftUpMove")->FrameRender(renderPos.x, renderPos.y, FrameIndex, 0);
		break;
	case Player::STATE::RightUp:
		IMAGEMANAGER->FindImage("RightUpMove")->SetAlpha(Alpha);
		IMAGEMANAGER->FindImage("RightUpMove")->SetSize(Vector2(64, 64) * CAMERA->GetZoom());
		IMAGEMANAGER->FindImage("RightUpMove")->FrameRender(renderPos.x, renderPos.y, FrameIndex, 0);
		break;
	case Player::STATE::LeftDown:
		IMAGEMANAGER->FindImage("LeftDownMove")->SetAlpha(Alpha);
		IMAGEMANAGER->FindImage("LeftDownMove")->SetSize(Vector2(64, 64) * CAMERA->GetZoom());
		IMAGEMANAGER->FindImage("LeftDownMove")->FrameRender(renderPos.x, renderPos.y, FrameIndex, 0);
		break;
	case Player::STATE::RightDown:
		IMAGEMANAGER->FindImage("RightDownMove")->SetAlpha(Alpha);
		IMAGEMANAGER->FindImage("RightDownMove")->SetSize(Vector2(64, 64) * CAMERA->GetZoom());
		IMAGEMANAGER->FindImage("RightDownMove")->FrameRender(renderPos.x, renderPos.y, FrameIndex, 0);
		break;
	case Player::STATE::LeftUpStop:
		IMAGEMANAGER->FindImage("LeftUpStop")->SetAlpha(Alpha);
		IMAGEMANAGER->FindImage("LeftUpStop")->SetSize(Vector2(64, 64) * CAMERA->GetZoom());
		IMAGEMANAGER->FindImage("LeftUpStop")->FrameRender(renderPos.x, renderPos.y, FrameIndex, 0);
		break;
	case Player::STATE::RightUpStop:
		IMAGEMANAGER->FindImage("RightUpStop")->SetAlpha(Alpha);
		IMAGEMANAGER->FindImage("RightUpStop")->SetSize(Vector2(64, 64) * CAMERA->GetZoom());
		IMAGEMANAGER->FindImage("RightUpStop")->FrameRender(renderPos.x, renderPos.y, FrameIndex, 0);
		break;
	case Player::STATE::LeftDownStop:
		IMAGEMANAGER->FindImage("LeftDownStop")->SetAlpha(Alpha);
		IMAGEMANAGER->FindImage("LeftDownStop")->SetSize(Vector2(64, 64) * CAMERA->GetZoom());
		IMAGEMANAGER->FindImage("LeftDownStop")->FrameRender(renderPos.x, renderPos.y, FrameIndex, 0);
		break;
	case Player::STATE::RightDownStop:
		IMAGEMANAGER->FindImage("RightDownStop")->SetAlpha(Alpha);
		IMAGEMANAGER->FindImage("RightDownStop")->SetSize(Vector2(64, 64) * CAMERA->GetZoom());
		IMAGEMANAGER->FindImage("RightDownStop")->FrameRender(renderPos.x, renderPos.y, FrameIndex, 0);
		break;
	case Player::STATE::LeftHold:
		IMAGEMANAGER->FindImage("LeftHold")->SetAlpha(Alpha);
		IMAGEMANAGER->FindImage("LeftHold")->SetSize(Vector2(64, 64) * CAMERA->GetZoom());
		IMAGEMANAGER->FindImage("LeftHold")->FrameRender(renderPos.x, renderPos.y, FrameIndex, 0);
		break;
	case Player::STATE::RightHold:
		IMAGEMANAGER->FindImage("RightHold")->SetAlpha(Alpha);
		IMAGEMANAGER->FindImage("RightHold")->SetSize(Vector2(64, 64) * CAMERA->GetZoom());
		IMAGEMANAGER->FindImage("RightHold")->FrameRender(renderPos.x, renderPos.y, FrameIndex, 0);
		break;
	case Player::STATE::UpHold:
		IMAGEMANAGER->FindImage("UpHold")->SetAlpha(Alpha);
		IMAGEMANAGER->FindImage("UpHold")->SetSize(Vector2(64, 64) * CAMERA->GetZoom());
		IMAGEMANAGER->FindImage("UpHold")->FrameRender(renderPos.x, renderPos.y, FrameIndex, 0);
		break;
	case Player::STATE::DownHold:
		IMAGEMANAGER->FindImage("DownHold")->SetAlpha(Alpha);
		IMAGEMANAGER->FindImage("DownHold")->SetSize(Vector2(64, 64) * CAMERA->GetZoom());
		IMAGEMANAGER->FindImage("DownHold")->FrameRender(renderPos.x, renderPos.y, FrameIndex, 0);
		break;
	case Player::STATE::LeftThrow:
		if (ThrowCount == 1) {
			IMAGEMANAGER->FindImage("LeftThrow")->SetAlpha(Alpha);
			IMAGEMANAGER->FindImage("LeftThrow")->SetSize(Vector2(64, 64) * CAMERA->GetZoom());
			IMAGEMANAGER->FindImage("LeftThrow")->FrameRender(renderPos.x, renderPos.y, FrameIndex, 0);
		}
		else if ( ThrowCount == 2){
			IMAGEMANAGER->FindImage("LeftThrow2")->SetAlpha(Alpha);
			IMAGEMANAGER->FindImage("LeftThrow2")->SetSize(Vector2(64, 64) * CAMERA->GetZoom());
			IMAGEMANAGER->FindImage("LeftThrow2")->FrameRender(renderPos.x, renderPos.y, FrameIndex, 0);
		}
		break;
	case Player::STATE::RightThrow:
		if (ThrowCount == 1) {
			IMAGEMANAGER->FindImage("RightThrow")->SetAlpha(Alpha);
			IMAGEMANAGER->FindImage("RightThrow")->SetSize(Vector2(64, 64) * CAMERA->GetZoom());
			IMAGEMANAGER->FindImage("RightThrow")->FrameRender(renderPos.x, renderPos.y, FrameIndex, 0);
		}
		else if (ThrowCount == 2) {
			IMAGEMANAGER->FindImage("RightThrow2")->SetAlpha(Alpha);
			IMAGEMANAGER->FindImage("RightThrow2")->SetSize(Vector2(64, 64) * CAMERA->GetZoom());
			IMAGEMANAGER->FindImage("RightThrow2")->FrameRender(renderPos.x, renderPos.y, FrameIndex, 0);
		}
		break;
	case Player::STATE::UpThrow:
		if (ThrowCount == 1) {
			IMAGEMANAGER->FindImage("UpThrow")->SetAlpha(Alpha);
			IMAGEMANAGER->FindImage("UpThrow")->SetSize(Vector2(64, 64) * CAMERA->GetZoom());
			IMAGEMANAGER->FindImage("UpThrow")->FrameRender(renderPos.x, renderPos.y, FrameIndex, 0);
		}
		else if (ThrowCount == 2) {
			IMAGEMANAGER->FindImage("UpThrow2")->SetAlpha(Alpha);
			IMAGEMANAGER->FindImage("UpThrow2")->SetSize(Vector2(64, 64) * CAMERA->GetZoom());
			IMAGEMANAGER->FindImage("UpThrow2")->FrameRender(renderPos.x, renderPos.y, FrameIndex, 0);
		}
		break;
	case Player::STATE::DownThrow:
		if (ThrowCount == 1) {
			IMAGEMANAGER->FindImage("DownThrow")->SetAlpha(Alpha);
			IMAGEMANAGER->FindImage("DownThrow")->SetSize(Vector2(64, 64) * CAMERA->GetZoom());
			IMAGEMANAGER->FindImage("DownThrow")->FrameRender(renderPos.x, renderPos.y, FrameIndex, 0);
		}
		else if (ThrowCount == 2) {
			IMAGEMANAGER->FindImage("DownThrow2")->SetAlpha(Alpha);
			IMAGEMANAGER->FindImage("DownThrow2")->SetSize(Vector2(64, 64) * CAMERA->GetZoom());
			IMAGEMANAGER->FindImage("DownThrow2")->FrameRender(renderPos.x, renderPos.y, FrameIndex, 0);
		}
		break;
	case Player::STATE::LeftAttack:
		if (AttackCount == 1)
		{
			IMAGEMANAGER->FindImage("LeftAttack")->SetAlpha(Alpha);
			IMAGEMANAGER->FindImage("LeftAttack")->SetSize(Vector2(64, 64) * CAMERA->GetZoom());
			IMAGEMANAGER->FindImage("LeftAttack")->FrameRender(renderPos.x, renderPos.y, FrameIndex, 0);
		}
		else if (AttackCount == 2) {
			IMAGEMANAGER->FindImage("LeftAttack2")->SetAlpha(Alpha);
			IMAGEMANAGER->FindImage("LeftAttack2")->SetSize(Vector2(64, 64) * CAMERA->GetZoom());
			IMAGEMANAGER->FindImage("LeftAttack2")->FrameRender(renderPos.x, renderPos.y, FrameIndex, 0);
		}
		else if (AttackCount == 3) {
			IMAGEMANAGER->FindImage("LeftAttack")->SetAlpha(Alpha);
			IMAGEMANAGER->FindImage("LeftAttack")->SetSize(Vector2(64, 64) * CAMERA->GetZoom());
			IMAGEMANAGER->FindImage("LeftAttack")->FrameRender(renderPos.x, renderPos.y, FrameIndex, 0);
		}
		else if (AttackCount == 4) {
			IMAGEMANAGER->FindImage("LeftAttack3")->SetAlpha(Alpha);
			IMAGEMANAGER->FindImage("LeftAttack3")->SetSize(Vector2(64, 64) * CAMERA->GetZoom());
			IMAGEMANAGER->FindImage("LeftAttack3")->FrameRender(renderPos.x, renderPos.y, FrameIndex, 0);

			AttackDir -= 27.f;;
			Vector2 dirvector; dirvector.SetDirVector(-AttackDir);
			dirvector = dirvector * 30.f;
			EFFECT->ShowEffect("근접공격3", _position + dirvector, Vector2(32, 155), AttackDir, 0.05f);
		}
		break;
	case Player::STATE::RightAttack:
		if (AttackCount == 1)
		{
			IMAGEMANAGER->FindImage("RightAttack")->SetAlpha(Alpha);
			IMAGEMANAGER->FindImage("RightAttack")->SetSize(Vector2(64, 64) * CAMERA->GetZoom());
			IMAGEMANAGER->FindImage("RightAttack")->FrameRender(renderPos.x, renderPos.y, FrameIndex, 0);
		}
		else if (AttackCount == 2) {
			IMAGEMANAGER->FindImage("RightAttack2")->SetAlpha(Alpha);
			IMAGEMANAGER->FindImage("RightAttack2")->SetSize(Vector2(64, 64) * CAMERA->GetZoom());
			IMAGEMANAGER->FindImage("RightAttack2")->FrameRender(renderPos.x, renderPos.y, FrameIndex, 0);
		}
		else if (AttackCount == 3) {
			IMAGEMANAGER->FindImage("RightAttack")->SetAlpha(Alpha);
			IMAGEMANAGER->FindImage("RightAttack")->SetSize(Vector2(64, 64) * CAMERA->GetZoom());
			IMAGEMANAGER->FindImage("RightAttack")->FrameRender(renderPos.x, renderPos.y, FrameIndex, 0);
		}
		else if (AttackCount == 4) {
			IMAGEMANAGER->FindImage("RightAttack3")->SetAlpha(Alpha);
			IMAGEMANAGER->FindImage("RightAttack3")->SetSize(Vector2(64, 64) * CAMERA->GetZoom());
			IMAGEMANAGER->FindImage("RightAttack3")->FrameRender(renderPos.x, renderPos.y, FrameIndex, 0);

			AttackDir -= 27.f;;
			Vector2 dirvector; dirvector.SetDirVector(-AttackDir);
			dirvector = dirvector * 30.f;
			EFFECT->ShowEffect("근접공격3", _position + dirvector, Vector2(32, 155), AttackDir, 0.05f);
		}
		break;
	case Player::STATE::UpAttack:
		if (AttackCount == 1)
		{
			IMAGEMANAGER->FindImage("UpAttack")->SetAlpha(Alpha);
			IMAGEMANAGER->FindImage("UpAttack")->SetSize(Vector2(64, 64) * CAMERA->GetZoom());
			IMAGEMANAGER->FindImage("UpAttack")->FrameRender(renderPos.x, renderPos.y, FrameIndex, 0);

		}
		else if (AttackCount == 2) {
			IMAGEMANAGER->FindImage("UpAttack2")->SetAlpha(Alpha);
			IMAGEMANAGER->FindImage("UpAttack2")->SetSize(Vector2(64, 64) * CAMERA->GetZoom());
			IMAGEMANAGER->FindImage("UpAttack2")->FrameRender(renderPos.x, renderPos.y, FrameIndex, 0);
		}
		else if (AttackCount == 3) {
			IMAGEMANAGER->FindImage("UpAttack")->SetAlpha(Alpha);
			IMAGEMANAGER->FindImage("UpAttack")->SetSize(Vector2(64, 64) * CAMERA->GetZoom());
			IMAGEMANAGER->FindImage("UpAttack")->FrameRender(renderPos.x, renderPos.y, FrameIndex, 0);
		}
		else if (AttackCount == 4) {
			IMAGEMANAGER->FindImage("UpAttack3")->SetAlpha(Alpha);
			IMAGEMANAGER->FindImage("UpAttack3")->SetSize(Vector2(64, 64) * CAMERA->GetZoom());
			IMAGEMANAGER->FindImage("UpAttack3")->FrameRender(renderPos.x, renderPos.y, FrameIndex, 0);

			AttackDir -= 27.f;;
			Vector2 dirvector; dirvector.SetDirVector(-AttackDir);
			dirvector = dirvector * 30.f;
			EFFECT->ShowEffect("근접공격3", _position + dirvector, Vector2(32, 155), AttackDir, 0.05f);
		}
		break;
	case Player::STATE::DownAttack:
		if (AttackCount == 1)
		{
			IMAGEMANAGER->FindImage("DownAttack")->SetAlpha(Alpha);
			IMAGEMANAGER->FindImage("DownAttack")->SetSize(Vector2(64, 64) * CAMERA->GetZoom());
			IMAGEMANAGER->FindImage("DownAttack")->FrameRender(renderPos.x, renderPos.y, FrameIndex, 0);
		}
		else if (AttackCount == 2) {
			IMAGEMANAGER->FindImage("DownAttack2")->SetAlpha(Alpha);
			IMAGEMANAGER->FindImage("DownAttack2")->SetSize(Vector2(64, 64) * CAMERA->GetZoom());
			IMAGEMANAGER->FindImage("DownAttack2")->FrameRender(renderPos.x, renderPos.y, FrameIndex, 0);
		}
		else if (AttackCount == 3) {
			IMAGEMANAGER->FindImage("DownAttack")->SetAlpha(Alpha);
			IMAGEMANAGER->FindImage("DownAttack")->SetSize(Vector2(64, 64) * CAMERA->GetZoom());
			IMAGEMANAGER->FindImage("DownAttack")->FrameRender(renderPos.x, renderPos.y, FrameIndex, 0);
		}
		else if (AttackCount == 4) {
			IMAGEMANAGER->FindImage("DownAttack3")->SetAlpha(Alpha);
			IMAGEMANAGER->FindImage("DownAttack3")->SetSize(Vector2(64, 64) * CAMERA->GetZoom());
			IMAGEMANAGER->FindImage("DownAttack3")->FrameRender(renderPos.x, renderPos.y, FrameIndex, 0);

			AttackDir -= 27.f;
			Vector2 dirvector; dirvector.SetDirVector(-AttackDir);
			dirvector = dirvector * 30.f;;
			EFFECT->ShowEffect("근접공격3", _position + dirvector, Vector2(32, 155), AttackDir, 0.05f);
		}
		break;
	case Player::STATE::LeftUpHold:
		IMAGEMANAGER->FindImage("LeftUpHold")->SetAlpha(Alpha);
		IMAGEMANAGER->FindImage("LeftUpHold")->SetSize(Vector2(64, 64) * CAMERA->GetZoom());
		IMAGEMANAGER->FindImage("LeftUpHold")->FrameRender(renderPos.x, renderPos.y, FrameIndex, 0);
		break;
	case Player::STATE::RightUpHold:
		IMAGEMANAGER->FindImage("RightUpHold")->SetAlpha(Alpha);
		IMAGEMANAGER->FindImage("RightUpHold")->SetSize(Vector2(64, 64) * CAMERA->GetZoom());
		IMAGEMANAGER->FindImage("RightUpHold")->FrameRender(renderPos.x, renderPos.y, FrameIndex, 0);
		break;
	case Player::STATE::LeftDownHold:
		IMAGEMANAGER->FindImage("LeftDownHold")->SetAlpha(Alpha);
		IMAGEMANAGER->FindImage("LeftDownHold")->SetSize(Vector2(64, 64) * CAMERA->GetZoom());
		IMAGEMANAGER->FindImage("LeftDownHold")->FrameRender(renderPos.x, renderPos.y, FrameIndex, 0);
		break;
	case Player::STATE::RightDownHold:
		IMAGEMANAGER->FindImage("RightDownHold")->SetAlpha(Alpha);
		IMAGEMANAGER->FindImage("RightDownHold")->SetSize(Vector2(64, 64) * CAMERA->GetZoom());
		IMAGEMANAGER->FindImage("RightDownHold")->FrameRender(renderPos.x, renderPos.y, FrameIndex, 0);
		break;
	case Player::STATE::LeftUpThrow:
		if (ThrowCount == 1) {
			IMAGEMANAGER->FindImage("LeftUpThrow")->SetAlpha(Alpha);
			IMAGEMANAGER->FindImage("LeftUpThrow")->SetSize(Vector2(64, 64) * CAMERA->GetZoom());
			IMAGEMANAGER->FindImage("LeftUpThrow")->FrameRender(renderPos.x, renderPos.y, FrameIndex, 0);
		}
		else if (ThrowCount == 2) {
			IMAGEMANAGER->FindImage("LeftUpThrow2")->SetAlpha(Alpha);
			IMAGEMANAGER->FindImage("LeftUpThrow2")->SetSize(Vector2(64, 64) * CAMERA->GetZoom());
			IMAGEMANAGER->FindImage("LeftUpThrow2")->FrameRender(renderPos.x, renderPos.y, FrameIndex, 0);
		}
		break;
	case Player::STATE::RightUpThrow:
		if (ThrowCount == 1) {
			IMAGEMANAGER->FindImage("RightUpThrow")->SetAlpha(Alpha);
			IMAGEMANAGER->FindImage("RightUpThrow")->SetSize(Vector2(64, 64) * CAMERA->GetZoom());
			IMAGEMANAGER->FindImage("RightUpThrow")->FrameRender(renderPos.x, renderPos.y, FrameIndex, 0);
		}
		else if (ThrowCount == 2) {
			IMAGEMANAGER->FindImage("RightUpThrow2")->SetAlpha(Alpha);
			IMAGEMANAGER->FindImage("RightUpThrow2")->SetSize(Vector2(64, 64) * CAMERA->GetZoom());
			IMAGEMANAGER->FindImage("RightUpThrow2")->FrameRender(renderPos.x, renderPos.y, FrameIndex, 0);
		}
		break;
	case Player::STATE::LeftDownThrow:
		if (ThrowCount == 1) {
			IMAGEMANAGER->FindImage("LeftDownThrow")->SetAlpha(Alpha);
			IMAGEMANAGER->FindImage("LeftDownThrow")->SetSize(Vector2(64, 64) * CAMERA->GetZoom());
			IMAGEMANAGER->FindImage("LeftDownThrow")->FrameRender(renderPos.x, renderPos.y, FrameIndex, 0);
		}
		else if (ThrowCount == 2) {
			IMAGEMANAGER->FindImage("LeftDownThrow2")->SetAlpha(Alpha);
			IMAGEMANAGER->FindImage("LeftDownThrow2")->SetSize(Vector2(64, 64) * CAMERA->GetZoom());
			IMAGEMANAGER->FindImage("LeftDownThrow2")->FrameRender(renderPos.x, renderPos.y, FrameIndex, 0);
		}
		break;
	case Player::STATE::RightDownThrow:
		if (ThrowCount == 1) {
			IMAGEMANAGER->FindImage("RightDownThrow")->SetAlpha(Alpha);
			IMAGEMANAGER->FindImage("RightDownThrow")->SetSize(Vector2(64, 64) * CAMERA->GetZoom());
			IMAGEMANAGER->FindImage("RightDownThrow")->FrameRender(renderPos.x, renderPos.y, FrameIndex, 0);
		}
		else if (ThrowCount == 2) {
			IMAGEMANAGER->FindImage("RightDownThrow2")->SetAlpha(Alpha);
			IMAGEMANAGER->FindImage("RightDownThrow2")->SetSize(Vector2(64, 64) * CAMERA->GetZoom());
			IMAGEMANAGER->FindImage("RightDownThrow2")->FrameRender(renderPos.x, renderPos.y, FrameIndex, 0);
		}
		break;
	case Player::STATE::LeftUpAttack:
		if (AttackCount == 1)
		{
			IMAGEMANAGER->FindImage("LeftUpAttack")->SetAlpha(Alpha);
			IMAGEMANAGER->FindImage("LeftUpAttack")->SetSize(Vector2(64, 64) * CAMERA->GetZoom());
			IMAGEMANAGER->FindImage("LeftUpAttack")->FrameRender(renderPos.x, renderPos.y, FrameIndex, 0);
		}
		else if (AttackCount == 2) {
			IMAGEMANAGER->FindImage("LeftUpAttack2")->SetAlpha(Alpha);
			IMAGEMANAGER->FindImage("LeftUpAttack2")->SetSize(Vector2(64, 64) * CAMERA->GetZoom());
			IMAGEMANAGER->FindImage("LeftUpAttack2")->FrameRender(renderPos.x, renderPos.y, FrameIndex, 0);
		}
		else if (AttackCount == 3) {
			IMAGEMANAGER->FindImage("LeftUpAttack")->SetAlpha(Alpha);
			IMAGEMANAGER->FindImage("LeftUpAttack")->SetSize(Vector2(64, 64) * CAMERA->GetZoom());
			IMAGEMANAGER->FindImage("LeftUpAttack")->FrameRender(renderPos.x, renderPos.y, FrameIndex, 0);
		}
		else if (AttackCount == 4) {
			IMAGEMANAGER->FindImage("LeftUpAttack3")->SetAlpha(Alpha);
			IMAGEMANAGER->FindImage("LeftUpAttack3")->SetSize(Vector2(64, 64) * CAMERA->GetZoom());
			IMAGEMANAGER->FindImage("LeftUpAttack3")->FrameRender(renderPos.x, renderPos.y, FrameIndex, 0);

			AttackDir -= 27.f;
			Vector2 dirvector; dirvector.SetDirVector(-AttackDir);
			dirvector = dirvector * 30.f;
			EFFECT->ShowEffect("근접공격3", _position + dirvector, Vector2(32, 155), AttackDir, 0.05f);
		}
		break;
	case Player::STATE::RightUpAttack:
		if (AttackCount == 1)
		{
			IMAGEMANAGER->FindImage("RightUpAttack")->SetAlpha(Alpha);
			IMAGEMANAGER->FindImage("RightUpAttack")->SetSize(Vector2(64, 64) * CAMERA->GetZoom());
			IMAGEMANAGER->FindImage("RightUpAttack")->FrameRender(renderPos.x, renderPos.y, FrameIndex, 0);
		}
		else if (AttackCount == 2) {
			IMAGEMANAGER->FindImage("RightUpAttack2")->SetAlpha(Alpha);
			IMAGEMANAGER->FindImage("RightUpAttack2")->SetSize(Vector2(64, 64) * CAMERA->GetZoom());
			IMAGEMANAGER->FindImage("RightUpAttack2")->FrameRender(renderPos.x, renderPos.y, FrameIndex, 0);
		}
		else if (AttackCount == 3) {
			IMAGEMANAGER->FindImage("RightUpAttack")->SetAlpha(Alpha);
			IMAGEMANAGER->FindImage("RightUpAttack")->SetSize(Vector2(64, 64) * CAMERA->GetZoom());
			IMAGEMANAGER->FindImage("RightUpAttack")->FrameRender(renderPos.x, renderPos.y, FrameIndex, 0);
		}
		else if (AttackCount == 4) {
			IMAGEMANAGER->FindImage("RightUpAttack3")->SetAlpha(Alpha);
			IMAGEMANAGER->FindImage("RightUpAttack3")->SetSize(Vector2(64, 64) * CAMERA->GetZoom());
			IMAGEMANAGER->FindImage("RightUpAttack3")->FrameRender(renderPos.x, renderPos.y, FrameIndex, 0);

			AttackDir -= 27.f;
			Vector2 dirvector; dirvector.SetDirVector(-AttackDir);
			dirvector = dirvector * 30.f;
			EFFECT->ShowEffect("근접공격3", _position + dirvector, Vector2(32, 155), AttackDir, 0.05f);
		}
		break;
	case Player::STATE::LeftDownAttack:
		if (AttackCount == 1)
		{
			IMAGEMANAGER->FindImage("LeftDownAttack")->SetAlpha(Alpha);
			IMAGEMANAGER->FindImage("LeftDownAttack")->SetSize(Vector2(64, 64) * CAMERA->GetZoom());
			IMAGEMANAGER->FindImage("LeftDownAttack")->FrameRender(renderPos.x, renderPos.y, FrameIndex, 0);
		}
		else if (AttackCount == 2) {
			IMAGEMANAGER->FindImage("LeftDownAttack2")->SetAlpha(Alpha);
			IMAGEMANAGER->FindImage("LeftDownAttack2")->SetSize(Vector2(64, 64) * CAMERA->GetZoom());
			IMAGEMANAGER->FindImage("LeftDownAttack2")->FrameRender(renderPos.x, renderPos.y, FrameIndex, 0);
		}
		else if (AttackCount == 3) {
			IMAGEMANAGER->FindImage("LeftDownAttack")->SetAlpha(Alpha);
			IMAGEMANAGER->FindImage("LeftDownAttack")->SetSize(Vector2(64, 64) * CAMERA->GetZoom());
			IMAGEMANAGER->FindImage("LeftDownAttack")->FrameRender(renderPos.x, renderPos.y, FrameIndex, 0);
		}
		else if (AttackCount == 4) {
			IMAGEMANAGER->FindImage("LeftDownAttack3")->SetAlpha(Alpha);
			IMAGEMANAGER->FindImage("LeftDownAttack3")->SetSize(Vector2(64, 64) * CAMERA->GetZoom());
			IMAGEMANAGER->FindImage("LeftDownAttack3")->FrameRender(renderPos.x, renderPos.y, FrameIndex, 0);

			AttackDir -= 27.f;;
			Vector2 dirvector; dirvector.SetDirVector(-AttackDir);
			dirvector = dirvector * 30.f;
			EFFECT->ShowEffect("근접공격3", _position + dirvector, Vector2(32, 155), AttackDir, 0.05f);
		}
		break;
	case Player::STATE::RightDownAttack:
		if (AttackCount == 1)
		{
			IMAGEMANAGER->FindImage("RightDownAttack")->SetAlpha(Alpha);
			IMAGEMANAGER->FindImage("RightDownAttack")->SetSize(Vector2(64, 64) * CAMERA->GetZoom());
			IMAGEMANAGER->FindImage("RightDownAttack")->FrameRender(renderPos.x, renderPos.y, FrameIndex, 0);
		}
		else if (AttackCount == 2) {
			IMAGEMANAGER->FindImage("RightDownAttack2")->SetAlpha(Alpha);
			IMAGEMANAGER->FindImage("RightDownAttack2")->SetSize(Vector2(64, 64) * CAMERA->GetZoom());
			IMAGEMANAGER->FindImage("RightDownAttack2")->FrameRender(renderPos.x, renderPos.y, FrameIndex, 0);
		}
		else if (AttackCount == 3) {
			IMAGEMANAGER->FindImage("RightDownAttack")->SetAlpha(Alpha);
			IMAGEMANAGER->FindImage("RightDownAttack")->SetSize(Vector2(64, 64) * CAMERA->GetZoom());
			IMAGEMANAGER->FindImage("RightDownAttack")->FrameRender(renderPos.x, renderPos.y, FrameIndex, 0);
		}
		else if (AttackCount == 4) {
			IMAGEMANAGER->FindImage("RightDownAttack3")->SetAlpha(Alpha);
			IMAGEMANAGER->FindImage("RightDownAttack3")->SetSize(Vector2(64, 64) * CAMERA->GetZoom());
			IMAGEMANAGER->FindImage("RightDownAttack3")->FrameRender(renderPos.x, renderPos.y, FrameIndex, 0);

			AttackDir -= 27.f;;
			Vector2 dirvector; dirvector.SetDirVector(-AttackDir);
			dirvector = dirvector * 30.f;
			EFFECT->ShowEffect("근접공격3", _position + dirvector, Vector2(32, 155), AttackDir, 0.05f);
		}
		break;
	case Player::STATE::Rolling:
		IMAGEMANAGER->FindImage("Rolling")->SetAlpha(Alpha);
		IMAGEMANAGER->FindImage("Rolling")->SetSize(Vector2(64, 64) * CAMERA->GetZoom());
		IMAGEMANAGER->FindImage("Rolling")->FrameRender(renderPos.x, renderPos.y, FrameIndex, 0);

		DustDelay += TIMEMANAGER->getElapedTime();
		if (DustDelay > 0.05f)
		{
			while (DustDelay > 0.05f)
				DustDelay -= 0.05f;
			EFFECT->ShowEffect("먼지", Vector2(_position.x, _position.y + _size.y/2.f), Vector2(24, 32), true, 0, 0.05f);
		}
		
		break;
	case Player::STATE::LeftHoldMove:
		IMAGEMANAGER->FindImage("LeftHoldMove")->SetAlpha(Alpha);
		IMAGEMANAGER->FindImage("LeftHoldMove")->SetSize(Vector2(64, 64) * CAMERA->GetZoom());
		IMAGEMANAGER->FindImage("LeftHoldMove")->FrameRender(renderPos.x, renderPos.y, FrameIndex, 0);
		// 앙 여기띠
		break;
	case Player::STATE::RightHoldMove:
		IMAGEMANAGER->FindImage("RightHoldMove")->SetAlpha(Alpha);
		IMAGEMANAGER->FindImage("RightHoldMove")->SetSize(Vector2(64, 64) * CAMERA->GetZoom());
		IMAGEMANAGER->FindImage("RightHoldMove")->FrameRender(renderPos.x, renderPos.y, FrameIndex, 0);
		break;
	case Player::STATE::UpHoldMove:
		IMAGEMANAGER->FindImage("UpHoldMove")->SetAlpha(Alpha);
		IMAGEMANAGER->FindImage("UpHoldMove")->SetSize(Vector2(64, 64) * CAMERA->GetZoom());
		IMAGEMANAGER->FindImage("UpHoldMove")->FrameRender(renderPos.x, renderPos.y, FrameIndex, 0);
		break;
	case Player::STATE::DownHoldMove:
		IMAGEMANAGER->FindImage("DownHoldMove")->SetAlpha(Alpha);
		IMAGEMANAGER->FindImage("DownHoldMove")->SetSize(Vector2(64, 64) * CAMERA->GetZoom());
		IMAGEMANAGER->FindImage("DownHoldMove")->FrameRender(renderPos.x, renderPos.y, FrameIndex, 0);
		break;
	case Player::STATE::LeftUpHoldMove:
		IMAGEMANAGER->FindImage("LeftUpHoldMove")->SetAlpha(Alpha);
		IMAGEMANAGER->FindImage("LeftUpHoldMove")->SetSize(Vector2(64, 64) * CAMERA->GetZoom());
		IMAGEMANAGER->FindImage("LeftUpHoldMove")->FrameRender(renderPos.x, renderPos.y, FrameIndex, 0);
		break;
	case Player::STATE::RightUpHoldMove:
		IMAGEMANAGER->FindImage("RightUpHoldMove")->SetAlpha(Alpha);
		IMAGEMANAGER->FindImage("RightUpHoldMove")->SetSize(Vector2(64, 64) * CAMERA->GetZoom());
		IMAGEMANAGER->FindImage("RightUpHoldMove")->FrameRender(renderPos.x, renderPos.y, FrameIndex, 0);
		break;
	case Player::STATE::LeftDownHoldMove:
		IMAGEMANAGER->FindImage("LeftDownHoldMove")->SetAlpha(Alpha);
		IMAGEMANAGER->FindImage("LeftDownHoldMove")->SetSize(Vector2(64, 64) * CAMERA->GetZoom());
		IMAGEMANAGER->FindImage("LeftDownHoldMove")->FrameRender(renderPos.x, renderPos.y, FrameIndex, 0);
		break;
	case Player::STATE::RightDownHoldMove:
		IMAGEMANAGER->FindImage("RightDownHoldMove")->SetAlpha(Alpha);
		IMAGEMANAGER->FindImage("RightDownHoldMove")->SetSize(Vector2(64, 64) * CAMERA->GetZoom());
		IMAGEMANAGER->FindImage("RightDownHoldMove")->FrameRender(renderPos.x, renderPos.y, FrameIndex, 0);
		break;

	case Player::STATE::LeftHit:
		IMAGEMANAGER->FindImage("LeftHit")->SetAlpha(Alpha);
		IMAGEMANAGER->FindImage("LeftHit")->SetSize(Vector2(64, 64) * CAMERA->GetZoom());
		IMAGEMANAGER->FindImage("LeftHit")->FrameRender(renderPos.x, renderPos.y, FrameIndex, 0);
		break;
	case Player::STATE::RightHit:
		IMAGEMANAGER->FindImage("RightHit")->SetAlpha(Alpha);
		IMAGEMANAGER->FindImage("RightHit")->SetSize(Vector2(64, 64) * CAMERA->GetZoom());
		IMAGEMANAGER->FindImage("RightHit")->FrameRender(renderPos.x, renderPos.y, FrameIndex, 0);
		break;
	case Player::STATE::UpHit:
		IMAGEMANAGER->FindImage("UpHit")->SetAlpha(Alpha);
		IMAGEMANAGER->FindImage("UpHit")->SetSize(Vector2(64, 64) * CAMERA->GetZoom());
		IMAGEMANAGER->FindImage("UpHit")->FrameRender(renderPos.x, renderPos.y, FrameIndex, 0);
		break;
	case Player::STATE::DownHit:
		IMAGEMANAGER->FindImage("DownHit")->SetAlpha(Alpha);
		IMAGEMANAGER->FindImage("DownHit")->SetSize(Vector2(64, 64) * CAMERA->GetZoom());
		IMAGEMANAGER->FindImage("DownHit")->FrameRender(renderPos.x, renderPos.y, FrameIndex, 0);
		break;
	case Player::STATE::LeftUpHit:
		IMAGEMANAGER->FindImage("LeftUpHit")->SetAlpha(Alpha);
		IMAGEMANAGER->FindImage("LeftUpHit")->SetSize(Vector2(64, 64) * CAMERA->GetZoom());
		IMAGEMANAGER->FindImage("LeftUpHit")->FrameRender(renderPos.x, renderPos.y, FrameIndex, 0);
		break;
	case Player::STATE::LeftDownHit:
		IMAGEMANAGER->FindImage("LeftDownHit")->SetAlpha(Alpha);
		IMAGEMANAGER->FindImage("LeftDownHit")->SetSize(Vector2(64, 64) * CAMERA->GetZoom());
		IMAGEMANAGER->FindImage("LeftDownHit")->FrameRender(renderPos.x, renderPos.y, FrameIndex, 0);
		break;
	case Player::STATE::RightUpHit:
		IMAGEMANAGER->FindImage("RightUpHit")->SetAlpha(Alpha);
		IMAGEMANAGER->FindImage("RightUpHit")->SetSize(Vector2(64, 64) * CAMERA->GetZoom());
		IMAGEMANAGER->FindImage("RightUpHit")->FrameRender(renderPos.x, renderPos.y, FrameIndex, 0);
		break;
	case Player::STATE::RightDownHit:
		IMAGEMANAGER->FindImage("RightDownHit")->SetAlpha(Alpha);
		IMAGEMANAGER->FindImage("RightDownHit")->SetSize(Vector2(64, 64) * CAMERA->GetZoom());
		IMAGEMANAGER->FindImage("RightDownHit")->FrameRender(renderPos.x, renderPos.y, FrameIndex, 0);
		break;

	case Player::STATE::Death:
		switch (PlayerDir)
		{
		case Player::Dir::Left:
			IMAGEMANAGER->FindImage("LeftHit")->SetAlpha(Alpha);
			IMAGEMANAGER->FindImage("LeftHit")->SetSize(Vector2(64, 64)* CAMERA->GetZoom());
			IMAGEMANAGER->FindImage("LeftHit")->FrameRender(renderPos.x, renderPos.y, 1, 0);
			break;
		case Player::Dir::Right:
			IMAGEMANAGER->FindImage("RightHit")->SetAlpha(Alpha);
			IMAGEMANAGER->FindImage("RightHit")->SetSize(Vector2(64, 64)* CAMERA->GetZoom());
			IMAGEMANAGER->FindImage("RightHit")->FrameRender(renderPos.x, renderPos.y, 1, 0);
			break;
		case Player::Dir::Up:
			IMAGEMANAGER->FindImage("UpHit")->SetAlpha(Alpha);
			IMAGEMANAGER->FindImage("UpHit")->SetSize(Vector2(64, 64)* CAMERA->GetZoom());
			IMAGEMANAGER->FindImage("UpHit")->FrameRender(renderPos.x, renderPos.y, 1, 0);
			break;
		case Player::Dir::Down:
			IMAGEMANAGER->FindImage("DownHit")->SetAlpha(Alpha);
			IMAGEMANAGER->FindImage("DownHit")->SetSize(Vector2(64, 64)* CAMERA->GetZoom());
			IMAGEMANAGER->FindImage("DownHit")->FrameRender(renderPos.x, renderPos.y, 1, 0);
			break;
		case Player::Dir::LeftUp:
			IMAGEMANAGER->FindImage("LeftUpHit")->SetAlpha(Alpha);
			IMAGEMANAGER->FindImage("LeftUpHit")->SetSize(Vector2(64, 64)* CAMERA->GetZoom());
			IMAGEMANAGER->FindImage("LeftUpHit")->FrameRender(renderPos.x, renderPos.y, 1, 0);
			break;
		case Player::Dir::RightUp:
			IMAGEMANAGER->FindImage("RightUpHit")->SetAlpha(Alpha);
			IMAGEMANAGER->FindImage("RightUpHit")->SetSize(Vector2(64, 64)* CAMERA->GetZoom());
			IMAGEMANAGER->FindImage("RightUpHit")->FrameRender(renderPos.x, renderPos.y, 1, 0);
			break;
		case Player::Dir::LeftDown:
			IMAGEMANAGER->FindImage("LeftDownHit")->SetAlpha(Alpha);
			IMAGEMANAGER->FindImage("LeftDownHit")->SetSize(Vector2(64, 64)* CAMERA->GetZoom());
			IMAGEMANAGER->FindImage("LeftDownHit")->FrameRender(renderPos.x, renderPos.y, 1, 0);
			break;
		case Player::Dir::RightDown:
			IMAGEMANAGER->FindImage("RightDownHit")->SetAlpha(Alpha);
			IMAGEMANAGER->FindImage("RightDownHit")->SetSize(Vector2(64, 64)* CAMERA->GetZoom());
			IMAGEMANAGER->FindImage("RightDownHit")->FrameRender(renderPos.x, renderPos.y, 1, 0);
			break;
		}
		break;
	}

	if (isAttack && AttackType)
	{
		IMAGEMANAGER->FindImage("공격범위")->SetSize(Vector2(250, 250));
		IMAGEMANAGER->FindImage("공격범위")->Render(CAMERA->GetRelativeVector2(_position).x, CAMERA->GetRelativeVector2(_position).y);
	}
}

void Player::UpdateRect()
{
	HitRect = RectMakeCenter(_position.x, _position.y, _size.x / 2.f, _size.y);
	ColRect = RectMakeCenter(_position.x, _position.y + _size.y / 3.f, _size.x, _size.y / 3.f);
	if (AttackCount != 4)
	{
		switch (PlayerState)
		{
		case Player::STATE::LeftAttack:
			AttackRect = RectMakeCenter(_position.x - _size.x / 2.f, _position.y, _size.x, _size.y);
			break;
		case Player::STATE::RightAttack:
			AttackRect = RectMakeCenter(_position.x + _size.x / 2.f, _position.y, _size.x, _size.y);
			break;
		case Player::STATE::UpAttack:
			AttackRect = RectMakeCenter(_position.x, _position.y - _size.y / 2.f, _size.x, _size.y);
			break;
		case Player::STATE::DownAttack:
			AttackRect = RectMakeCenter(_position.x, _position.y + _size.y / 2.f, _size.x, _size.y);
			break;
		case Player::STATE::LeftUpAttack:
			AttackRect = RectMakeCenter(_position.x - _size.x / 2.f, _position.y - _size.y / 2.f, _size.x, _size.y);
			break;
		case Player::STATE::RightUpAttack:
			AttackRect = RectMakeCenter(_position.x + _size.x / 2.f, _position.y - _size.y / 2.f, _size.x, _size.y);
			break;
		case Player::STATE::LeftDownAttack:
			AttackRect = RectMakeCenter(_position.x - _size.x / 2.f, _position.y + _size.x / 2.f, _size.x, _size.y);
			break;
		case Player::STATE::RightDownAttack:
			AttackRect = RectMakeCenter(_position.x + _size.x / 2.f, _position.y + _size.x / 2.f, _size.x, _size.y);
			break;
		}
	}
	else
	{
		AttackRect = RectMakeCenter(_position.x, _position.y, _size.x * 2.2f, _size.y * 2.2f);
	}
	
}

void Player::AlphaUpdate()
{
	if (AlphaStart)
	{
		AlphaTime += TIMEMANAGER->getElapedTime();
		AlphaChangeTime += TIMEMANAGER->getElapedTime();
		if (AlphaTime > 2.5f)
		{
			while (AlphaTime > 2.5f)
				AlphaTime -= 2.5f;
			AlphaTime = 0.f;
			AlphaChangeTime = 0.f;
			AlphaStart = false;
			Alpha = 1.f;
		}
		else
		{
			if (AlphaChangeTime > 0.2f)
			{
				while (AlphaChangeTime > 0.2f)
					AlphaChangeTime -= 0.2f;
				isAlpha = !isAlpha;
			}

			if (isAlpha)
			{
				Alpha = 0.4f;
			}
			else
			{
				Alpha = 1.f;
			}
		}
	}
}

void Player::DirCheck()
{
	if (MoveValue == Vector2(0.f, 0.f)) return;
	float Degree = MoveValue.GetDegree();

	if (Degree <= 200.5f &&
		Degree > 157.5f)
	{
		PlayerDir = Dir::Left;
	}
	else if (Degree <= 157.5f &&
		Degree > 112.5f)
	{
		PlayerDir = Dir::LeftUp;
	}
	else if (Degree <= 247.5f &&
		Degree > 200.5f)
	{
		PlayerDir = Dir::LeftDown;
	}
	else if (Degree <= 112.5f &&
		Degree > 67.5f)
	{
		PlayerDir = Dir::Up;
	}
	else if (Degree <= 67.5f &&
		Degree > 22.5f)
	{
		PlayerDir = Dir::RightUp;
	}
	else if (Degree <= 337.5f &&
		Degree > 292.5f)
	{
		PlayerDir = Dir::RightDown;
	}
	else if (Degree <= 292.5f &&
		Degree > 247.5f)
	{
		PlayerDir = Dir::Down;
	}
	else {
		PlayerDir = Dir::Right;
	}
}

void Player::FinalAttack(float startangle)
{
	float angle = startangle;
	Vector2 dirvector; dirvector.SetDirVector(-angle);
	dirvector = dirvector * 45.f;
	EFFECT->ShowEffect("근접공격3", _position + dirvector, Vector2(32, 124), angle);
}

void Player::FrameUpdate(int max)
{
	FrameCount += TIMEMANAGER->getElapedTime();
	if (FrameCount > 0.1f)
	{
		FrameIndex++;
		if (FrameIndex > max)
		{
			FrameIndex = 0;
		}
		while (FrameCount > 0.1f)
			FrameCount -= 0.1f;
	}
}

void Player::FrameUpdate(int max, float fra)
{
	FrameCount += TIMEMANAGER->getElapedTime();
	if (FrameCount > fra)
	{
		FrameIndex++;
		if (FrameIndex > max)
		{
			FrameIndex = 0;
		}
		while (FrameCount > fra)
			FrameCount -= fra;
	}
}

void Player::FrameHold(int max)
{
	FrameCount += TIMEMANAGER->getElapedTime();
	if (FrameCount > 0.05f)
	{
		FrameIndex++;
		if (FrameIndex > max)
		{
			FrameIndex--;
			isRoll = false;
		}
		while (FrameCount > 0.05f)
			FrameCount -= 0.05f;
	}
}

void Player::FrameHold(int max, float frame)
{
	FrameCount += TIMEMANAGER->getElapedTime();
	if (FrameCount > frame)
	{
		FrameIndex++;
		if (FrameIndex > max)
		{
			FrameIndex--;
			isRoll = false;
		}
		while (FrameCount > frame)
			FrameCount -= frame;
	}
}

void Player::FrameHoldMove(int max,float speed)
{
	FrameCount += TIMEMANAGER->getElapedTime();
	if (FrameCount > 0.05f)
	{
		FrameIndex++;		
		if (FrameIndex > max)
		{
			FrameIndex--;
		}
		while (FrameCount > 0.05f)
			FrameCount -= 0.05f;
	}

	if (FrameIndex != max)
	{
		if (isMove)
			_position += MoveValue.Normalize() * speed *TIMEMANAGER->getElapedTime();
	}
	
	this->UpdateRect();
}

void Player::FrameHoldMove(int max, float frame, float speed)
{
	FrameCount += TIMEMANAGER->getElapedTime();
	if (FrameCount > frame)
	{
		FrameIndex++;
		if (FrameIndex > max)
		{
			FrameIndex--;
		}
		while (FrameCount > frame)
			FrameCount -= frame;
	}

	if (FrameIndex != max)
	{
		if (isMove)
			_position += MoveValue * speed *TIMEMANAGER->getElapedTime();
	}

	this->UpdateRect();
}

void Player::FrameHoldMove2(int max, float frame, float speed)
{
	switch (PlayerState)
	{
	case Player::STATE::LeftAttack:
		MoveValue = Vector2(-1.f, 0.f);
		break;
	case Player::STATE::RightAttack:
		MoveValue = Vector2(1.f, 0.f);
		break;
	case Player::STATE::UpAttack:
		MoveValue = Vector2(0.f, -1.f);
		break;
	case Player::STATE::DownAttack:
		MoveValue = Vector2(0.f, 1.f);
		break;
	case Player::STATE::LeftUpAttack:
		MoveValue = Vector2(-1.f, -1.f);
		break;
	case Player::STATE::RightUpAttack:
		MoveValue = Vector2(1.f, -1.f);
		break;
	case Player::STATE::LeftDownAttack:
		MoveValue = Vector2(-1.f, 1.f);
		break;
	case Player::STATE::RightDownAttack:
		MoveValue = Vector2(1.f, 1.f);
		break;
	}
	FrameCount += TIMEMANAGER->getElapedTime();
	if (FrameCount > frame)
	{
		FrameIndex++;
		if (FrameIndex > max)
		{
			FrameIndex--;
		}
		while (FrameCount > frame)
			FrameCount -= frame;
	}

	if (FrameIndex != max)
	{
		_position += MoveValue.Normalize() * speed *TIMEMANAGER->getElapedTime();
		this->UpdateRect();
	}
}



Player::Player(Vector2 pos)
	: GameObject(pos)
{
	_name = "플레이어";
}

Player::~Player()
{
}
