#include "stdafx.h"
#include "Slime.h"
#include "Player.h"
#include "MiniSlime.h"
#include "Bullet.h"
#include "SlimeBullet.h"
#include "BossProgressBar.h"
#include "ProgressBar.h"
#include "Event.h"
#include "Particle.h"
#include "Thunder.h"


void Slime::Init()
{
	Speed = 400.f;					// ū ������ �̵��ӵ� 400.f �ʱ�ȭ
	Gravity = 0.f;					// ū ������ �߷� 0.f �ʱ�ȭ
	FrameCount = 0.f;				// ū ������ ������ ī��Ʈ 0.f �ʱ�ȭ
	FrameIndex = 0;					// ū ������ ������ �ε��� 0 �ʱ�ȭ
	BreakPoint = 50;				// ū ������ ����Ƚ�� ������ ���صȴ�. ( ���� ü���� �ȱ￩ )
	Hp = 5000;						// ū ������ Hp 500 �ʱ�ȭ ( ���� ü�� )

	EffectDelay = 0.f;				// ū ������ �� Effect DelayTime 0 �ʱ�ȭ
	isShow = false;					// ū ������ ��ü�� �����ٰų�...? ( Effect, ���� ��ü )
	isGather = false;				// ū ������ ��ü �Ϸ�

	isUp = false;					// ū ������ Idle ���� Up/Down �� ?
	IdleTime = 0.f;					// ū ������ Idle ���� Up/Down ��ȭ �ð�

	EnergyCount = 5;				// ū ������ ������ ź ���� 5��
	isWave = false;					// ū ������ ������ ź ��� �� ��������
	isCrush = true;					// ū ������ �浹 ���� ���� ( ���� �� false )

	SlimePattern = Pattern::Idle;	// ū ������ Pattern �ʱⰪ �⺻ ( Idle )
	SlimePhase = Phase::One;		// ū ������ Phase �ʱⰪ One
	SlimeMax = 16;					// ���� ������ �ִ� ���� �ʱⰪ 16
	SlimeSize = 0.f;				// ū ������ ũ�� 16 * ( ���� ������ ���� ) 256.f 
	SlimeAlpha = 0.f;				// ū ������ ���İ� 1.f

	CoreCount = 0.f;				// ū ������ Core ������ ī��Ʈ 0.f �ʱ�ȭ
	CoreIndex = 0;					// ū ������ Core ������ �ε��� 0 �ʱ�ȭ

	isJump = false;					// ū ������ ���� �� ( true )
	JumpDelay = 0.f;				// ū ������ ���� ������
	JumpCount = 3;					// ū ������ ���� Ƚ�� ( 3�� ���� )
	DirVector = Vector2(0.f, 0.f);	// ū ������ ���� ���� �ʱ�ȭ

	AfterTime = 0.f;				// ū ������ �ܻ� �ð� �ʱ�ȭ
	AttackTime = 0.f;				// ū ������ ���� �ð� �ʱ�ȭ
	AttackIdleCount = 0;			// ū ������ ���� �غ� Count
	AttackCount = 3;				// ū ������ ���� Ƚ��
	BackTime = 0.f;

	SizeCheck = 0;					// ū ������ ������ Check
	SizeCount = 0.f;				// ū ������ ������ ��ȭ �ð�
	SizeUp = false;					// ū ������ ������ ��ȭ ����
	Event = true;
	ShockTime = 0.f;


	EffectBullet = new Bullet;
	ShotBullet = new SlimeBullet;
	player = (Player*)OBJECTMANAGER->FindObject(ObjectType::Player, "�÷��̾�");
	Shadow = IMAGEMANAGER->FindImage("�����ӱ׸���"); // ū ������ �׸��� Image
	JumpPos = _position;


	Hpbar = new BossProgressBar("����ü��",
		Vector2(1200, 35), Vector2(50, 660), Pivot::LeftTop);
	UIMANAGER->addUi(ObjectType::Object, Hpbar);

	BreakBar = new ProgressBar("��������", 
		Vector2(80, 8),
		Vector2(JumpPos.x - 30.f, JumpPos.y + SlimeMax),
		Pivot::LeftTop);
	BreakBar->Init(50, 80, 8, true);
	BreakBar->SetisActive(false);
	UIMANAGER->addUi(ObjectType::UI, BreakBar);

	isHit = false;
	HitTime = 0.f;

	BreakTime = 0.f;
	isBreak = false;
	
	PatternChange = 0.f;

	Die = false;
	Boom = false;
	DieTime = 0.f;

	SlimeParticle = new Particle;

	isDash = false;
	Pong = false;
	PongTime = 0.f;

	ChargeCount = 5;
	ChargeTime = 0.f;
	ThunderRect = new Thunder;

	// ===================================================
	// ############ ū ������ �� Effect �ʱ�ȭ ############
	// ===================================================
	for (int i = 0; i < 8; ++i)
	{
		Circle[i].mImage = IMAGEMANAGER->FindImage("�����ӿ�");
		Circle[i].Alpha = 0.4f;
		Circle[i].Angle = Math::PI / i;
		Circle[i].Size = Vector2(100.f, 100.f);
	}

	for (int i = 0; i < 10; ++i)
	{
		DieCircle[i].mImage = IMAGEMANAGER->FindImage("�����ӿ�");
		DieCircle[i].Alpha = 1.f;
		DieCircle[i].Angle = Math::PI / i;
		DieCircle[i].Size = Vector2(50.f, 50.f);
	}

	// ====================================================
	// ############### ���� ������ ���� ####################
	// ====================================================
	this->MiniSlimeMake();
}

void Slime::Update()
{
	
		// Debug�� F1 
		if (KEYMANAGER->IsOnceKeyDown(VK_F1))
		{
			SlimePhase = (Phase)((int)SlimePhase + 1);
			if (SlimePhase > (Phase)3) SlimePhase = Phase::One;
		}

		if (KEYMANAGER->IsOnceKeyDown('N'))
		{
			this->Hit(5000);
		}

		if (KEYMANAGER->IsOnceKeyDown('G'))
		{
			this->Gather();
		}

		if (KEYMANAGER->IsOnceKeyDown('B') && SlimeMax > 4)
		{
			this->Break(); CAMERA->Shake(10.f, 0.5f);
			SlimePhase = (Phase)((int)SlimePhase + 1);
		}

		if (KEYMANAGER->IsOnceKeyDown('J'))
		{
			SlimePattern = Pattern::Jump;
			Gravity = -6.f;
		}

		if (KEYMANAGER->IsOnceKeyDown('K'))
		{
			SlimePattern = Pattern::AttackIdle;
		}

		if (KEYMANAGER->IsOnceKeyDown('H'))
		{
			SlimePattern = Pattern::Energy;
			isWave = true; EffectDelay = 0;
		}

		EffectBullet->Update();					// Effect Bullet Update
		ShotBullet->Update();					// ��� Bullet Update

		this->FrameUpdate();					// ������ �ε��� Update
		this->StateUpdate();					// ���� Ȯ���ϸ� ���� Update
		this->CircleUpdate(Vector2(100.f / ((float)SlimePhase + 1.f), 100.f / ((float)SlimePhase + 1.f)), 0.3f);
		this->SlimeCoreUpdate();				// ������ Core ������ �ε��� Update
		this->RectUpdate();
		this->GatherCheck();					// ���� ������ ��� �𿴴��� Ȯ��
		this->Hiting();
		this->BreakHiting();
		this->ProgressBarUpdate();


		for (int i = 0; i < SlimeList.size(); ++i)
		{
			if (SlimeList[i]->GetisActive()) continue;
			if (!isGather) continue;
			SlimeList[i]->SetPosition(_position);
			SlimeList[i]->SetJumpPos(JumpPos);
		}

		SlimeParticle->Update();
		ThunderRect->Update();

		if (KEYMANAGER->IsOnceKeyDown('P'))
		{
			//Vector2 temp;
			//temp.SetDirVector(_position, player->GetPosition());
			////ThunderRect->FireOff();
			//ThunderRect->Fire(_position, Vector2(64.f, 64.f), temp.GetDegree());
			SlimePattern = Pattern::ChargeIdle;
			isWave = true; EffectDelay = 0;
		}
}

