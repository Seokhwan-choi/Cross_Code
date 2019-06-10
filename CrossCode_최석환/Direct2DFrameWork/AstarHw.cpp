#include "stdafx.h"
#include "AstarHw.h"

void AstarHw::Init(int startX, int startY, int endX, int endY)
{
	SaveStartX = startX;
	SaveStartY = startY;
	SaveEndX = endX;
	SaveEndY = endY;

	SlimeImage[0] = IMAGEMANAGER->FindImage("ū������");
	SlimeImage[1] = IMAGEMANAGER->FindImage("ū������2");
	SlimeImage[2] = IMAGEMANAGER->FindImage("ū������3");
	SlimeImage[3] = IMAGEMANAGER->FindImage("ū������4");

	this->LoadTile();
	//��迭�� �̸��� �����Ϳ� ������, �Ʊ� �̸� ������ �����͸� �迭�� �̴�
	nodes = new tagField[nMapWidth * nMapHeight];
	for (int x = 0; x < nMapWidth; x++) //����
		for (int y = 0; y < nMapHeight; y++) //����
		{
			if ((test[x][y].Terrain2x >= 1 && test[x][y].Terrain2y < 16))
			{
				nodes[y * nMapWidth + x].fType = FieldType::Obstacle;
			}
			else
			{
				nodes[y * nMapWidth + x].fType = FieldType::Normal;
			}
			nodes[y * nMapWidth + x].x = x;
			nodes[y * nMapWidth + x].y = y;
			nodes[y * nMapWidth + x].parent = nullptr;
			nodes[y * nMapWidth + x].bVisited = false;
		}

	//�嵿������ 4���� ����. ���⼭ ���� �����¿� �ε��� ����°ſ� ���� ����ó���� �̷��������.
	//Ÿ���� ������ ������ �̿� Ÿ�Ͽ� ���� �����͵��� ���ͷ� ������
	for (int x = 0; x < nMapWidth; x++)
		for (int y = 0; y < nMapHeight; y++)
		{
			if (x > 0) //�� ���� ����ó��
				nodes[y*nMapWidth + x].vecNeighbours.push_back(&nodes[(y + 0) * nMapWidth + (x - 1)]);
			if (x < nMapWidth - 1) //�� ���� ����ó��
				nodes[y*nMapWidth + x].vecNeighbours.push_back(&nodes[(y + 0) * nMapWidth + (x + 1)]);
			if (y > 0) //���κ� ����ó��
				nodes[y*nMapWidth + x].vecNeighbours.push_back(&nodes[(y - 1) * nMapWidth + (x + 0)]);
			if (y < nMapHeight - 1) // �ٴ� ����ó��
				nodes[y*nMapWidth + x].vecNeighbours.push_back(&nodes[(y + 1) * nMapWidth + (x + 0)]);
			
			//���̰� �ּ� Ǯ�� 4������ �ƴ� 8�����. �밢�� 4���� �����͵� ���� ����ִ°�

			//if (y>0 && x>0)
			//	nodes[y*nMapWidth + x].vecNeighbours.push_back(&nodes[(y - 1) * nMapWidth + (x - 1)]);
			//if (y<nMapHeight-1 && x>0)
			//	nodes[y*nMapWidth + x].vecNeighbours.push_back(&nodes[(y + 1) * nMapWidth + (x - 1)]);
			//if (y>0 && x<nMapWidth-1)
			//	nodes[y*nMapWidth + x].vecNeighbours.push_back(&nodes[(y - 1) * nMapWidth + (x + 1)]);
			//if (y<nMapHeight - 1 && x<nMapWidth-1)
			//	nodes[y*nMapWidth + x].vecNeighbours.push_back(&nodes[(y + 1) * nMapWidth + (x + 1)]);
		}

	//��������� ���� ���Ǽ���
	nodeStart = &nodes[startY * nMapHeight + startX]; //���� ������ ���� ��ġ
	nodeEnd = &nodes[endY * nMapHeight + endX];
}

