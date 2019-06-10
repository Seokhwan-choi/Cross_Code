#include "stdafx.h"
#include "Event.h"
#include "GameObject.h"
#include "Player.h"
#include "Slime.h"
#include "BossProgressBar.h"
#include "ProgressBar.h"

// ==========================================================
// ################ Camera Move ī�޶� �̵� ##################
// ==========================================================
bool CameraMove::UpdateEvent()
{
	if (Math::GetDistance(Target->GetPosition().x, Target->GetPosition().y,
		CAMERA->GetWorldViewportCenter().x, CAMERA->GetWorldViewportCenter().y) < 51)
	{
		return true;
	}
	return false;
}

void CameraMove::StartEvent()
{
	CAMERA->ChangeTarget(Target);
}




CameraMove::CameraMove(GameObject* object)
	: Target(object)
{
}

void CameraZoom::StartEvent()
{
	CAMERA->SetZoomStart(true);
	SOUNDMANAGER->play("���");
}

bool CameraZoom::UpdateEvent()
{
	if (CAMERA->TargetZoom())
	{
		return true;
	}
	return false;
}

void CameraZoom2::StartEvent()
{
	CAMERA->SetZoomStart(true);
}

bool CameraZoom2::UpdateEvent()
{
	if (CAMERA->TargetZoom())
	{
		return true;
	}
	return false;
}

// ==========================================================
// ################ Boss Move Boss �̵� #####################
// ==========================================================
BossMove::BossMove(GameObject* boss)
	: Boss(boss)
{

}

void BossMove::StartEvent()
{
	CAMERA->ChangeTarget(Boss);
	SOUNDMANAGER->play("����������");
}

bool BossMove::UpdateEvent()
{
	if (Math::GetDistance(Boss->GetPosition().x, Boss->GetPosition().y, 735.f, (float)64 * 23 / 2.f) < 3.5f)
	{
		Boss->SetPosition(Vector2(735.f, (float)64 * 23 / 2.f));
		((Slime*)Boss)->SetJPos(Vector2(735.f, (float)64 * 23 / 2.f));
		return true;
	}
	else
	{
		Boss->SetPosition(Vector2(735.f, Boss->GetPosition().y + 3.5f));
		((Slime*)Boss)->SetJPos(Vector2(735.f, ((Slime*)Boss)->GetJPos().y + 3.5f));
	}
	return false;
}

// =============================================================
// ################ Boss Gather Boss ���̱� #####################
// =============================================================
void BossGather::StartEvent()
{
	((Slime*)Boss)->Gather();
}

bool BossGather::UpdateEvent()
{
	if (((Slime*)Boss)->GatherCheck())
	{
		return true;
	}
	return false;
}

BossGather::BossGather(GameObject* boss)
	: Boss(boss)
{

}

// =============================================================
// ################ Boss Break Boss ���� #######################
// =============================================================

void BossBreak::StartEvent()
{
	((Slime*)Boss)->Break();
}

bool BossBreak::UpdateEvent()
{
	return true;
}

BossBreak::BossBreak(GameObject* boss)
	: Boss(boss)
{
}

// ===================================================================================
// Event ���۵Ǿ����� ĳ���� ������ �� ����.
// Event ����Ǹ� ĳ���� �ٽ� ������ �� �ִ�.
// Event ���۰� ����� ���� ȣ��
// Start �� �� �� �� �� �� End
// ===================================================================================
void Start::StartEvent()
{
	((Player*)OBJECTMANAGER->FindObject(ObjectType::Player, "�÷��̾�"))->SetEvent(true);
}

bool Start::UpdateEvent()
{
	return true;
}

void End::StartEvent()
{
	SOUNDMANAGER->play("��Ʋ", 0.15f);
	SOUNDMANAGER->stop("��������");
	((Player*)OBJECTMANAGER->FindObject(ObjectType::Player, "�÷��̾�"))->SetEvent(false);
}

bool End::UpdateEvent()
{
	return true;
}

void Delay::StartEvent()
{
}

bool Delay::UpdateEvent()
{
	Current += TIMEMANAGER->getElapedTime();
	if (Current > time)
	{
		return true;
	}
	else {
		return false;
	}
}

Delay::Delay(float time)
{
	this->time = time;
	Current = 0.f;
}

// ===============================================================

void BossBar::StartEvent()
{
	((BossProgressBar*)UIMANAGER->FindUi(ObjectType::Object, "����ü��"))->SetisShow(true);
}

bool BossBar::UpdateEvent()
{
	if (((BossProgressBar*)UIMANAGER->FindUi(ObjectType::Object, "����ü��"))->BarDone(1072) == true)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void CameraShake::StartEvent()
{
	CAMERA->Shake(10.f, 0.7f);
}

bool CameraShake::UpdateEvent()
{
	return true;
}

void CameraChange::StartEvent()
{
	CAMERA->ChangeTarget(CameraTarget);
}

bool CameraChange::UpdateEvent()
{
	return true;
}

CameraChange::CameraChange(GameObject* target)
	: CameraTarget(target)
{

}

void FadeIn::StartEvent()
{
	SCENEMANAGER->SetAlpha(0.f);
	SCENEMANAGER->SetSceneState(SceneState::FadeOut);
}

bool FadeIn::UpdateEvent()
{
	if (SCENEMANAGER->GetAlpha() == 1.f)
	{
		SOUNDMANAGER->stop("��Ʋ");
		return true;
	}
	return false;
}

void ChangeScene::StartEvent()
{
}

bool ChangeScene::UpdateEvent()
{
	SCENEMANAGER->LoadScene(Name);
	return true;
}

ChangeScene::ChangeScene(string name)
	: Name(name)
{
}


