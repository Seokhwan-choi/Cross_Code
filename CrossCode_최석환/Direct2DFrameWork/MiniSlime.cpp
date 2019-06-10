#include "stdafx.h"
#include "Player.h"
#include "MiniSlime.h"
#include "Slime.h"
#include "ProgressBar.h"

MiniSlime::MiniSlime(string name, Vector2 size, Vector2 position, Pivot pivot)
	: GameObject(name, size, position, pivot)
{
	SlimePattern = Pattern::Idle;	
	FrameCount = 0.f;				// 작은 슬라임 프레임 카운트 0.f 초기화
	FrameIndex = 0;					// 작은 슬라임 프레임 인덱스 0 초기화
	Gravity = 0.f;					// 작은 슬라임 중력 0.f
	Speed = 100.f;					// 작은 슬라임 이동속도 400. f 초기화
	Hp = 150;						// 작은 슬라임 체력 80 초기화
	EffectDelay = 0;				// 작은 슬라임 원 EffectDelay 시간 0.f 초기화
	AttackTime = 0.f;				// 작은 슬라임 공격 준비 시간 0.f 초기화
	isGather = false;				// 작은 슬라임 모였니..? 
	isBreak = true;					// 작은 슬라임 흩어졌니..?
	isCrush = true;					// 작은 슬라임 충돌가능
	isCol = true;					// 작은 슬라임 벽 충돌 가능
	isAttack = false;				// 작은 슬라임 공격 중
	ActiveTime = 0.f;				// 작은 슬라임 사라질꺼야

	RebirthPos = _position;							 // 작은 슬라임 되살아나는 좌표
	JumpPos = _position;							 // 작은 슬라임 점프용 좌표
	Shadow = IMAGEMANAGER->FindImage("슬라임그림자");	 // 작은 슬라임 그림자 Image

	AfterTime = 0.f;								// 작은 슬라임 잔상 Delay
	GatherUpTime = 0.f;								// 작은 슬라임 GatherUp 지속 시간
	GatherIdleTime = 0.f;							// 작은 슬라임 GatherIdle 지속 시간
	IdleChangeTime = 0.f;							// 작은 슬라임 GatherIdle 위/아래 변화 시간
	IdleChange = false;								// 작은 슬라임 GatherIdle 변화 여부

	isChasor = false;
	HitTime = 0.f;									// 작은 슬라임 Hit 지속시간
	AlphaStart = false;								// 작은 슬라임 알파 시작
	isAlpha = false;								// 작은 슬라임 알파 여부
	Alpha = 1.f;									// 작은 슬라임 알파
	AlphaTime = 0.f;								// 작은 슬라임 알파 지속시간
	AlphaChangeTime = 0.f;							// 작은 슬라임 알파 변화시간

	isDying = false;								// 작은 슬라임 죽어간다.
	DieCheck = 0.f;									// 작은 슬라임 죽음Check

	Index = 0;										// 작은 슬라임 최단 경로 Index

	HpBar = new ProgressBar("작은체력바", Vector2(36.f, 8.f),
		Vector2(_position.x - _size.x / 2.f + 10.f, _position.y + _size.y / 2));
	HpBar->Init((float)Hp, 36.f, 8.f);
	UIMANAGER->addUi(ObjectType::UI, HpBar);

	// ========================
	// 슬라임 원 Effect 초기화
	// 알파값 ( Alpha ) : 0.2f
	// 크기 ( Size ) : 1.f, 1.f
	// ========================
	for (int i = 0; i < 2; ++i)
	{
		if (i == 0)
		{
			Circle[0].mImage = IMAGEMANAGER->FindImage("슬라임원");;
			Circle[0].Size = Vector2(1.f, 1.f);
		}
		else
		{
			Circle[1].mImage = IMAGEMANAGER->FindImage("슬라임원");
			Circle[1].Size = Vector2(1.f, 1.f);
		}
			
		Circle[i].Alpha = 0.2f;
	}
	player = (Player*)OBJECTMANAGER->FindObject(ObjectType::Player, "플레이어");
}

MiniSlime::~MiniSlime()
{

}


void MiniSlime::Init()
{
	
}