void AstarHw::Release(void)
{
	//Ÿ�� ���Ҵ��Ѱ� ���� Ǯ���ָ� ��. ���� Ŀ���Ƽ� ��������!!!
}

void AstarHw::Update()
{
}

vector<Vector2> AstarHw::Update(int startX, int startY, int endX, int endY)
{

	if ((SaveEndX == endX && SaveEndY == endY) && 0
		(SaveStartX == startX && SaveStartY == startY))
	{
		return vector<Vector2>();
	}

	if (SaveEndX != endX)
	{
		SaveStartX = startX;
		SaveStartY = startY;
		SaveEndX = endX;
		SaveEndY = endY;

		nodeStart = &nodes[SaveStartY * nMapWidth + SaveStartX];
		nodeEnd = &nodes[SaveEndY * nMapWidth + SaveEndX];

		Astar();

		return GetPath();
	}
	else if (SaveEndY != endY)
	{
		SaveStartX = startX;
		SaveStartY = startY;
		SaveEndX = endX;
		SaveEndY = endY;

		nodeStart = &nodes[SaveStartY * nMapWidth + SaveStartX];
		nodeEnd = &nodes[SaveEndY * nMapWidth + SaveEndX];

		Astar();

		return GetPath();
	}
	else if (SaveStartX != startX)
	{
		SaveStartX = startX;
		SaveStartY = startY;
		SaveEndX = endX;
		SaveEndY = endY;

		nodeStart = &nodes[SaveStartY * nMapWidth + SaveStartX];
		nodeEnd = &nodes[SaveEndY * nMapWidth + SaveEndX];

		Astar();

		return GetPath();
	}
	else if (SaveStartY != startY)
	{
		SaveStartX = startX;
		SaveStartY = startY;
		SaveEndX = endX;
		SaveEndY = endY;

		nodeStart = &nodes[SaveStartY * nMapWidth + SaveStartX];
		nodeEnd = &nodes[SaveEndY * nMapWidth + SaveEndX];

		Astar();

		return GetPath();
	}

	return vector<Vector2>();
	//if (KEYMANAGER->isOnceKeyDown(VK_LBUTTON)) //���� ��Ŭ�� ��ư�� ���ȴٸ�!
	//{
	//	if (nSelectedNodeX >= 0 && nSelectedNodeX < nMapWidth) //X��ǥ�� �̻��Ѱ� ������ �ƴ϶��!
	//		if (nSelectedNodeY >= 0 && nSelectedNodeY < nMapHeight) //Y��ǥ�� �̻��Ѱ� ������ �ƴ϶��! (�� ���ǰ� ���ٿ� ��� ��)
	//		{
	//			if (KEYMANAGER->isStayKeyDown(VK_SHIFT)) //�׸��� ����Ʈ���� �����ִ� ���¿��ٸ�!
	//				nodeStart = &nodes[nSelectedNodeY * nMapWidth + nSelectedNodeX]; //�� ���� ��ŸƮ��� ����.
	//			else if (KEYMANAGER->isStayKeyDown(VK_CONTROL)) //����Ʈ�� �ƴ� ��Ʈ���� �������־��ٸ�!
	//				nodeEnd = &nodes[nSelectedNodeY * nMapWidth + nSelectedNodeX]; //��Ÿ���� �װɷ�.
	//			else //����Ʈ��, ��Ʈ�ѵ� ������ �ʾҴٸ� (���� ���� �׳� Ÿ���� ���ȴٸ�!)
	//			{
	//				if (nodes[nSelectedNodeY * nMapWidth + nSelectedNodeX].fType == FieldType::Normal) //��� ��忴�ٸ�!
	//					nodes[nSelectedNodeY * nMapWidth + nSelectedNodeX].fType = FieldType::Obstacle; //��ֹ��� �ٲ���.
	//			}
	//			Astar(); //Ŭ�� �ѹ� �Ͼ���� ���̽�Ÿ ����
	//			
	//		}
	//}
}

