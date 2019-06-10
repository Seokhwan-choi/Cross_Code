#pragma once
#include "GameNode.h"

class SlimeScene :
	public GameNode
{
private:

	TagTile Tile[XNUM][YNUM];					// 타일 
	//deque<tile*> PathInfo;					// 최단경로

	class Player* player;						// 캐릭터
	class Slime* slime;							// 슬라임 보스
	class AstarHw* Astar;						// 에이스타

	Image* SlimeImage[4];						// 큰 슬라임맵 이미지
	Image* Circle[2];

	RECT Col[7];								// 스테이지 충돌용 RECT

	float DelayTime;							// 에이스타 호출 Delay
	int PathIndex;								// 에이스타 호출 Index

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

