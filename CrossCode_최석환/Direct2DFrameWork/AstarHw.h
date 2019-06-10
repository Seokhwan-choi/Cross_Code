#pragma once
#include "gameNode.h"
#include "Tileheader.h"

class AstarHw :public GameNode
{
public:
	//▼일반 타일인지, 장애물 타일인지
	enum class FieldType : UINT
	{
		Normal, Obstacle
	};


private:
	//▼타일임.
	struct tagField
	{
		FieldType fType = FieldType::Normal; //일반타일인지 장애물인지
		bool bVisited = false;				 //방문한적 있는지 여부
		float fGlobalGoal;					 //목적지까지의 거리
		float fLocalGoal;					 //시작점서부터의 거리
		int x; int y;						 //실좌표
		vector<tagField*> vecNeighbours;	 //주변 타일들 포인터 보관해둔 백터
		tagField* parent;					 //가장 가까운 부모 찾아가기 위한 포인터
	};

	//▼맵 크기임
	int nMapWidth = 100;
	int nMapHeight = 100;
	LONG nNodeSize = 64;

	tagField *nodes = nullptr; 
	tagField *nodeStart = nullptr;
	tagField *nodeEnd = nullptr;

	int SaveStartX;
	int SaveStartY;
	int SaveEndX;
	int SaveEndY;

	vector<Vector2> Path;
	TagTile test[XNUM][YNUM];
	Image* SlimeImage[4];

	float calculateDistance(tagField* a, tagField* b); //거리 계산
	float calculateHeuristic(tagField* a, tagField* b); //휴리스틱 계산

public:

	void Init() override;
	void Release(void) override;
	void Update() override;
	void Render(void) override;

public:

	bool Astar();
	void Init(int startX, int startY, int endX, int endY);
	vector<Vector2> Update(int startX, int startY, int endX, int endY);
	vector<Vector2> GetPath();

public:

	void LoadTile();
	void TileTerrain();
	void TileObject();

public:


	AstarHw() {};
	~AstarHw() {};
};