vector<Vector2> AstarHw::GetPath()
{
	if (nodeEnd != nullptr) //��� ���尡 �����Ѵٸ�
	{
		vector<Vector2> RPath;
		Path.clear();
		tagField* p = nodeEnd; //�ӽ÷� �� ��带 ��Ƶΰ�
		while (p->parent != nullptr) //�� ��弭 �θ� �����Ѵٸ�
		{
			RPath.push_back(Vector2(p->x, p->y));
			p = p->parent; //�� ��� �θ� ���� �����͸� �ٲ�
		}

		// �������� ����� ���
		// �ٽ� ����� ��Ƶд�.
		for (int i = RPath.size() - 1; i >= 0 ; --i)
		{
			Vector2 temp = RPath[i] * 64;
			Path.push_back(temp);
		}

		return Path;
	}
}

void AstarHw::LoadTile()
{
	HANDLE file;
	DWORD read;

	file = CreateFile(L"Boss1save.map", GENERIC_READ, 0, NULL, OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL, NULL);

	ReadFile(file, test, sizeof(test), &read, NULL);
	CloseHandle(file);
}

void AstarHw::TileTerrain()
{
	RECT temp;
	for (int i = 0; i < YNUM; ++i) {
		for (int j = 0; j < XNUM; ++j) {
			if (IntersectRect(&temp, &test[j][i].rc, &CAMERA->GetCameraRect()))
			{
				if (test[j][i].show == true)
				{
					// ======================================================================
					// ###################### ������ ���� �׸� ###############################
					// ======================================================================
					switch (test[j][i].TerrainNum)
					{
						//case PageNum::Zero:
						//	SlimeImage[0]->SetSize(Vector2(64, 64) * CAMERA->GetZoom());
						//	SlimeImage[0]->FrameRender(
						//		CAMERA->GetRelativeRect(test[j][i].rc).left,
						//		CAMERA->GetRelativeRect(test[j][i].rc).top,
						//		test[j][i].Terrainx, test[j][i].Terrainy, Pivot::LeftTop);// , CAMERA->GetZoom());
						//	break;

					case PageNum::One:
						SlimeImage[0]->SetSize(Vector2(64, 64) * CAMERA->GetZoom());
						SlimeImage[0]->FrameRender(
							CAMERA->GetRelativeRect(test[j][i].rc).left,
							CAMERA->GetRelativeRect(test[j][i].rc).top,
							test[j][i].Terrainx, test[j][i].Terrainy, Pivot::LeftTop);// , CAMERA->GetZoom());
						break;
					case PageNum::Two:
						SlimeImage[1]->SetSize(Vector2(64, 64) * CAMERA->GetZoom());
						SlimeImage[1]->FrameRender(
							CAMERA->GetRelativeRect(test[j][i].rc).left,
							CAMERA->GetRelativeRect(test[j][i].rc).top,
							test[j][i].Terrainx, test[j][i].Terrainy, Pivot::LeftTop);// , CAMERA->GetZoom());
						break;
					case PageNum::Three:
						SlimeImage[2]->SetSize(Vector2(64, 64) * CAMERA->GetZoom());
						SlimeImage[2]->FrameRender(
							CAMERA->GetRelativeRect(test[j][i].rc).left,
							CAMERA->GetRelativeRect(test[j][i].rc).top,
							test[j][i].Terrainx, test[j][i].Terrainy, Pivot::LeftTop);// , CAMERA->GetZoom());
						break;
					case PageNum::Four:
						SlimeImage[3]->SetSize(Vector2(64, 64) * CAMERA->GetZoom());
						SlimeImage[3]->FrameRender(
							CAMERA->GetRelativeRect(test[j][i].rc).left,
							CAMERA->GetRelativeRect(test[j][i].rc).top,
							test[j][i].Terrainx, test[j][i].Terrainy, Pivot::LeftTop);// , CAMERA->GetZoom());
						break;
					}

					switch (test[j][i].Terrain2Num)
					{
					case PageNum::One:
						SlimeImage[0]->SetSize(Vector2(64, 64) * CAMERA->GetZoom());
						SlimeImage[0]->FrameRender(
							CAMERA->GetRelativeRect(test[j][i].rc).left,
							CAMERA->GetRelativeRect(test[j][i].rc).top,
							test[j][i].Terrain2x, test[j][i].Terrain2y, Pivot::LeftTop);// , CAMERA->GetZoom());
						break;
					case PageNum::Two:
						SlimeImage[1]->SetSize(Vector2(64, 64) * CAMERA->GetZoom());
						SlimeImage[1]->FrameRender(
							CAMERA->GetRelativeRect(test[j][i].rc).left,
							CAMERA->GetRelativeRect(test[j][i].rc).top,
							test[j][i].Terrain2x, test[j][i].Terrain2y, Pivot::LeftTop);// , CAMERA->GetZoom());
						break;
					case PageNum::Three:
						SlimeImage[2]->SetSize(Vector2(64, 64) * CAMERA->GetZoom());
						SlimeImage[2]->FrameRender(
							CAMERA->GetRelativeRect(test[j][i].rc).left,
							CAMERA->GetRelativeRect(test[j][i].rc).top,
							test[j][i].Terrain2x, test[j][i].Terrain2y, Pivot::LeftTop);// , CAMERA->GetZoom());
						break;
					case PageNum::Four:
						SlimeImage[3]->SetSize(Vector2(64, 64) * CAMERA->GetZoom());
						SlimeImage[3]->FrameRender(
							CAMERA->GetRelativeRect(test[j][i].rc).left,
							CAMERA->GetRelativeRect(test[j][i].rc).top,
							test[j][i].Terrain2x, test[j][i].Terrain2y, Pivot::LeftTop);// , CAMERA->GetZoom());
						break;
					}
				}
			}
		}
	}
}

