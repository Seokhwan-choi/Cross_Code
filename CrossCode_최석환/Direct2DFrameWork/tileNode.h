#pragma once

//게임화면 타일
#define TILESIZE 32
#define TILEX 20
#define TILEY 20
#define TILESIZEX (TILEX * TILESIZE)
#define TILESIZEY (TILEY * TILESIZE)

//이미지 타일(화면 좌상단에 붙임)
#define SAMPLETILEX 20
#define SAMPLETILEY 8

//맵툴에서 사용할 컨트롤 이넘문
enum CTRL
{
	CTRL_SAVE, CTRL_LOAD,
	CTRL_TERRAIN, CTRL_OBJECT, CTRL_ERASER,
	CTRL_MAP1, CTRL_MAP2, CTRL_MAP3, CTRL_END
};

//지형
enum TERRAIN
{
	TR_CEMENT, TR_GROUND, TR_GRASS, TR_WATER, TR_END
};

//오브젝트
enum OBJECT
{
	OBJ_BLOCK1, OBJ_BLOCK3, OBJ_BLOCKS,
	OBJ_TANK1, OBJ_TANK2,
	OBJ_FLAG1, OBJ_FLAG2,
	OBJ_NONE
};

//타일구조체
struct tagTile
{
	TERRAIN terrain;
	OBJECT obj;
	RECT rc;

	int terrainFrameX;
	int terrainFrameY;
	int objFrameX;
	int objFrameY;
};

//이미지 타일 구조체
struct tagSampleTile
{
	RECT rcTile;
	int terrainFrameX;
	int terrainFrameY;
};

//현재타일 구조체
struct tagCurrentTile
{
	int x, y;
};