void MiniSlime::Update()
{
		this->UpdateState();					// 작은 슬라임 상태 업데이트	
		this->DistanceCheck();					// 작은 슬라임 거리 확인
		this->UpdateCircle();					// 작은 슬라임 원 Effect 업데이트
		this->ImageFrameUpdate();				// 작은 슬라임 프레임 Update()
		this->UpdateRect();						// 작은 슬라임 RECT Update()
		this->AlphaUpdate();					// 작은 슬라임 Alpha Update()
		HpBar->WhiteSetGauge(Hp, MAXHP);
		if (!_isActive)
		{
			JumpPos = _position;
		}
		//if (KEYMANAGER->IsToggleKey(VK_F1))
		//{
		//	isChasor = !isChasor;
		//}

		if (isDying)
		{
			DieCheck += TIMEMANAGER->getElapedTime();
			if (DieCheck >= 0.45f)
			{
				EFFECT->ShowEffect("죽음잔상", _position, Vector2(96.f, 96.f), true, 0, 0.1f);
				_isActive = false; isDying = false;
				while (DieCheck > 0.45f)
					DieCheck -= 0.45f;
			}
		}
	
}

void MiniSlime::Render()
{
	Vector2 TempPos = CAMERA->GetRelativeVector2(_position);
	Vector2 TempJumpPos = CAMERA->GetRelativeVector2(JumpPos);
	if (!isDying)
	{
		Shadow->SetSize(Vector2(Math::Clampf(48.f + (JumpPos.y - _position.y), 15.f, 55.f)
			, (48.f / 2.f)) * CAMERA->GetZoom());
		Shadow->Render(TempPos.x, TempPos.y + 16.f);
		switch (SlimePattern)
		{
		case Pattern::Idle:
			IMAGEMANAGER->FindImage("슬라임Idle")->SetAlpha(Alpha);
			IMAGEMANAGER->FindImage("슬라임Idle")->SetSize(Vector2(48, 48) * CAMERA->GetZoom());
			IMAGEMANAGER->FindImage("슬라임Idle")->FrameRender(TempPos.x,
				TempPos.y, FrameIndex, 0);
			HpBar->SetPosition(Vector2(_position.x - _size.x / 2.f + 7.f, _position.y + _size.y / 2 + 3.f));
			break;

		case Pattern::Move:
			IMAGEMANAGER->FindImage("슬라임Idle")->SetAlpha(Alpha);
			IMAGEMANAGER->FindImage("슬라임Idle")->SetSize(Vector2(48, 48) * CAMERA->GetZoom());
			IMAGEMANAGER->FindImage("슬라임Idle")->FrameRender(TempPos.x,
				TempPos.y, FrameIndex, 0);
			HpBar->SetPosition(Vector2(_position.x - _size.x / 2.f + 7.f, _position.y + _size.y / 2 + 3.f));
			break;

		case Pattern::Chasor:
			IMAGEMANAGER->FindImage("슬라임Idle")->SetAlpha(Alpha);
			IMAGEMANAGER->FindImage("슬라임Idle")->SetSize(Vector2(48, 48) * CAMERA->GetZoom());
			IMAGEMANAGER->FindImage("슬라임Idle")->FrameRender(TempPos.x,
				TempPos.y, FrameIndex, 0);
			HpBar->SetPosition(Vector2(_position.x - _size.x / 2.f + 7.f, _position.y + _size.y / 2 + 3.f));
			break;

		case Pattern::Attack:
			//cout << DirVector.GetDegree() << endl;
			if (DirVector.GetDegree() <= 210 &&
				DirVector.GetDegree() > 150)
			{
				IMAGEMANAGER->FindImage("슬라임Left")->SetAlpha(Alpha);
				IMAGEMANAGER->FindImage("슬라임Left")->SetSize(Vector2(48, 48) * CAMERA->GetZoom());
				IMAGEMANAGER->FindImage("슬라임Left")->FrameRender(TempJumpPos.x,
					TempJumpPos.y, FrameIndex, 0);
			}
			else if (DirVector.GetDegree() <= 150 &&
				DirVector.GetDegree() > 90)
			{
				IMAGEMANAGER->FindImage("슬라임LeftUp")->SetAlpha(Alpha);
				IMAGEMANAGER->FindImage("슬라임LeftUp")->SetSize(Vector2(48, 48) * CAMERA->GetZoom());
				IMAGEMANAGER->FindImage("슬라임LeftUp")->FrameRender(TempJumpPos.x,
					TempJumpPos.y, FrameIndex, 0);
			}
			else if (DirVector.GetDegree() <= 270 &&
				DirVector.GetDegree() > 210)
			{
				IMAGEMANAGER->FindImage("슬라임LeftDown")->SetAlpha(Alpha);
				IMAGEMANAGER->FindImage("슬라임LeftDown")->SetSize(Vector2(48, 48) * CAMERA->GetZoom());
				IMAGEMANAGER->FindImage("슬라임LeftDown")->FrameRender(TempJumpPos.x,
					TempJumpPos.y, FrameIndex, 0);
			}
			else if (DirVector.GetDegree() <= 90 &&
				DirVector.GetDegree() > 30)
			{
				IMAGEMANAGER->FindImage("슬라임RightUp")->SetAlpha(Alpha);
				IMAGEMANAGER->FindImage("슬라임RightUp")->SetSize(Vector2(48, 48) * CAMERA->GetZoom());
				IMAGEMANAGER->FindImage("슬라임RightUp")->FrameRender(TempJumpPos.x,
					TempJumpPos.y, FrameIndex, 0);
			}
			else if (DirVector.GetDegree() <= 330 &&
				DirVector.GetDegree() > 270)
			{
				IMAGEMANAGER->FindImage("슬라임RightDown")->SetAlpha(Alpha);
				IMAGEMANAGER->FindImage("슬라임RightDown")->SetSize(Vector2(48, 48) * CAMERA->GetZoom());
				IMAGEMANAGER->FindImage("슬라임RightDown")->FrameRender(TempJumpPos.x,
					TempJumpPos.y, FrameIndex, 0);
			}
			else {
				IMAGEMANAGER->FindImage("슬라임Right")->SetAlpha(Alpha);
				IMAGEMANAGER->FindImage("슬라임Right")->SetSize(Vector2(48, 48) * CAMERA->GetZoom());
				IMAGEMANAGER->FindImage("슬라임Right")->FrameRender(TempJumpPos.x,
					TempJumpPos.y, FrameIndex, 0);
			}

			HpBar->SetPosition(Vector2(JumpPos.x - _size.x / 2.f + 7.f, JumpPos.y + _size.y / 2 + 3.f));
			break;

		case Pattern::Jump:

			IMAGEMANAGER->FindImage("슬라임Jump")->SetSize(Vector2(48, 48) * CAMERA->GetZoom());
			IMAGEMANAGER->FindImage("슬라임Jump")->FrameRender(TempJumpPos.x,
				TempJumpPos.y, FrameIndex, 0);

			HpBar->SetPosition(Vector2(JumpPos.x - _size.x / 2.f + 7.f, JumpPos.y + _size.y / 2 + 3.f));
			break;

		case Pattern::Gather:
			AlphaTime += TIMEMANAGER->getElapedTime();
			if (AlphaTime > 0.8f)
			{
				IMAGEMANAGER->FindImage("슬라임Hold")->SetAlpha(1.f);
				while (AlphaTime > 0.8f)
					AlphaTime -= 0.8f;
			}
			else
			{
				IMAGEMANAGER->FindImage("슬라임Hold")->SetAlpha(AlphaTime);
			}

			IMAGEMANAGER->FindImage("슬라임Hold")->SetSize(Vector2(48, 48) * CAMERA->GetZoom());
			IMAGEMANAGER->FindImage("슬라임Hold")->FrameRender(TempJumpPos.x, TempJumpPos.y, 1, 0);
			break;

		case Pattern::GatherUp:
			AlphaTime += TIMEMANAGER->getElapedTime();
			if (AlphaTime > 0.8f)
			{
				IMAGEMANAGER->FindImage("슬라임Hold")->SetAlpha(1.f);
				while (AlphaTime > 0.8f)
					AlphaTime -= 0.8f;
			}
			else
			{
				IMAGEMANAGER->FindImage("슬라임Hold")->SetAlpha(AlphaTime);
			}
			IMAGEMANAGER->FindImage("슬라임Hold")->SetSize(Vector2(48, 48) * CAMERA->GetZoom());
			IMAGEMANAGER->FindImage("슬라임Hold")->FrameRender(TempJumpPos.x, TempJumpPos.y, 1, 0);
			break;
		case Pattern::GatherIdle:
			AlphaTime += TIMEMANAGER->getElapedTime();
			if (AlphaTime > 0.8f)
			{
				IMAGEMANAGER->FindImage("슬라임Hold")->SetAlpha(1.f);
				while (AlphaTime > 0.8f)
					AlphaTime -= 0.8f;
			}
			else
			{
				IMAGEMANAGER->FindImage("슬라임Hold")->SetAlpha(AlphaTime);
			}
			IMAGEMANAGER->FindImage("슬라임Hold")->SetSize(Vector2(48, 48) * CAMERA->GetZoom());
			IMAGEMANAGER->FindImage("슬라임Hold")->FrameRender(TempJumpPos.x, TempJumpPos.y, 1, 0);
			break;
		case Pattern::GatherMove:
			AlphaTime += TIMEMANAGER->getElapedTime();
			if (AlphaTime > 0.8f)
			{
				IMAGEMANAGER->FindImage("슬라임Hold")->SetAlpha(1.f);
				while (AlphaTime > 0.8f)
					AlphaTime -= 0.8f;
			}
			else
			{
				IMAGEMANAGER->FindImage("슬라임Hold")->SetAlpha(AlphaTime);
			}
			IMAGEMANAGER->FindImage("슬라임Hold")->SetSize(Vector2(48, 48) * CAMERA->GetZoom());
			IMAGEMANAGER->FindImage("슬라임Hold")->FrameRender(TempJumpPos.x, TempJumpPos.y, 0, 0);
			break;

		case Pattern::Hit:
			IMAGEMANAGER->FindImage("슬라임Hit")->SetAlpha(Alpha);
			IMAGEMANAGER->FindImage("슬라임Hit")->SetSize(Vector2(48, 48) * CAMERA->GetZoom());
			IMAGEMANAGER->FindImage("슬라임Hit")->Render(TempPos.x, TempPos.y);
			HpBar->SetPosition(Vector2(_position.x - _size.x / 2.f + 7.f, _position.y + _size.y / 2 + 3.f));
			break;

		case Pattern::HitMove:
			IMAGEMANAGER->FindImage("슬라임Hit")->SetAlpha(Alpha);
			IMAGEMANAGER->FindImage("슬라임Hit")->SetSize(Vector2(48, 48) * CAMERA->GetZoom());
			IMAGEMANAGER->FindImage("슬라임Hit")->Render(TempPos.x, TempPos.y);
			HpBar->SetPosition(Vector2(_position.x - _size.x / 2.f + 7.f, _position.y + _size.y / 2 + 3.f));
			break;

		case Pattern::Break:
			AlphaTime += TIMEMANAGER->getElapedTime();
			if (AlphaTime > 0.8f)
			{
				IMAGEMANAGER->FindImage("슬라임Hold")->SetAlpha(1.f);
				while (AlphaTime > 0.8f)
					AlphaTime -= 0.8f;
			}
			else
			{
				IMAGEMANAGER->FindImage("슬라임Hold")->SetAlpha(AlphaTime);
			}

			IMAGEMANAGER->FindImage("슬라임Hold")->SetSize(Vector2(48, 48) * CAMERA->GetZoom());
			if (!isBreak)
				IMAGEMANAGER->FindImage("슬라임Hold")->FrameRender(TempJumpPos.x, TempJumpPos.y, 0, 0);
			else
				IMAGEMANAGER->FindImage("슬라임Hold")->FrameRender(TempJumpPos.x, TempJumpPos.y, 1, 0);
			break;

		case Pattern::SetPos:
			IMAGEMANAGER->FindImage("슬라임Idle")->SetAlpha(Alpha);
			IMAGEMANAGER->FindImage("슬라임Idle")->SetSize(Vector2(48, 48) * CAMERA->GetZoom());
			IMAGEMANAGER->FindImage("슬라임Idle")->FrameRender(TempJumpPos.x,
				TempJumpPos.y, FrameIndex, 0);
			HpBar->SetPosition(Vector2(JumpPos.x - _size.x / 2.f + 7.f, JumpPos.y + _size.y / 2 + 3.f));
			break;
		}

		this->RenderCircle();

		if (KEYMANAGER->IsToggleKey(VK_F1))
		{
			D2DRENDERER->DrawRectangle(CAMERA->GetRelativeRect(HitRect), D2DRenderer::DefaultBrush::Green);
			D2DRENDERER->DrawRectangle(CAMERA->GetRelativeRect(AttackRect), D2DRenderer::DefaultBrush::Green);
			D2DRENDERER->DrawRectangle(CAMERA->GetRelativeRect(ColRect), D2DRenderer::DefaultBrush::Green);

			for (auto& toExamine : Path)
			{
				RECT rc = RectMake(toExamine.x, toExamine.y, 64, 64);
				rc = CAMERA->GetRelativeRect(rc);
				D2DRENDERER->DrawRectangle(rc);
			}
		}

		
	}
}