void Slime::Render()
{
	Vector2 TempPos = CAMERA->GetRelativeVector2(_position);
	Vector2 TempJumpPos = CAMERA->GetRelativeVector2(JumpPos);

	ShockTime += TIMEMANAGER->getElapedTime();

	if (isShow)
	{
		if (!Die)
		{
			Shadow->SetSize(Vector2(SlimeSize + (JumpPos.y - _position.y), SlimeSize / 3.f) * CAMERA->GetZoom());
			Shadow->Render(TempPos.x, TempPos.y + SlimeSize / 2.f);

			ThunderRect->Render();

			if (ShockTime > 0.2f)
			{
				switch (RND->Range(0, 3))
				{
				case 0:
					EFFECT->ShowEffect("��������1", Vector2(JumpPos.x + RND->Range(-SlimeSize / 3.f, SlimeSize / 3.f)
						, JumpPos.y + RND->Range(-SlimeSize / 3.f, SlimeSize / 3.f)), Vector2(32, 32), true, 0, 0.1f);
					break;
				case 1:
					EFFECT->ShowEffect("��������2", Vector2(JumpPos.x + RND->Range(-SlimeSize / 3.f, SlimeSize / 3.f)
						, JumpPos.y + RND->Range(-SlimeSize / 3.f, SlimeSize / 3.f)), Vector2(32, 32), true, 0, 0.1f);
					break;
				case 2:
					EFFECT->ShowEffect("ū����1", Vector2(JumpPos.x + RND->Range(-SlimeSize / 3.f, SlimeSize / 3.f)
						, JumpPos.y + RND->Range(-SlimeSize / 3.f, SlimeSize / 3.f)), Vector2(32, 32), true, 0, 0.1f);
					break;
				case 3:
					EFFECT->ShowEffect("ū����2", Vector2(JumpPos.x + RND->Range(-SlimeSize / 3.f, SlimeSize / 3.f)
						, JumpPos.y + RND->Range(-SlimeSize / 3.f, SlimeSize / 3.f)), Vector2(32, 32), true, 0, 0.1f);
					break;
				}
				while (ShockTime > 0.2f)
					ShockTime -= 0.2f;
			}
		}
		switch (SlimePattern)
		{
		case Pattern::Idle:
			IMAGEMANAGER->FindImage("������PageOneIdle")->SetAlpha(SlimeAlpha);
			IMAGEMANAGER->FindImage("������PageOneIdle")->SetSize(Vector2(SlimeSize, SlimeSize) * CAMERA->GetZoom());
			IMAGEMANAGER->FindImage("������PageOneIdle")->FrameRender(TempJumpPos.x, TempJumpPos.y, FrameIndex, 0);
			break;

		case Pattern::Jump:
			if (FrameIndex == 0)
			{
				IMAGEMANAGER->FindImage("������PageOneIdle")->SetSize(Vector2(SlimeSize + 30.f, SlimeSize - 30.f) * CAMERA->GetZoom());
			}
			else if (FrameIndex == 1)
			{
				IMAGEMANAGER->FindImage("������PageOneIdle")->SetSize(Vector2(SlimeSize + 55.f, SlimeSize - 55.f) * CAMERA->GetZoom());
			}
			else if (FrameIndex == 2)
			{
				IMAGEMANAGER->FindImage("������PageOneIdle")->SetSize(Vector2(SlimeSize + 30.f, SlimeSize - 30.f) * CAMERA->GetZoom());
			}
			else if (FrameIndex == 3)
			{
				IMAGEMANAGER->FindImage("������PageOneIdle")->SetSize(Vector2(SlimeSize, SlimeSize) * CAMERA->GetZoom());
			}
			else if (FrameIndex == 4)
			{
				IMAGEMANAGER->FindImage("������PageOneIdle")->SetSize(Vector2(SlimeSize - 30.f, SlimeSize + 30.f) * CAMERA->GetZoom());
			}
			else {
				IMAGEMANAGER->FindImage("������PageOneIdle")->SetSize(Vector2(SlimeSize - 60.f, SlimeSize + 60.f) * CAMERA->GetZoom());
			}
			IMAGEMANAGER->FindImage("������PageOneIdle")->FrameRender(TempJumpPos.x, TempJumpPos.y, FrameIndex, 0);
			break;

		case Pattern::Jumping:
			IMAGEMANAGER->FindImage("������PageOneIdle")->SetSize(Vector2(SlimeSize, SlimeSize) * CAMERA->GetZoom());
			IMAGEMANAGER->FindImage("������PageOneIdle")->FrameRender(TempJumpPos.x, TempJumpPos.y, FrameIndex, 0);
			break;

		case Pattern::JumpIdle:
			if (FrameIndex == 0)
			{
				IMAGEMANAGER->FindImage("������PageOneIdle")->SetSize(Vector2(SlimeSize + 30.f, SlimeSize - 30.f) * CAMERA->GetZoom());
			}
			else if (FrameIndex == 1)
			{
				IMAGEMANAGER->FindImage("������PageOneIdle")->SetSize(Vector2(SlimeSize + 55.f, SlimeSize - 55.f) * CAMERA->GetZoom());
			}
			else if (FrameIndex == 2)
			{
				IMAGEMANAGER->FindImage("������PageOneIdle")->SetSize(Vector2(SlimeSize + 30.f, SlimeSize - 30.f) * CAMERA->GetZoom());
			}
			else if (FrameIndex == 3)
			{
				IMAGEMANAGER->FindImage("������PageOneIdle")->SetSize(Vector2(SlimeSize, SlimeSize) * CAMERA->GetZoom());
			}
			else if (FrameIndex == 4)
			{
				IMAGEMANAGER->FindImage("������PageOneIdle")->SetSize(Vector2(SlimeSize - 30.f, SlimeSize + 30.f) * CAMERA->GetZoom());
			}
			else {
				IMAGEMANAGER->FindImage("������PageOneIdle")->SetSize(Vector2(SlimeSize - 60.f, SlimeSize + 60.f) * CAMERA->GetZoom());
			}
			IMAGEMANAGER->FindImage("������PageOneIdle")->FrameRender(TempJumpPos.x, TempJumpPos.y, FrameIndex, 0);
			break;

		case Pattern::ComeBack:
			IMAGEMANAGER->FindImage("������PageOneIdle")->SetSize(Vector2(SlimeSize, SlimeSize) * CAMERA->GetZoom());
			IMAGEMANAGER->FindImage("������PageOneIdle")->FrameRender(TempPos.x, TempPos.y, FrameIndex, 0);
			break;

		case Pattern::Shot:
			IMAGEMANAGER->FindImage("������PageOneIdle")->SetSize(Vector2(SlimeSize, SlimeSize)* CAMERA->GetZoom());
			IMAGEMANAGER->FindImage("������PageOneIdle")->FrameRender(TempJumpPos.x, TempJumpPos.y, FrameIndex, 0);
			break;

		case Pattern::Charge:
			IMAGEMANAGER->FindImage("������PageOneIdle")->SetSize(Vector2(SlimeSize, SlimeSize)* CAMERA->GetZoom());
			IMAGEMANAGER->FindImage("������PageOneIdle")->FrameRender(TempJumpPos.x, TempJumpPos.y, FrameIndex, 0);
			break;

		case Pattern::ChargeIdle:
			IMAGEMANAGER->FindImage("������PageOneIdle")->SetSize(Vector2(SlimeSize, SlimeSize)* CAMERA->GetZoom());
			IMAGEMANAGER->FindImage("������PageOneIdle")->FrameRender(TempJumpPos.x, TempJumpPos.y, FrameIndex, 0);
			break;

		case Pattern::Attack:
			IMAGEMANAGER->FindImage("������PageOneIdle")->SetSize(Vector2(SlimeSize, SlimeSize) * CAMERA->GetZoom());
			IMAGEMANAGER->FindImage("������PageOneIdle")->FrameRender(TempJumpPos.x, TempJumpPos.y, FrameIndex, 0);
			break;
		case Pattern::AttackIdle:
			IMAGEMANAGER->FindImage("������PageOneIdle")->SetSize(Vector2(SlimeSize, SlimeSize) * CAMERA->GetZoom());
			IMAGEMANAGER->FindImage("������PageOneIdle")->FrameRender(TempJumpPos.x, TempJumpPos.y, FrameIndex, 0);
			break;
		case Pattern::AttackBack:
			IMAGEMANAGER->FindImage("������PageOneIdle")->SetAlpha(SlimeAlpha);
			IMAGEMANAGER->FindImage("������PageOneIdle")->SetSize(Vector2(SlimeSize, SlimeSize) * CAMERA->GetZoom());
			IMAGEMANAGER->FindImage("������PageOneIdle")->FrameRender(TempJumpPos.x, TempJumpPos.y, FrameIndex, 0);
			break;

		case Pattern::Energy:
			IMAGEMANAGER->FindImage("������PageOneIdle")->SetSize(Vector2(SlimeSize, SlimeSize) * CAMERA->GetZoom());
			IMAGEMANAGER->FindImage("������PageOneIdle")->FrameRender(TempJumpPos.x, TempJumpPos.y, FrameIndex, 0);
			break;

		case Pattern::EnergyIdle:
			IMAGEMANAGER->FindImage("������PageOneIdle")->SetSize(Vector2(SlimeSize, SlimeSize) * CAMERA->GetZoom());
			IMAGEMANAGER->FindImage("������PageOneIdle")->FrameRender(TempJumpPos.x, TempJumpPos.y, FrameIndex, 0);
			break;

		case Pattern::ReSet:
			if (FrameIndex == 0)
			{
				IMAGEMANAGER->FindImage("������PageOneIdle")->SetSize(Vector2(SlimeSize + 30.f, SlimeSize - 30.f) * CAMERA->GetZoom());
			}
			else if (FrameIndex == 1)
			{
				IMAGEMANAGER->FindImage("������PageOneIdle")->SetSize(Vector2(SlimeSize + 55.f, SlimeSize - 55.f) * CAMERA->GetZoom());
			}
			else if (FrameIndex == 2)
			{
				IMAGEMANAGER->FindImage("������PageOneIdle")->SetSize(Vector2(SlimeSize + 30.f, SlimeSize - 30.f) * CAMERA->GetZoom());
			}
			else if (FrameIndex == 3)
			{
				IMAGEMANAGER->FindImage("������PageOneIdle")->SetSize(Vector2(SlimeSize, SlimeSize) * CAMERA->GetZoom());
			}
			else if (FrameIndex == 4)
			{
				IMAGEMANAGER->FindImage("������PageOneIdle")->SetSize(Vector2(SlimeSize - 30.f, SlimeSize + 30.f)* CAMERA->GetZoom());
			}
			else {
				IMAGEMANAGER->FindImage("������PageOneIdle")->SetSize(Vector2(SlimeSize - 60.f, SlimeSize + 60.f)* CAMERA->GetZoom());
			}
			IMAGEMANAGER->FindImage("������PageOneIdle")->FrameRender(TempJumpPos.x, TempJumpPos.y, FrameIndex, 0);
			break;

		case Pattern::Death:
			IMAGEMANAGER->FindImage("������PageOneIdle")->SetAlpha(SlimeAlpha);
			IMAGEMANAGER->FindImage("������PageOneIdle")->SetSize(Vector2(SlimeSize, SlimeSize)* CAMERA->GetZoom());
			IMAGEMANAGER->FindImage("������PageOneIdle")->FrameRender(TempJumpPos.x, TempJumpPos.y, FrameIndex, 0);
			break;

		case Slime::Pattern::Dash:
			IMAGEMANAGER->FindImage("������PageOneIdle")->SetSize(Vector2(SlimeSize, SlimeSize)* CAMERA->GetZoom());
			IMAGEMANAGER->FindImage("������PageOneIdle")->FrameRender(TempJumpPos.x, TempJumpPos.y, FrameIndex, 0);
			break;
		case Slime::Pattern::DashIdle:
			IMAGEMANAGER->FindImage("������PageOneIdle")->SetSize(Vector2(SlimeSize, SlimeSize)* CAMERA->GetZoom());
			IMAGEMANAGER->FindImage("������PageOneIdle")->FrameRender(TempJumpPos.x, TempJumpPos.y, FrameIndex, 0);
			break;
		case Slime::Pattern::DashComeBack:
			IMAGEMANAGER->FindImage("������PageOneIdle")->SetSize(Vector2(SlimeSize, SlimeSize)* CAMERA->GetZoom());
			IMAGEMANAGER->FindImage("������PageOneIdle")->FrameRender(TempJumpPos.x, TempJumpPos.y, FrameIndex, 0);
			break;
		}
	}
	EffectBullet->Render();				// Effect Bullet Render
	ShotBullet->Render();				// ��� Bullet Render
	this->SlimeCoreRender();			// ������ Core Render
	this->CircleRender();				// ������ �� Effect Render

	if (KEYMANAGER->IsToggleKey(VK_F1))
	{
		D2DRENDERER->DrawRectangle(CAMERA->GetRelativeRect(HitRect), D2DRenderer::DefaultBrush::Green);
		D2DRENDERER->DrawRectangle(CAMERA->GetRelativeRect(AttackRect), D2DRenderer::DefaultBrush::Green);
		D2DRENDERER->DrawRectangle(CAMERA->GetRelativeRect(ColRect), D2DRenderer::DefaultBrush::Green);
	}

	SlimeParticle->Render();
	
}

