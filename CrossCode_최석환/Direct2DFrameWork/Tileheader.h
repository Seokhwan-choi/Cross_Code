#pragma once

#define SPEED 8

#define SIZE 64
#define XNUM 100 
#define YNUM 100

#define SAMPLESIZE 16
#define SAMPLENUMX 16
#define SAMPLENUMY 18
// =====================================
// Ÿ���� ���� Enum 
// Zero, First, Second, Third,
// =====================================
enum class Order : int
{
	Zero, First, Second, Third, END
};

// =====================================
// ������ �ѹ� Enum
// One, Two, Three, Four, Five
// =====================================
enum class PageNum : int
{
	One, Two, Three, Four, Five, Six, Seven, Eight, Nine, END
};

// =====================================
// Ÿ���� ���� Enum
// Terrain(����), Object(������Ʈ), Erase(���찳)
// =====================================
enum class TileType : int
{
	Terrain, Object, Enemy, Erase, END
};

// =====================================
// �� ���� Enum
// Town(����), Boss1, Boss2, Boss3
// =====================================
enum class SceneNum : int
{
	Town, Boss1, Boss2, Boss3 ,END
};

struct TagTile
{
	RECT rc;				// Ÿ�� RECT

	int Terrainx;			// Ÿ�� ���� ������ X			
	int Terrainy;			// Ÿ�� ���� ������ Y
	PageNum TerrainNum;		// Ÿ�� ���� �������ѹ�
	Order TerrainOrder;		// Ÿ�� ���� ���� ����

	int Objectx;			// Ÿ�� ������Ʈ ������ X
	int Objecty;			// Ÿ�� ������Ʈ ������ Y
	PageNum ObjectNum;		// Ÿ�� ������Ʈ �������ѹ�
	Order ObjectOrder;		// Ÿ�� ������Ʈ ���� ����

	int Terrain2x;
	int Terrain2y;
	PageNum Terrain2Num;
	Order Terrain2Order;		

	string atrribute1;		// Ÿ�� �Ӽ�1
	string attribute2;		// Ÿ�� �Ӽ�2
	bool show;				// Ÿ�� �����ٱ� ? �� ��
};