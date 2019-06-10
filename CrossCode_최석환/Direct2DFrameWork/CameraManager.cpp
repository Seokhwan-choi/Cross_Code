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
ī�޶� ���¿� ���� Update
***************************************************************************/
void CameraManager::Update()
{
	//TODO Ÿ�� ī�޶��ϰ� ī�޶� ���� �߰�
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
ī�޶� �����ǥ�� RECT ����
@@ RECT rc : ī�޶� ���� ���� ��Ʈ

@@ return RECT : ī�޶� ������ ��Ʈ ��ȯ
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
ī�޶� �����ǥ�� POINT ���� �� ��ȯ
@@ POINT pt : ���� ���� POINT

@@ return POINT : ���� �� POINT ��ȯ
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
ī�޶� �����ǥ�� Vector2 ���� �� ��ȯ
@@ Vector2 v : ���� ���� Vector2

@@ return Vector2 : ���� �� Vector2��ȯ
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
ī�޶� ���� ���� ���콺��ǥ ��ȯ

@@ return Vector2 : ���� ������ ���콺��ǥ
***************************************************************************/
Vector2 CameraManager::GetWorldMouse()
{
	Vector2 mouse = _ptMouse;
	return  Vector2(mouse.x / zoomFactor + cameraRect.left, mouse.y / zoomFactor + cameraRect.top);
}
/***************************************************************************
## GetWorldViewportCenter ##

@@ return Vector2 : ���� �������� ȭ�� �߽� ��ȯ
***************************************************************************/
Vector2 CameraManager::GetWorldViewportCenter()
{
	return Vector2(position.x , position.y);
}
/***************************************************************************
## SetZoom ##
@@ float Zoom : �� ��ġ
***************************************************************************/
void CameraManager::SetZoom(float zoom)
{
	// ī�޶� �� ��ġ�� Set ���ش�.
	// Clamptf (�� ��ġ, �ּҰ�, �ִ밪)
	// ī�޶� �� ��ġ�� �ּҰ� <= �� ��ġ <= �ִ밪 �� ���� ���ش�. F12 ����
	this->zoomFactor = Math::Clampf(zoom, CameraZoomMin, CameraZoomMax);
}
/***************************************************************************
## UpdateRenderRect ##
ȭ�� Rect��ȯ
***************************************************************************/
void CameraManager::UpdateRenderRect()
{
	// ����� ī�޶� �߽���ǥ��
	// ī�޶� RECT�� �ٽ� �׷��ش�.
	this->cameraRect = RectMakeCenter(
		(int)this->position.x, (int)this->position.y, WINSIZEX / zoomFactor, WINSIZEY / zoomFactor);
		//Math::Clampf(WINSIZEX / zoomFactor, WINSIZEX / 2.f, mapSize.x),
		//Math::Clampf(WINSIZEY / zoomFactor, WINSIZEY / 2.f, mapSize.y));
}
/***************************************************************************
## UpdateFreeCameraMode ##
FreeCaemra ������ ���� ������Ʈ
***************************************************************************/
void CameraManager::UpdateFreeCameraMode()
{
	Vector2 currentMouse = _ptMouse;
	// ���콺 ������ ��ư�� ������
	// ���콺 ��ǥ ������ ������ ī�޶� �̵��Ѵ�.
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
������ Target���� ī�޶� �̵�
***************************************************************************/
void CameraManager::UpdateTargetCameraMode()
{
	if (pTarget)
	{
		{
			//target�� ��ġ�� ������ �����δ�.
			Vector2 centerPos = position;
			Vector2 toTarget = pTarget->GetPosition() - centerPos;
			float length = toTarget.GetLength();
			switch (state)
			{
			case MoveState::None:
			{
				// ī�޶��� �߽����� Target�� �߽��� �Ÿ��� 50.f �̻��� �Ǹ�
				// ī�޶� Target�� ���� �� �ְ�
				// ī�޶��� ���°� MoveToTarget���� ��ȯ�ȴ�.
				if (length > moveStartDistance)
				{
					this->state = MoveState::MoveToTarget;
				}
			}
			break;

			case MoveState::MoveToTarget:
			{
				// ī�޶��� �߽����� Target�� �߽��� �Ÿ��� ����
				// ī�޶� Target�� ���� ��������� 
				// ī�޶� Target�� ���󰡴� �ӵ��� ���� ��������.
				// �Ÿ� / ( ī�޶� �ִ�Ÿ� - ī�޶� �ּҰŸ� ) * ( ī�޶� �ִ�ӵ� - ī�޶� �ּҼӵ� )
				// ��Ȯ�� ������ �ƴ����� ���� ������ �������� ������ Target�� ī�޶� �����.
				this->speed = (length / (CameraMaxDistance - CameraMinDistance)) * (CameraMaxSpeed - CameraMinSpeed);
				this->speed = Math::Clampf(speed, CameraMinSpeed, CameraMaxSpeed);

				centerPos += toTarget.Normalize() * speed * TIMEMANAGER->getElapedTime();
				this->position = centerPos;
				this->UpdateRenderRect();

				// ī�޶� �߽����� Target�� �߽��� �Ÿ��� 50.f ���ϰ� �Ǹ�
				// ī�޶��� ���¸� None���� ��ȯ
				// ī�޶� Target�� �� �̻� ������ �ʴ´�.
				if (length <= moveStartDistance)
				{
					this->state = MoveState::None;
					this->position = centerPos;
					this->UpdateRenderRect();
				}

				// ī�޶� �������ش�.
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
���� ����� �ʰ� ī�޶� ����
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
	// Shake�� true��� ����
	if (isShake == true)
	{
		// Shake DeltaTime ��ŭ ����
		shakeTime -= TIMEMANAGER->getElapedTime();
		shakeChangeDirectionTime += TIMEMANAGER->getElapedTime();
		if (shakeChangeDirectionTime >= shakeChangeDelayTime)
		{
			// ������ �Ǵ� ���� ��ȯ �ð����� Ŀ���ٸ�
			// Shake ������ ��ȯ���ش�. -1.f ���ϸ� ���� �ȴ�.
			shakeDirection = -1.f * shakeDirection;
			// ���� ��ȯ �ð� 0.f���� �ٽ� �ʱ�ȭ
			while (shakeChangeDirectionTime >= shakeChangeDelayTime)
				shakeChangeDirectionTime -= shakeChangeDelayTime;
		}
		// Shake�� ����� ���� ���� ���� ���� �������� ���� ���
		shakeStrength -= (shakeTime / totalShakeTime) * TIMEMANAGER->getElapedTime();
		float strengh = shakeStrength * shakeDirection;
		position += Vector2(strengh, 0.f);
		this->UpdateRenderRect();

		// Shake �ð��� ��� ������ Shake�� ���ش�.
		if (shakeTime <= 0.f)
		{
			isShake = false;
			shakeChangeDelayTime = CameraDefaultChangeDirectionDelay;
		}
		// Shake�ϴ� ���� ȭ���� ��� �� �����Ƿ� ī�޶� ���� ���ش�.
		this->AmendCamera();
	}
}

/***************************************************************************
## CameraProc ##
���콺 �ٿ� ���� ��
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
// ī�޶� Target�� ���� ���ش�.
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
// ī�޶� Target�� ���� ���ش�.
void CameraManager::ChangeTarget(GameObject * object)
{
	pTarget = object;
	state = MoveState::None;
}

// ī�޶� ���¸� FreeCamera�� ����
// ���콺�� ī�޶� ������ �� �ִ�.
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
ī�޶� Shake ���� 
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

// ī�޶��� Target�� Player�� ����
void CameraManager::InitCameraToPlayer()
{
	//GameObject* object = OBJECTMANAGER->FindObject(ObjectType::Player, "Will");
	//if (object)
	//{
	//	this->SetTarget(object);
	//	zoomFactor = 1.f;
	//}
}
