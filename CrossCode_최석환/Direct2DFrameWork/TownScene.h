#pragma once
#include "Tileheader.h"

class TownScene :
	public GameNode
{
private:
	TagTile Tile[XNUM][YNUM];			// ≈∏¿œ 

	class Player* player;

	Image* caveImage[9];

public:

	void Init() override;
	void Update() override;
	void Render() override;
	void Release() override;

	void LoadMap();
	void RenderMap();
	void TerrainRender();
	void Terrain2Render();
	void ObjectRender();

	TownScene();
	~TownScene();
};

