/*************************************************************
## CameraManager ##
ī�޶� �̱���
*************************************************************/
#pragma once

#define CameraMoveStartDistance 30.f		

#define CameraMaxSpeed 800.0f			// ī�޶� �ִ� ���ǵ�
#define CameraMinSpeed 100.0f			// ī�޶� �ּ� ���ǵ�
#define CameraMaxDistance 250.f			// ī�޶� �ִ� �Ÿ�
#define CameraMinDistance 30.f			// ī�޶� �ּ� �Ÿ�

#define CameraZoomMax 2.5f				// ī�޶� �� �ִ밪
#define CameraZoomMin 0.5f				// ī�޶� �� �ּҰ�

#define CameraDefaultStrength 2.58f		// ī�޶� Shake �⺻ �Ŀ�
#define CameraDefaultShakeTime 0.2f		// ī�޶� Shake �⺻ �ð�
#define CameraDefaultChangeDirectionDelay 0.043f	// ī�޶� Shake �⺻ ������ȯ �ð�

class CameraManager 
	: public SingletonBase<CameraManager>
{
public:
	enum class MoveState : int
	{	
		None,
		MoveToTarget,
		FreeCamera,
		MapTool,
		End 
	};		
private:
	MoveState state;								//ī�޶� ����
	float zoomFactor;								//ī�޶� �� 
	RECT cameraRect;								//ī�޶� ��Ʈ
	class GameObject* pTarget;						//ī�޶� ���� GameObject(����ī�޶� ���� �� ���)
	Vector2 position;								//ī�޶� ��ǥ
	Vector2 mapSize;								//��ü �� ������ (ī�޶� ������ ��)
	float speed;									//ī�޶� ���ǵ� ��			
	Vector2 saveMouse;								//�������� ī�޶� ���(���� ������ ���콺 ��ǥ ���� ��)

	bool isShake;									//ī�޶� Shake �ұ� ?
	float shakeStrength;							//ī�޶� Shake �Ŀ�
	float totalShakeTime;							//ī�޶� Shake �� �ϴ� �ð� ( �ѹ�.. �ѹ�.. ) 
	float shakeTime;								//ī�޶� Shake �ѹ� �ð�
	float shakeDirection;							//ī�޶� Shake ����
	float shakeChangeDelayTime;						//ī�޶� Shake ���� ��ȯ ���� �ð�
	float shakeChangeDirectionTime;					//ī�޶� Shake ���� ��ȯ �ð�

	float moveStartDistance;						//ī�޶� �����̱� �����ϴ� �Ÿ�


	float ZoomTime;									//ī�޶� Zoom Time
	bool ZoomStart;									//ī�޶� Zoom Start
	bool ZoomEnd;									//ī�޶� Zoom End
public:
	CameraManager();
	~CameraManager();
	void Update();										// Update() ī�޶� ���¸� ��� Ȯ���ϰ� ��ȯ���ش�.

	RECT GetRelativeRect(RECT rc);						// ī�޶� �����ǥ RECT ��ȯ
	POINT GetRelatviePoint(POINT pt);					// ī�޶� �����ǥ POINT ��ȯ
	Vector2 GetRelativeVector2(Vector2 v);				// ī�޶� �����ǥ Vector2 ��ȯ
	Vector2 GetWorldVector2(Vector2 v);					// ī�޶� ���� ������ǥ Vector2 ��ȯ
	Vector2 GetWorldMouse();							// ī�޶� ���� ���콺 ��ǥ ��ȯ
	Vector2 GetWorldViewportCenter();
	void SetZoom(float zoom);							// ī�޶� Zoom �� Set
	float GetZoom()const { return this->zoomFactor; }	// ī�޶� Zoom �� Get
	void CameraProc(UINT message, WPARAM wParam, LPARAM lParam);	// ī�޶� MouseWheel �� Set �Լ�
	bool TargetZoom();												// ī�޶� Target Zoom �Լ�
	bool TargetZoom(class GameObject* object);						// ī�޶� Target Zoom overloading
	void SetTarget(class GameObject* object);						// ī�޶� Target Set �Լ�
	void ChangeTarget(class GameObject* object);					// ī�޶� Target ���� �Լ�

	// ī�޶� Shake ���� �Լ�
	void Shake(float strength = CameraDefaultStrength, float shakeTime = CameraDefaultShakeTime,
		float delayChangeDirection = CameraDefaultChangeDirectionDelay);
	// ī�޶� Target Player�� ���� �Լ�
	void InitCameraToPlayer();

	// ====================================================================================================
	// ##################################### CAMERA GET / SET �Լ� ########################################
	// ====================================================================================================
	void SetCameraMoveDistance(float distance = CameraMoveStartDistance) { moveStartDistance = distance; }
	void SetState(MoveState state) { this->state = state; }
	void SetFreeCamera();
	void SetNoneCamera();

	void SetPosition(Vector2 pos) {
		position = pos;
		this->UpdateRenderRect();
	}
	Vector2 GetPosition() { return position; }

	void SetMapSize(Vector2 size) { mapSize = size; }
	Vector2 GetMapSize() { return mapSize; }

	void SetSpeed(float spd) { speed = spd; }
	float GetSpeed() { return speed; }

	RECT GetCameraRect() { return cameraRect; }

	void SetZoomStart(bool zoom) { ZoomStart = zoom; }
	bool GetZoomStart() { return ZoomStart; }

private:
	void UpdateRenderRect();				// ī�޶��� ����� �߽������� RECT�� �ٽ� �׸��� �Լ�
	void UpdateFreeCameraMode();			// ī�޶� Mouse�� ���󰡴� �Լ�
	void UpdateTargetCameraMode();			// ī�޶� Target�� �����ϴ� �Լ�
	void AmendCamera();						// ī�޶� ���� �Լ�
	void ShakingUpdate();					// ī�޶� Shake ó�� �Լ�
};