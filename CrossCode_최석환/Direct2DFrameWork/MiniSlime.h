#pragma once
#include "GameObject.h"
#define MAXHP 150
#define MOVEDISTANCE 150

struct CirCleTag 
{
	Image* mImage;
	float Alpha;
	Vector2 Size;
};

enum class Pattern : int
{
	// 기본, 이동, 점프, 추적
	Idle, Move, Jump, Chasor,

	// 공격, 맞음, 맞고밀림
	Attack, Hit, HitMove,

	// 모이기
	Gather, GatherUp, GatherIdle, GatherMove,
	
	// 흩어지기
	Break, SetPos, END
};

class MiniSlime :
	public GameObject
{
private:
	float AttackTime;				// 작은 슬라임 공격 시간 Delay
	float Speed;					// 작은 슬라임 이동 속도
	float FrameCount;				// 작은 슬라임 프레임 카운트
	float Gravity;					// 작은 슬라임 중력
	
	float ActiveTime;				// 작은 슬라임 사라질꺼야

	int FrameIndex;					// 작은 슬라임 프레임 인덱스
	int Hp;							// 작은 슬라임 체력
	
	bool isCol;						// 작은 슬라임 벽 충돌 가능
	bool isCrush;					// 작은 슬라임 충돌 가능
	bool isAttack;					// 작은 슬라임 공격
	bool isBreak;					// 작은 슬라임 다 흩어졌니?
	bool isGather;					// 작은 슬라임 다 모였니?
	Vector2 RebirthPos;				// 작은 슬라임 다시 태어난다.
	Vector2 DirVector;				// 작은 슬라임 방향 벡터
	Pattern SlimePattern;			// 작은 슬라임 패턴

	class Player* player;			// 플레이어 
	CirCleTag Circle[2];			// 작은 슬라임 원 Effect
	int EffectDelay;				// 작은 슬라임 원 Effect DelayTime ( 2개 엇박으로 팡팡 ! )
	
	bool AlphaStart;				// 작은 슬라임 알파 시작
	bool isAlpha;					// 작은 슬라임 알파 진행여부
	float Alpha;					// 작은 슬라임 알파값
	float AlphaTime;				// 작은 슬라임 알파 카운트
	float AlphaChangeTime;			// 작은 슬라임 알파 변경시간
	float HitTime;					// 작은 슬라임 Hit 지속시간

	RECT HitRect;					// 작은 슬라임 Hit용 RECT
	RECT ColRect;					// 작은 슬라임 충돌용 RECT
	RECT AttackRect;				// 작은 슬라임 공격용 RECT

	Image* Shadow;					// 작은 슬라임 그림자
	Vector2 JumpPos;				// 작은 슬라임 점프용 좌표

	float GatherUpTime;				// 작은 슬라임 GatherUp 지속시간
	float GatherIdleTime;			// 작은 슬라임 GatherIdle 지속 시간
	float IdleChangeTime;			// 작은 슬라임 GatherIdle 위/아래 변화 시간
	float AfterTime;				// 작은 슬라임 잔상 Delay
	bool IdleChange;				// 작은 슬라임 GatherIdle 변화 여부



	bool isDying;					// 작은 슬라임 죽어간다.
	float DieCheck;					// 작은 슬라임 죽음확인

	class ProgressBar* HpBar;		// 작은 슬라임 ProgressBar


	bool isChasor;					// 작은 슬라임 추적시작

	vector<Vector2> Path;			// 작은 슬라임 이동경로
	int Index;						// 작은 슬라임 최단경로 Index

public:

	virtual void Init() override;
	virtual void Update() override;
	virtual void Render() override;
	virtual void Release() override;

	// =================================================
	// ############## 멤버 변수 Get / Set ##############
	// =================================================

	void SetHp(int hp) { Hp = hp; }
	int GetHp() { return Hp; }
	void SetGather(bool gather) { isGather = gather; }
	bool GetGather() { return isGather; }
	void SetGravity(float gravity) { Gravity = gravity; }
	float GetGravity() { return Gravity; }
	void SetActiveTime(float time) { ActiveTime = time; }
	float GetActiveTime() { return ActiveTime; }

	void SetisCol(bool col) { isCol = col; }
	bool GetisCol() { return isCol; }
	void SetisCrush(bool crush) { isCrush = crush; }
	bool GetisCrush() { return isCrush; }
	void SetisAttack(bool att) { isAttack = att; }
	bool GetisAttack() { return isAttack; }

	void SetHitRect(RECT rc) { HitRect = rc; }
	RECT GetHitRect() { return HitRect; }
	void SetColRect(RECT rc) { ColRect = rc; }
	RECT GetColRect() { return ColRect; }
	void SetAttackRect(RECT rc) { AttackRect = rc; }
	RECT GetAttackRect() { return AttackRect; }

	void SetJumpPos(Vector2 jumppos) { JumpPos = jumppos; }

	void SetisChasor(bool c) { isChasor = c; }
	bool GetisChasor() { return isChasor; }


	void SetisDying(bool d) { isDying = d; }
	bool GetisDying() { return isDying; }

	void SetPath(vector<Vector2> path) { 
		int count = 0;
		if (Path.size() == path.size())
		{
			for (int i = 0; i < Path.size(); ++i)
			{
				for (int j = 0; j < path.size(); ++j)
				{
					if (Path[i] == path[j])
					{
						count++;
					}
				}
			}
			if (count != Path.size())
			{
				Index = 0;
			}
		}
		if (Path.size() != path.size())
			Index = 0;
		Path = path;
	}
	vector<Vector2> GetPath() { return Path; }

	void SetIndex(int i) { Index = i; }
	int GetIndex() { return Index; }
	// =================================================
	// #################################################
	// =================================================

	void Idle();						// 작은 슬라임 대기 상태
	void Chasor();						// 작은 슬라임 플레이어 추적
	void Move();						// 작은 슬라임 플레이어 쪽으로 이동
	void Jump();						// 작은 슬라임 플레이어 쪽으로 점프
	void Attack();						// 작은 슬라임 플레이어를 향해 공격
	void Hit(int damage);				// 작은 슬라임 공격 당함
	void HitMove();						// 작은 슬라임 맞고 밀림
	void Gather();						// 작은 슬라임 모으기 시작
	void GatherUp();					// 작은 슬라임 모으기 떠오르기
	void GatherIdle();					// 작은 슬라임 모으기 둥둥
	void GatherMove();					// 작은 슬라임 모으기 이동
	void SetPos();						// 작은 슬라임 좌표 재설정
	void Break();						// 작은 슬라임 흩뿌리기
	void DistanceCheck();				// 작은 슬라임 거리 확인
	void UpdateState();					// 작은 슬라임 상태 업데이트
	void ChangeState(Pattern pattern);	// 작은 슬라임 상태 변화

	void RenderCircle();				// 작은 슬라임 원 Render()
	void UpdateCircle();				// 작은 슬라임 원 Update()
	void Rebirth();						// 작은 슬라임 다시 활동
	void ImageFrameUpdate();			// 작은 슬라임 Image 프레임 Update()
	void UpdateRect();					// 작은 슬라임 RECT Update()
	void AlphaUpdate();					// 작은 슬라임 알파 Update()

	MiniSlime(string name, Vector2 size, Vector2 position, Pivot pivot);
	~MiniSlime();
};