void MiniSlime::Release()
{

}

void MiniSlime::RenderCircle()
{
	Vector2 TempPos = CAMERA->GetRelativeVector2(_position);
	Vector2 TempJumpPos = CAMERA->GetRelativeVector2(JumpPos);
	if (!isDying)
	{
		if (EffectDelay > 50)
		{
			for (int i = 0; i < 2; ++i)
			{
				Circle[i].mImage->SetAlpha(Circle[i].Alpha);
				Circle[i].mImage->SetSize(Circle[i].Size * CAMERA->GetZoom());
				Circle[i].mImage->FrameRender((int)TempJumpPos.x, (int)TempJumpPos.y, 0, 0);
			}
		}
		else
		{
			Circle[0].mImage->SetAlpha(Circle[0].Alpha);
			Circle[0].mImage->SetSize(Circle[0].Size * CAMERA->GetZoom());
			Circle[0].mImage->FrameRender((int)TempJumpPos.x, (int)TempJumpPos.y, 0, 0);
		}
	}
}

void MiniSlime::UpdateCircle()
{
	EffectDelay++;						// 두번째 원 Effect 약간의 딜레이
	if (EffectDelay > 50)
	{
		for (int i = 0; i < 2; ++i)
		{
			Circle[i].Size.x = Circle[i].Size.x + 1.f;
			Circle[i].Size.y = Circle[i].Size.y + 1.f;

			Circle[i].Alpha -= 0.002f;
			if (Circle[i].Alpha <= 0.f)
			{

				Circle[i].Alpha = 0.2f;
				Circle[i].Size = Vector2(1.f, 1.f);
			}
		}
	}
	else
	{
		Circle[0].Size.x = Circle[0].Size.x + 1.f;
		Circle[0].Size.y = Circle[0].Size.y + 1.f;

		Circle[0].Alpha -= 0.002f;
		if (Circle[0].Alpha <= 0.f)
		{

			Circle[0].Alpha = 0.2f;
			Circle[0].Size = Vector2(1.f, 1.f);
		}
	}
}

