#pragma once
#include "gameNode.h"
#include "Tileheader.h"

class AstarHw :public GameNode
{
public:
	//���Ϲ� Ÿ������, ��ֹ� Ÿ������
	enum class FieldType : UINT
	{
		Normal, Obstacle
	};


private:
	//��Ÿ����.
	struct tagField
	{
		FieldType fType = FieldType::Normal; //�Ϲ�Ÿ������ ��ֹ�����
		bool bVisited = false;				 //�湮���� �ִ��� ����
		float fGlobalGoal;					 //������������ �Ÿ�
		float fLocalGoal;					 //�������������� �Ÿ�
		int x; int y;						 //����ǥ
		vector<tagField*> vecNeighbours;	 //�ֺ� Ÿ�ϵ� ������ �����ص� ����
		tagField* parent;					 //���� ����� �θ� ã�ư��� ���� ������
	};

	//��� ũ����
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

	float calculateDistance(tagField* a, tagField* b); //�Ÿ� ���
	float calculateHeuristic(tagField* a, tagField* b); //�޸���ƽ ���

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