void Slime::Release()
{
	EffectBullet->Release();
	SAFE_DELETE(EffectBullet);
	SAFE_DELETE(ShotBullet);
}
void Slime::SetJumpPos()
{
	if (Math::Abs(JumpPos.y - _position.y) < 10)
	{
		JumpPos = _position;
		SlimePattern = Pattern::JumpIdle;
	}
	else
	{
		if (JumpPos.y > _position.y)
		{
			JumpPos.y -= 4.5f;
		}
		else
		{
			JumpPos.y += 4.5f;
		}
	}
}
// ============================================W
// ######## ū ������ �⺻ ( Idle ) ###########
// ============================================
void Slime::Idle()
{
	// ���� �ð��� ������ ���� ū ������ ��, �Ʒ� �ܷ��ܷ�
	if (isShow)
	{
		IdleTime += TIMEMANAGER->getElapedTime();
		PatternChange += TIMEMANAGER->getElapedTime();
	}
	if (IdleTime > 0.75f)
	{
		isUp = !isUp;
		while (IdleTime > 0.75f)
			IdleTime -= 0.75f;
	}

	if (isShow)
	{
		if (isUp)
		{
			JumpPos.y += 50.f * TIMEMANAGER->getElapedTime();
		}
		else
		{
			JumpPos.y -= 50.f * TIMEMANAGER->getElapedTime();
		}
	}
	if (EVENT->Eventing() != true && isShow)
	{
		switch (SlimePhase)
		{
		case Slime::Phase::One:
			if (PatternChange > 3.5f)
			{
				while (PatternChange > 3.5f)
					PatternChange -= 3.5f;

				switch (RND->Range(0, 4))
				{
				case 1:
					SlimePattern = Pattern::Jump;
					Gravity = -6.f;
					break;
				case 2:
					SlimePattern = Pattern::AttackIdle;
					break;
				case 3:
					SlimePattern = Pattern::Energy;
					isWave = true; EffectDelay = 0;
					break;
				case 4:
					SlimePattern = Pattern::ChargeIdle;
					isWave = true; EffectDelay = 0;
					break;
				}
			}
			break;
		case Slime::Phase::Two:
			if (PatternChange > 2.5f)
			{
				while (PatternChange > 2.5f)
					PatternChange -= 2.5f;

				switch (RND->Range(0, 3))
				{
				case 1:
					SlimePattern = Pattern::Jump;
					Gravity = -6.f;
					break;
				case 2:
					SlimePattern = Pattern::AttackIdle;
					break;
				case 3:
					SlimePattern = Pattern::Energy;
					isWave = true; EffectDelay = 0;
					break;
				case 4:
					SlimePattern = Pattern::ChargeIdle;
					isWave = true; EffectDelay = 0;
					break;
				}
			}
			break;
		case Slime::Phase::Three:
			if (PatternChange > 1.5f)
			{
				while (PatternChange > 1.5f)
					PatternChange -= 1.5f;

				switch (RND->Range(0, 3))
				{
				case 1:
					SlimePattern = Pattern::Jump;
					Gravity = -6.f;
					break;
				case 2:
					SlimePattern = Pattern::AttackIdle;
					break;
				case 3:
					SlimePattern = Pattern::Energy;
					isWave = true; EffectDelay = 0;
					break;
				case 4:
					SlimePattern = Pattern::ChargeIdle;
					isWave = true; EffectDelay = 0;
					break;
				}
			}
			break;
		case Slime::Phase::Final:
			if (PatternChange > 0.5f)
			{
				while (PatternChange > 0.5f)
					PatternChange -= 0.5f;

				switch (RND->Range(0, 3))
				{
				case 1:
					SlimePattern = Pattern::Jump;
					Gravity = -6.f;
					break;
				case 2:
					SlimePattern = Pattern::AttackIdle;
					break;
				case 3:
					SlimePattern = Pattern::Energy;
					isWave = true; EffectDelay = 0;
					break;
				case 4:
					SlimePattern = Pattern::ChargeIdle;
					isWave = true; EffectDelay = 0;
					break;
				}
			}
			break;
		}
		
	}
}

// ===========================================
// ######## ū ������ ���� ( Jump ) ###########
// ===========================================
void Slime::Jump()
{
	
}

// =======================================================
// ######## ū ������ ���� ��� ( JumpIdle ) ##############
// =======================================================
void Slime::JumpIdle()
{
	JumpDelay += TIMEMANAGER->getElapedTime();
	if (JumpDelay > 0.4f)
	{
		while (JumpDelay > 0.4f)
			JumpDelay -= 0.4f;

		JumpCount--;
		if (JumpCount > 0)
		{
			FrameIndex = 0;						// ������ �ε��� �ʱ�ȭ
			SlimePattern = Pattern::Jump;		// ���� ���� ��ȭ
			Gravity = -6.f;						// �߷� -4.f �ٽ� Ƣ�����
			isCrush = false;					// ���� �� �浹 X
		}
		else {
			FrameIndex = 0;						// ������ �ε��� �ʱ�ȭ
			Gravity = 0.f;						// �߷� �ʱ�ȭ
			JumpCount = 3;						// ���� Ƚ�� �ʱ�ȭ
			isJump = false;						// ���� ���� off	
			isCrush = true;						// ���� �� �浹 O
			SlimePattern = Pattern::ComeBack;	// �ǵ��ư��� ���� ��ȭ
		}
	}
}