void MiniSlime::DistanceCheck()
{
	// 플레이어와 슬라임 사이의 거리를 확인
	// 거리가 일정 범위 벗어나면 플레이어 쪽으로 이동한다.
	// 작은 슬라임이 공격중이면 이동하지 않는다.
	if (isChasor)
	{
		if (SlimePattern != Pattern::Attack &&
			SlimePattern != Pattern::Jump &&
			SlimePattern != Pattern::Gather &&
			SlimePattern != Pattern::GatherUp &&
			SlimePattern != Pattern::GatherIdle &&
			SlimePattern != Pattern::GatherMove &&
			SlimePattern != Pattern::SetPos &&
			SlimePattern != Pattern::Hit &&
			SlimePattern != Pattern::HitMove &&
			SlimePattern != Pattern::Break)
		{
			if (Math::GetDistance(
				player->GetPosition().x, player->GetPosition().y,
				_position.x, _position.y) > 350 && Path.size() == 0)
			{
				this->ChangeState(Pattern::Move);
			}
			else if (Math::GetDistance(
				player->GetPosition().x, player->GetPosition().y,
				_position.x, _position.y) > 350 && Path.size() != 0)
			{
				this->ChangeState(Pattern::Chasor);
			}
			else
			{
				this->ChangeState(Pattern::Idle);
			}
		}
	}
}