void AstarHw::TileObject()
{
	RECT temp;
	for (int i = 0; i < YNUM; ++i)
	{
		for (int j = 0; j < XNUM; ++j)
		{
			if (IntersectRect(&temp, &test[j][i].rc, &CAMERA->GetCameraRect()))
			{
				if (test[j][i].show == true)
				{
					// ======================================================================
					// ###################### ������ ���� �׸� ###############################
					// ======================================================================

					switch (test[j][i].ObjectNum)
					{
					case PageNum::One:
						SlimeImage[0]->SetSize(Vector2(64, 64) * CAMERA->GetZoom());
						SlimeImage[0]->FrameRender(
							CAMERA->GetRelativeRect(test[j][i].rc).left,
							CAMERA->GetRelativeRect(test[j][i].rc).top,
							test[j][i].Objectx, test[j][i].Objecty, Pivot::LeftTop);// , CAMERA->GetZoom());
						break;
					case PageNum::Two:
						SlimeImage[1]->SetSize(Vector2(64, 64) * CAMERA->GetZoom());
						SlimeImage[1]->FrameRender(
							CAMERA->GetRelativeRect(test[j][i].rc).left,
							CAMERA->GetRelativeRect(test[j][i].rc).top,
							test[j][i].Objectx, test[j][i].Objecty, Pivot::LeftTop);// , CAMERA->GetZoom());
						break;
					case PageNum::Three:
						SlimeImage[2]->SetSize(Vector2(64, 64) * CAMERA->GetZoom());
						SlimeImage[2]->FrameRender(
							CAMERA->GetRelativeRect(test[j][i].rc).left,
							CAMERA->GetRelativeRect(test[j][i].rc).top,
							test[j][i].Objectx, test[j][i].Objecty, Pivot::LeftTop);// , CAMERA->GetZoom());
						break;
					case PageNum::Four:
						SlimeImage[3]->SetSize(Vector2(64, 64) * CAMERA->GetZoom());
						SlimeImage[3]->FrameRender(
							CAMERA->GetRelativeRect(test[j][i].rc).left,
							CAMERA->GetRelativeRect(test[j][i].rc).top,
							test[j][i].Objectx, test[j][i].Objecty, Pivot::LeftTop);// , CAMERA->GetZoom());
						break;
					}
				}
			}
		}
	}
}