// =================================================
// ######## ū ������ ���� �� ( Jumping ) ###########
// =================================================
void Slime::Jumping()
{
	AfterTime += TIMEMANAGER->getElapedTime();
	if (Math::GetDistance(JumpPos.x, JumpPos.y, player->GetPosition().x, player->GetPosition().y) < 5 || Gravity >= 7.5f)
	{
		SlimePattern = Pattern::ReSet;
		Gravity = 0.f;
		isCrush = true;
		CAMERA->Shake(10.5f, 0.3f);
		SOUNDMANAGER->play("����");
		for (int i = 0; i < 32; ++i)
		{
			EffectBullet->Fire("�������̺�",(Math::PI / 4.f) * (float)i, JumpPos, Vector2(128.f, 128.f), Vector2(100.f,100.f), 1.3f, 150.f);
		}
	}
	else
	{
		DirVector.SetDirVector(_position, player->GetPosition());
		_position += (DirVector * Speed * TIMEMANAGER->getElapedTime()) / 1.1f;
		JumpPos += (DirVector * Speed * TIMEMANAGER->getElapedTime()) / 1.1f;

		Gravity += 0.16f;
		JumpPos.y += Gravity;
		isCrush = false;

		if (AfterTime > 0.25f) {
			while (AfterTime > 0.25f)
				AfterTime -= 0.25f;
			EFFECT->ShowEffect("������PageOneIdle", JumpPos, Vector2(SlimeSize, SlimeSize), false, FrameIndex);
		}

	}
}

// ====================================================
// ######## ū ������ ���� �� ( AttackBack ) ###########
// ====================================================

void Slime::AttackBack()
{
	isCol = true;
	BackTime += TIMEMANAGER->getElapedTime();
	if (BackTime > 0.55f)
	{
		while (BackTime > 0.55f)
			BackTime -= 0.55f;
		SlimePattern = Pattern::AttackIdle;
	}
	else
	{
		_position += DirVector * Speed * (-1) * TIMEMANAGER->getElapedTime() / 4.f;
		JumpPos = _position;
	}
}

// =============================================
// ######## ū ������ ���� ( Attack ) ###########
// =============================================
void Slime::Attack()
{
	isCol = false;
	AfterTime += TIMEMANAGER->getElapedTime();
	_position += DirVector * Speed * 4.f * TIMEMANAGER->getElapedTime();
	JumpPos += DirVector * Speed * 4.f * TIMEMANAGER->getElapedTime();
	if (AfterTime > 0.1f)
	{
		while (AfterTime > 0.1f)
			AfterTime -= 0.1f;
		EFFECT->ShowEffect("������PageOneIdle", JumpPos, Vector2(SlimeSize,SlimeSize), false, FrameIndex);
	}
	
}

// ==========================================================
// ############ ū ������ ���� ���( AttackIdle ) ############
// =============================================s=============
void Slime::AttackIdle()
{
	if (AttackCount < 0)
	{
		AttackCount = 3;
		SlimePattern = Pattern::ComeBack;
	}
	IdleTime += TIMEMANAGER->getElapedTime();
	if (IdleTime > 0.1f)
	{
		while (IdleTime > 0.1f)
			IdleTime -= 0.1f;
		isUp = !isUp;
		AttackIdleCount++;
	}

	if (isUp)
	{
		JumpPos.y += 25.f * AttackIdleCount * TIMEMANAGER->getElapedTime();
	}
	else {
		JumpPos.y -= 25.f * AttackIdleCount * TIMEMANAGER->getElapedTime();
	}

	// �� �Ʒ� 10�� ������ ���� ( Attack ) 
	if (AttackIdleCount > 20 && AttackCount >= 0)
	{
		// �÷��̾� �������� �����Ѵٰ� ���⺤�� ����
		// ���� ��� ī��Ʈ �ʱ�ȭ
		// ���� ( Attack ) ���·� ��ȭ
		SOUNDMANAGER->play("��", 0.15f);
		DirVector.SetDirVector(JumpPos, player->GetPosition());
		AttackIdleCount = 0;
		SlimePattern = Pattern::Attack;	
		IdleTime = 0.f;
		AttackCount--;
	}
}

// =======================================================
// ######## ū ������ �ǵ��ư��� ( ComeBack ) #############
// =======================================================
void Slime::ComeBack()
{
	if (!EVENT->Eventing())
	{
		// ���ư��� �Ÿ��� ����ؼ� ���� ��������.
		float DirSpeed = Speed * Math::GetDistance(_position.x, _position.y,
			(float)64 * 23 / 2.f, (float)64 * 23 / 2.f);
		DirSpeed = Math::Clampf(DirSpeed, 50.f, 1000.f);
		DirVector.SetDirVector(_position, Vector2(64 * 23 / 2, 64 * 23 / 2));
		_position += (DirVector * DirSpeed * TIMEMANAGER->getElapedTime()) / 2.f;
		JumpPos = _position;

		// ��ġ�� �ǵ��ƿ��� �ٽ� Idle
		if (Math::GetDistance(_position.x, _position.y,
			(float)64 * 23 / 2.f, (float)64 * 23 / 2.f) < 10)
		{
			
			if (Hp <= 0)
			{
				SlimePattern = Pattern::Dash;
				this->Death();
			}
			else
			{
				SlimePattern = Pattern::Idle;
			}
		}
	}
}

// =======================================================
// ######## ū ������ ������ ź ( Energy ) ################
// =======================================================
void Slime::Energy()
{
	if (SizeUp && ShotBullet->GetActive())
	{
		SlimeSize++;
		if (SlimeSize > SlimeMax * 16)
			SlimeSize = SlimeMax * 16;
	}
	else
	{
		SlimeSize--;
		if (SlimeSize < 64.f)
			SlimeSize = 64.f;
	}
	if (!ShotBullet->GetFire())
	{
		DirVector.SetDirVector(_position, player->GetPosition());
		SOUNDMANAGER->play("������ź");
		ShotBullet->Fire(_position, DirVector, 450.f);
		SizeUp = true;
		EnergyCount--;
		if (EnergyCount < 0)
		{
			EffectDelay = 0;						// ������ �� Effet Delay �ʱ�ȭ
			EnergyCount = 5;						// ������ ź Ƚ�� �ʱ�ȭ
			SlimePattern = Pattern::EnergyIdle;		// ������ �⺻ ( Idle ) ���� �ʱ�ȭ
			isWave = false;							// ������ �� Effect �ʱ�ȭ
			ShotBullet->SetFire(false);				// ������ź Fire �ʱ�ȭ
		}
	}
}

// =======================================================
// ######## ū ������ ������ ź ��� ( EnergyIdle ) #######
// =======================================================
void Slime::EnergyIdle()
{
	if (SizeUp)
	{
		SlimeSize += 6.f;
		if (SlimeSize > SlimeMax * 16)
		{
			SlimeSize = SlimeMax * 16;
			SlimePattern = Pattern::Idle;
		}
	}
}

// =======================================================
// ######### ū ������ ���̱� Ȯ�� ( GatherCheck ) #########
// =======================================================
bool Slime::GatherCheck()
{
	if (!isGather)
	{
		for (int i = 0; i < SlimeMax; ++i)
		{
			if (SlimeList[i]->GetGather())
			{
				if (SlimeList[i]->GetActiveTime() > 0.125f)
				{
					SOUNDMANAGER->play("���̱�");
					isShow = true; BreakPoint = 50;
					SlimeSize += 16.f;
					SlimeAlpha += 0.05f;
					SlimeList[i]->SetActiveTime(0.f);
					SlimeList[i]->SetisActive(false);
					CAMERA->Shake(2.5f);
					return false;
				}
			}
		}
	}

	if (SlimeSize == SlimeMax * 16.f && !isGather)
	{
		SlimeAlpha = 1.f;
		isGather = true;
		CAMERA->Shake(10.f, 0.9f);
		BreakBar->SetisActive(true);
		BreakBar->SetHpWidth(80);
		BreakBar->SetWhiteHpWidth(80);
		return true;
	}
}

// =======================================================
// ############## ���� ������ ���̱� ( EnergyIdle ) #######
// =======================================================
void Slime::Gather()
{
	for (int i = 0; i < SlimeMax; ++i)
	{
		SlimeList[i]->Gather();
	}
}

// =======================================================
// ############## ���� ������ ���� ( Break ) ##############
// =======================================================
void Slime::Break()
{
	// �������� ���ذ��Ǿ�
	// �������� ��ü�� �� �̻� ������ �ʴ´�.
	SlimeSize = 0.f;
	SlimeAlpha = 0.f;
	SlimeMax -= 4;
	isShow = false;
	isGather = false;
	SOUNDMANAGER->play("�����");
	for (int i = 0; i < SlimeMax; ++i)
	{
		if (!SlimeList[i]->GetisActive()) SlimeList[i]->Rebirth();
		SlimeList[i]->SetGravity(-7.5f);
		SlimeList[i]->Break();
	}
	BreakBar->SetisActive(false);
}

void Slime::Charge()
{
	ChargeTime += TIMEMANAGER->getElapedTime();
	EFFECT->ShowEffect("����", _position, Vector2(144.f, 162.f), true, 0, 0.15f);
	if (ChargeTime > 1.2f)
	{
		while (ChargeTime > 1.2f)
			ChargeTime -= 1.2f;
		SlimePattern = Pattern::Shot;
		ChargeCount--;
	}

	if (ChargeCount < 0)
	{
		ChargeCount = 5; 
		EffectDelay = 0;
		isWave = false;
		SlimePattern = Pattern::Idle;
	}
}