void MiniSlime::UpdateState()
{
		switch (SlimePattern)
		{
		case Pattern::Idle:
			this->Idle();
			break;
		case Pattern::Move:
			this->Move();
			break;
		case Pattern::Chasor:
			this->Chasor();
			break;
		case Pattern::Attack:
			this->Attack();
			break;
		case Pattern::Gather:
			this->Gather();
			break;
		case Pattern::GatherUp:
			this->GatherUp();
			break;
		case Pattern::GatherIdle:
			this->GatherIdle();
			break;
		case Pattern::GatherMove:
			this->GatherMove();
			break;
		case Pattern::HitMove:
			this->HitMove();
			break;
		case Pattern::Jump:
			this->Jump();
			break;
		case Pattern::Break:
			this->Break();
			break;
		case Pattern::SetPos:
			this->SetPos();
			break;
		}
}

void MiniSlime::ChangeState(Pattern pattern)
{
	SlimePattern = pattern;
}

void MiniSlime::Move()
{
	// 플레이어의 중점좌표 player->GetPosition() / 슬라임의 중점좌표 _position
	// 방향 벡터를 계산한다. Vector2.Nomalize()
	DirVector.SetDirVector(_position, player->GetPosition());

	// 방향 벡터를 사용하여 해당 방향으로 슬라임을 이동시킨다.
	_position += DirVector * Speed * TIMEMANAGER->getElapedTime();
	JumpPos = _position;
}

