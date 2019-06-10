#include "stdafx.h"
#include "CameraManager.h"
#include "GameObject.h"


/***************************************************************************
## CameraManager ##
***************************************************************************/
CameraManager::CameraManager()
	:mapSize((float)6400, (float)6400), pTarget(nullptr), zoomFactor(1.0f),
	speed(100.0f), state(MoveState::MoveToTarget), shakeDirection(1.f), moveStartDistance(CameraMoveStartDistance)
{
	this->position = Vector2(0.f, 0.f);
	this->cameraRect = RectMakeCenter(
		(int)this->position.x, (int)this->position.y, WINSIZEX, WINSIZEY);
	this->saveMouse = _ptMouse;
	this->isShake = false;
	shakeChangeDelayTime = CameraDefaultChangeDirectionDelay;
	this->ZoomTime = 0.f;
	this->ZoomStart = false;
	this->ZoomEnd = false;
}

/***************************************************************************
## ~CameraManager ##
***************************************************************************/
CameraManager::~CameraManager()
{

}

/***************************************************************************
## Update ##
카메라 상태에 따라 Update
***************************************************************************/
void CameraManager::Update()
{
	//TODO 타겟 카메라하고 카메라 보간 추가
	switch (state)
	{
	case MoveState::None:
	case MoveState::MoveToTarget:
		this->UpdateTargetCameraMode();
		break;
	case MoveState::FreeCamera:
		this->UpdateFreeCameraMode();
		break;
	case MoveState::End:
		this->ShakingUpdate();
		break;
	default:
		break;
	}
}

/***************************************************************************
## GetRelaticeRect ##
카메라 상대좌표로 RECT 보정
@@ RECT rc : 카메라 보정 받을 렉트

@@ return RECT : 카메라 보정한 렉트 반환
***************************************************************************/
RECT CameraManager::GetRelativeRect(RECT rc)
{
	rc.left -= cameraRect.left;
	rc.top -= cameraRect.top;
	rc.right -= cameraRect.left;
	rc.bottom -= cameraRect.top;

	rc.left = (LONG)rc.left * zoomFactor;
	rc.top = (LONG)rc.top * zoomFactor;
	rc.right = (LONG)rc.right * zoomFactor;
	rc.bottom = (LONG)rc.bottom * zoomFactor;

	return rc;
}
/***************************************************************************
## GetRelativePoint ##
카메라 상대좌표로 POINT 보정 후 반환
@@ POINT pt : 보정 받을 POINT

@@ return POINT : 보정 한 POINT 반환
***************************************************************************/
POINT CameraManager::GetRelatviePoint(POINT pt)
{
	pt.x -= cameraRect.left;
	pt.y -= cameraRect.top;
	pt.x = (LONG)(pt.x) * zoomFactor;
	pt.y = (LONG)(pt.y) * zoomFactor;
	return pt;
}

