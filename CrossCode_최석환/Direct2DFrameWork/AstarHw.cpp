#include "stdafx.h"
#include "AstarHw.h"

void AstarHw::Init(int startX, int startY, int endX, int endY)
{
	SaveStartX = startX;
	SaveStartY = startY;
	SaveEndX = endX;
	SaveEndY = endY;

	SlimeImage[0] = IMAGEMANAGER->FindImage("큰슬라임");
	SlimeImage[1] = IMAGEMANAGER->FindImage("큰슬라임2");
	SlimeImage[2] = IMAGEMANAGER->FindImage("큰슬라임3");
	SlimeImage[3] = IMAGEMANAGER->FindImage("큰슬라임4");

	this->LoadTile();
	//▼배열의 이름은 포인터와 같으니, 아까 미리 만들어둔 포인터를 배열로 이닛
	nodes = new tagField[nMapWidth * nMapHeight];
	for (int x = 0; x < nMapWidth; x++) //가로
		for (int y = 0; y < nMapHeight; y++) //세로
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

	//▼동서남북 4방향 연결. 여기서 벌써 상하좌우 인덱스 벗어나는거에 대한 예외처리가 이루어져있음.
	//타일은 본인의 인접한 이웃 타일에 대한 포인터들을 벡터로 보유함
	for (int x = 0; x < nMapWidth; x++)
		for (int y = 0; y < nMapHeight; y++)
		{
			if (x > 0) //맨 우측 예외처리
				nodes[y*nMapWidth + x].vecNeighbours.push_back(&nodes[(y + 0) * nMapWidth + (x - 1)]);
			if (x < nMapWidth - 1) //맨 좌측 예외처리
				nodes[y*nMapWidth + x].vecNeighbours.push_back(&nodes[(y + 0) * nMapWidth + (x + 1)]);
			if (y > 0) //윗부분 예외처리
				nodes[y*nMapWidth + x].vecNeighbours.push_back(&nodes[(y - 1) * nMapWidth + (x + 0)]);
			if (y < nMapHeight - 1) // 바닥 예외처리
				nodes[y*nMapWidth + x].vecNeighbours.push_back(&nodes[(y + 1) * nMapWidth + (x + 0)]);
			
			//▼이거 주석 풀면 4방향이 아닌 8방향됨. 대각선 4방향 포인터들 또한 들고있는것

			//if (y>0 && x>0)
			//	nodes[y*nMapWidth + x].vecNeighbours.push_back(&nodes[(y - 1) * nMapWidth + (x - 1)]);
			//if (y<nMapHeight-1 && x>0)
			//	nodes[y*nMapWidth + x].vecNeighbours.push_back(&nodes[(y + 1) * nMapWidth + (x - 1)]);
			//if (y>0 && x<nMapWidth-1)
			//	nodes[y*nMapWidth + x].vecNeighbours.push_back(&nodes[(y - 1) * nMapWidth + (x + 1)]);
			//if (y<nMapHeight - 1 && x<nMapWidth-1)
			//	nodes[y*nMapWidth + x].vecNeighbours.push_back(&nodes[(y + 1) * nMapWidth + (x + 1)]);
		}

	//▼시작점과 끝점 임의설정
	nodeStart = &nodes[startY * nMapHeight + startX]; //내가 지정한 임의 위치
	nodeEnd = &nodes[endY * nMapHeight + endX];
}

void AstarHw::Release(void)
{
	//타일 뉴할당한거 여기 풀어주면 됨. 내가 커찮아서 안했을뿐!!!
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
	//if (KEYMANAGER->isOnceKeyDown(VK_LBUTTON)) //만약 좌클릭 버튼이 눌렸다면!
	//{
	//	if (nSelectedNodeX >= 0 && nSelectedNodeX < nMapWidth) //X좌표가 이상한곳 찍힌게 아니라면!
	//		if (nSelectedNodeY >= 0 && nSelectedNodeY < nMapHeight) //Y좌표가 이상한곳 찍힌게 아니라면! (위 조건과 한줄에 적어도 됨)
	//		{
	//			if (KEYMANAGER->isStayKeyDown(VK_SHIFT)) //그리고 쉬프트까지 눌려있는 상태였다면!
	//				nodeStart = &nodes[nSelectedNodeY * nMapWidth + nSelectedNodeX]; //그 노드로 스타트노드 설정.
	//			else if (KEYMANAGER->isStayKeyDown(VK_CONTROL)) //쉬프트가 아닌 컨트롤이 눌러져있었다면!
	//				nodeEnd = &nodes[nSelectedNodeY * nMapWidth + nSelectedNodeX]; //끝타일을 그걸로.
	//			else //쉬프트도, 컨트롤도 눌리지 않았다면 (쉽게 말해 그냥 타일이 눌렸다면!)
	//			{
	//				if (nodes[nSelectedNodeY * nMapWidth + nSelectedNodeX].fType == FieldType::Normal) //노멀 노드였다면!
	//					nodes[nSelectedNodeY * nMapWidth + nSelectedNodeX].fType = FieldType::Obstacle; //장애물로 바꾸자.
	//			}
	//			Astar(); //클릭 한번 일어났으니 에이스타 굴림
	//			
	//		}
	//}
}

