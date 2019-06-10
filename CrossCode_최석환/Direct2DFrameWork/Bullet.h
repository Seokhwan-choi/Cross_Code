#pragma once

struct CircleTag2
{
	Image* mImage;				// �� Effect Image
	float Angle;				// �� Effect ����
	float Alpha;				// �� Effect ����
	Vector2 Size;				// �� Effect Size
};

struct TagBullet
{
	RECT bulletRect;				// �Ѿ� RECT
	Vector2 bulletpos;				// �Ѿ� ��ǥ
	Vector2 bulletfir;				// �Ѿ� ó������ ��ǥ
	Vector2 bulletsize;				// �Ѿ� size
	float speed;					// �Ѿ� �ӵ�
	float angle;					// �Ѿ� ����
	float range;					// �Ѿ� �����Ÿ�
	float alpha;					// �Ѿ� ���İ�
	float lifetime;					// �Ѿ� ���ӽð�
	bool isFire;					// �Ѿ� �߻翩��
	Image* bulletImage;				// �Ѿ� Image

	int FrameIndex;					// �Ѿ� ������ �ε���
	float FrameCount;				// �Ѿ� ������ ī��Ʈ
	bool player;					
	bool Power;						// ����� �Ѿ�!
	float EffectDelay;				// �Ѿ� �� Effect DelayTime
	CircleTag2 Circle[2];			// �Ѿ� �� Effect
};

class Bullet
{
private:
	vector<TagBullet> BulletList;		// �Ѿ� List
public:

	vector<TagBullet> GetBulletList() { return BulletList; }
	void SetisFire(int i, bool f) { BulletList[i].isFire = f; }
	bool GetisFire(int i) { return BulletList[i].isFire; }
	RECT GetBulletRect(int i) { return BulletList[i].bulletRect; }

public:

	void Fire(string key,										// �Ѿ� �̹���
		float angle,											// �Ѿ� ����
		Vector2 pos,											// �Ѿ� ��ǥ
		Vector2 size,											// �Ѿ� ũ��
		Vector2 csize = Vector2(100.f,100.f),					// �Ѿ� �� Effect ũ��
		float life = 1000.f,									// �Ѿ� ���ӽð�
		float speed = 1000.f,bool power = false, bool player = false);				// �Ѿ� �߻�
	void BulletMove();											// �Ѿ� �̵�
					
	void Update();				// �Ѿ� Update()
	void Render();				// �Ѿ� Render()
	void Release();				// �Ѿ� Release();

	void RenderCircle();		// �Ѿ� �� Effect Render()
	void UpdateCircle();		// �Ѿ� �� Effect Update()

	Bullet();
	~Bullet() {}
};