void AstarHw::Render(void)
{
	RECT temp;

	if (KEYMANAGER->IsToggleKey(VK_F1))
	{
		for (int x = 0; x < nMapWidth; x++)
			for (int y = 0; y < nMapHeight; y++)
			{
				
				RECT rc = { x * nNodeSize, y * nNodeSize ,(x + 1) * nNodeSize, (y + 1) * nNodeSize };
				if (IntersectRect(&temp, &rc, &CAMERA->GetCameraRect()))
				{
					Vector2 point = Vector2(x * nNodeSize, y * nNodeSize);
					Vector2 renderpoint = CAMERA->GetRelativeVector2(point);
					//����ֹ��� ȸ������
					if (nodes[y * nMapWidth + x].fType == FieldType::Obstacle)
					{
						D2DRENDERER->FillRectangle({ (int)renderpoint.x, (int)renderpoint.y ,(x + 1) * nNodeSize, (y + 1) * nNodeSize },
							D2DRenderer::DefaultBrush::Gray);
					}
					//��湮�ߴ� ���� ���������
					else if (nodes[y * nMapWidth + x].bVisited)
					{
						D2DRENDERER->FillRectangle({ (int)renderpoint.x, (int)renderpoint.y ,(x + 1) * nNodeSize, (y + 1) * nNodeSize },
							D2DRenderer::DefaultBrush::Yellow);
					}
					//��������� ����������
					else if (&nodes[y * nMapWidth + x] == nodeStart)
					{
						D2DRENDERER->FillRectangle({ (int)renderpoint.x, (int)renderpoint.y ,(x + 1) * nNodeSize, (y + 1) * nNodeSize },
							D2DRenderer::DefaultBrush::Red);
					}
					//�峡���� �Ķ�������
					else if (&nodes[y * nMapWidth + x] == nodeEnd)
					{
						D2DRENDERER->FillRectangle({ (int)renderpoint.x, (int)renderpoint.y ,(x + 1) * nNodeSize, (y + 1) * nNodeSize },
							D2DRenderer::DefaultBrush::Blue);
					}
					else
					{
						D2DRENDERER->FillRectangle({ (int)renderpoint.x, (int)renderpoint.y ,(x + 1) * nNodeSize, (y + 1) * nNodeSize },
							D2DRenderer::DefaultBrush::White);
					}
				}
			}
	}
	else 
	{
		this->TileTerrain();
		this->TileObject();
	}


	//����������� ���ϴºκ��ε�, �� ��Ʈ �ߺ��� �������� ��� ���Ϳ� ��Ƽ� �� ĳ������ �ָ� �� ���ͺ��� ������� �ϸ� ��!
	//if (nodeEnd != nullptr) //��� ���尡 �����Ѵٸ�
	//{
	//	int count = 0; Path.clear();
	//	tagField *p = nodeEnd; //�ӽ÷� �� ��带 ��Ƶΰ�
	//	while (p->parent != nullptr) //�� ��弭 �θ� �����Ѵٸ�
	//	{
	//		count++; Path.push_back(Vector2(p->x, p->y));
	//		Vector2 pos(p->x * nNodeSize, p->y * nNodeSize);
	//		Vector2 renderpos = CAMERA->GetRelativeVector2(pos);
	//		D2DRENDERER->FillRectangle({ (int)renderpos.x, (int)renderpos.y ,nNodeSize, nNodeSize },
	//			D2DRenderer::DefaultBrush::Green); //�ݺ��� ���鼭 �� ����
	//		p = p->parent; //�� ��� �θ� ���� �����͸� �ٲ�
	//	}
	//	cout << count << endl;
	//}

}



