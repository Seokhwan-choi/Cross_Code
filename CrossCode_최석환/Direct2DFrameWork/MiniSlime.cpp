#include "stdafx.h"
#include "Player.h"
#include "MiniSlime.h"
#include "Slime.h"
#include "ProgressBar.h"

MiniSlime::MiniSlime(string name, Vector2 size, Vector2 position, Pivot pivot)
	: GameObject(name, size, position, pivot)
{
	SlimePattern = Pattern::Idle;	
	FrameCount = 0.f;				// ���� ������ ������ ī��Ʈ 0.f �ʱ�ȭ
	FrameIndex = 0;					// ���� ������ ������ �ε��� 0 �ʱ�ȭ
	Gravity = 0.f;					// ���� ������ �߷� 0.f
	Speed = 100.f;					// ���� ������ �̵��ӵ� 400. f �ʱ�ȭ
	Hp = 150;						// ���� ������ ü�� 80 �ʱ�ȭ
	EffectDelay = 0;				// ���� ������ �� EffectDelay �ð� 0.f �ʱ�ȭ
	AttackTime = 0.f;				// ���� ������ ���� �غ� �ð� 0.f �ʱ�ȭ
	isGather = false;				// ���� ������ �𿴴�..? 
	isBreak = true;					// ���� ������ �������..?
	isCrush = true;					// ���� ������ �浹����
	isCol = true;					// ���� ������ �� �浹 ����
	isAttack = false;				// ���� ������ ���� ��
	ActiveTime = 0.f;				// ���� ������ ���������

	RebirthPos = _position;							 // ���� ������ �ǻ�Ƴ��� ��ǥ
	JumpPos = _position;							 // ���� ������ ������ ��ǥ
	Shadow = IMAGEMANAGER->FindImage("�����ӱ׸���");	 // ���� ������ �׸��� Image

	AfterTime = 0.f;								// ���� ������ �ܻ� Delay
	GatherUpTime = 0.f;								// ���� ������ GatherUp ���� �ð�
	GatherIdleTime = 0.f;							// ���� ������ GatherIdle ���� �ð�
	IdleChangeTime = 0.f;							// ���� ������ GatherIdle ��/�Ʒ� ��ȭ �ð�
	IdleChange = false;								// ���� ������ GatherIdle ��ȭ ����

	isChasor = false;
	HitTime = 0.f;									// ���� ������ Hit ���ӽð�
	AlphaStart = false;								// ���� ������ ���� ����
	isAlpha = false;								// ���� ������ ���� ����
	Alpha = 1.f;									// ���� ������ ����
	AlphaTime = 0.f;								// ���� ������ ���� ���ӽð�
	AlphaChangeTime = 0.f;							// ���� ������ ���� ��ȭ�ð�

	isDying = false;								// ���� ������ �׾��.
	DieCheck = 0.f;									// ���� ������ ����Check

	Index = 0;										// ���� ������ �ִ� ��� Index

	HpBar = new ProgressBar("����ü�¹�", Vector2(36.f, 8.f),
		Vector2(_position.x - _size.x / 2.f + 10.f, _position.y + _size.y / 2));
	HpBar->Init((float)Hp, 36.f, 8.f);
	UIMANAGER->addUi(ObjectType::UI, HpBar);

	// ========================
	// ������ �� Effect �ʱ�ȭ
	// ���İ� ( Alpha ) : 0.2f
	// ũ�� ( Size ) : 1.f, 1.f
	// ========================
	for (int i = 0; i < 2; ++i)
	{
		if (i == 0)
		{
			Circle[0].mImage = IMAGEMANAGER->FindImage("�����ӿ�");;
			Circle[0].Size = Vector2(1.f, 1.f);
		}
		else
		{
			Circle[1].mImage = IMAGEMANAGER->FindImage("�����ӿ�");
			Circle[1].Size = Vector2(1.f, 1.f);
		}
			
		Circle[i].Alpha = 0.2f;
	}
	player = (Player*)OBJECTMANAGER->FindObject(ObjectType::Player, "�÷��̾�");
}

MiniSlime::~MiniSlime()
{

}


void MiniSlime::Init()
{
	
}

