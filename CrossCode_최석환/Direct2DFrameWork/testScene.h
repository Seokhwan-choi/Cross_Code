#pragma once
#include "GameNode.h"
class testScene : public GameNode
{
private:
	float randomBeat;
	int counter;
public:


	testScene() {};
	~testScene() {};

	void Init();
	void Release();
	void Update();
	void Render();
};

