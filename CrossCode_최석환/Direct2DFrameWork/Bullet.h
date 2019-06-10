#pragma once

struct CircleTag2
{
	Image* mImage;				// ¿ø Effect Image
	float Angle;				// ¿ø Effect °¢µµ
	float Alpha;				// ¿ø Effect ¾ËÆÄ
	Vector2 Size;				// ¿ø Effect Size
};

struct TagBullet
{
	RECT bulletRect;				// ÃÑ¾Ë RECT
	Vector2 bulletpos;				// ÃÑ¾Ë ÁÂÇ¥
	Vector2 bulletfir;				// ÃÑ¾Ë Ã³À½½ÃÀÛ ÁÂÇ¥
	Vector2 bulletsize;				// ÃÑ¾Ë size
	float speed;					// ÃÑ¾Ë ¼Óµµ
	float angle;					// ÃÑ¾Ë °¢µµ
	float range;					// ÃÑ¾Ë »çÁ¤°Å¸®
	float alpha;					// ÃÑ¾Ë ¾ËÆÄ°ª
	float lifetime;					// ÃÑ¾Ë Áö¼Ó½Ã°£
	bool isFire;					// ÃÑ¾Ë ¹ß»ç¿©ºÎ
	Image* bulletImage;				// ÃÑ¾Ë Image

	int FrameIndex;					// ÃÑ¾Ë ÇÁ·¹ÀÓ ÀÎµ¦½º
	float FrameCount;				// ÃÑ¾Ë ÇÁ·¹ÀÓ Ä«¿îÆ®
	bool player;					
	bool Power;						// ±â¸ðÀº ÃÑ¾Ë!
	float EffectDelay;				// ÃÑ¾Ë ¿ø Effect DelayTime
	CircleTag2 Circle[2];			// ÃÑ¾Ë ¿ø Effect
};

class Bullet
{
private:
	vector<TagBullet> BulletList;		// ÃÑ¾Ë List
public:

	vector<TagBullet> GetBulletList() { return BulletList; }
	void SetisFire(int i, bool f) { BulletList[i].isFire = f; }
	bool GetisFire(int i) { return BulletList[i].isFire; }
	RECT GetBulletRect(int i) { return BulletList[i].bulletRect; }

public:

	void Fire(string key,										// ÃÑ¾Ë ÀÌ¹ÌÁö
		float angle,											// ÃÑ¾Ë °¢µµ
		Vector2 pos,											// ÃÑ¾Ë ÁÂÇ¥
		Vector2 size,											// ÃÑ¾Ë Å©±â
		Vector2 csize = Vector2(100.f,100.f),					// ÃÑ¾Ë ¿ø Effect Å©±â
		float life = 1000.f,									// ÃÑ¾Ë Áö¼Ó½Ã°£
		float speed = 1000.f,bool power = false, bool player = false);				// ÃÑ¾Ë ¹ß»ç
	void BulletMove();											// ÃÑ¾Ë ÀÌµ¿
					
	void Update();				// ÃÑ¾Ë Update()
	void Render();				// ÃÑ¾Ë Render()
	void Release();				// ÃÑ¾Ë Release();

	void RenderCircle();		// ÃÑ¾Ë ¿ø Effect Render()
	void UpdateCircle();		// ÃÑ¾Ë ¿ø Effect Update()

	Bullet();
	~Bullet() {}
};

