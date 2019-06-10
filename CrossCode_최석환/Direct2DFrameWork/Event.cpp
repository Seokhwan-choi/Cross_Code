#include "stdafx.h"
#include "Event.h"
#include "GameObject.h"
#include "Player.h"
#include "Slime.h"
#include "BossProgressBar.h"
#include "ProgressBar.h"

// ==========================================================
// ################ Camera Move 카메라 이동 ##################
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
	SOUNDMANAGER->play("비명");
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
// ################ Boss Move Boss 이동 #####################
// ==========================================================
BossMove::BossMove(GameObject* boss)
	: Boss(boss)
{

}

void BossMove::StartEvent()
{
	CAMERA->ChangeTarget(Boss);
	SOUNDMANAGER->play("보스슬라임");
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
// ################ Boss Gather Boss 모이기 #####################
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
// ################ Boss Break Boss 분해 #######################
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
// Event 시작되었으니 캐릭터 움직일 수 없다.
// Event 종료되면 캐릭터 다시 움직일 수 있다.
// Event 시작과 종료시 각각 호출
// Start ▶ ▶ ▶ ▶ ▶ ▶ End
// ===================================================================================
void Start::StartEvent()
{
	((Player*)OBJECTMANAGER->FindObject(ObjectType::Player, "플레이어"))->SetEvent(true);
}

bool Start::UpdateEvent()
{
	return true;
}

void End::StartEvent()
{
	SOUNDMANAGER->play("배틀", 0.15f);
	SOUNDMANAGER->stop("보스시작");
	((Player*)OBJECTMANAGER->FindObject(ObjectType::Player, "플레이어"))->SetEvent(false);
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
	((BossProgressBar*)UIMANAGER->FindUi(ObjectType::Object, "보스체력"))->SetisShow(true);
}

bool BossBar::UpdateEvent()
{
	if (((BossProgressBar*)UIMANAGER->FindUi(ObjectType::Object, "보스체력"))->BarDone(1072) == true)
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
		SOUNDMANAGER->stop("배틀");
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


