#pragma once
#include "GameObject.h"
#define BREAK 50

struct CircleTag
{
	Image* mImage;				// 원 Effect Image
	float Angle;				// 원 Effect 각도
	float Alpha;				// 원 Effect 알파
	Vector2 Size;				// 원 Effect Size
};

class Slime :
	public GameObject
{
	// 큰 슬라임 Phase 상태
	enum class Phase : int
	{
		// 체력에 따라 페이즈를 나눔
		// ====================
		// One : 체력 75% 이상
		One, 
		// ====================
		// Two : 체력 75% 미만 50% 이상
		Two, 
		// ====================
		// Three : 체력 50% 미만 25% 이상
		Three,
		// ====================
		// Final : 체력 25% 미만
		Final, 
		// ====================
		END
	};

	enum class Pattern : int
	{
	//  기본,  공격,   공격 대기,   공격 밀려남
		Idle, Attack, AttackIdle, AttackBack,
		
	//  점프, 점프 중, 점프 대기
		Jump, Jumping, JumpIdle,

	//  돌아가기,   좌표재설정
		ComeBack, ReSet,

	//	폭주 대기,  폭주,  폭주 끝
		DashIdle,  Dash, DashComeBack,

	//  기모으기, 번개 샷
		Charge, Shot, ChargeIdle,

	//  에너지탄, 에너지탄 대기, 분해, 모음
		Energy, EnergyIdle, Break, Gather,
		
	//  죽음
		Death, END
	};

private:
	float CoreCount;		// 큰 슬라임 Core 프레임 카운트
	int CoreIndex;			// 큰 슬라임 Core 프레임 인덱스

	Vector2 DirVector;		// 큰 슬라임 방향벡터	
	int Hp;					// 큰 슬라임 체력
	int BreakPoint;			// 큰 슬라임 분해체력 ( 일정 횟수 타격 당하면 분해! )

	float PatternChange;	// 큰 슬라임 주기적으로 패턴 변경
	float Speed;			// 큰 슬라임 이동속도
	float Gravity;			// 큰 슬라임 중력
	float FrameCount;		// 큰 슬라임 프레임 카운트
	int FrameIndex;			// 큰 슬라임 프레임 인덱스

	float JumpDelay;		// 큰 슬라임 점프 딜레이
	int JumpCount;			// 큰 슬라임 점프 공격 횟수

	int SizeCheck;			// 큰 슬라임 Size 체크
	bool SizeUp;			// 큰 슬라임 Size 변화
	float SizeCount;		// 큰 슬라임 Size 변화 시간

	float EffectDelay;		// 큰 슬라임 원 Effect DelayTime
	bool isWave;			// 큰 슬라임 원 Effect 변화
	int EnergyCount;		// 큰 슬라임 에너지탄 갯수

	float BackTime;			// 큰 슬라임 되돌아가는 시간
	float AfterTime;		// 큰 슬라임 잔상 시간
	float AttackTime;		// 큰 슬라임 공격 시간
	int AttackIdleCount;	// 큰 슬라임 공격 준비 Count
	int AttackCount;		// 큰 슬라임 공격 횟수

	bool isCrush;			// 큰 슬라임 충돌 가능
	bool isJump;			// 큰 슬라임 점프 중
	bool isGather;			// 큰 슬라임 합체 완료
	bool isShow;			// 큰 슬라임 몸체를 보여준다.
	
	bool isCol;				// 큰 슬라임 충돌했어

	bool isUp;				// 큰 슬라임 Idle 상태에서 Up / Down
	float IdleTime;			// 큰 슬라임 Idle 상태에서 위, 아래 이동 Time
	float SlimeSize;		// 큰 슬라임 Size 초기값 256.f 
	float SlimeAlpha;		// 큰 슬라임 Alpha 초기값 1.f

	float PatternTime;		// 큰 슬라임 패턴 변화 시간

	RECT AttackRect;		// 큰 슬라임 공격용 RECT
	RECT HitRect;			// 큰 슬라임 Hit용 RECT
	RECT ColRect;			// 큰 슬라임 충돌용 RECT

	CircleTag Circle[8];	// 큰 슬라임 원 Effect
	Phase SlimePhase;		// 큰 슬라임 페이즈 ( 1, 2, 3, 4 )
	Pattern SlimePattern;	// 큰 슬라임 패턴

	class Bullet* EffectBullet;			// 슬라임 이펙트 총알
	class SlimeBullet* ShotBullet;		// 슬라임 쏘는 총알
	class Player* player;				// 플레이어 
	vector<class MiniSlime*> SlimeList;	// 작은 슬라임 뭉치
	int SlimeMax;						// 작은 슬라임 뭉치 Phase에 따라 갯수 달라짐

	Vector2 JumpPos;					// 큰 슬라임 점프용 좌표
	Image* Shadow;						// 큰 슬라임 그림자 Image

	// ============================================
	// ################### 맞음 ###################
	// ============================================

	class BossProgressBar* Hpbar;		// 큰 슬라임 Hp Bar
	class ProgressBar* BreakBar;		// 큰 슬라임 Break Bar

	bool isHit;							// 큰 슬라임 히트 중
	float HitTime;						// 큰 슬라임 히트 지속시간
	bool isBreak;						// 큰 슬라임 분해 수치 감소
	float BreakTime;					// 큰 슬라임 분해 수치 감소 지속시간
	bool Event;							// 큰 슬라임 이벤트

	float ShockTime;					// 큰 슬라임 전기생성시간

	// ============================================
	// ################### 죽음 ###################
	// ============================================

	bool Boom;							// 큰 슬라임 터질꺼야
	bool Die;							// 큰 슬라임 죽을꺼야
	float DieTime;						// 큰 슬라임 죽음 원 Effect
	CircleTag DieCircle[10];			// 큰 슬라임 죽으면서 원 Effect

	class Particle* SlimeParticle;

	// ============================================
	// ################## 폭주 ####################
	// ============================================

	bool isDash;
	float PongTime;
	bool Pong;

	// ============================================
	// ################## 번개 ####################
	// ============================================

	int ChargeCount;				// 번개 횟수
	float ChargeTime;				// 번개 차징 시간
	class Thunder* ThunderRect;		// 번개 충돌 용 RECT

public:

	virtual void Init() override;
	virtual void Update() override;
	virtual void Render() override;
	virtual void Release() override;

public:

	// ==============================================
	// ############ Get / Set 함수 ##################
	// ==============================================

	void SetPong(bool p) {
		Pong = p;
		PongTime = 0.f;
	}

	class Thunder* GetThunder() { return ThunderRect; }
	bool GetPong() { return Pong; }

	void SetSlimeSize(float size) { SlimeSize = size; }
	float GetSlimeSize() { return SlimeSize; }

	void SetSlimeList(vector<class MiniSlime*> slist) { SlimeList = slist; }
	vector<class MiniSlime*> GetSlimeList() { return SlimeList; }

	void SetHitRect(RECT rc) { HitRect = rc; }
	RECT GetHitRect() { return HitRect; }
	void SetColRect(RECT rc) { ColRect = rc; }
	RECT GetColRect() { return ColRect; }
	void SetAttackRect(RECT rc) { AttackRect = rc; }
	RECT GetAttackRect() { return AttackRect; }

	bool GetisShow() { return isShow; }

	Bullet* GetEffectBullet() { return EffectBullet; }
	SlimeBullet* GetShotBullet() { return ShotBullet; }

	Vector2 GetJPos() { return JumpPos; }
	void SetJPos(Vector2 v) { JumpPos = v; }

	void SetEvent(bool e) { Event = e; }
	bool GetEvent() { return Event; }

	void SetisCrush(bool c) { isCrush = c; }
	bool GetisCrush() { return isCrush; }

	bool GetisDash() { return isDash; }
	bool GetisHit() { return isHit; }
	bool GetisJump() { return isJump; }
	bool GetisCol() { return isCol; }
	void SetisCol(bool c) { isCol = c; }

	void SetDirVector(Vector2 d) {
		DirVector = d;
		AttackCount++;
	}
	Vector2 GetDirVector() { return DirVector; }


	// =================================================
	// #################################################
	// =================================================

public:

	// =================================================
	// ########### 큰 슬라임 Pattern 함수 ###############
	// =================================================

	void SetJumpPos();		// 큰 슬라임 점프좌표 Reset
	void Idle();			// 큰 슬라임 기본 ( Idle )
	//void Move();			// 큰 슬라임 이동 ( Move )
	void JumpIdle();		// 큰 슬라임 점프 중 대기 ( JumpIdle )
	void ComeBack();		// 큰 슬라임 되돌아가기 ( ComeBack )
	void Jump();			// 큰 슬라임 점프 ( Jump )
	void Jumping();			// 큰 슬라임 점프 중 ( Jumping )
	void AttackBack();		// 큰 슬라임 공격 밀림
	void AttackIdle();		// 큰 슬라임 공격 대기 ( AttackIdle )
	void Attack();			// 큰 슬라임 공격 ( Attack )
	void Energy();			// 큰 슬라임 에너지 탄 ( Energy )
	void EnergyIdle();		// 큰 슬라임 에너지 탄 대기 ( EnergyIdle )
	bool GatherCheck();		// 작은 슬라임 구성 되었는지 확인
	void Gather();			// 큰 슬라임 재구성 ( 작은 슬라임 다시 모인다. ( Gather ) )
	void Break();			// 큰 슬라임 작은 슬라임으로 분해 ( Break )

	void Charge();			// 큰 슬라임 번개 발사 충전
	void Shot();			// 큰 슬라임 번개 발사
	void ChargeIdle();		// 큰 슬라임 번개 발사 후 대기

	void PingPongIdle();	// 큰 슬라임 DashIdle
	void PingPong();		// 큰 슬라임 Dash
	void PingPongBack();	// 큰 슬라임 DashBack

	void Hit(int damage);	// 큰 슬라임 맞았다 ( 체력 감소 )
	void Hiting();			// 큰 슬라임 맞는 중
	void BreakHit();		// 큰 슬라임 맞았다 ( 분해 게이지 감소 )
	void BreakHiting();		// 큰 슬라임 맞는 중
	void Death();			// 큰 슬라임 죽음

	// =================================================
	// #################################################
	// =================================================

public:

	void StateUpdate();								// 큰 슬라임 상태 Update()
	void FrameUpdate();								// 큰 슬라임 프레임 업데이트
		
	void PhaseCheck();								// 큰 슬라임 체력으로 현재 페이즈 확인
	void CircleUpdate(Vector2 size, float alpha);	// 큰 슬라임 원 Effect Update()
	void CircleRender();							// 큰 슬라임 원 Effect Render()

	void SlimeCoreRender();							// 큰 슬라임 Core Redender()
	void SlimeCoreUpdate();							// 큰 슬라임 Core 프레임 Update()

	void MiniSlimeMake();							// 작은 슬라임 생성
	void RectUpdate();								// 충돌, 공격, Hit, RECT Update()

	void CrushWall() { 
		SOUNDMANAGER->play("탱", 0.15f); 
		SlimePattern = Pattern::AttackBack; }
	void PlayerDeath() { SlimePattern = Pattern::ComeBack; }
	void ProgressBarUpdate();						// ProgressBar Update()

	Slime(string name, Vector2 size, Vector2 pos, Pivot pivot);
	~Slime();
};