void Slime::Shot()
{
	Vector2 temp;
	temp.SetDirVector(_position, player->GetPosition());
	ThunderRect->Fire(_position, Vector2(64.f, 64.f), temp.GetDegree());
	SlimePattern = Pattern::ChargeIdle;
}

void Slime::ChargeIdle()
{
	ChargeTime += TIMEMANAGER->getElapedTime();
	if (ChargeTime > 1.1f)
	{
		while (ChargeTime > 1.1f)
			ChargeTime -= 1.1f;
		ChargeTime = 0.f;
		SlimePattern = Pattern::Charge;
		SOUNDMANAGER->play("��¡",0.15f);
	}
}

void Slime::PingPongIdle()
{
	IdleTime += TIMEMANAGER->getElapedTime();
	if (IdleTime > 0.1f)
	{
		while (IdleTime > 0.1f)
			IdleTime -= 0.1f;
		isUp = !isUp;
		AttackIdleCount++;
	}

	if (isUp)
	{
		JumpPos.y += 25.f * AttackIdleCount * TIMEMANAGER->getElapedTime();
	}
	else {
		JumpPos.y -= 25.f * AttackIdleCount * TIMEMANAGER->getElapedTime();
	}

	if (AttackIdleCount > 20 && AttackCount >= 0)
	{
		DirVector.SetDirVector(JumpPos, player->GetPosition());
		AttackIdleCount = 0;
		SlimePattern = Pattern::Dash;
		IdleTime = 0.f;
		AttackCount--;
	}
}

void Slime::PingPong() 
{
	PongTime += TIMEMANAGER->getElapedTime();
	if (PongTime > 0.01f)
	{
		Pong = false;
	}
	isCol = false; isDash = true; 
	AfterTime += TIMEMANAGER->getElapedTime();

	_position += DirVector * Speed * TIMEMANAGER->getElapedTime() * 4.f;
	JumpPos += DirVector * Speed * 4.f * TIMEMANAGER->getElapedTime();


	if (AfterTime > 0.1f)
	{
		while (AfterTime > 0.1f)
			AfterTime -= 0.1f;
		EFFECT->ShowEffect("������PageOneIdle", _position, Vector2(SlimeSize, SlimeSize), false, FrameIndex);
	}

	if (AttackCount > 15)
	{
		AttackCount = 0;
		SlimePattern = Pattern::DashComeBack;
	}
}

void Slime::PingPongBack()
{
	if (!EVENT->Eventing())
	{
		// ���ư��� �Ÿ��� ����ؼ� ���� ��������.
		float DirSpeed = Speed * Math::GetDistance(_position.x, _position.y,
			(float)64 * 23 / 2.f, (float)64 * 23 / 2.f);
		DirSpeed = Math::Clampf(DirSpeed, 50.f, 1000.f);
		DirVector.SetDirVector(_position, Vector2(64 * 23 / 2, 64 * 23 / 2));
		_position += (DirVector * DirSpeed * TIMEMANAGER->getElapedTime()) / 4.5f;
		JumpPos = _position;

		// ��ġ�� �ǵ��ƿ��� �ٽ� Idle
		if (Math::GetDistance(_position.x, _position.y,
			(float)64 * 23 / 2.f, (float)64 * 23 / 2.f) < 10)
		{

			if (Hp <= 0)
			{
				SlimePattern = Pattern::Death;
				this->Death();
			}
			else
			{
				SlimePattern = Pattern::Idle;
			}
		}
	}
}

void Slime::FrameUpdate()
{
	switch (SlimePattern)
	{
		// ======================================
		// ######## ū ������ �⺻ ( Idle ) #####
		// ======================================
	case Pattern::Idle:
		FrameCount += TIMEMANAGER->getElapedTime();
		if (FrameCount > 0.1f)
		{
			FrameIndex++;
			while (FrameCount > 0.1f)
				FrameCount -= 0.1f;
			if (FrameIndex > 5)
				FrameIndex = 0;
		}
		break;
		// ======================================
		// ######## ū ������ ���� ( Jump ) #####
		// ======================================
	case Pattern::Jump:
		FrameCount += TIMEMANAGER->getElapedTime();
		if (FrameCount > 0.1f)
		{
			FrameIndex++;
			while (FrameCount > 0.1f)
				FrameCount -= 0.1f;
			if (FrameIndex > 5)
			{
				FrameIndex = 0;
				SlimePattern = Pattern::Jumping;
			}
		}
				
		break;
		// ===========================================
		// ######## ū ������ ���� �� ( Jumping ) #####
		// ===========================================
	case Pattern::Jumping:
		FrameCount += TIMEMANAGER->getElapedTime();
		if (FrameCount > 0.1f)
		{
			FrameIndex++;
			while (FrameCount > 0.1f)
				FrameCount -= 0.1f;
			if (FrameIndex > 5)
				FrameIndex = 0;
		}

		// ===========================================
		// #### ū ������ ���� ��� ( JumpIdle ) #####
		// ===========================================
	case Pattern::JumpIdle:
		FrameCount += TIMEMANAGER->getElapedTime();
		if (FrameCount > 0.1f)
		{
			FrameIndex++;
			while (FrameCount > 0.1f)
				FrameCount -= 0.1f;
			if (FrameIndex > 5)
				FrameIndex = 0;
		}
		break;

		// ===========================================
		// #### ū ������ �ǵ��ư��� ( ComeBack ) #####
		// ===========================================
	case Pattern::ComeBack:
		FrameCount += TIMEMANAGER->getElapedTime();
		if (FrameCount > 0.1f)
		{
			FrameIndex++;
			while (FrameCount > 0.1f)
				FrameCount -= 0.1f;
			if (FrameIndex > 5)
				FrameIndex = 0;
		}
		break;

		// ===========================================
		// #### ū ������ ���� ( Attack ) #####
		// ===========================================
	case Pattern::Attack:
		FrameCount += TIMEMANAGER->getElapedTime();
		if (FrameCount > 0.1f)
		{
			FrameIndex++;
			while (FrameCount > 0.1f)
				FrameCount -= 0.1f;
			if (FrameIndex > 5)
				FrameIndex = 0;
		}

		break;
		// ===========================================
		// #### ū ������ ���� ��� ( AttackIdle ) #####
		// ===========================================
	case Pattern::AttackIdle:
		FrameCount += TIMEMANAGER->getElapedTime();
		if (FrameCount > 0.1f)
		{
			FrameIndex++;
			while (FrameCount > 0.1f)
				FrameCount -= 0.1f;
			if (FrameIndex > 5)
				FrameIndex = 0;
		}

		break;
		// ===========================================
		// #### ū ������ ���� �� ( AttackBack ) ######
		// ===========================================
	case Pattern::AttackBack:
		FrameCount += TIMEMANAGER->getElapedTime();
		if (FrameCount > 0.1f)
		{
			FrameIndex++;
			while (FrameCount > 0.1f)
				FrameCount -= 0.1f;
			if (FrameIndex > 5)
				FrameIndex = 0;
		}
		break;

		// ===========================================
		// #### ū ������ ���� ( Dash ) #########
		// ===========================================
		case Pattern::Dash:
			FrameCount += TIMEMANAGER->getElapedTime();
			if (FrameCount > 0.1f)
			{
				FrameIndex++;
				while (FrameCount > 0.1f)
					FrameCount -= 0.1f;
				if (FrameIndex > 5)
					FrameIndex = 0;
			}

		break;
		// ===========================================
		// #### ū ������ ���� ��� ( DashIdle ) ######
		// ===========================================
		case Pattern::DashIdle:
			FrameCount += TIMEMANAGER->getElapedTime();
			if (FrameCount > 0.1f)
			{
				FrameIndex++;
				while (FrameCount > 0.1f)
					FrameCount -= 0.1f;
				if (FrameIndex > 5)
					FrameIndex = 0;
			}

		break;
		// =============================================
		// #### ū ������ �뽬 �� ( DashComeBack ) ######
		// =============================================
		case Pattern::DashComeBack:
			FrameCount += TIMEMANAGER->getElapedTime();
			if (FrameCount > 0.1f)
			{
				FrameIndex++;
				while (FrameCount > 0.1f)
					FrameCount -= 0.1f;
				if (FrameIndex > 5)
					FrameIndex = 0;
			}
		break;

		// ===========================================
		// ##### ū ������ ������ ź ( Energy ) #######
		// ===========================================
	case Pattern::Energy:
		FrameCount += TIMEMANAGER->getElapedTime();
		if (FrameCount > 0.1f)
		{
			FrameIndex++;
			while (FrameCount > 0.1f)
				FrameCount -= 0.1f;
			if (FrameIndex > 5)
				FrameIndex = 0;
		}
		break;

		// ====================================================
		// ##### ū ������ ������ ź ��� ( EnergyIdle ) ########
		// ====================================================
	case Pattern::EnergyIdle:
		FrameCount += TIMEMANAGER->getElapedTime();
		if (FrameCount > 0.1f)
		{
			FrameIndex++;
			while (FrameCount > 0.1f)
				FrameCount -= 0.1f;
			if (FrameIndex > 5)
				FrameIndex = 0;
		}
		break;

		// ====================================================
		// ########### ū ������ ���� ���� ( Charge ) #######
		// ====================================================
	case Pattern::Charge:
		FrameCount += TIMEMANAGER->getElapedTime();
		if (FrameCount > 0.1f)
		{
			FrameIndex++;
			while (FrameCount > 0.1f)
				FrameCount -= 0.1f;
			if (FrameIndex > 5)
				FrameIndex = 0;
		}
		break;

		// ====================================================
		// ########### ū ������ ���� �߻� ( Shot ) ############
		// ====================================================
	case Pattern::Shot:
		FrameCount += TIMEMANAGER->getElapedTime();
		if (FrameCount > 0.1f)
		{
			FrameIndex++;
			while (FrameCount > 0.1f)
				FrameCount -= 0.1f;
			if (FrameIndex > 5)
				FrameIndex = 0;
		}
		break;

		// ==========================================================
		// ########### ū ������ ���� �߻� �� ���( ChargeIdle) #######
		// ==========================================================
	case Pattern::ChargeIdle:
		FrameCount += TIMEMANAGER->getElapedTime();
		if (FrameCount > 0.1f)
		{
			FrameIndex++;
			while (FrameCount > 0.1f)
				FrameCount -= 0.1f;
			if (FrameIndex > 5)
				FrameIndex = 0;
		}
		break;

		// ====================================================
		// ########## ū ������ ��ǥ �缳�� ( ReSet ) ###########
		// ====================================================
	case Pattern::ReSet:
		FrameCount += TIMEMANAGER->getElapedTime();
		if (FrameCount > 0.1f)
		{
			FrameIndex++;
			while (FrameCount > 0.1f)
				FrameCount -= 0.1f;
			if (FrameIndex > 5)
				FrameIndex = 0;
		}
		break;

		// ====================================================
		// ############### ū ������ ���� ( Death ) ############
		// ====================================================

	case Pattern::Death:
		FrameCount += TIMEMANAGER->getElapedTime();
		if (FrameCount > 0.1f)
		{
			FrameIndex++;
			while (FrameCount > 0.1f)
				FrameCount -= 0.1f;
			if (FrameIndex > 5)
				FrameIndex = 0;
		}
		break;
	}
}