void MiniSlime::Jump()
{
	DirVector.SetDirVector(_position, player->GetPosition());
	_position += DirVector * Speed * TIMEMANAGER->getElapedTime() * 2.f;
	JumpPos += DirVector * Speed * TIMEMANAGER->getElapedTime() * 2.f;

	// 슬라임 중력값을 증가 시켜 Y축으로 일정 높이 올라간다.
	// 일정 높이까지 슬라임이 도달하면 중력값을 다시 증가시켜 원래 자리로 되돌린다.
	// 중력값이 0보다 커지면 중력값을 0으로 고정한다.
	Gravity += 0.16f;
	JumpPos.y += Gravity;
	if (Gravity >= 2.f)
	{
		Gravity = 0.f;
		FrameIndex = 0;
		ChangeState(Pattern::SetPos);
	}
}

void MiniSlime::Attack()
{
	// 이전에 구했던 방향벡터 ( DirVector ) 쪽으로 공격한다.
	_position += DirVector * Speed * TIMEMANAGER->getElapedTime() * 4.f;
	JumpPos += DirVector * Speed * TIMEMANAGER->getElapedTime() * 4.f;

	Gravity += 0.16f;
	JumpPos.y += Gravity;
	if (Gravity >= 2.f)
	{
		Gravity = 0.f;
		FrameIndex = 0;
		ChangeState(Pattern::SetPos);
	}
}

void MiniSlime::Idle()
{
	// 기본 ( Idle ) 상태를 유지하고 있으면
	// AttackCount 증가.. 일정 시간 뒤
	if (isChasor)
	{
		AttackTime += TIMEMANAGER->getElapedTime();
		if (AttackTime > 0.5f)
		{
			// AttackTime = 0 으로 초기화 후
			// 공격 ( Attack ) 상태로 변화
			while (AttackTime > 0.5f)
				AttackTime -= 0.5f;

			DirVector.SetDirVector(_position, player->GetPosition());
			if (Math::GetDistance(
				player->GetPosition().x, player->GetPosition().y,
				_position.x, _position.y) < 125)
			{
				SOUNDMANAGER->play("작은슬라임공격", 0.15f);
				FrameIndex = 0;
				this->ChangeState(Pattern::Attack);
				Path.clear(); Index = 0;
				Gravity -= 3.f;
			}
			else
			{
				SOUNDMANAGER->play("작은슬라임점프", 0.15f);
				FrameIndex = 0;
				this->ChangeState(Pattern::Jump);
				Path.clear(); Index = 0;
				Gravity -= 3.f;
			}
		}
	}
}

void MiniSlime::Chasor()
{
	if (Path.size() != 0)
	{
		// 선정된 최단경로 방향으로 슬라임이 이동할 것이다.
		Index = Math::Clamp(Index, 0, Path.size() - 1);
		DirVector.SetDirVector(_position, Path[Index]);
		_position += DirVector * Speed * TIMEMANAGER->getElapedTime();
		JumpPos = _position;
		// 이미 이동한 최단경로는 vector에서 삭제해준다.
		if (Math::GetDistance(_position.x, _position.y,
			Path[Index].x, Path[Index].y) < 25)
		{
			Index++;
		}
	}
	else
	{
		ChangeState(Pattern::Idle);
		Index = 0;
	}
}

