#pragma once

#define SPEED 8

#define SIZE 64
#define XNUM 100 
#define YNUM 100

#define SAMPLESIZE 16
#define SAMPLENUMX 16
#define SAMPLENUMY 18
// =====================================
// 타일의 층수 Enum 
// Zero, First, Second, Third,
// =====================================
enum class Order : int
{
	Zero, First, Second, Third, END
};

// =====================================
// 페이지 넘버 Enum
// One, Two, Three, Four, Five
// =====================================
enum class PageNum : int
{
	One, Two, Three, Four, Five, Six, Seven, Eight, Nine, END
};

// =====================================
// 타일의 종류 Enum
// Terrain(지형), Object(오브젝트), Erase(지우개)
// =====================================
enum class TileType : int
{
	Terrain, Object, Enemy, Erase, END
};

// =====================================
// 씬 종류 Enum
// Town(마을), Boss1, Boss2, Boss3
// =====================================
enum class SceneNum : int
{
	Town, Boss1, Boss2, Boss3 ,END
};

struct TagTile
{
	RECT rc;				// 타일 RECT

	int Terrainx;			// 타일 지형 프레임 X			
	int Terrainy;			// 타일 지형 프레임 Y
	PageNum TerrainNum;		// 타일 지형 페이지넘버
	Order TerrainOrder;		// 타일 지형 현재 높이

	int Objectx;			// 타일 오브젝트 프레임 X
	int Objecty;			// 타일 오브젝트 프레임 Y
	PageNum ObjectNum;		// 타일 오브젝트 페이지넘버
	Order ObjectOrder;		// 타일 오브젝트 현재 높이

	int Terrain2x;
	int Terrain2y;
	PageNum Terrain2Num;
	Order Terrain2Order;		

	string atrribute1;		// 타일 속성1
	string attribute2;		// 타일 속성2
	bool show;				// 타일 보여줄까 ? ㅇ ㅇ
};