void Slime::PhaseCheck()
{
	// �ִ� ü�� 5000
	// --------------------
	// �ִ� ü���� 75% �̻�
	// Phase : One , SlimeMax : 16
	if (Hp > 3750)
	{
		SlimePhase = Phase::One;
		SlimeMax = 16;
	}
	// --------------------
	// �ִ� ü���� 50% �̻�
	// Phase : Two , SlimeMax : 12
	else if (Hp <= 3750 && Hp > 2500)
	{
		if (SlimePhase == Phase::One)
		{
			SlimeMax = 14;
			this->Gather();
			CAMERA->Shake(10.f, 0.7f);
			EVENT->AddEvent(new CameraChange(this));
			EVENT->AddEvent(new CameraZoom);
			EVENT->AddEvent(new CameraChange(player));
			SlimePhase = Phase::Two;
		}
	}
	// --------------------
	// �ִ� ü���� 25% �̻�
	// Phase : Three , SlimeMax : 8
	else if (Hp <= 2500 && Hp > 1250)
	{
		if (SlimePhase == Phase::Two)
		{
			SlimeMax = 12;
			this->Gather();
			CAMERA->Shake(10.f, 0.7f);
			EVENT->AddEvent(new CameraChange(this));
			EVENT->AddEvent(new CameraZoom);
			EVENT->AddEvent(new CameraChange(player));
			SlimePhase = Phase::Three;
		}
	}
	// --------------------
	// �ִ� ü���� 25 �̸�
	// Phase : Final , SlimeMax : 4
	else {
		if (SlimePhase == Phase::Three)
		{
			SlimeMax = 10;
			this->Gather();
			CAMERA->Shake(10.f, 0.7f);
			EVENT->AddEvent(new CameraChange(this));
			EVENT->AddEvent(new CameraZoom);
			EVENT->AddEvent(new CameraChange(player));
			SlimePhase = Phase::Final;
		}
	}
}

void Slime::CircleUpdate(Vector2 size, float alpha)
{
	if (isShow)
	{
		EffectDelay += TIMEMANAGER->getElapedTime();			// �ι�° �� Effect �ణ�� ������
		if (!isWave && !Die)
		{
			if (EffectDelay > 1.5f)
			{
				for (int i = 0; i < 2; ++i)
				{
					Circle[i].Size.x = Circle[i].Size.x + 4.f;
					Circle[i].Size.y = Circle[i].Size.y + 4.f;

					Circle[i].Alpha -= 0.002f;
					if (Circle[i].Alpha <= 0.f)
					{

						Circle[i].Alpha = alpha;
						Circle[i].Size = size;
					}
				}
			}
			else
			{
				Circle[0].Size.x = Circle[0].Size.x + 4.f;
				Circle[0].Size.y = Circle[0].Size.y + 4.f;

				Circle[0].Alpha -= 0.002f;
				if (Circle[0].Alpha <= 0.f)
				{

					Circle[0].Alpha = alpha;
					Circle[0].Size = size;
				}
			}
		}
		else if(isWave && !Die)
		{
			if (EffectDelay > 4.5f)
			{
				for (int i = 2; i < 8; ++i)
				{
					Circle[i].Angle += 4.f + ((2 - i) * (Math::PI / 6.f));
					//Circle[i].Angle += 3.f ;
					Circle[i].Size.x = Circle[i].Size.x + 5.f;
					Circle[i].Size.y = Circle[i].Size.y + 5.f;
			
					Circle[i].Alpha -= 0.002f;
					if (Circle[i].Alpha <= 0.f)
					{
						Circle[i].Alpha = alpha;
						Circle[i].Size = size;
					}
				}
			}
			else if (EffectDelay > 3.f)
			{
				SizeUp = false;
				for (int i = 2; i < 6; ++i)
				{
					Circle[i].Angle += 2.5f + ((2 - i) * (Math::PI / 6.f));
					//Circle[i].Angle += 3.f;
					Circle[i].Size.x = Circle[i].Size.x + 5.f;
					Circle[i].Size.y = Circle[i].Size.y + 5.f;

					Circle[i].Alpha -= 0.002f;
					if (Circle[i].Alpha <= 0.f)
					{

						Circle[i].Alpha = alpha;
						Circle[i].Size = size;
					}
				}
			}
			else {
				for (int i = 2; i < 4; ++i)
				{
					Circle[i].Angle += 1.5f + ((2 - i) * (Math::PI / 6.f));
					//Circle[i].Angle += 3.f;
					Circle[i].Size.x = Circle[i].Size.x + 5.f;
					Circle[i].Size.y = Circle[i].Size.y + 5.f;

					Circle[i].Alpha -= 0.002f;
					if (Circle[i].Alpha <= 0.f)
					{

						Circle[i].Alpha = alpha;
						Circle[i].Size = size;
					}
				}
			}
		}

		if (Die)
		{
			DieTime += TIMEMANAGER->getElapedTime();

			if (DieTime > 0.5f)
			{
				for (int i = 0; i < 1; ++i)
				{
					DieCircle[i].Size.x += 40.f;
					DieCircle[i].Size.y += 40.f;
					DieCircle[i].Alpha -= 0.002f;
					if (DieCircle[i].Alpha <= 0.f)
					{
						DieCircle[i].Alpha = 0.f;
					}
				}
			}
			else if (DieTime > 1.0f)
			{
				for (int i = 0; i < 2; ++i)
				{
					DieCircle[i].Size.x += 40.f;
					DieCircle[i].Size.y += 40.f;
					DieCircle[i].Alpha -= 0.002f;
					if (DieCircle[i].Alpha <= 0.f)
					{
						DieCircle[i].Alpha = 0.f;
					}
				}
			}
			else if (DieTime > 1.5f)
			{
				for (int i = 0; i < 3; ++i)
				{
					DieCircle[i].Size.x += 40.f;
					DieCircle[i].Size.y += 40.f;
					DieCircle[i].Alpha -= 0.002f;
					if (DieCircle[i].Alpha <= 0.f)
					{
						DieCircle[i].Alpha = 0.f;
					}
				}
			}
			else if (DieTime > 2.0f)
			{
				for (int i = 0; i < 4; ++i)
				{
					DieCircle[i].Size.x += 40.f;
					DieCircle[i].Size.y += 40.f;
					DieCircle[i].Alpha -= 0.002f;
					if (DieCircle[i].Alpha <= 0.f)
					{
						DieCircle[i].Alpha = 0.f;
					}
				}
			}
			else if (DieTime > 2.5f)
			{
				for (int i = 0; i < 5; ++i)
				{
					DieCircle[i].Size.x += 40.f;
					DieCircle[i].Size.y += 40.f;
					DieCircle[i].Alpha -= 0.002f;
					if (DieCircle[i].Alpha <= 0.f)
					{
						DieCircle[i].Alpha = 0.f;
					}
				}
			}
			else if (DieTime > 3.0f)
			{
				for (int i = 0; i < 6; ++i)
				{
					DieCircle[i].Size.x += 40.f;
					DieCircle[i].Size.y += 40.f;
					DieCircle[i].Alpha -= 0.002f;
					if (DieCircle[i].Alpha <= 0.f)
					{
						DieCircle[i].Alpha = 0.f;
					}
				}
			}
			else if (DieTime > 3.5f)
			{
				for (int i = 0; i < 7; ++i)
				{
					DieCircle[i].Size.x += 40.f;
					DieCircle[i].Size.y += 40.f;
					DieCircle[i].Alpha -= 0.002f;
					if (DieCircle[i].Alpha <= 0.f)
					{
						DieCircle[i].Alpha = 0.f;
					}
				}
			}
			else if (DieTime > 4.0f)
			{
				for (int i = 0; i < 8; ++i)
				{
					DieCircle[i].Size.x += 40.f;
					DieCircle[i].Size.y += 40.f;
					DieCircle[i].Alpha -= 0.002f;
					if (DieCircle[i].Alpha <= 0.f)
					{
						DieCircle[i].Alpha = 0.f;
					}
				}
			}
			else if (DieTime > 4.5f)
			{
				for (int i = 0; i < 9; ++i)
				{
					DieCircle[i].Size.x += 40.f;
					DieCircle[i].Size.y += 40.f;
					DieCircle[i].Alpha -= 0.002f;
					if (DieCircle[i].Alpha <= 0.f)
					{
						DieCircle[i].Alpha = 0.f;
					}
				}
			}
			else if (DieTime > 5.f)
			{
				for (int i = 0; i < 10; ++i)
				{
					DieCircle[i].Size.x += 40.f;
					DieCircle[i].Size.y += 40.f;
					DieCircle[i].Alpha -= 0.002f;
					if (DieCircle[i].Alpha <= 0.f)
					{
						DieCircle[i].Alpha = 0.f;
					}
				}
			}
		}
	}
}