void MiniSlime::Hit(int damage)
{
	Hp -= damage; 
	isCrush = false;
	AlphaStart = true;
	SlimePattern = Pattern::HitMove;
	DirVector.SetDirVector(_position, player->GetPosition());
	HpBar->SetisWhite(false);
}

void MiniSlime::HitMove()
{
	HitTime += TIMEMANAGER->getElapedTime();
	
	if (HitTime > 0.25)
	{
		while (HitTime > 0.25)
			HitTime -= 0.25;
		SlimePattern = Pattern::Idle;
		isCrush = true;
		HpBar->SetisWhite(true);
		if (Hp <= 0 && !isDying)
		{
			SOUNDMANAGER->play("슬라임죽음", 0.15f);
			isDying = true; isChasor = false;
			HpBar->SetisActive(false);
			EFFECT->ShowEffect("작은폭파", _position, Vector2(96, 96), true, 0, 0.05f);
		}
	}
	else
	{
		_position += DirVector * (-1) * Speed * TIMEMANAGER->getElapedTime() * 2.f;
		JumpPos = _position;
		HpBar->SetGauge(Hp, MAXHP);
	}
}

void MiniSlime::Gather()
{
	_isActive = true;
	isCol = false;
	isCrush = false;
	isChasor = false;
	SlimePattern = Pattern::GatherUp;
	HpBar->SetisActive(false);
}

void MiniSlime::GatherUp()
{
	GatherUpTime += TIMEMANAGER->getElapedTime();
	if (GatherUpTime > 0.75f)
	{
		while (GatherUpTime > 0.75f)
			GatherUpTime -= 0.75f;
		SlimePattern = Pattern::GatherIdle;
	}
	else 
	{
		JumpPos.y -= 1.5f;
	}
}

void MiniSlime::GatherIdle()
{
	GatherIdleTime += TIMEMANAGER->getElapedTime();
	IdleChangeTime += TIMEMANAGER->getElapedTime();
	float time = RND->Range(0.5f, 1.f);
	if (GatherIdleTime > time)
	{
		while (GatherIdleTime > time)
			GatherIdleTime -= time;
		SlimePattern = Pattern::GatherMove;
	}
	else
	{
		if (IdleChangeTime > 0.05f)
		{
			while (IdleChangeTime > 0.05f)
				IdleChangeTime -= 0.5f;
			IdleChange = !IdleChange;
		}

		if (IdleChange)
		{
			JumpPos.y += 0.3f;
		}
		else 
		{
			JumpPos.y -= 0.3f;
		}
	}
}

void MiniSlime::GatherMove()
{
	AfterTime += TIMEMANAGER->getElapedTime();
	// 거리가 가까워 졌으니까 그만모여라 ㅎㅎ	
	if (Math::GetDistance((float)64 * 23 / 2.f, (float)64 * 23 / 2.f, JumpPos.x, JumpPos.y) < 10)
	{
		isGather = true;
		isBreak = false;
	}	
	DirVector.SetDirVector(JumpPos, Vector2(64 * 23 / 2, 64 * 23 / 2));
	JumpPos += (DirVector * Speed * TIMEMANAGER->getElapedTime()) * 4.5f;
	_position += (DirVector * Speed * TIMEMANAGER->getElapedTime()) * 4.5f;

	if (AfterTime > 0.15f)
	{
		while (AfterTime > 0.15f)
			AfterTime -= 0.15f;
		EFFECT->ShowEffect("슬라임Hold", JumpPos, Vector2(48.f,48.f), false, 0);
	}

	if (isGather)
	{
		ActiveTime += TIMEMANAGER->getElapedTime();
	}
}

void MiniSlime::SetPos()
{
	if (Math::Abs(JumpPos.y - _position.y) < 5)
	{
		SOUNDMANAGER->play("슬라임맞음", 0.15f);
		SlimePattern = Pattern::Idle;
		JumpPos = _position;
	}
	else
	{

		if (JumpPos.y < _position.y)
		{
			JumpPos.y += 5.f;
		}
		else
		{
			JumpPos.y -= 5.f;
		}
	}
}

