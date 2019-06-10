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
	//  �� �⺻    ���� �⺻  �� �⺻  �Ʒ� �⺻
		IdleLeft, IdleRight, IdleUp	, IdleDown,

	//   ��     ��   ��   �Ʒ�											
		Left, Right, Up, Down,

	//   �� ��ž   �� ��ž    �� ��ž  �Ʒ� ��ž
		LeftStop, RightStop, UpStop, DownStop,

	//	 �� Ȧ��	   �� Ȧ��	 �� Ȧ��	 �Ʒ� Ȧ��
		LeftHold, RightHold, UpHold, DownHold,

	//	 �� Ȧ�� �̵�   �� Ȧ�� �̵�	 �� Ȧ�� �̵� �Ʒ� Ȧ�� �̵�
		LeftHoldMove, RightHoldMove, UpHoldMove, DownHoldMove,

	//	 �� ������	�� ������  �� ������	�Ʒ� ������
		LeftThrow, RightThrow, UpThrow, DownThrow,

	//	 �� ����		  �� ����	  �� ����   �Ʒ� ����
		LeftAttack, RightAttack, UpAttack, DownAttack,

	//	 �� ����	  �� ����   �� ����  �Ʒ� ����
		LeftHit, RightHit, UpHit, DownHit,
	
	//   ��/�� �⺻  ��/�� �⺻    ��/�Ʒ� �⺻   ��/�Ʒ� �⺻
		IdleLeftUp, IdleRightUp, IdleLeftDown, IdleRightDown,

	//   ��/��   ��/��     ��/�Ʒ�    ��/�Ʒ�
		LeftUp, RightUp, LeftDown, RightDown,

	//   ��/�� ��ž  ��/�� ��ž    ��/�Ʒ� ��ž    ��/�Ʒ� ��ž
		LeftUpStop, RightUpStop, LeftDownStop, RightDownStop,

	//   ��/�� Ȧ��	 ��/�� Ȧ��	  ��/�Ʒ� Ȧ��	��/�Ʒ� Ȧ��
		LeftUpHold, RightUpHold, LeftDownHold, RightDownHold,

	//   ��/�� Ȧ�� �̵�  ��/�� Ȧ�� �̵�   ��/�Ʒ� Ȧ�� �̵�  ��/�Ʒ� Ȧ�� �̵�
		LeftUpHoldMove, RightUpHoldMove, LeftDownHoldMove, RightDownHoldMove,

	//	 ��/�� ������  ��/�� ������   ��/�Ʒ� ������  ��/�Ʒ� ������
		LeftUpThrow, RightUpThrow, LeftDownThrow, RightDownThrow,

	//	 ��/�� ����		��/�� ����	  ��/�Ʒ� ����	  ��/�Ʒ� ����
		LeftUpAttack, RightUpAttack, LeftDownAttack, RightDownAttack,

	//  ��/�� ����	��/�� ����	��/�Ʒ� ����	 ��/�Ʒ� ����
		LeftUpHit, RightUpHit, LeftDownHit, RightDownHit,

	//   ������    ����
		Rolling, Death, END
	};
private:
	Dir PlayerDir;						// ĳ���� ��������
	STATE PlayerState;					// ĳ���� ���� ����
	int PlayerOrder;					// ĳ���� ���� ������ �ֳ� ?
	float Speed;						// ĳ���� �ӵ�
	Vector2 MoveValue;					// ĳ���� ���� ����
	bool isMove;						// ĳ���� �̵� ���̳� ?
	bool isRoll;						// ĳ���� ���� �뽬 ���̳� ?
	bool AttackType;					// ĳ���� ���Ÿ� / �ٰŸ�
	
	float AttackDir;					// ĳ���� ���� ����
	float AttackRound;					// ĳ���� ���� ����
	float AttackAngle;					// ĳ���� ���� ����
	float AttackTime;					// ĳ���� ���� ���� �ð� ( �޺� ����  )
	int AttackCount;					// ĳ���� �������� �޺� ��ġ
	bool isAttack;						// ĳ���� ��������

	float RollingTime;					// ĳ���� �뽬 ���� �ð�
	int RollingCount;					// ĳ���� �뽬(�Ѹ�) Ƚ�� ���� ( 3�� ���� )
	class Bullet* PlayerBullet;			// ĳ���� �Ѿ�
	float PowerTime;					// ĳ���� ���Ÿ����� �󸶳� ���ҳ�?
	float ThrowTime;					// ĳ���� ���Ÿ����� ���� �ð�
	int ThrowCount;						// ĳ���� ���Ÿ����� �޺� ��ġ
	bool isThrow;						// ĳ���� ���Ÿ�����
	bool isHold;						// ĳ���� ���Ÿ����� ����

	int FrameIndex;						// ĳ���� �̹��� ������ �ε���
	float FrameCount;					// ĳ���� �̹��� ������ ī��Ʈ
	float KeyTime;						// ĳ���� Ű�Է� Ȯ�� �ð�
		
	RECT HitRect;						// ĳ���� Hit �� RECT
	RECT ColRect;						// ĳ���� �� �浹 �� RECT
	RECT AttackRect;					// ĳ���� �������� RECT

	int PlayerHp;						// ĳ���� Hp
	bool isHit;							// ĳ���� �������� ����
	bool AlphaStart;					// ĳ���� ���� ����
	bool isAlpha;						// ĳ���� ���� ���࿩��
	float Alpha;						// ĳ���� ���İ�
	float AlphaTime;					// ĳ���� ���� ī��Ʈ
	float AlphaChangeTime;				// ĳ���� ���� ����ð�
	float HitTime;						// ĳ���� Hit ���ӽð�

	float DustDelay;					// ĳ���� ���� �����ð�
	bool Event;							// ĳ���� �̺�Ʈ �� ���� X
	bool isDying;						// ĳ���� �׾��..
	bool Boom;							// ĳ���� ��

	class PlayerProgressBar* PlayerHpBar;	// ĳ���� ü�¹�
	class Particle* PlayerParticle;			// ĳ���� ����

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

	void Death();								// ĳ���� ���� ��
	void Hit(int damage);						// ĳ���� ����
	void HitMove();								// ĳ���� �°� �и�
	void Move();								// ĳ���� �̵� 
	void Attack();								// ĳ���� ����
	void ImageFrameUpdate();					// ĳ���� �̹��� ������ Update()
	void ImageResource();						// ĳ���� �̹��� �߰�
	void ImageRender();							// ĳ���� �̹��� Render()
	void UpdateRect();							// ĳ���� RECT Update()
	void AlphaUpdate();							// ĳ���� Alpha �� Update()
	void DirCheck();							// ĳ���� ���� Check

	void FinalAttack(float startangle);			// ĳ���� ������ ���� Effect

public:
	void FrameUpdate(int max);					// ������ ������Ʈ
	void FrameUpdate(int max, float frame);		// ������ ������Ʈ ( �ӵ� ���� )
	void FrameHold(int max);					// ������ ������Ʈ X ( �ѹ��� )
	void FrameHold(int max, float frame);		// ������ ������Ʈ X, �ӵ� ���� O ( �ѹ��� )
	void FrameHoldMove(int max, float speed);	// ������ ������Ʈ X, �̵� O ( �ѹ��� ) 
	void FrameHoldMove2(int max, float frame, float speed);	// ������ ������Ʈ X, �̵� O ( isMove������� ������ �̵� )
	void FrameHoldMove(int max, float frame, float speed);  // ������ ������Ʈ X, �̵� O, �ӵ� ���� O

	Player(Vector2 pos);
	virtual ~Player();
};