bool AstarHw::Astar()
{
	//��� ã�� ���� ���� �ʱ�ȭ
	for (int x = 0; x < nMapWidth; x++)
		for (int y = 0; y < nMapHeight; y++)
		{
			nodes[y*nMapWidth + x].bVisited = false; //�˻� ���۽� �湮���� ���� ����
			nodes[y*nMapWidth + x].fGlobalGoal = INFINITY; //�����Ҷ� ��� Ÿ�ϵ��� ������������ �Ÿ��� ū �� ��Ƶ�
			nodes[y*nMapWidth + x].fLocalGoal = INFINITY; //�����Ҷ� ��� Ÿ�ϵ��� ������������ �Ÿ��� ū �� ��Ƶ�
			nodes[y*nMapWidth + x].parent = nullptr; //�θ� ������ �����
		}

	//������ ��带 ���۳��� ����
	tagField *nodeCurrent = nodeStart;
	nodeStart->fLocalGoal = 0.0f;
	nodeStart->fGlobalGoal = calculateHeuristic(nodeStart, nodeEnd);
	//nodeStart->fGlobalGoal = heuristic(nodeStart, nodeEnd); //�͸��Լ� ���ٽ� ���

	//��Ÿ�� �����͵��� ���� �ӽ� ����Ʈ ����
	list<tagField*> listNotTestedNodes;
	listNotTestedNodes.push_back(nodeStart); //�������� �־ �˻� ����

	//�忩�⼭���� ����
	while (!listNotTestedNodes.empty() && nodeCurrent != nodeEnd) //Ÿ�������͵��� ����Ʈ�� ������� �ʰ�, �������� �������� �ʾ������
	{
		//������ �˻����� �������� �̿�Ÿ�ϵ��� ���� �˻� �����Ѵ�
		for (auto &nodeNeighbour : nodeCurrent->vecNeighbours)
		{
			//������ ��� �ִ� �̿�Ÿ���� �̹� �湮�� Ÿ���� �ƴϰų� ��ֹ��� �ƴ϶��, 
			if (!(nodeNeighbour->bVisited || nodeNeighbour->fType == FieldType::Obstacle))
				listNotTestedNodes.push_back(nodeNeighbour); //���� �˻����� �̿�Ÿ���� �̰˻� ����Ʈ�� �������

			//��(���� �˻����� �̿�Ÿ��)�� (�̿�Ÿ���� �Ļ��� ���� Ÿ��)���� �Ÿ� + ���������κ����� �Ÿ� ����ؼ� ��� �����ص�
			float fPossiblyLowerGoal = nodeCurrent->fLocalGoal + calculateDistance(nodeCurrent, nodeNeighbour);
			//float fPossiblyLowerGoal = nodeCurrent->fLocalGoal + distance(nodeCurrent, nodeNeighbour); ���ٹ��

			//�������� ����� ����, �̿�Ÿ���� �����ϴ������� ������ ��ü. �� ÷�� fLocal�� ���Ѵ� ����־����� ����ϼ�!
			if (fPossiblyLowerGoal < nodeNeighbour->fLocalGoal)
			{
				nodeNeighbour->parent = nodeCurrent; //�̿� Ÿ���� �θ��尡 ���� ���� �˻����� Ÿ����
				nodeNeighbour->fLocalGoal = fPossiblyLowerGoal; //���� ��갪 ����
				nodeNeighbour->fGlobalGoal = (float)nodeNeighbour->fLocalGoal + calculateHeuristic(nodeNeighbour, nodeEnd); //�۷ι� �ԽѰ� ����
			}
		}
		//���ǥ���� ���� ����� ��带 �����ϱ� ���� �޸���ƽ ���� ���� ������ ����
		listNotTestedNodes.sort([](const tagField* lhs, const tagField* rhs) { return lhs->fGlobalGoal < rhs->fGlobalGoal; });

		//���ǥ���� �޸���ƽ�� ������ ���� ������ �����Ƿ� �� �� ����
		while (!listNotTestedNodes.empty() && listNotTestedNodes.front()->bVisited)
			listNotTestedNodes.pop_front(); //������� �޸���ƽ�� {8,8,8,9,9,9,9} �̷����̸� {8,9,9,9,9} �̷��� ����ġ �ϳ��� ���ΰ�

		//����̻� � ��尡 ������� ���� ����
		if (listNotTestedNodes.empty())
			break;

		//���޸���ƽ ���������� �����ѰŴ�, ���� �� �տ��ִ�, �� �޸���ƽ �� ���� ���� �̵� �� �ٽ� �ݺ��� ���� ����
		nodeCurrent = listNotTestedNodes.front();
		nodeCurrent->bVisited = true;
	}
	listNotTestedNodes.clear(); //�̹� �˻�� �� ������ ���
	return true;

}