void MiniSlime::Break()
{
	SlimePattern = Pattern::Break;
	DirVector.SetDirVector(_position, RebirthPos);
	_position += (DirVector * Speed * TIMEMANAGER->getElapedTime()) * 4.5f;
	JumpPos += (DirVector * Speed * TIMEMANAGER->getElapedTime()) * 4.5f;
	
	if ( Math::Abs(RebirthPos.x - JumpPos.x) < 5 )
	{
		SlimePattern = Pattern::SetPos;
		Gravity = 0.f;
		isBreak = true;
		isCol = true;
		isCrush = true;
		isChasor = true;
		HpBar->SetisActive(true);
	}
}

void MiniSlime::Rebirth()
{
	// 슬라임 다시 활성화
	// 체력 150으로 초기화
	// 슬라임 원 Effect 딜레이 시간 초기화
	if (_isActive == false)
		_isActive = true;
	Hp = 150;
	EffectDelay = 0;
	isGather = false;
	ActiveTime = 0;
	isDying = false;
	DieCheck = 0.f;
	HpBar->SetHpWidth(36.f);
	HpBar->SetWhiteHpWidth(36.f);
}

void MiniSlime::ImageFrameUpdate()
{
	switch (SlimePattern)
	{
		// ==============================================
		// ############# 슬라임 기본 ( Idle ) ############
		// ==============================================
	case Pattern::Idle:
		FrameCount += TIMEMANAGER->getElapedTime();
		if (FrameCount > 0.3f)
		{
			FrameIndex++;
			if (FrameIndex > 2) FrameIndex = 0;
			while (FrameCount > 0.2f)
				FrameCount -= 0.2f;
		}
		break;

		// ==============================================
		// ############# 슬라임 기본 ( Move ) ############
		// ==============================================
	case Pattern::Move:
		FrameCount += TIMEMANAGER->getElapedTime();
		if (FrameCount > 0.3f)
		{
			FrameIndex++;
			if (FrameIndex > 2) FrameIndex = 0;
			while (FrameCount > 0.2f)
				FrameCount -= 0.2f;
		}
		break;

		// ================================================
		// ############# 슬라임 추적 ( Chasor ) ############
		// ================================================
	case Pattern::Chasor:
		FrameCount += TIMEMANAGER->getElapedTime();
		if (FrameCount > 0.3f)
		{
			FrameIndex++;
			if (FrameIndex > 2) FrameIndex = 0;
			while (FrameCount > 0.2f)
				FrameCount -= 0.2f;
		}
		break;

		// ==============================================
		// ############# 슬라임 점프 ( Jump ) ############
		// ==============================================
	case Pattern::Jump:
		FrameCount += TIMEMANAGER->getElapedTime();
		if (FrameCount > 0.2f)
		{
			FrameIndex++;
			if (FrameIndex > 3) FrameIndex = 3;
			while (FrameCount > 0.2f)
				FrameCount -= 0.2f;
		}
		break;
		// ==============================================
		// ############# 슬라임 공격 ( Attack ) ##########
		// ==============================================
	case Pattern::Attack:
		FrameCount += TIMEMANAGER->getElapedTime();
		if (FrameCount > 0.2f)
		{
			FrameIndex++;
			if (FrameIndex > 2) FrameIndex = 2;
			while (FrameCount > 0.2f)
				FrameCount -= 0.2f;
		}
		break;

	case Pattern::SetPos:
		FrameCount += TIMEMANAGER->getElapedTime();
		if (FrameCount > 0.3f)
		{
			FrameIndex++;
			if (FrameIndex > 2) FrameIndex = 0;
			while (FrameCount > 0.2f)
				FrameCount -= 0.2f;
		}
		break;
	}
}

void MiniSlime::UpdateRect()
{
	ColRect = RectMakeCenter(_position.x, _position.y + _size.y / 3.f, _size.x, _size.y / 3.f);
	HitRect = RectMakeCenter(_position.x, _position.y, _size.x, _size.y);
	AttackRect = HitRect;
}

void MiniSlime::AlphaUpdate()
{
	if (AlphaStart)
	{

		AlphaTime += TIMEMANAGER->getElapedTime();
		AlphaChangeTime += TIMEMANAGER->getElapedTime();

		if (AlphaTime > 1.f)
		{
			while (AlphaTime > 1.f)
				AlphaTime -= 1.f;
			AlphaStart = false;
			Alpha = 1.f;
			AlphaTime = 0.f;
			AlphaChangeTime = 0.f;
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
