#pragma once
#include "GameNode.h"

class SlimeScene :
	public GameNode
{
private:

	TagTile Tile[XNUM][YNUM];					// Ÿ�� 
	//deque<tile*> PathInfo;					// �ִܰ��

	class Player* player;						// ĳ����
	class Slime* slime;							// ������ ����
	class AstarHw* Astar;						// ���̽�Ÿ

	Image* SlimeImage[4];						// ū �����Ӹ� �̹���
	Image* Circle[2];

	RECT Col[7];								// �������� �浹�� RECT

	float DelayTime;							// ���̽�Ÿ ȣ�� Delay
	int PathIndex;								// ���̽�Ÿ ȣ�� Index

	int count;
	float angle;
	float alpha;
	Vector2 Size;

	class DamageFont* Damage;
	bool Event;
public:

	virtual void Init() override;
	virtual void Update() override;
	virtual void Render() override;
	virtual void Release() override;

public:

	void TileLoad();
	void TileTerrain();
	void TileObject();

public:

	void TileCol();
	void PlayerBulletAttack();
	void PlayerCloseAttack();
	void PlayerHit();

	void ShowEvent();

	SlimeScene() {}
	virtual ~SlimeScene() {}
};

