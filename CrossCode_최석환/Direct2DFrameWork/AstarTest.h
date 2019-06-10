#pragma once
#include "GameNode.h"
#include "Tileheader.h"

class AstarTest : public GameNode
{
private:
	class MiniSlime* slime[8];
	class Player* player;
	class AstarHw* Astar;

	float Delay;
	int Index;

public:

	void Init() override;
	void Update() override;
	void Render() override;
	void Release() override;

public:
	AstarTest() {}
	~AstarTest() {}
};