/***************************************************************************
## GetRelativeVector2 ##
카메라 상대좌표로 Vector2 보정 후 반환
@@ Vector2 v : 보정 받을 Vector2

@@ return Vector2 : 보정 한 Vector2반환
***************************************************************************/
Vector2 CameraManager::GetRelativeVector2(Vector2 v)
{
	v.x -= cameraRect.left;
	v.y -= cameraRect.top;
	v = v * zoomFactor;
	return v;
}
Vector2 CameraManager::GetWorldVector2(Vector2 v)
{
	return Vector2(v.x / zoomFactor + cameraRect.left, v.y / zoomFactor + cameraRect.top);
}
/***************************************************************************
## GetWorldMouse ##
카메라 공간 상의 마우스좌표 반환

@@ return Vector2 : 월드 공간상 마우스좌표
***************************************************************************/
Vector2 CameraManager::GetWorldMouse()
{
	Vector2 mouse = _ptMouse;
	return  Vector2(mouse.x / zoomFactor + cameraRect.left, mouse.y / zoomFactor + cameraRect.top);
}
/***************************************************************************
## GetWorldViewportCenter ##

@@ return Vector2 : 월드 공간상의 화면 중심 반환
***************************************************************************/
Vector2 CameraManager::GetWorldViewportCenter()
{
	return Vector2(position.x , position.y);
}
/***************************************************************************
## SetZoom ##
@@ float Zoom : 줌 수치
***************************************************************************/
void CameraManager::SetZoom(float zoom)
{
	// 카메라 줌 수치를 Set 해준다.
	// Clamptf (줌 수치, 최소값, 최대값)
	// 카메라 줌 수치를 최소값 <= 줌 수치 <= 최대값 로 제한 해준다. F12 참고
	this->zoomFactor = Math::Clampf(zoom, CameraZoomMin, CameraZoomMax);
}
/***************************************************************************
## UpdateRenderRect ##
화면 Rect반환
***************************************************************************/
void CameraManager::UpdateRenderRect()
{
	// 변경된 카메라 중심좌표로
	// 카메라 RECT를 다시 그려준다.
	this->cameraRect = RectMakeCenter(
		(int)this->position.x, (int)this->position.y, WINSIZEX / zoomFactor, WINSIZEY / zoomFactor);
		//Math::Clampf(WINSIZEX / zoomFactor, WINSIZEX / 2.f, mapSize.x),
		//Math::Clampf(WINSIZEY / zoomFactor, WINSIZEY / 2.f, mapSize.y));
}
/***************************************************************************
## UpdateFreeCameraMode ##
FreeCaemra 상태일 때의 업데이트
***************************************************************************/
void CameraManager::UpdateFreeCameraMode()
{
	Vector2 currentMouse = _ptMouse;
	// 마우스 오른쪽 버튼을 누르면
	// 마우스 좌표 쪽으로 서서히 카메라가 이동한다.
	if (KEYMANAGER->IsOnceKeyDown(VK_RBUTTON))
	{
		if (this->saveMouse != currentMouse)
		{
			this->position += (saveMouse - currentMouse) / zoomFactor;
			this->UpdateRenderRect();
			saveMouse = currentMouse;
		}
	}
	saveMouse = currentMouse;
}
/***************************************************************************
## UpdateTargetCameraMode ##
지정된 Target으로 카메라가 이동
***************************************************************************/
void CameraManager::UpdateTargetCameraMode()
{
	if (pTarget)
	{
		{
			//target의 위치로 서서히 움직인다.
			Vector2 centerPos = position;
			Vector2 toTarget = pTarget->GetPosition() - centerPos;
			float length = toTarget.GetLength();
			switch (state)
			{
			case MoveState::None:
			{
				// 카메라의 중심점과 Target의 중심점 거리가 50.f 이상이 되면
				// 카메라가 Target을 따라갈 수 있게
				// 카메라의 상태가 MoveToTarget으로 전환된다.
				if (length > moveStartDistance)
				{
					this->state = MoveState::MoveToTarget;
				}
			}
			break;

			case MoveState::MoveToTarget:
			{
				// 카메라의 중심점과 Target의 중심점 거리를 통해
				// 카메라가 Target에 점점 가까워지면 
				// 카메라가 Target을 따라가는 속도가 점점 느려진다.
				// 거리 / ( 카메라 최대거리 - 카메라 최소거리 ) * ( 카메라 최대속도 - 카메라 최소속도 )
				// 정확한 공식은 아니지만 일정 비율을 유지하지 않으면 Target이 카메라를 벗어난다.
				this->speed = (length / (CameraMaxDistance - CameraMinDistance)) * (CameraMaxSpeed - CameraMinSpeed);
				this->speed = Math::Clampf(speed, CameraMinSpeed, CameraMaxSpeed);

				centerPos += toTarget.Normalize() * speed * TIMEMANAGER->getElapedTime();
				this->position = centerPos;
				this->UpdateRenderRect();

				// 카메라 중심점과 Target의 중심점 거리가 50.f 이하가 되면
				// 카메라의 상태를 None으로 전환
				// 카메라가 Target을 더 이상 따라가지 않는다.
				if (length <= moveStartDistance)
				{
					this->state = MoveState::None;
					this->position = centerPos;
					this->UpdateRenderRect();
				}

				// 카메라를 보정해준다.
				this->AmendCamera();

			}
			break;
			}
			this->ShakingUpdate();
		}

	}
}

/***************************************************************************
## UpdateTargetCameraMode ##
맵을 벗어나지 않게 카메라를 보정
***************************************************************************/
void CameraManager::AmendCamera()
{
	if (cameraRect.left < 0.f)
	{
		position.x -= cameraRect.left;
	}
	else if (cameraRect.right >(LONG)mapSize.x)
	{
		position.x -= ((float)cameraRect.right) - mapSize.x;
	}
	if (cameraRect.top < 0.f)
	{
		position.y -= cameraRect.top;
	}
	else if (cameraRect.bottom >(LONG)mapSize.y)
	{
		position.y -= (float(cameraRect.bottom)) - mapSize.y;
	}
	this->UpdateRenderRect();
}

void CameraManager::ShakingUpdate()
{
	// Shake가 true라면 시작
	if (isShake == true)
	{
		// Shake DeltaTime 만큼 감소
		shakeTime -= TIMEMANAGER->getElapedTime();
		shakeChangeDirectionTime += TIMEMANAGER->getElapedTime();
		if (shakeChangeDirectionTime >= shakeChangeDelayTime)
		{
			// 기준이 되는 방향 전환 시간보다 커진다면
			// Shake 방향을 전환해준다. -1.f 곱하면 역이 된다.
			shakeDirection = -1.f * shakeDirection;
			// 방향 전환 시간 0.f으로 다시 초기화
			while (shakeChangeDirectionTime >= shakeChangeDelayTime)
				shakeChangeDirectionTime -= shakeChangeDelayTime;
		}
		// Shake가 진행될 수록 흔드는 힘이 점점 약해지게 비율 계산
		shakeStrength -= (shakeTime / totalShakeTime) * TIMEMANAGER->getElapedTime();
		float strengh = shakeStrength * shakeDirection;
		position += Vector2(strengh, 0.f);
		this->UpdateRenderRect();

		// Shake 시간이 모두 끝나면 Shake를 꺼준다.
		if (shakeTime <= 0.f)
		{
			isShake = false;
			shakeChangeDelayTime = CameraDefaultChangeDirectionDelay;
		}
		// Shake하는 동안 화면을 벗어날 수 있으므로 카메라 보정 해준다.
		this->AmendCamera();
	}
}