void AstarHw::Init()
{
	SlimeImage[0] = IMAGEMANAGER->FindImage("ū������");
	SlimeImage[1] = IMAGEMANAGER->FindImage("ū������2");
	SlimeImage[2] = IMAGEMANAGER->FindImage("ū������3");
	SlimeImage[3] = IMAGEMANAGER->FindImage("ū������4");

	this->LoadTile();
	//��迭�� �̸��� �����Ϳ� ������, �Ʊ� �̸� ������ �����͸� �迭�� �̴�
	nodes = new tagField[nMapWidth * nMapHeight];
	for (int x = 0; x < nMapWidth; x++) //����
		for (int y = 0; y < nMapHeight; y++) //����
		{
			if ((test[x][y].Terrain2x >= 1 && test[x][y].Terrain2y < 16))
			{
				nodes[y * nMapWidth + x].fType = FieldType::Obstacle;
			}
			else
			{
				nodes[y * nMapWidth + x].fType = FieldType::Normal;
			}
			nodes[y * nMapWidth + x].x = x;
			nodes[y * nMapWidth + x].y = y;
			nodes[y * nMapWidth + x].parent = nullptr;
			nodes[y * nMapWidth + x].bVisited = false;
		}

	//�嵿������ 4���� ����. ���⼭ ���� �����¿� �ε��� ����°ſ� ���� ����ó���� �̷��������.
	//Ÿ���� ������ ������ �̿� Ÿ�Ͽ� ���� �����͵��� ���ͷ� ������
	for (int x = 0; x < nMapWidth; x++)
		for (int y = 0; y < nMapHeight; y++)
		{
			if (x > 0) //�� ���� ����ó��
				nodes[y * nMapWidth + x].vecNeighbours.push_back(&nodes[(y + 0) * nMapWidth + (x - 1)]);
			if (x < nMapWidth - 1) //�� ���� ����ó��
				nodes[y * nMapWidth + x].vecNeighbours.push_back(&nodes[(y + 0) * nMapWidth + (x + 1)]);
			if (y > 0) //���κ� ����ó��
				nodes[y * nMapWidth + x].vecNeighbours.push_back(&nodes[(y - 1) * nMapWidth + (x + 0)]);
			if (y < nMapHeight - 1) // �ٴ� ����ó��
				nodes[y * nMapWidth + x].vecNeighbours.push_back(&nodes[(y + 1) * nMapWidth + (x + 0)]);

			//���̰� �ּ� Ǯ�� 4������ �ƴ� 8�����. �밢�� 4���� �����͵� ���� ����ִ°�

			//if (y>0 && x>0)
			//	nodes[y*nMapWidth + x].vecNeighbours.push_back(&nodes[(y - 1) * nMapWidth + (x - 1)]);
			//if (y<nMapHeight-1 && x>0)
			//	nodes[y*nMapWidth + x].vecNeighbours.push_back(&nodes[(y + 1) * nMapWidth + (x - 1)]);
			//if (y>0 && x<nMapWidth-1)
			//	nodes[y*nMapWidth + x].vecNeighbours.push_back(&nodes[(y - 1) * nMapWidth + (x + 1)]);
			//if (y<nMapHeight - 1 && x<nMapWidth-1)
			//	nodes[y*nMapWidth + x].vecNeighbours.push_back(&nodes[(y + 1) * nMapWidth + (x + 1)]);
		}
}

//��Ÿ� ���ϴ� ��
float AstarHw::calculateDistance(tagField* a, tagField* b)
{
	return sqrtf((a->x - b->x) * (a->x - b->x) + (a->y - b->y) * (a->y - b->y)); //�Ǵٰ�󽺷� �޸���ƽ ���
	//return abs(a->x - b->x) + abs(a->y - b->y); //���Q�� ������� �޸���ƽ ���
}

//���޸���ƽ ���ϴ� ��
float AstarHw::calculateHeuristic(tagField* a, tagField* b)
{
	return calculateDistance(a, b);
}
