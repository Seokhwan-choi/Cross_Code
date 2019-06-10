#pragma once
#include "GameObject.h"
#define BREAK 50

struct CircleTag
{
	Image* mImage;				// �� Effect Image
	float Angle;				// �� Effect ����
	float Alpha;				// �� Effect ����
	Vector2 Size;				// �� Effect Size
};

class Slime :
	public GameObject
{
	// ū ������ Phase ����
	enum class Phase : int
	{
		// ü�¿� ���� ����� ����
		// ====================
		// One : ü�� 75% �̻�
		One, 
		// ====================
		// Two : ü�� 75% �̸� 50% �̻�
		Two, 
		// ====================
		// Three : ü�� 50% �̸� 25% �̻�
		Three,
		// ====================
		// Final : ü�� 25% �̸�
		Final, 
		// ====================
		END
	};

	enum class Pattern : int
	{
	//  �⺻,  ����,   ���� ���,   ���� �з���
		Idle, Attack, AttackIdle, AttackBack,
		
	//  ����, ���� ��, ���� ���
		Jump, Jumping, JumpIdle,

	//  ���ư���,   ��ǥ�缳��
		ComeBack, ReSet,

	//	���� ���,  ����,  ���� ��
		DashIdle,  Dash, DashComeBack,

	//  �������, ���� ��
		Charge, Shot, ChargeIdle,

	//  ������ź, ������ź ���, ����, ����
		Energy, EnergyIdle, Break, Gather,
		
	//  ����
		Death, END
	};

private:
	float CoreCount;		// ū ������ Core ������ ī��Ʈ
	int CoreIndex;			// ū ������ Core ������ �ε���

	Vector2 DirVector;		// ū ������ ���⺤��	
	int Hp;					// ū ������ ü��
	int BreakPoint;			// ū ������ ����ü�� ( ���� Ƚ�� Ÿ�� ���ϸ� ����! )

	float PatternChange;	// ū ������ �ֱ������� ���� ����
	float Speed;			// ū ������ �̵��ӵ�
	float Gravity;			// ū ������ �߷�
	float FrameCount;		// ū ������ ������ ī��Ʈ
	int FrameIndex;			// ū ������ ������ �ε���

	float JumpDelay;		// ū ������ ���� ������
	int JumpCount;			// ū ������ ���� ���� Ƚ��

	int SizeCheck;			// ū ������ Size üũ
	bool SizeUp;			// ū ������ Size ��ȭ
	float SizeCount;		// ū ������ Size ��ȭ �ð�

	float EffectDelay;		// ū ������ �� Effect DelayTime
	bool isWave;			// ū ������ �� Effect ��ȭ
	int EnergyCount;		// ū ������ ������ź ����

	float BackTime;			// ū ������ �ǵ��ư��� �ð�
	float AfterTime;		// ū ������ �ܻ� �ð�
	float AttackTime;		// ū ������ ���� �ð�
	int AttackIdleCount;	// ū ������ ���� �غ� Count
	int AttackCount;		// ū ������ ���� Ƚ��

	bool isCrush;			// ū ������ �浹 ����
	bool isJump;			// ū ������ ���� ��
	bool isGather;			// ū ������ ��ü �Ϸ�
	bool isShow;			// ū ������ ��ü�� �����ش�.
	
	bool isCol;				// ū ������ �浹�߾�

	bool isUp;				// ū ������ Idle ���¿��� Up / Down
	float IdleTime;			// ū ������ Idle ���¿��� ��, �Ʒ� �̵� Time
	float SlimeSize;		// ū ������ Size �ʱⰪ 256.f 
	float SlimeAlpha;		// ū ������ Alpha �ʱⰪ 1.f

	float PatternTime;		// ū ������ ���� ��ȭ �ð�

	RECT AttackRect;		// ū ������ ���ݿ� RECT
	RECT HitRect;			// ū ������ Hit�� RECT
	RECT ColRect;			// ū ������ �浹�� RECT

	CircleTag Circle[8];	// ū ������ �� Effect
	Phase SlimePhase;		// ū ������ ������ ( 1, 2, 3, 4 )
	Pattern SlimePattern;	// ū ������ ����

	class Bullet* EffectBullet;			// ������ ����Ʈ �Ѿ�
	class SlimeBullet* ShotBullet;		// ������ ��� �Ѿ�
	class Player* player;				// �÷��̾� 
	vector<class MiniSlime*> SlimeList;	// ���� ������ ��ġ
	int SlimeMax;						// ���� ������ ��ġ Phase�� ���� ���� �޶���

	Vector2 JumpPos;					// ū ������ ������ ��ǥ
	Image* Shadow;						// ū ������ �׸��� Image

	// ============================================
	// ################### ���� ###################
	// ============================================

	class BossProgressBar* Hpbar;		// ū ������ Hp Bar
	class ProgressBar* BreakBar;		// ū ������ Break Bar

	bool isHit;							// ū ������ ��Ʈ ��
	float HitTime;						// ū ������ ��Ʈ ���ӽð�
	bool isBreak;						// ū ������ ���� ��ġ ����
	float BreakTime;					// ū ������ ���� ��ġ ���� ���ӽð�
	bool Event;							// ū ������ �̺�Ʈ

	float ShockTime;					// ū ������ ��������ð�

	// ============================================
	// ################### ���� ###################
	// ============================================

	bool Boom;							// ū ������ ��������
	bool Die;							// ū ������ ��������
	float DieTime;						// ū ������ ���� �� Effect
	CircleTag DieCircle[10];			// ū ������ �����鼭 �� Effect

	class Particle* SlimeParticle;

	// ============================================
	// ################## ���� ####################
	// ============================================

	bool isDash;
	float PongTime;
	bool Pong;

	// ============================================
	// ################## ���� ####################
	// ============================================

	int ChargeCount;				// ���� Ƚ��
	float ChargeTime;				// ���� ��¡ �ð�
	class Thunder* ThunderRect;		// ���� �浹 �� RECT

public:

	virtual void Init() override;
	virtual void Update() override;
	virtual void Render() override;
	virtual void Release() override;

public:

	// ==============================================
	// ############ Get / Set �Լ� ##################
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
	// ########### ū ������ Pattern �Լ� ###############
	// =================================================

	void SetJumpPos();		// ū ������ ������ǥ Reset
	void Idle();			// ū ������ �⺻ ( Idle )
	//void Move();			// ū ������ �̵� ( Move )
	void JumpIdle();		// ū ������ ���� �� ��� ( JumpIdle )
	void ComeBack();		// ū ������ �ǵ��ư��� ( ComeBack )
	void Jump();			// ū ������ ���� ( Jump )
	void Jumping();			// ū ������ ���� �� ( Jumping )
	void AttackBack();		// ū ������ ���� �и�
	void AttackIdle();		// ū ������ ���� ��� ( AttackIdle )
	void Attack();			// ū ������ ���� ( Attack )
	void Energy();			// ū ������ ������ ź ( Energy )
	void EnergyIdle();		// ū ������ ������ ź ��� ( EnergyIdle )
	bool GatherCheck();		// ���� ������ ���� �Ǿ����� Ȯ��
	void Gather();			// ū ������ �籸�� ( ���� ������ �ٽ� ���δ�. ( Gather ) )
	void Break();			// ū ������ ���� ���������� ���� ( Break )

	void Charge();			// ū ������ ���� �߻� ����
	void Shot();			// ū ������ ���� �߻�
	void ChargeIdle();		// ū ������ ���� �߻� �� ���

	void PingPongIdle();	// ū ������ DashIdle
	void PingPong();		// ū ������ Dash
	void PingPongBack();	// ū ������ DashBack

	void Hit(int damage);	// ū ������ �¾Ҵ� ( ü�� ���� )
	void Hiting();			// ū ������ �´� ��
	void BreakHit();		// ū ������ �¾Ҵ� ( ���� ������ ���� )
	void BreakHiting();		// ū ������ �´� ��
	void Death();			// ū ������ ����

	// =================================================
	// #################################################
	// =================================================

public:

	void StateUpdate();								// ū ������ ���� Update()
	void FrameUpdate();								// ū ������ ������ ������Ʈ
		
	void PhaseCheck();								// ū ������ ü������ ���� ������ Ȯ��
	void CircleUpdate(Vector2 size, float alpha);	// ū ������ �� Effect Update()
	void CircleRender();							// ū ������ �� Effect Render()

	void SlimeCoreRender();							// ū ������ Core Redender()
	void SlimeCoreUpdate();							// ū ������ Core ������ Update()

	void MiniSlimeMake();							// ���� ������ ����
	void RectUpdate();								// �浹, ����, Hit, RECT Update()

	void CrushWall() { 
		SOUNDMANAGER->play("��", 0.15f); 
		SlimePattern = Pattern::AttackBack; }
	void PlayerDeath() { SlimePattern = Pattern::ComeBack; }
	void ProgressBarUpdate();						// ProgressBar Update()

	Slime(string name, Vector2 size, Vector2 pos, Pivot pivot);
	~Slime();
};