void Slime::CircleRender()
{
	Vector2 TempPos = CAMERA->GetRelativeVector2(_position);
	Vector2 TempJumpPos = CAMERA->GetRelativeVector2(JumpPos);
	if (isShow)
	{
		if (!isWave && !Die)
		{
			if (EffectDelay > 1.5f)
			{
				for (int i = 0; i < 2; ++i)
				{
					Circle[i].mImage->SetAlpha(Circle[i].Alpha);
					Circle[i].mImage->SetSize(Circle[i].Size / ((float)SlimePhase + 0.5f) * CAMERA->GetZoom());

					if (SlimePattern == Pattern::AttackIdle)
					{
						Circle[i].mImage->FrameRender((int)TempPos.x, (int)TempPos.y, 0, 0);
					}
					else
					{
						Circle[i].mImage->FrameRender((int)TempJumpPos.x, (int)TempJumpPos.y, 0, 0);
					}
				}
			}
			else
			{
				Circle[0].mImage->SetAlpha(Circle[0].Alpha);
				Circle[0].mImage->SetSize(Circle[0].Size / ((float)SlimePhase + 0.5f) * CAMERA->GetZoom());
				if (SlimePattern == Pattern::AttackIdle)
				{
					Circle[0].mImage->FrameRender((int)TempPos.x, (int)TempPos.y, 0, 0);
				}
				else
				{
					Circle[0].mImage->FrameRender((int)TempJumpPos.x, (int)TempJumpPos.y, 0, 0);
				}
			}
		}
		else if (!Die && isWave)
		{
			if (EffectDelay > 4.5f)
			{
				for (int i = 2; i < 8; ++i)
				{
					if (i % 2 == 0)
						Circle[i].mImage->SetAngle(-Circle[i].Angle);
					else
						Circle[i].mImage->SetAngle(Circle[i].Angle);
					Circle[i].mImage->SetAlpha(Circle[i].Alpha);
					Circle[i].mImage->SetSize(Circle[i].Size / ((float)SlimePhase + 0.5f) * CAMERA->GetZoom());
					Circle[i].mImage->FrameRender((int)TempJumpPos.x, (int)TempJumpPos.y, 1, 0);
				}
				
			}
			else if (EffectDelay > 3.f)
			{
				SizeUp = true;
				for (int i = 2; i < 6; ++i)
				{
					if (i % 2 == 0)
						Circle[i].mImage->SetAngle(-Circle[i].Angle);
					else
						Circle[i].mImage->SetAngle(Circle[i].Angle);
					Circle[i].mImage->SetAlpha(Circle[i].Alpha);
					Circle[i].mImage->SetSize(Circle[i].Size / ((float)SlimePhase + 0.5f) * CAMERA->GetZoom());
					Circle[i].mImage->FrameRender((int)TempJumpPos.x, (int)TempJumpPos.y, 1, 0);
				}
			}
			else
			{
				for (int i = 2; i < 4; ++i)
				{
					if (i % 2 == 0)
						Circle[i].mImage->SetAngle(-Circle[i].Angle);
					else
						Circle[i].mImage->SetAngle(Circle[i].Angle);
					Circle[i].mImage->SetAlpha(Circle[i].Alpha);
					Circle[i].mImage->SetSize(Circle[i].Size / ((float)SlimePhase + 0.5f) * CAMERA->GetZoom());
					Circle[i].mImage->FrameRender((int)TempJumpPos.x, (int)TempJumpPos.y, 1, 0);
				}
	
			}
		}

		if (Die)
		{
			if (DieTime > 0.1f)
			{
				for (int i = 0; i < 1; ++i)
				{
					DieCircle[i].mImage->SetAlpha(DieCircle[i].Alpha);
					DieCircle[i].mImage->SetSize(DieCircle[i].Size);
					DieCircle[i].mImage->FrameRender((int)TempJumpPos.x, (int)TempJumpPos.y, 0, 0); 
				}
			}
			else if (DieTime > 0.2f)
			{
				for (int i = 0; i < 2; ++i)
				{
					DieCircle[i].mImage->SetAlpha(DieCircle[i].Alpha);
					DieCircle[i].mImage->SetSize(DieCircle[i].Size);
					DieCircle[i].mImage->FrameRender((int)TempJumpPos.x, (int)TempJumpPos.y, 0, 0);
				}
			}
			else if (DieTime > 0.3f)
			{
				for (int i = 0; i < 3; ++i)
				{
					DieCircle[i].mImage->SetAlpha(DieCircle[i].Alpha);
					DieCircle[i].mImage->SetSize(DieCircle[i].Size);
					DieCircle[i].mImage->FrameRender((int)TempJumpPos.x, (int)TempJumpPos.y, 0, 0);
				}
			}
			else if (DieTime > 0.4f)
			{
				for (int i = 0; i < 4; ++i)
				{
					DieCircle[i].mImage->SetAlpha(DieCircle[i].Alpha);
					DieCircle[i].mImage->SetSize(DieCircle[i].Size);
					DieCircle[i].mImage->FrameRender((int)TempJumpPos.x, (int)TempJumpPos.y, 0, 0);
				}
			}
			else if (DieTime > 0.5f)
			{
				for (int i = 0; i < 5; ++i)
				{
					DieCircle[i].mImage->SetAlpha(DieCircle[i].Alpha);
					DieCircle[i].mImage->SetSize(DieCircle[i].Size);
					DieCircle[i].mImage->FrameRender((int)TempJumpPos.x, (int)TempJumpPos.y, 0, 0);
				}
			}
			else if (DieTime > 0.6f)
			{
				for (int i = 0; i < 6; ++i)
				{
					DieCircle[i].mImage->SetAlpha(DieCircle[i].Alpha);
					DieCircle[i].mImage->SetSize(DieCircle[i].Size);
					DieCircle[i].mImage->FrameRender((int)TempJumpPos.x, (int)TempJumpPos.y, 0, 0);
				}
			}
			else if (DieTime > 0.7f)
			{
				for (int i = 0; i < 7; ++i)
				{
					DieCircle[i].mImage->SetAlpha(DieCircle[i].Alpha);
					DieCircle[i].mImage->SetSize(DieCircle[i].Size);
					DieCircle[i].mImage->FrameRender((int)TempJumpPos.x, (int)TempJumpPos.y, 0, 0);
				}
			}
			else if (DieTime > 0.8f)
			{
				for (int i = 0; i < 8; ++i)
				{
					DieCircle[i].mImage->SetAlpha(DieCircle[i].Alpha);
					DieCircle[i].mImage->SetSize(DieCircle[i].Size);
					DieCircle[i].mImage->FrameRender((int)TempJumpPos.x, (int)TempJumpPos.y, 0, 0);
				}
			}
			else if (DieTime > 0.9f)
			{
				for (int i = 0; i < 9; ++i)
				{
					DieCircle[i].mImage->SetAlpha(DieCircle[i].Alpha);
					DieCircle[i].mImage->SetSize(DieCircle[i].Size);
					DieCircle[i].mImage->FrameRender((int)TempJumpPos.x, (int)TempJumpPos.y, 0, 0);
				}
			}
			else if (DieTime > 1.f)
			{
				for (int i = 0; i < 10; ++i)
				{
					DieCircle[i].mImage->SetAlpha(DieCircle[i].Alpha);
					DieCircle[i].mImage->SetSize(DieCircle[i].Size);
					DieCircle[i].mImage->FrameRender((int)TempJumpPos.x, (int)TempJumpPos.y, 0, 0);
				}
			}
		}
	}
}

