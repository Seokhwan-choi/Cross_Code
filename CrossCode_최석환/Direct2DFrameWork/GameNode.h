#pragma once
#include "Tileheader.h"

class GameNode
{
protected:
	class tile* _Tile[XNUM][YNUM];					// 복사할 타일
public:
	GameNode();
	virtual ~GameNode();

	virtual void Init() = 0;
	virtual void Release() = 0;
	virtual void Update() = 0;
	virtual void Render() = 0; 

	class tile* GetTile(int x, int y) { return _Tile[x][y]; }
};