void MiniSlime::Update()
{
		this->UpdateState();					// ���� ������ ���� ������Ʈ	
		this->DistanceCheck();					// ���� ������ �Ÿ� Ȯ��
		this->UpdateCircle();					// ���� ������ �� Effect ������Ʈ
		this->ImageFrameUpdate();				// ���� ������ ������ Update()
		this->UpdateRect();						// ���� ������ RECT Update()
		this->AlphaUpdate();					// ���� ������ Alpha Update()
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
				EFFECT->ShowEffect("�����ܻ�", _position, Vector2(96.f, 96.f), true, 0, 0.1f);
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
			IMAGEMANAGER->FindImage("������Idle")->SetAlpha(Alpha);
			IMAGEMANAGER->FindImage("������Idle")->SetSize(Vector2(48, 48) * CAMERA->GetZoom());
			IMAGEMANAGER->FindImage("������Idle")->FrameRender(TempPos.x,
				TempPos.y, FrameIndex, 0);
			HpBar->SetPosition(Vector2(_position.x - _size.x / 2.f + 7.f, _position.y + _size.y / 2 + 3.f));
			break;

		case Pattern::Move:
			IMAGEMANAGER->FindImage("������Idle")->SetAlpha(Alpha);
			IMAGEMANAGER->FindImage("������Idle")->SetSize(Vector2(48, 48) * CAMERA->GetZoom());
			IMAGEMANAGER->FindImage("������Idle")->FrameRender(TempPos.x,
				TempPos.y, FrameIndex, 0);
			HpBar->SetPosition(Vector2(_position.x - _size.x / 2.f + 7.f, _position.y + _size.y / 2 + 3.f));
			break;

		case Pattern::Chasor:
			IMAGEMANAGER->FindImage("������Idle")->SetAlpha(Alpha);
			IMAGEMANAGER->FindImage("������Idle")->SetSize(Vector2(48, 48) * CAMERA->GetZoom());
			IMAGEMANAGER->FindImage("������Idle")->FrameRender(TempPos.x,
				TempPos.y, FrameIndex, 0);
			HpBar->SetPosition(Vector2(_position.x - _size.x / 2.f + 7.f, _position.y + _size.y / 2 + 3.f));
			break;

		case Pattern::Attack:
			//cout << DirVector.GetDegree() << endl;
			if (DirVector.GetDegree() <= 210 &&
				DirVector.GetDegree() > 150)
			{
				IMAGEMANAGER->FindImage("������Left")->SetAlpha(Alpha);
				IMAGEMANAGER->FindImage("������Left")->SetSize(Vector2(48, 48) * CAMERA->GetZoom());
				IMAGEMANAGER->FindImage("������Left")->FrameRender(TempJumpPos.x,
					TempJumpPos.y, FrameIndex, 0);
			}
			else if (DirVector.GetDegree() <= 150 &&
				DirVector.GetDegree() > 90)
			{
				IMAGEMANAGER->FindImage("������LeftUp")->SetAlpha(Alpha);
				IMAGEMANAGER->FindImage("������LeftUp")->SetSize(Vector2(48, 48) * CAMERA->GetZoom());
				IMAGEMANAGER->FindImage("������LeftUp")->FrameRender(TempJumpPos.x,
					TempJumpPos.y, FrameIndex, 0);
			}
			else if (DirVector.GetDegree() <= 270 &&
				DirVector.GetDegree() > 210)
			{
				IMAGEMANAGER->FindImage("������LeftDown")->SetAlpha(Alpha);
				IMAGEMANAGER->FindImage("������LeftDown")->SetSize(Vector2(48, 48) * CAMERA->GetZoom());
				IMAGEMANAGER->FindImage("������LeftDown")->FrameRender(TempJumpPos.x,
					TempJumpPos.y, FrameIndex, 0);
			}
			else if (DirVector.GetDegree() <= 90 &&
				DirVector.GetDegree() > 30)
			{
				IMAGEMANAGER->FindImage("������RightUp")->SetAlpha(Alpha);
				IMAGEMANAGER->FindImage("������RightUp")->SetSize(Vector2(48, 48) * CAMERA->GetZoom());
				IMAGEMANAGER->FindImage("������RightUp")->FrameRender(TempJumpPos.x,
					TempJumpPos.y, FrameIndex, 0);
			}
			else if (DirVector.GetDegree() <= 330 &&
				DirVector.GetDegree() > 270)
			{
				IMAGEMANAGER->FindImage("������RightDown")->SetAlpha(Alpha);
				IMAGEMANAGER->FindImage("������RightDown")->SetSize(Vector2(48, 48) * CAMERA->GetZoom());
				IMAGEMANAGER->FindImage("������RightDown")->FrameRender(TempJumpPos.x,
					TempJumpPos.y, FrameIndex, 0);
			}
			else {
				IMAGEMANAGER->FindImage("������Right")->SetAlpha(Alpha);
				IMAGEMANAGER->FindImage("������Right")->SetSize(Vector2(48, 48) * CAMERA->GetZoom());
				IMAGEMANAGER->FindImage("������Right")->FrameRender(TempJumpPos.x,
					TempJumpPos.y, FrameIndex, 0);
			}

			HpBar->SetPosition(Vector2(JumpPos.x - _size.x / 2.f + 7.f, JumpPos.y + _size.y / 2 + 3.f));
			break;

		case Pattern::Jump:

			IMAGEMANAGER->FindImage("������Jump")->SetSize(Vector2(48, 48) * CAMERA->GetZoom());
			IMAGEMANAGER->FindImage("������Jump")->FrameRender(TempJumpPos.x,
				TempJumpPos.y, FrameIndex, 0);

			HpBar->SetPosition(Vector2(JumpPos.x - _size.x / 2.f + 7.f, JumpPos.y + _size.y / 2 + 3.f));
			break;

		case Pattern::Gather:
			AlphaTime += TIMEMANAGER->getElapedTime();
			if (AlphaTime > 0.8f)
			{
				IMAGEMANAGER->FindImage("������Hold")->SetAlpha(1.f);
				while (AlphaTime > 0.8f)
					AlphaTime -= 0.8f;
			}
			else
			{
				IMAGEMANAGER->FindImage("������Hold")->SetAlpha(AlphaTime);
			}

			IMAGEMANAGER->FindImage("������Hold")->SetSize(Vector2(48, 48) * CAMERA->GetZoom());
			IMAGEMANAGER->FindImage("������Hold")->FrameRender(TempJumpPos.x, TempJumpPos.y, 1, 0);
			break;

		case Pattern::GatherUp:
			AlphaTime += TIMEMANAGER->getElapedTime();
			if (AlphaTime > 0.8f)
			{
				IMAGEMANAGER->FindImage("������Hold")->SetAlpha(1.f);
				while (AlphaTime > 0.8f)
					AlphaTime -= 0.8f;
			}
			else
			{
				IMAGEMANAGER->FindImage("������Hold")->SetAlpha(AlphaTime);
			}
			IMAGEMANAGER->FindImage("������Hold")->SetSize(Vector2(48, 48) * CAMERA->GetZoom());
			IMAGEMANAGER->FindImage("������Hold")->FrameRender(TempJumpPos.x, TempJumpPos.y, 1, 0);
			break;
		case Pattern::GatherIdle:
			AlphaTime += TIMEMANAGER->getElapedTime();
			if (AlphaTime > 0.8f)
			{
				IMAGEMANAGER->FindImage("������Hold")->SetAlpha(1.f);
				while (AlphaTime > 0.8f)
					AlphaTime -= 0.8f;
			}
			else
			{
				IMAGEMANAGER->FindImage("������Hold")->SetAlpha(AlphaTime);
			}
			IMAGEMANAGER->FindImage("������Hold")->SetSize(Vector2(48, 48) * CAMERA->GetZoom());
			IMAGEMANAGER->FindImage("������Hold")->FrameRender(TempJumpPos.x, TempJumpPos.y, 1, 0);
			break;
		case Pattern::GatherMove:
			AlphaTime += TIMEMANAGER->getElapedTime();
			if (AlphaTime > 0.8f)
			{
				IMAGEMANAGER->FindImage("������Hold")->SetAlpha(1.f);
				while (AlphaTime > 0.8f)
					AlphaTime -= 0.8f;
			}
			else
			{
				IMAGEMANAGER->FindImage("������Hold")->SetAlpha(AlphaTime);
			}
			IMAGEMANAGER->FindImage("������Hold")->SetSize(Vector2(48, 48) * CAMERA->GetZoom());
			IMAGEMANAGER->FindImage("������Hold")->FrameRender(TempJumpPos.x, TempJumpPos.y, 0, 0);
			break;

		case Pattern::Hit:
			IMAGEMANAGER->FindImage("������Hit")->SetAlpha(Alpha);
			IMAGEMANAGER->FindImage("������Hit")->SetSize(Vector2(48, 48) * CAMERA->GetZoom());
			IMAGEMANAGER->FindImage("������Hit")->Render(TempPos.x, TempPos.y);
			HpBar->SetPosition(Vector2(_position.x - _size.x / 2.f + 7.f, _position.y + _size.y / 2 + 3.f));
			break;

		case Pattern::HitMove:
			IMAGEMANAGER->FindImage("������Hit")->SetAlpha(Alpha);
			IMAGEMANAGER->FindImage("������Hit")->SetSize(Vector2(48, 48) * CAMERA->GetZoom());
			IMAGEMANAGER->FindImage("������Hit")->Render(TempPos.x, TempPos.y);
			HpBar->SetPosition(Vector2(_position.x - _size.x / 2.f + 7.f, _position.y + _size.y / 2 + 3.f));
			break;

		case Pattern::Break:
			AlphaTime += TIMEMANAGER->getElapedTime();
			if (AlphaTime > 0.8f)
			{
				IMAGEMANAGER->FindImage("������Hold")->SetAlpha(1.f);
				while (AlphaTime > 0.8f)
					AlphaTime -= 0.8f;
			}
			else
			{
				IMAGEMANAGER->FindImage("������Hold")->SetAlpha(AlphaTime);
			}

			IMAGEMANAGER->FindImage("������Hold")->SetSize(Vector2(48, 48) * CAMERA->GetZoom());
			if (!isBreak)
				IMAGEMANAGER->FindImage("������Hold")->FrameRender(TempJumpPos.x, TempJumpPos.y, 0, 0);
			else
				IMAGEMANAGER->FindImage("������Hold")->FrameRender(TempJumpPos.x, TempJumpPos.y, 1, 0);
			break;

		case Pattern::SetPos:
			IMAGEMANAGER->FindImage("������Idle")->SetAlpha(Alpha);
			IMAGEMANAGER->FindImage("������Idle")->SetSize(Vector2(48, 48) * CAMERA->GetZoom());
			IMAGEMANAGER->FindImage("������Idle")->FrameRender(TempJumpPos.x,
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
	EffectDelay++;						// �ι�° �� Effect �ణ�� ������
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
	// �÷��̾�� ������ ������ �Ÿ��� Ȯ��
	// �Ÿ��� ���� ���� ����� �÷��̾� ������ �̵��Ѵ�.
	// ���� �������� �������̸� �̵����� �ʴ´�.
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
	// �÷��̾��� ������ǥ player->GetPosition() / �������� ������ǥ _position
	// ���� ���͸� ����Ѵ�. Vector2.Nomalize()
	DirVector.SetDirVector(_position, player->GetPosition());

	// ���� ���͸� ����Ͽ� �ش� �������� �������� �̵���Ų��.
	_position += DirVector * Speed * TIMEMANAGER->getElapedTime();
	JumpPos = _position;
}

void MiniSlime::Jump()
{
	DirVector.SetDirVector(_position, player->GetPosition());
	_position += DirVector * Speed * TIMEMANAGER->getElapedTime() * 2.f;
	JumpPos += DirVector * Speed * TIMEMANAGER->getElapedTime() * 2.f;

	// ������ �߷°��� ���� ���� Y������ ���� ���� �ö󰣴�.
	// ���� ���̱��� �������� �����ϸ� �߷°��� �ٽ� �������� ���� �ڸ��� �ǵ�����.
	// �߷°��� 0���� Ŀ���� �߷°��� 0���� �����Ѵ�.
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
	// ������ ���ߴ� ���⺤�� ( DirVector ) ������ �����Ѵ�.
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
	// �⺻ ( Idle ) ���¸� �����ϰ� ������
	// AttackCount ����.. ���� �ð� ��
	if (isChasor)
	{
		AttackTime += TIMEMANAGER->getElapedTime();
		if (AttackTime > 0.5f)
		{
			// AttackTime = 0 ���� �ʱ�ȭ ��
			// ���� ( Attack ) ���·� ��ȭ
			while (AttackTime > 0.5f)
				AttackTime -= 0.5f;

			DirVector.SetDirVector(_position, player->GetPosition());
			if (Math::GetDistance(
				player->GetPosition().x, player->GetPosition().y,
				_position.x, _position.y) < 125)
			{
				SOUNDMANAGER->play("���������Ӱ���", 0.15f);
				FrameIndex = 0;
				this->ChangeState(Pattern::Attack);
				Path.clear(); Index = 0;
				Gravity -= 3.f;
			}
			else
			{
				SOUNDMANAGER->play("��������������", 0.15f);
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
		// ������ �ִܰ�� �������� �������� �̵��� ���̴�.
		Index = Math::Clamp(Index, 0, Path.size() - 1);
		DirVector.SetDirVector(_position, Path[Index]);
		_position += DirVector * Speed * TIMEMANAGER->getElapedTime();
		JumpPos = _position;
		// �̹� �̵��� �ִܰ�δ� vector���� �������ش�.
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
			SOUNDMANAGER->play("����������", 0.15f);
			isDying = true; isChasor = false;
			HpBar->SetisActive(false);
			EFFECT->ShowEffect("��������", _position, Vector2(96, 96), true, 0, 0.05f);
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
	// �Ÿ��� ����� �����ϱ� �׸��𿩶� ����	
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
		EFFECT->ShowEffect("������Hold", JumpPos, Vector2(48.f,48.f), false, 0);
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
		SOUNDMANAGER->play("�����Ӹ���", 0.15f);
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
	// ������ �ٽ� Ȱ��ȭ
	// ü�� 150���� �ʱ�ȭ
	// ������ �� Effect ������ �ð� �ʱ�ȭ
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
		// ############# ������ �⺻ ( Idle ) ############
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
		// ############# ������ �⺻ ( Move ) ############
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
		// ############# ������ ���� ( Chasor ) ############
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
		// ############# ������ ���� ( Jump ) ############
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
		// ############# ������ ���� ( Attack ) ##########
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