/***************************************************************************
## CameraProc ##
마우스 휠에 따라 줌
***************************************************************************/
void CameraManager::CameraProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	//if (state == MoveState::FreeCamera)
	{
		if ((SHORT)HIWORD(wParam) > 0)
		{
			this->zoomFactor += 0.07f;
			this->zoomFactor = Math::Clampf(zoomFactor, CameraZoomMin, CameraZoomMax);
			this->position = pTarget->GetPosition();
		}
		else
		{
			this->zoomFactor -= 0.07f;
			this->zoomFactor = Math::Clampf(zoomFactor, CameraZoomMin, CameraZoomMax);
			this->position = pTarget->GetPosition();
		}
		this->cameraRect = RectMakeCenter(
			(int)this->position.x, (int)this->position.y, WINSIZEX / zoomFactor, WINSIZEY / zoomFactor);
		this->AmendCamera();
	}
}
bool CameraManager::TargetZoom()
{
	if (ZoomStart)
	{
		ZoomTime += TIMEMANAGER->getElapedTime();
		if (ZoomTime > 0.65f)
		{
			while (ZoomTime > 0.65f)
				ZoomTime -= 0.65f;
			ZoomEnd = true;
			ZoomStart = false;
		}
		else
		{
			zoomFactor += 0.2f;
			if (zoomFactor > 2.5f)
			{
				zoomFactor = 2.5f;
			}
			this->position = pTarget->GetPosition();

		}
	}

	if (ZoomEnd)
	{
		zoomFactor -= 0.1f;
		if (zoomFactor <= 1.f)
		{
			zoomFactor = 1.f;
			ZoomEnd = false;
			return true;
		}
		this->position = pTarget->GetPosition();
	}
	this->cameraRect = RectMakeCenter(
		(int)this->position.x, (int)this->position.y, WINSIZEX / zoomFactor, WINSIZEY / zoomFactor);
	this->AmendCamera();
	return false;
	
}
bool CameraManager::TargetZoom(GameObject* object)
{

	if (ZoomStart)
	{
		ZoomTime += TIMEMANAGER->getElapedTime();
		if (ZoomTime > 0.65f)
		{
			while (ZoomTime > 0.65f)
				ZoomTime -= 0.65f;
			ZoomEnd = true;
			ZoomStart = false;
		}
		else
		{
			zoomFactor += 0.2f;
			if (zoomFactor > 2.5f)
			{
				zoomFactor = 2.5f;
			}
			this->position = object->GetPosition();

		}
	}

	if (ZoomEnd)
	{
		zoomFactor -= 0.1f;
		if (zoomFactor <= 1.f)
		{
			zoomFactor = 1.f;
			ZoomEnd = false;
			return true;
		}
		this->position = object->GetPosition();
	}
	this->cameraRect = RectMakeCenter(
		(int)this->position.x, (int)this->position.y, WINSIZEX / zoomFactor, WINSIZEY / zoomFactor);
	this->AmendCamera();
	return false;
}
// 카메라 Target을 세팅 해준다.
void CameraManager::SetTarget(GameObject * object)
{
	if (object)
	{
		this->pTarget = object;
		this->state = MoveState::None;
		this->position = object->GetPosition();
		this->UpdateRenderRect();
		this->AmendCamera();
	}
}
// 카메라 Target을 변경 해준다.
void CameraManager::ChangeTarget(GameObject * object)
{
	pTarget = object;
	state = MoveState::None;
}

// 카메라 상태를 FreeCamera로 변경
// 마우스로 카메라를 움직일 수 있다.
void CameraManager::SetFreeCamera()
{
	state = MoveState::FreeCamera;
}

void CameraManager::SetNoneCamera()
{
	state = MoveState::None;
}

/***************************************************************************
## Shake ##
카메라 Shake 시작 
***************************************************************************/
void CameraManager::Shake(float strength, float shakeTime, float changeDirectionTime)
{
	//if (isShake == false)
	{
		this->isShake = true;
		this->shakeStrength = strength;
		this->shakeTime = this->totalShakeTime = shakeTime;
		this->shakeChangeDirectionTime = 0.f;
		this->shakeChangeDelayTime = changeDirectionTime;
	}
}

// 카메라의 Target을 Player로 세팅
void CameraManager::InitCameraToPlayer()
{
	//GameObject* object = OBJECTMANAGER->FindObject(ObjectType::Player, "Will");
	//if (object)
	//{
	//	this->SetTarget(object);
	//	zoomFactor = 1.f;
	//}
}
