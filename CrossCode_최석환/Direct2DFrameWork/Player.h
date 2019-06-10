#pragma once
#include "GameObject.h"
#define AttackROUND 150
class Player : public GameObject
{
	enum class Dir : int
	{
		Left, Right, Up, Down, LeftUp, RightUp, LeftDown, RightDown, END
	};
	enum class STATE : int
	{
	//  왼 기본    오른 기본  위 기본  아래 기본
		IdleLeft, IdleRight, IdleUp	, IdleDown,

	//   왼     오   위   아래											
		Left, Right, Up, Down,

	//   왼 스탑   오 스탑    위 스탑  아래 스탑
		LeftStop, RightStop, UpStop, DownStop,

	//	 왼 홀드	   오 홀드	 위 홀드	 아래 홀드
		LeftHold, RightHold, UpHold, DownHold,

	//	 왼 홀드 이동   오 홀드 이동	 위 홀드 이동 아래 홀드 이동
		LeftHoldMove, RightHoldMove, UpHoldMove, DownHoldMove,

	//	 왼 던지기	오 던지기  위 던지기	아래 던지기
		LeftThrow, RightThrow, UpThrow, DownThrow,

	//	 왼 공격		  오 공격	  위 공격   아래 공격
		LeftAttack, RightAttack, UpAttack, DownAttack,

	//	 왼 맞음	  오 맞음   위 맞음  아래 맞음
		LeftHit, RightHit, UpHit, DownHit,
	
	//   왼/위 기본  오/위 기본    왼/아래 기본   오/아래 기본
		IdleLeftUp, IdleRightUp, IdleLeftDown, IdleRightDown,

	//   왼/위   오/위     왼/아래    오/아래
		LeftUp, RightUp, LeftDown, RightDown,

	//   왼/위 스탑  오/위 스탑    왼/아래 스탑    오/아래 스탑
		LeftUpStop, RightUpStop, LeftDownStop, RightDownStop,

	//   왼/위 홀드	 오/위 홀드	  왼/아래 홀드	오/아래 홀드
		LeftUpHold, RightUpHold, LeftDownHold, RightDownHold,

	//   왼/위 홀드 이동  오/위 홀드 이동   왼/아래 홀드 이동  오/아래 홀드 이동
		LeftUpHoldMove, RightUpHoldMove, LeftDownHoldMove, RightDownHoldMove,

	//	 왼/위 던지기  오/위 던지기   왼/아래 던지기  오/아래 던지기
		LeftUpThrow, RightUpThrow, LeftDownThrow, RightDownThrow,

	//	 왼/위 공격		오/위 공격	  왼/아래 공격	  오/아래 공격
		LeftUpAttack, RightUpAttack, LeftDownAttack, RightDownAttack,

	//  왼/위 맞음	오/위 맞음	왼/아래 맞음	 오/아래 맞음
		LeftUpHit, RightUpHit, LeftDownHit, RightDownHit,

	//   구르기    죽음
		Rolling, Death, END
	};
private:
	Dir PlayerDir;						// 캐릭터 방향저장
	STATE PlayerState;					// 캐릭터 현재 상태
	int PlayerOrder;					// 캐릭터 현재 몇층에 있냐 ?
	float Speed;						// 캐릭터 속도
	Vector2 MoveValue;					// 캐릭터 방향 벡터
	bool isMove;						// 캐릭터 이동 중이냐 ?
	bool isRoll;						// 캐릭터 현재 대쉬 중이냐 ?
	bool AttackType;					// 캐릭터 원거리 / 근거리
	
	float AttackDir;					// 캐릭터 공격 방향
	float AttackRound;					// 캐릭터 공격 범위
	float AttackAngle;					// 캐릭터 공격 각도
	float AttackTime;					// 캐릭터 공격 제한 시간 ( 콤보 공격  )
	int AttackCount;					// 캐릭터 근접공격 콤보 수치
	bool isAttack;						// 캐릭터 근접공격

	float RollingTime;					// 캐릭터 대쉬 제한 시간
	int RollingCount;					// 캐릭터 대쉬(롤링) 횟수 제한 ( 3번 까지 )
	class Bullet* PlayerBullet;			// 캐릭터 총알
	float PowerTime;					// 캐릭터 원거리공격 얼마나 기모았냐?
	float ThrowTime;					// 캐릭터 원거리공격 제한 시간
	int ThrowCount;						// 캐릭터 원거리공격 콤보 수치
	bool isThrow;						// 캐릭터 원거리공격
	bool isHold;						// 캐릭터 원거리공격 조준

