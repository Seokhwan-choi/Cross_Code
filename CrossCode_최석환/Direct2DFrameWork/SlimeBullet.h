#pragma once
class SlimeBullet
{
private:
	Image* BulletImage;		// 총알의 Image
	class Bullet* EBullet;	// 총알의 총알 List
	Vector2 DirVector;		// 방향 벡터
	Vector2 Pos;			// 총알의 위치
	float Speed;			// 총알의 이동속도
	float ActiveTime;		// 총알의 지속시간
	float FrameCount;		// 총알의 프레임 카운트 
	int  FrameIndex;		// 총알의 프레임 인덱스
	bool isActive;			// 총알 활성화되어있냐 ?
	bool isFire;			// 총알 발사되었냐?

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

