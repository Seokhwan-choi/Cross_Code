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
	// �⺻, �̵�, ����, ����
	Idle, Move, Jump, Chasor,

	// ����, ����, �°�и�
	Attack, Hit, HitMove,

	// ���̱�
	Gather, GatherUp, GatherIdle, GatherMove,
	
	// �������
	Break, SetPos, END
};

class MiniSlime :
	public GameObject
{
private:
	float AttackTime;				// ���� ������ ���� �ð� Delay
	float Speed;					// ���� ������ �̵� �ӵ�
	float FrameCount;				// ���� ������ ������ ī��Ʈ
	float Gravity;					// ���� ������ �߷�
	
	float ActiveTime;				// ���� ������ ���������

	int FrameIndex;					// ���� ������ ������ �ε���
	int Hp;							// ���� ������ ü��
	
	bool isCol;						// ���� ������ �� �浹 ����
	bool isCrush;					// ���� ������ �浹 ����
	bool isAttack;					// ���� ������ ����
	bool isBreak;					// ���� ������ �� �������?
	bool isGather;					// ���� ������ �� �𿴴�?
	Vector2 RebirthPos;				// ���� ������ �ٽ� �¾��.
	Vector2 DirVector;				// ���� ������ ���� ����
	Pattern SlimePattern;			// ���� ������ ����

	class Player* player;			// �÷��̾� 
	CirCleTag Circle[2];			// ���� ������ �� Effect
	int EffectDelay;				// ���� ������ �� Effect DelayTime ( 2�� �������� ���� ! )
	
	bool AlphaStart;				// ���� ������ ���� ����
	bool isAlpha;					// ���� ������ ���� ���࿩��
	float Alpha;					// ���� ������ ���İ�
	float AlphaTime;				// ���� ������ ���� ī��Ʈ
	float AlphaChangeTime;			// ���� ������ ���� ����ð�
	float HitTime;					// ���� ������ Hit ���ӽð�

	RECT HitRect;					// ���� ������ Hit�� RECT
	RECT ColRect;					// ���� ������ �浹�� RECT
	RECT AttackRect;				// ���� ������ ���ݿ� RECT

	Image* Shadow;					// ���� ������ �׸���
	Vector2 JumpPos;				// ���� ������ ������ ��ǥ

	float GatherUpTime;				// ���� ������ GatherUp ���ӽð�
	float GatherIdleTime;			// ���� ������ GatherIdle ���� �ð�
	float IdleChangeTime;			// ���� ������ GatherIdle ��/�Ʒ� ��ȭ �ð�
	float AfterTime;				// ���� ������ �ܻ� Delay
	bool IdleChange;				// ���� ������ GatherIdle ��ȭ ����



	bool isDying;					// ���� ������ �׾��.
	float DieCheck;					// ���� ������ ����Ȯ��

	class ProgressBar* HpBar;		// ���� ������ ProgressBar


	bool isChasor;					// ���� ������ ��������

	vector<Vector2> Path;			// ���� ������ �̵����
	int Index;						// ���� ������ �ִܰ�� Index

public:

	virtual void Init() override;
	virtual void Update() override;
	virtual void Render() override;
	virtual void Release() override;

	// =================================================
	// ############## ��� ���� Get / Set ##############
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

	void Idle();						// ���� ������ ��� ����
	void Chasor();						// ���� ������ �÷��̾� ����
	void Move();						// ���� ������ �÷��̾� ������ �̵�
	void Jump();						// ���� ������ �÷��̾� ������ ����
	void Attack();						// ���� ������ �÷��̾ ���� ����
	void Hit(int damage);				// ���� ������ ���� ����
	void HitMove();						// ���� ������ �°� �и�
	void Gather();						// ���� ������ ������ ����
	void GatherUp();					// ���� ������ ������ ��������
	void GatherIdle();					// ���� ������ ������ �յ�
	void GatherMove();					// ���� ������ ������ �̵�
	void SetPos();						// ���� ������ ��ǥ �缳��
	void Break();						// ���� ������ ��Ѹ���
	void DistanceCheck();				// ���� ������ �Ÿ� Ȯ��
	void UpdateState();					// ���� ������ ���� ������Ʈ
	void ChangeState(Pattern pattern);	// ���� ������ ���� ��ȭ

	void RenderCircle();				// ���� ������ �� Render()
	void UpdateCircle();				// ���� ������ �� Update()
	void Rebirth();						// ���� ������ �ٽ� Ȱ��
	void ImageFrameUpdate();			// ���� ������ Image ������ Update()
	void UpdateRect();					// ���� ������ RECT Update()
	void AlphaUpdate();					// ���� ������ ���� Update()

	MiniSlime(string name, Vector2 size, Vector2 position, Pivot pivot);
	~MiniSlime();
};