vector<Vector2> AstarHw::GetPath()
{
	if (nodeEnd != nullptr) //노드 엔드가 존재한다면
	{
		vector<Vector2> RPath;
		Path.clear();
		tagField* p = nodeEnd; //임시로 끝 노드를 담아두고
		while (p->parent != nullptr) //현 노드서 부모가 존재한다면
		{
			RPath.push_back(Vector2(p->x, p->y));
			p = p->parent; //색 찍고 부모 노드로 포인터를 바꿈
		}

		// 역순으로 저장된 경로
		// 다시 뒤집어서 담아둔다.
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
					// ###################### 슬라임 보스 테마 ###############################
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
					// ###################### 슬라임 보스 테마 ###############################
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
					//▼장애물은 회색으로
					if (nodes[y * nMapWidth + x].fType == FieldType::Obstacle)
					{
						D2DRENDERER->FillRectangle({ (int)renderpoint.x, (int)renderpoint.y ,(x + 1) * nNodeSize, (y + 1) * nNodeSize },
							D2DRenderer::DefaultBrush::Gray);
					}
					//▼방문했던 노드는 노란색으로
					else if (nodes[y * nMapWidth + x].bVisited)
					{
						D2DRENDERER->FillRectangle({ (int)renderpoint.x, (int)renderpoint.y ,(x + 1) * nNodeSize, (y + 1) * nNodeSize },
							D2DRenderer::DefaultBrush::Yellow);
					}
					//▼시작점은 빨간색으로
					else if (&nodes[y * nMapWidth + x] == nodeStart)
					{
						D2DRENDERER->FillRectangle({ (int)renderpoint.x, (int)renderpoint.y ,(x + 1) * nNodeSize, (y + 1) * nNodeSize },
							D2DRenderer::DefaultBrush::Red);
					}
					//▼끝점은 파란색으로
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


	//▼목적지까지 구하는부분인데, 이 파트 잘보고 지나오는 노드 백터에 담아서 적 캐릭한테 주면 그 벡터보고 따라오게 하면 됨!
	//if (nodeEnd != nullptr) //노드 엔드가 존재한다면
	//{
	//	int count = 0; Path.clear();
	//	tagField *p = nodeEnd; //임시로 끝 노드를 담아두고
	//	while (p->parent != nullptr) //현 노드서 부모가 존재한다면
	//	{
	//		count++; Path.push_back(Vector2(p->x, p->y));
	//		Vector2 pos(p->x * nNodeSize, p->y * nNodeSize);
	//		Vector2 renderpos = CAMERA->GetRelativeVector2(pos);
	//		D2DRENDERER->FillRectangle({ (int)renderpos.x, (int)renderpos.y ,nNodeSize, nNodeSize },
	//			D2DRenderer::DefaultBrush::Green); //반복문 돌면서 색 찍음
	//		p = p->parent; //색 찍고 부모 노드로 포인터를 바꿈
	//	}
	//	cout << count << endl;
	//}

}



bool AstarHw::Astar()
{
	//▼길 찾기 전에 전부 초기화
	for (int x = 0; x < nMapWidth; x++)
		for (int y = 0; y < nMapHeight; y++)
		{
			nodes[y*nMapWidth + x].bVisited = false; //검색 시작시 방문여부 전부 끄고
			nodes[y*nMapWidth + x].fGlobalGoal = INFINITY; //시작할때 모든 타일들의 목적지까지의 거리에 큰 값 담아둠
			nodes[y*nMapWidth + x].fLocalGoal = INFINITY; //시작할때 모든 타일들의 시작점부터의 거리에 큰 값 담아둠
			nodes[y*nMapWidth + x].parent = nullptr; //부모 포인터 비워둠
		}

	//▼현재 노드를 시작노드로 설정
	tagField *nodeCurrent = nodeStart;
	nodeStart->fLocalGoal = 0.0f;
	nodeStart->fGlobalGoal = calculateHeuristic(nodeStart, nodeEnd);
	//nodeStart->fGlobalGoal = heuristic(nodeStart, nodeEnd); //익명함수 람다식 방식

	//▼타일 포인터들을 넣을 임시 리스트 생성
	list<tagField*> listNotTestedNodes;
	listNotTestedNodes.push_back(nodeStart); //시작지점 넣어서 검사 시작

	//▼여기서부턴 진행
	while (!listNotTestedNodes.empty() && nodeCurrent != nodeEnd) //타일포인터들은 리스트가 비어있지 않고, 목적지에 도달하지 않았을경우
	{
		//▼현재 검사중인 포인터의 이웃타일들을 전부 검사 시작한다
		for (auto &nodeNeighbour : nodeCurrent->vecNeighbours)
		{
			//▼지금 까보고 있는 이웃타일이 이미 방문한 타일이 아니거나 장애물이 아니라면, 
			if (!(nodeNeighbour->bVisited || nodeNeighbour->fType == FieldType::Obstacle))
				listNotTestedNodes.push_back(nodeNeighbour); //지금 검사중인 이웃타일을 미검사 리스트에 집어넣음

			//▼(지금 검사중인 이웃타일)과 (이웃타일이 파생된 원본 타일)과의 거리 + 시작점으로부터의 거리 계산해서 잠시 저장해둠
			float fPossiblyLowerGoal = nodeCurrent->fLocalGoal + calculateDistance(nodeCurrent, nodeNeighbour);
			//float fPossiblyLowerGoal = nodeCurrent->fLocalGoal + distance(nodeCurrent, nodeNeighbour); 람다방식

			//▼위에서 계산한 값이, 이웃타일이 보유하던값보다 작으면 대체. 맨 첨엔 fLocal에 무한대 들어있었던거 기억하셈!
			if (fPossiblyLowerGoal < nodeNeighbour->fLocalGoal)
			{
				nodeNeighbour->parent = nodeCurrent; //이웃 타일의 부모노드가 이젠 현재 검사중인 타일임
				nodeNeighbour->fLocalGoal = fPossiblyLowerGoal; //로컬 계산값 변동
				nodeNeighbour->fGlobalGoal = (float)nodeNeighbour->fLocalGoal + calculateHeuristic(nodeNeighbour, nodeEnd); //글로벌 게싼값 변경
			}
		}
		//▼목표까지 가장 가까운 노드를 선택하기 위해 휴리스틱 가장 낮은 값으로 정렬
		listNotTestedNodes.sort([](const tagField* lhs, const tagField* rhs) { return lhs->fGlobalGoal < rhs->fGlobalGoal; });

		//▼목표까지 휴리스틱이 동일한 길이 있을수 있으므로 그 길 제거
		while (!listNotTestedNodes.empty() && listNotTestedNodes.front()->bVisited)
			listNotTestedNodes.pop_front(); //예를들어 휴리스틱이 {8,8,8,9,9,9,9} 이런식이면 {8,9,9,9,9} 이렇게 최저치 하나만 냅두게

		//▼더이상 까본 노드가 없을경우 루프 종료
		if (listNotTestedNodes.empty())
			break;

		//▼휴리스틱 낮은순으로 정렬한거니, 그중 맨 앞에있는, 즉 휴리스틱 젤 낮은 노드로 이동 후 다시 반복문 돌기 시작
		nodeCurrent = listNotTestedNodes.front();
		nodeCurrent->bVisited = true;
	}
	listNotTestedNodes.clear(); //이번 검사는 다 했으니 비움
	return true;

}

void AstarHw::Init()
{
	SlimeImage[0] = IMAGEMANAGER->FindImage("큰슬라임");
	SlimeImage[1] = IMAGEMANAGER->FindImage("큰슬라임2");
	SlimeImage[2] = IMAGEMANAGER->FindImage("큰슬라임3");
	SlimeImage[3] = IMAGEMANAGER->FindImage("큰슬라임4");

	this->LoadTile();
	//▼배열의 이름은 포인터와 같으니, 아까 미리 만들어둔 포인터를 배열로 이닛
	nodes = new tagField[nMapWidth * nMapHeight];
	for (int x = 0; x < nMapWidth; x++) //가로
		for (int y = 0; y < nMapHeight; y++) //세로
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

	//▼동서남북 4방향 연결. 여기서 벌써 상하좌우 인덱스 벗어나는거에 대한 예외처리가 이루어져있음.
	//타일은 본인의 인접한 이웃 타일에 대한 포인터들을 벡터로 보유함
	for (int x = 0; x < nMapWidth; x++)
		for (int y = 0; y < nMapHeight; y++)
		{
			if (x > 0) //맨 우측 예외처리
				nodes[y * nMapWidth + x].vecNeighbours.push_back(&nodes[(y + 0) * nMapWidth + (x - 1)]);
			if (x < nMapWidth - 1) //맨 좌측 예외처리
				nodes[y * nMapWidth + x].vecNeighbours.push_back(&nodes[(y + 0) * nMapWidth + (x + 1)]);
			if (y > 0) //윗부분 예외처리
				nodes[y * nMapWidth + x].vecNeighbours.push_back(&nodes[(y - 1) * nMapWidth + (x + 0)]);
			if (y < nMapHeight - 1) // 바닥 예외처리
				nodes[y * nMapWidth + x].vecNeighbours.push_back(&nodes[(y + 1) * nMapWidth + (x + 0)]);

			//▼이거 주석 풀면 4방향이 아닌 8방향됨. 대각선 4방향 포인터들 또한 들고있는것

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

//▼거리 구하는 식
float AstarHw::calculateDistance(tagField* a, tagField* b)
{
	return sqrtf((a->x - b->x) * (a->x - b->x) + (a->y - b->y) * (a->y - b->y)); //피다고라스로 휴리스틱 계산
	//return abs(a->x - b->x) + abs(a->y - b->y); //멘헡은 방식으로 휴리스틱 계산
}

//▼휴리스틱 구하는 식
float AstarHw::calculateHeuristic(tagField* a, tagField* b)
{
	return calculateDistance(a, b);
}