void Slime::SlimeCoreRender()
{
	if (!Die)
	{
		Vector2 TempJumpPos = CAMERA->GetRelativeVector2(JumpPos);
		IMAGEMANAGER->FindImage("������Core")->SetSize(Vector2(48.f, 48.f) * CAMERA->GetZoom());
		IMAGEMANAGER->FindImage("������Core")->FrameRender(TempJumpPos.x, TempJumpPos.y,
			CoreIndex, 0);
	}
}

void Slime::SlimeCoreUpdate()
{
	CoreCount += TIMEMANAGER->getElapedTime();
	if (CoreCount > 0.1f)
	{
		CoreIndex++;
		while (CoreCount > 0.1f)
			CoreCount -= 0.1f;
		if (CoreIndex > 6) CoreIndex = 0;
	}
}

void Slime::MiniSlimeMake()
{
	// ���� ������ ��ġ �����
	for (int i = 0; i < SlimeMax; ++i)
	{
		if (i < 8)
		{
			if (i % 2 == 0)
			{
				SlimeList.push_back(new MiniSlime("������", Vector2(48.f, 48.f),
					Vector2(
						256 + 64 * i,
						800 + 64 * i), Pivot::Center));
			}
			else
			{
				SlimeList.push_back(new MiniSlime("������", Vector2(48.f, 48.f),
					Vector2(
						256 + 64 * i,
						800 - 64 * i), Pivot::Center));
			}

		}
		else
		{
			if (i % 2 == 0)
			{
				SlimeList.push_back(new MiniSlime("������", Vector2(48.f, 48.f),
					Vector2(
						256 + 64 * i,
						800 - 64 * (15 - i)), Pivot::Center));
			}
			else
			{
				SlimeList.push_back(new MiniSlime("������", Vector2(48.f, 48.f),
					Vector2(
						256 + 64 * i,
						800 + 64 * (15 - i)), Pivot::Center));
			}
		}
	}

	for (int i = 0; i < 100; ++i)
	{
		int X = RND->Range(0, 15);
		int Y = RND->Range(0, 15);
		MiniSlime* temp = SlimeList[X];
		SlimeList[X] = SlimeList[Y];
		SlimeList[Y] = temp;
	}

	for (int i = 0; i < 16; ++i)
	{
		OBJECTMANAGER->addObject(ObjectType::Enemy, SlimeList[i]);
	}


}

void Slime::RectUpdate()
{
	HitRect = RectMakeCenter(_position.x, _position.y, _size.x, _size.y);
	AttackRect = HitRect;
	ColRect = RectMakeCenter(_position.x, _position.y + _size.y / 3.f, _size.x, _size.y / 3.f);
}

void Slime::ProgressBarUpdate()
{
	Hpbar->WhiteSetGauge(Hp, 5000.f);
	BreakBar->WhiteSetGauge(BreakPoint, 50);
	BreakBar->SetPosition(Vector2(JumpPos.x - 40.f, JumpPos.y + SlimeSize / 2.f));
}

void Slime::Hit(int damage)
{
	Hp -= damage;
	isHit = true;
	HitTime = 0.f;
	this->PhaseCheck();
	if (Hp <= 0)
	{
		EVENT->AddEvent(new CameraChange(this));
		EVENT->AddEvent(new CameraZoom);
		SlimePattern = Pattern::DashIdle;
	}
}

void Slime::Hiting()
{
	if (isHit)
	{
		HitTime += TIMEMANAGER->getElapedTime();
		if (HitTime > 0.15f)
		{
			while (HitTime > 0.15f)
				HitTime -= 0.15f;
			isHit = false;
			Hpbar->SetisWhite(true);
		}
		else
		{
			Hpbar->SetGauge(Hp, 5000.f);
			Hpbar->SetisWhite(false);
		}
		
	}
}

void Slime::BreakHit()
{
	BreakPoint--;
	isBreak = true;
	BreakTime = 0.f;
	if (BreakPoint <= 0 && !EVENT->Eventing())
	{
		CAMERA->Shake(10.f, 0.7f);
		EVENT->AddEvent(new CameraChange(this));
		EVENT->AddEvent(new CameraZoom);
		EVENT->AddEvent(new BossBreak(this));
		EVENT->AddEvent(new CameraChange(player));
		SlimePattern = Pattern::ComeBack;
	}
}

void Slime::BreakHiting()
{
	if (isBreak)
	{
		BreakTime += TIMEMANAGER->getElapedTime();
		if (BreakTime > 0.15f)
		{
			while (BreakTime > 0.15f)
				BreakTime -= 0.15f;
			isBreak = false;
			BreakBar->SetisWhite(true);
		}
		else
		{
			BreakBar->SetGauge(BreakPoint, 50);
			BreakBar->SetisWhite(false);
		}
	}
}

void Slime::Death()
{
	SizeCount += TIMEMANAGER->getElapedTime();
	if (SizeCount > 0.15f && SizeCheck != 20)
	{
		while (SizeCount > 0.15f)
			SizeCount -= 0.15f;
		SizeUp = !SizeUp;
		SizeCheck++;
	}

	if (SizeUp && SizeCheck != 20)
	{
		SlimeSize += 1.f * SizeCheck;
	}
	else if (!SizeUp && SizeCheck != 20)
	{
		SlimeSize -= 1.f * SizeCheck;
	}

	if (SizeCheck == 20 && !Boom)
	{
		SlimeSize -= 1.f * SizeCheck;
	}

	if (SlimeSize <= 0.f && !Die)
	{
		Boom = true;  Die = true; 
		SlimeSize = 0.f;
		BreakBar->SetisActive(false);
		SOUNDMANAGER->play("���");
		SOUNDMANAGER->play("���");
		SOUNDMANAGER->play("���");
		SOUNDMANAGER->stop("��Ʋ");
		EVENT->AddEvent(new CameraZoom);
		CAMERA->Shake(10.5f, 2.5f);
		SOUNDMANAGER->play("������");
		EFFECT->ShowEffect("ū����", _position, Vector2(512,512) * CAMERA->GetZoom(), true, 0,0.1f);
		EVENT->AddEvent(new Delay(2.5f));
		EVENT->AddEvent(new CameraChange(player));
		EVENT->AddEvent(new Delay(1.5f));
		EVENT->AddEvent(new FadeIn);
		EVENT->AddEvent(new ChangeScene("Ÿ��Ʋ"));
		for (int i = 0; i < 48; ++i)
		{
			for (int j = 0; j < 3; ++j)
			{
				SlimeParticle->Make(_position, 250.f, -3.5f, RND->Range(i * 30.f, (i + 1) * 30.f), 64.f);
			}
		}
	}
}

void Slime::StateUpdate()
{
	switch (SlimePattern)
	{
	case Slime::Pattern::Idle:
		this->Idle();
		break;
	case Slime::Pattern::Jump:
		this->Jump();
		break;
	case Slime::Pattern::Jumping:
		this->Jumping();
		break;
	case Slime::Pattern::JumpIdle:
		this->JumpIdle();
		break;
	case Slime::Pattern::ComeBack:
		this->ComeBack();
		break;
	case Slime::Pattern::Energy:
		this->Energy();
		break;
	case Slime::Pattern::EnergyIdle:
		this->EnergyIdle();
		break;
	case Slime::Pattern::Attack:
		this->Attack();
		break;
	case Slime::Pattern::AttackIdle:
		this->AttackIdle();
		break;
	case Slime::Pattern::AttackBack:
		this->AttackBack();
		break;
	case Slime::Pattern::Dash:
		this->PingPong();
		break;
	case Slime::Pattern::DashIdle:
		this->PingPongIdle();
		break;
	case Slime::Pattern::DashComeBack:
		this->PingPongBack();
		break;
	case Slime::Pattern::Break:
		this->Break();
		break;
	case Slime::Pattern::Gather:
		this->Gather();
		break;
	case Slime::Pattern::ReSet:
		this->SetJumpPos();
		break;
	case Slime::Pattern::ChargeIdle:
		this->ChargeIdle();
		break;
	case Slime::Pattern::Charge:
		this->Charge();
		break;
	case Slime::Pattern::Shot:
		this->Shot();
		break;
	case Slime::Pattern::Death:
		this->Death();
		break;
	}
}

Slime::Slime(string name, Vector2 size, Vector2 pos, Pivot pivot)
	: GameObject(name, pos, size, pivot)
{
}


Slime::~Slime()
{
}