	int FrameIndex;						// 캐릭터 이미지 프레임 인덱스
	float FrameCount;					// 캐릭터 이미지 프레임 카운트
	float KeyTime;						// 캐릭터 키입력 확인 시간
		
	RECT HitRect;						// 캐릭터 Hit 용 RECT
	RECT ColRect;						// 캐릭터 벽 충돌 용 RECT
	RECT AttackRect;					// 캐릭터 근접공격 RECT

	int PlayerHp;						// 캐릭터 Hp
	bool isHit;							// 캐릭터 근접공격 맞음
	bool AlphaStart;					// 캐릭터 알파 시작
	bool isAlpha;						// 캐릭터 알파 진행여부
	float Alpha;						// 캐릭터 알파값
	float AlphaTime;					// 캐릭터 알파 카운트
	float AlphaChangeTime;				// 캐릭터 알파 변경시간
	float HitTime;						// 캐릭터 Hit 지속시간

	float DustDelay;					// 캐릭터 먼지 지연시간
	bool Event;							// 캐릭터 이벤트 중 동작 X
	bool isDying;						// 캐릭터 죽어간다..
	bool Boom;							// 캐릭터 펑

	class PlayerProgressBar* PlayerHpBar;	// 캐릭터 체력바
	class Particle* PlayerParticle;			// 캐릭터 파편

public:
	virtual void Init() override;		
	virtual void Update() override;
	virtual void Render() override;
	virtual void Release() override;

public:

	void SetisHit(bool hit) { isHit = hit; }
	bool GetisHit() { return isHit; }
	void SetisAlphaStart(bool a) { AlphaStart = a; }
	bool GetisAlphaStart() { return AlphaStart; }

	void SetisAlpha(bool a) { isAlpha = a; }
	bool GetisAlpha() { return isAlpha; }

	void SetHitRect(RECT rc) { HitRect = rc; }
	RECT GetHitRect() { return HitRect; }
	void SetColRect(RECT rc) { ColRect = rc; }
	RECT GetColRect() { return ColRect; }
	void SetAttackRect(RECT rc) { AttackRect = rc; }
	RECT GetAttackRect() { return AttackRect; }

	Bullet* GetPlayerBullet() { return PlayerBullet; }

	bool GetisAttack() { return isAttack; }

	void SetEvent(bool e) { Event = e; }

	void SetAttackType(bool a) { AttackType = a; }
	bool GetAttackType() { return AttackType; }


public:

	void Death();								// 캐릭터 죽음 ㅠ
	void Hit(int damage);						// 캐릭터 맞음
	void HitMove();								// 캐릭터 맞고 밀림
	void Move();								// 캐릭터 이동 
	void Attack();								// 캐릭터 공격
	void ImageFrameUpdate();					// 캐릭터 이미지 프레임 Update()
	void ImageResource();						// 캐릭터 이미지 추가
	void ImageRender();							// 캐릭터 이미지 Render()
	void UpdateRect();							// 캐릭터 RECT Update()
	void AlphaUpdate();							// 캐릭터 Alpha 값 Update()
	void DirCheck();							// 캐릭터 방향 Check

	void FinalAttack(float startangle);			// 캐릭터 마지막 공격 Effect

public:
	void FrameUpdate(int max);					// 프레임 업데이트
	void FrameUpdate(int max, float frame);		// 프레임 업데이트 ( 속도 조절 )
	void FrameHold(int max);					// 프레임 업데이트 X ( 한번만 )
	void FrameHold(int max, float frame);		// 프레임 업데이트 X, 속도 조절 O ( 한번만 )
	void FrameHoldMove(int max, float speed);	// 프레임 업데이트 X, 이동 O ( 한번만 ) 
	void FrameHoldMove2(int max, float frame, float speed);	// 프레임 업데이트 X, 이동 O ( isMove상관없이 무조건 이동 )
	void FrameHoldMove(int max, float frame, float speed);  // 프레임 업데이트 X, 이동 O, 속도 조절 O

	Player(Vector2 pos);
	virtual ~Player();
};