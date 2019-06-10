#pragma once
class SlimeBullet
{
private:
	Image* BulletImage;		// �Ѿ��� Image
	class Bullet* EBullet;	// �Ѿ��� �Ѿ� List
	Vector2 DirVector;		// ���� ����
	Vector2 Pos;			// �Ѿ��� ��ġ
	float Speed;			// �Ѿ��� �̵��ӵ�
	float ActiveTime;		// �Ѿ��� ���ӽð�
	float FrameCount;		// �Ѿ��� ������ ī��Ʈ 
	int  FrameIndex;		// �Ѿ��� ������ �ε���
	bool isActive;			// �Ѿ� Ȱ��ȭ�Ǿ��ֳ� ?
	bool isFire;			// �Ѿ� �߻�Ǿ���?

public:

	void SetFire(bool fire) { isFire = fire; }
	bool GetFire() { return isFire; }

	void SetActive(bool active) { isActive = active; }
	bool GetActive() { return isActive; }

	Bullet* GetEBullet() { return EBullet; }

public:

	void Fire(Vector2 pos, Vector2 dirvector, float speed);
	void Move();
	void Update();
	void Render();

	SlimeBullet();
	~SlimeBullet();
};

