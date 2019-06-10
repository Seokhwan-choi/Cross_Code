/*************************************************************
## CameraManager ##
카메라 싱글톤
*************************************************************/
#pragma once

#define CameraMoveStartDistance 30.f		

#define CameraMaxSpeed 800.0f			// 카메라 최대 스피드
#define CameraMinSpeed 100.0f			// 카메라 최소 스피드
#define CameraMaxDistance 250.f			// 카메라 최대 거리
#define CameraMinDistance 30.f			// 카메라 최소 거리

#define CameraZoomMax 2.5f				// 카메라 줌 최대값
#define CameraZoomMin 0.5f				// 카메라 줌 최소값

#define CameraDefaultStrength 2.58f		// 카메라 Shake 기본 파워
#define CameraDefaultShakeTime 0.2f		// 카메라 Shake 기본 시간
#define CameraDefaultChangeDirectionDelay 0.043f	// 카메라 Shake 기본 방향전환 시간

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
	MoveState state;								//카메라 상태
	float zoomFactor;								//카메라 줌 
	RECT cameraRect;								//카메라 렉트
	class GameObject* pTarget;						//카메라가 따라갈 GameObject(추적카메라 구현 시 사용)
	Vector2 position;								//카메라 좌표
	Vector2 mapSize;								//전체 맵 사이즈 (카메라 보정이 들어감)
	float speed;									//카메라 스피드 값			
	Vector2 saveMouse;								//자유시점 카메라에 사용(이전 프레임 마우스 좌표 저장 값)

	bool isShake;									//카메라 Shake 할까 ?
	float shakeStrength;							//카메라 Shake 파워
	float totalShakeTime;							//카메라 Shake 총 하는 시간 ( 한번.. 한번.. ) 
	float shakeTime;								//카메라 Shake 한번 시간
	float shakeDirection;							//카메라 Shake 방향
	float shakeChangeDelayTime;						//카메라 Shake 방향 전환 기준 시간
	float shakeChangeDirectionTime;					//카메라 Shake 방향 전환 시간

	float moveStartDistance;						//카메라가 움직이기 시작하는 거리


	float ZoomTime;									//카메라 Zoom Time
	bool ZoomStart;									//카메라 Zoom Start
	bool ZoomEnd;									//카메라 Zoom End
public:
	CameraManager();
	~CameraManager();
	void Update();										// Update() 카메라 상태를 계속 확인하고 전환해준다.

	RECT GetRelativeRect(RECT rc);						// 카메라 상대좌표 RECT 반환
	POINT GetRelatviePoint(POINT pt);					// 카메라 상대좌표 POINT 반환
	Vector2 GetRelativeVector2(Vector2 v);				// 카메라 상대좌표 Vector2 반환
	Vector2 GetWorldVector2(Vector2 v);					// 카메라 기준 월드좌표 Vector2 반환
	Vector2 GetWorldMouse();							// 카메라 기준 마우스 좌표 반환
	Vector2 GetWorldViewportCenter();
	void SetZoom(float zoom);							// 카메라 Zoom 값 Set
	float GetZoom()const { return this->zoomFactor; }	// 카메라 Zoom 값 Get
	void CameraProc(UINT message, WPARAM wParam, LPARAM lParam);	// 카메라 MouseWheel 값 Set 함수
	bool TargetZoom();												// 카메라 Target Zoom 함수
	bool TargetZoom(class GameObject* object);						// 카메라 Target Zoom overloading
	void SetTarget(class GameObject* object);						// 카메라 Target Set 함수
	void ChangeTarget(class GameObject* object);					// 카메라 Target 변경 함수

	// 카메라 Shake 실행 함수
	void Shake(float strength = CameraDefaultStrength, float shakeTime = CameraDefaultShakeTime,
		float delayChangeDirection = CameraDefaultChangeDirectionDelay);
	// 카메라 Target Player로 지정 함수
	void InitCameraToPlayer();

	// ====================================================================================================
	// ##################################### CAMERA GET / SET 함수 ########################################
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
	void UpdateRenderRect();				// 카메라의 변경된 중심점으로 RECT를 다시 그리는 함수
	void UpdateFreeCameraMode();			// 카메라가 Mouse를 따라가는 함수
	void UpdateTargetCameraMode();			// 카메라 Target을 추적하는 함수
	void AmendCamera();						// 카메라 보정 함수
	void ShakingUpdate();					// 카메라 Shake 처리 함수
};