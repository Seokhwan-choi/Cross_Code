#include "stdafx.h"
//#include "aStarManager.h"
//
//int aStarManager::getHeuristic(int idX1, int idY1, int idX2, int idY2)
//{
//	int dx = abs(idX1 - idX2);
//	int dy = abs(idY1 - idY2);
//	int diagonal = min(dx, dy);
//	return (dx - diagonal + dy - diagonal) * 64 + diagonal * 64 * sqrtf(2);
//}
//
//deque<tile*> aStarManager::getPath(int startIdX, int startIdY, int endIdX, int endIdY)
//{
//	//시작점 끝점이 같으면 빈 덱을 반환한다. 
//	if (startIdX == endIdX && startIdY == endIdY)return deque<tile*>();
//	//시작점과 끝점이 타일 범위를 벗어나도 빈덱을 반환한다.
//	if (!(0 <= startIdX && startIdX < 100 && 0 <= startIdY && startIdY < 100 &&
//		0 <= endIdX && endIdX < 100 && 0 <= endIdY && endIdY < 100))return deque<tile*>();
//	//최댓값 상수 정의 (비용 초기화에 쓰인다) 그렇다고 한다. 
//	const int MAX = 0x0fffffff;
//	
//	//노드 구조체 정의
//	typedef struct tagNode
//	{
//		tile *parent;
//		bool isOpen;
//		bool isClosed;
//		int costTotal;
//		int costFromStart;
//		int costToEnd;
//
//		tagNode() : parent(NULL), isOpen(false), isClosed(false),
//			costTotal(MAX), costFromStart(MAX), costToEnd(MAX) {}
//		//생성자로 초기화 할 때 변수는 못넣는데 상수는 가능하네
//		//MAX를 const int 로 만들어야한다
//
//		void calculateTotal() { costTotal = costFromStart + costToEnd; }
//	}NODE;
//
//	//맵의 타일대로 노드 생성
//	//노드는 같은 인덱스번호인 타일의 메모장이라고 보면 된다
//	//부모노드, 열림여부, 닫힘여부, F, G, H
//	NODE node[100][100];
//	//노드 초기화는 멤버 이니셜라이저 사용했으니 생략
//
//	//시작점 노드 초기화
//	node[startIdX][startIdY].parent = NULL;
//	node[startIdX][startIdY].isOpen = true;
//	node[startIdX][startIdY].isClosed = true;
//	node[startIdX][startIdY].costFromStart = 0;
//	node[startIdX][startIdY].costToEnd = getHeuristic(startIdX, startIdY, endIdX, endIdY);
//	node[startIdX][startIdY].calculateTotal();
//	
//	//열린목록 생성
//	deque<tile*> vOpen;
//
//	//시작점과 끝점 설정, 현재점은 시작점으로
//	tile* startTile = SCENEMANAGER->GetCurrent()->GetTile(startIdX, startIdY);
//	tile* endTile = SCENEMANAGER->GetCurrent()->GetTile(endIdX, endIdY);
//	tile* current = startTile;
//
//	/*----------------------------------------------------------
//	위쪽은 변수선언 및 변수 초기화
//	아래부터 로직
//	----------------------------------------------------------*/
//	bool eightDirection = false;
//
//	//에이스타 길찾는 루프
//	while (true)
//	{
//		if (current == NULL) break;   //안전장치
//
//		int curX = current->getIdX();
//		int curY = current->getIdY();
//
//		//현재점 중심 9타일 탐색
//		for (int j = curY - 1; j <= curY + 1; ++j)
//		{
//			if (j < 0 || j >= 100) continue;
//			for (int i = curX - 1; i <= curX + 1; ++i)
//			{
//				if (i < 0 || i >= 100) continue;
//
//				//자기 자신은 continue
//				if (i == curX && j == curY) continue;
//
//				//8방향 이동 불가능하면 (4방향 이동만 가능하면) 상하좌우 아니면 continue
//				if (!eightDirection)
//				{
//					if (!(i == curX && j == curY - 1 ||
//						i == curX && j == curY + 1 ||
//						i == curX - 1 && j == curY ||
//						i == curX + 1 && j == curY)) continue;
//				}
//				else   //8방향 이동에서 직선방향 장애물이면 그 옆에 대각선 막아주기
//				{
//					if (curX - 1 >= 0 && SCENEMANAGER->GetCurrent()->GetTile(curX - 1, curY)->getAttribute() == "Wall")
//						if (i == curX - 1 && j != curY) continue;
//					if (curX + 1 < 100 && SCENEMANAGER->GetCurrent()->GetTile(curX + 1, curY)->getAttribute() == "Wall")
//						if (i == curX + 1 && j != curY) continue;
//					if (curY - 1 >= 0 && SCENEMANAGER->GetCurrent()->GetTile(curX, curY - 1)->getAttribute() == "Wall")
//						if (j == curY - 1 && i != curX) continue;
//					if (curY + 1 < 100 && SCENEMANAGER->GetCurrent()->GetTile(curX, curY + 1)->getAttribute() == "Wall")
//						if (j == curY + 1 && i != curX) continue;
//				}
//
//				//닫혀있으면 continue
//				if (node[i][j].isClosed) continue;
//				//장애물이면 continue
//				if (SCENEMANAGER->GetCurrent()->GetTile(i, j)->getAttribute() == "Wall")
//				{
//					node[i][j].isOpen = true;
//					node[i][j].isClosed = true;
//					continue;
//				}
//
//				//처음 간 노드인 상황
//				if (!node[i][j].isOpen)
//				{
//					node[i][j].isOpen = true;
//					node[i][j].parent = current;
//					node[i][j].costFromStart = node[curX][curY].costFromStart +
//						Math::GetDistance(current->getCenter().x, current->getCenter().y,
//							SCENEMANAGER->GetCurrent()->GetTile(i, j)->getCenter().x,
//							SCENEMANAGER->GetCurrent()->GetTile(i, j)->getCenter().y);
//					node[i][j].costToEnd = getHeuristic(i, j, endIdX, endIdY);
//					node[i][j].calculateTotal();
//
//					vOpen.push_back(SCENEMANAGER->GetCurrent()->GetTile(i, j));
//				}
//				//가봤던 노드인 상황
//				else
//				{
//					//휴리스틱 값은 변하지 않는다
//					//costFromStart만 비교해서 더 작으면 작은것
//					int newG = node[curX][curY].costFromStart +
//						Math::GetDistance(current->getCenter().x, current->getCenter().y,
//							SCENEMANAGER->GetCurrent()->GetTile(i, j)->getCenter().x,
//							SCENEMANAGER->GetCurrent()->GetTile(i, j)->getCenter().y);
//					if (newG < node[i][j].costFromStart)
//					{
//						node[i][j].costFromStart = newG;
//						node[i][j].calculateTotal();
//						node[i][j].parent = current;
//					}
//				}
//			}
//		}
//
//		//9타일 탐색은 끝났고 이제 열린목록에서 costTotal 최소인 곳 선택
//		tile* tileMin = NULL;
//		for (int i = 0; i < vOpen.size(); ++i)
//		{
//			if (vOpen[i] == current)
//			{
//				vOpen.erase(vOpen.begin() + i--);
//				continue;
//			}
//			if (tileMin == NULL)
//			{
//				tileMin = vOpen[i];
//				continue;
//			}
//			if (node[vOpen[i]->getIdX()][vOpen[i]->getIdY()].costTotal <
//				node[tileMin->getIdX()][tileMin->getIdY()].costTotal)
//			{
//				tileMin = vOpen[i];
//			}
//		}
//		//선택타일 닫힘여부 true로 하고 선택타일 열린목록에서 제거
//		if (tileMin != NULL)
//		{
//			node[tileMin->getIdX()][tileMin->getIdY()].isClosed = true;
//
//			//열린목록 돌려서 tileMin과 같은거 찾아서 제거하는 과정은
//			//다음 선택 과정에서 하면 불필요한 포문 한번을 없앨 수 있다
//			//for (int i = 0; i < vOpen.size(); ++i)
//			//{
//			//	if (vOpen[i] == tileMin)
//			//	{
//			//		vOpen.erase(vOpen.begin() + i);
//			//		break;
//			//	}
//			//}
//
//			//선택타일을 다음 current로 설정
//			current = tileMin;
//		}
//		else   //선택타일이 없다 == 열린목록이 비어있다 == 갈 수 있는 길이 없다
//		{
//			break;   //길찾기 루프 종료시킨다
//		}
//
//		// <종료조건> 선택타일이 목표한 끝점인 상황
//		if (tileMin == endTile)
//		{
//			deque<tile*> solution;
//			deque<tile*> solutionReversed;
//			tile* tempTile = current;
//
//			//선택타일부터 부모 따라가면서 solutionReversed에 추가
//			solutionReversed.push_back(tempTile);
//			while (node[tempTile->getIdX()][tempTile->getIdY()].parent != NULL)
//			{
//				tempTile = node[tempTile->getIdX()][tempTile->getIdY()].parent;
//				solutionReversed.push_back(tempTile);
//			}
//
//			//솔루션리버스 뒤집어서 솔루션에 집어넣고 반환
//			for (int i = solutionReversed.size() - 1; i >= 0; --i)
//			{
//				solution.push_back(solutionReversed[i]);
//			}
//			return solution;
//		}
//		// <>
//	}
//
//	//길찾기 루프가 비정상적으로 종료된 상황
//	//빈 벡터 반환으로 끝냄
//	return deque<tile*>();
//}
//
//
//
////=========================================================================================
//
////## 장애물 상관없이 제일 빠른길을 찾아준다 ##
//
////=========================================================================================
//
////deque<tile*> aStarManager::getShortPath(int startIdX, int startIdY, int endIdX, int endIdY)
////{
////	//시작점 끝점이 같으면 빈 덱을 반환한다. 
////	if (startIdX == endIdX && startIdY == endIdY)return deque<tile*>();
////	//시작점과 끝점이 타일 범위를 벗어나도 빈덱을 반환한다.
////	if (!(0 <= startIdX && startIdX < TILEX && 0 <= startIdY && startIdY < TILEY &&
////		0 <= endIdX && endIdX < TILEX && 0 <= endIdY && endIdY < TILEY))return deque<tile*>();
////	//최댓값 상수 정의 (비용 초기화에 쓰인다) 그렇다고 한다. 
////	const int MAX = 0x0fffffff;
////
////	//노드 구조체 정의
////	typedef struct tagNode
////	{
////		tile *parent;
////		bool isOpen;
////		bool isClosed;
////		int costTotal;
////		int costFromStart;
////		int costToEnd;
////
////		tagNode() : parent(NULL), isOpen(false), isClosed(false),
////			costTotal(MAX), costFromStart(MAX), costToEnd(MAX) {}
////		//생성자로 초기화 할 때 변수는 못넣는데 상수는 가능하네
////		//MAX를 const int 로 만들어야한다
////
////		void calculateTotal() { costTotal = costFromStart + costToEnd; }
////	}NODE;
////
////	//맵의 타일대로 노드 생성
////	//노드는 같은 인덱스번호인 타일의 메모장이라고 보면 된다
////	//부모노드, 열림여부, 닫힘여부, F, G, H
////	NODE node[TILEX][TILEY];
////	//노드 초기화는 멤버 이니셜라이저 사용했으니 생략
////
////	//시작점 노드 초기화
////	node[startIdX][startIdY].parent = NULL;
////	node[startIdX][startIdY].isOpen = true;
////	node[startIdX][startIdY].isClosed = true;
////	node[startIdX][startIdY].costFromStart = 0;
////	node[startIdX][startIdY].costToEnd = getHeuristic(startIdX, startIdY, endIdX, endIdY);
////	node[startIdX][startIdY].calculateTotal();
////
////	//열린목록 생성
////	deque<tile*> vOpen;
////
////	//시작점과 끝점 설정, 현재점은 시작점으로
////	tile* startTile = OBJECTMANAGER->_tile[startIdX][startIdY];
////	tile* endTile = OBJECTMANAGER->_tile[endIdX][endIdY];
////	tile* current = startTile;
////
////	/*----------------------------------------------------------
////	위쪽은 변수선언 및 변수 초기화
////	아래부터 로직
////	----------------------------------------------------------*/
////	bool eightDirection = false;
////
////	//에이스타 길찾는 루프
////	while (true)
////	{
////		if (current == NULL) break;   //안전장치
////
////		int curX = current->getIndexX();
////		int curY = current->getIndexY();
////
////		//현재점 중심 9타일 탐색
////		for (int j = curY - 1; j <= curY + 1; ++j)
////		{
////			if (j < 0 || j >= TILEY) continue;
////			for (int i = curX - 1; i <= curX + 1; ++i)
////			{
////				if (i < 0 || i >= TILEX) continue;
////
////				//자기 자신은 continue
////				if (i == curX && j == curY) continue;
////
////				//8방향 이동 불가능하면 (4방향 이동만 가능하면) 상하좌우 아니면 continue
////				if (!eightDirection)
////				{
////					if (!(i == curX && j == curY - 1 ||
////						i == curX && j == curY + 1 ||
////						i == curX - 1 && j == curY ||
////						i == curX + 1 && j == curY)) continue;
////				}
////				else   //8방향 이동에서 직선방향 장애물이면 그 옆에 대각선 막아주기
////				{
////					if (curX - 1 >= 0 && !OBJECTMANAGER->_tile[curX - 1][curY]->getIsMovable())
////						if (i == curX - 1 && j != curY) continue;
////					if (curX + 1 < TILEX && !OBJECTMANAGER->_tile[curX + 1][curY]->getIsMovable())
////						if (i == curX + 1 && j != curY) continue;
////					if (curY - 1 >= 0 && !OBJECTMANAGER->_tile[curX][curY - 1]->getIsMovable())
////						if (j == curY - 1 && i != curX) continue;
////					if (curY + 1 < TILEY && !OBJECTMANAGER->_tile[curX][curY + 1]->getIsMovable())
////						if (j == curY + 1 && i != curX) continue;
////				}
////
////				//닫혀있으면 continue
////				if (node[i][j].isClosed) continue;
////				//장애물이면 continue
////				//if (!OBJECTMANAGER->_tile[i][j]->getIsMovable())
////				//{
////				//	node[i][j].isOpen = true;
////				//	node[i][j].isClosed = true;
////				//	continue;
////				//}
////
////				//처음 간 노드인 상황
////				if (!node[i][j].isOpen)
////				{
////					node[i][j].isOpen = true;
////					node[i][j].parent = current;
////					node[i][j].costFromStart = node[curX][curY].costFromStart +
////						getDistance(current->_pos.x, current->_pos.y,
////							OBJECTMANAGER->_tile[i][j]->_pos.x, OBJECTMANAGER->_tile[i][j]->_pos.y);
////					node[i][j].costToEnd = getHeuristic(i, j, endIdX, endIdY);
////					node[i][j].calculateTotal();
////
////					vOpen.push_back(OBJECTMANAGER->_tile[i][j]);
////				}
////				//가봤던 노드인 상황
////				else
////				{
////					//휴리스틱 값은 변하지 않는다
////					//costFromStart만 비교해서 더 작으면 작은것
////					int newG = node[curX][curY].costFromStart +
////						getDistance(current->_pos.x, current->_pos.y,
////							OBJECTMANAGER->_tile[i][j]->_pos.x, OBJECTMANAGER->_tile[i][j]->_pos.y);
////					if (newG < node[i][j].costFromStart)
////					{
////						node[i][j].costFromStart = newG;
////						node[i][j].calculateTotal();
////						node[i][j].parent = current;
////					}
////				}
////			}
////		}
////
////		//9타일 탐색은 끝났고 이제 열린목록에서 costTotal 최소인 곳 선택
////		tile* tileMin = NULL;
////		for (int i = 0; i < vOpen.size(); ++i)
////		{
////			if (vOpen[i] == current)
////			{
////				vOpen.erase(vOpen.begin() + i--);
////				continue;
////			}
////			if (tileMin == NULL)
////			{
////				tileMin = vOpen[i];
////				continue;
////			}
////			if (node[vOpen[i]->getIndexX()][vOpen[i]->getIndexY()].costTotal <
////				node[tileMin->getIndexX()][tileMin->getIndexY()].costTotal)
////			{
////				tileMin = vOpen[i];
////			}
////		}
////		//선택타일 닫힘여부 true로 하고 선택타일 열린목록에서 제거
////		if (tileMin != NULL)
////		{
////			node[tileMin->getIndexX()][tileMin->getIndexY()].isClosed = true;
////
////		
////
////			//선택타일을 다음 current로 설정
////			current = tileMin;
////		}
////		else   //선택타일이 없다 == 열린목록이 비어있다 == 갈 수 있는 길이 없다
////		{
////			break;   //길찾기 루프 종료시킨다
////		}
////
////		// <종료조건> 선택타일이 목표한 끝점인 상황
////		if (tileMin == endTile)
////		{
////			deque<tile*> solution;
////			deque<tile*> solutionReversed;
////			tile* tempTile = current;
////
////			//선택타일부터 부모 따라가면서 solutionReversed에 추가
////			solutionReversed.push_back(tempTile);
////			while (node[tempTile->getIndexX()][tempTile->getIndexY()].parent != NULL)
////			{
////				tempTile = node[tempTile->getIndexX()][tempTile->getIndexY()].parent;
////				solutionReversed.push_back(tempTile);
////			}
////
////			//솔루션리버스 뒤집어서 솔루션에 집어넣고 반환
////			for (int i = solutionReversed.size() - 1; i >= 0; --i)
////			{
////				solution.push_back(solutionReversed[i]);
////			}
////			return solution;
////		}
////		// <>
////	}
////
////	//길찾기 루프가 비정상적으로 종료된 상황
////	//빈 벡터 반환으로 끝냄
////	return deque<tile*>();
////}
////
//////=========================================================================================
//////##대각선 에이스타##
//////==========================================================================================
////
////deque<tile*> aStarManager::getCrossPath(int startIdX, int startIdY, int endIdX, int endIdY)
////{
////	//시작점 끝점이 같으면 빈 덱을 반환한다. 
////	if (startIdX == endIdX && startIdY == endIdY)return deque<tile*>();
////	//시작점과 끝점이 타일 범위를 벗어나도 빈덱을 반환한다.
////	if (!(0 <= startIdX && startIdX < TILEX && 0 <= startIdY && startIdY < TILEY &&
////		0 <= endIdX && endIdX < TILEX && 0 <= endIdY && endIdY < TILEY))return deque<tile*>();
////	//최댓값 상수 정의 (비용 초기화에 쓰인다) 그렇다고 한다. 
////	const int MAX = 0x0fffffff;
////
////	//노드 구조체 정의
////	typedef struct tagNode
////	{
////		tile *parent;
////		bool isOpen;
////		bool isClosed;
////		int costTotal;
////		int costFromStart;
////		int costToEnd;
////
////		tagNode() : parent(NULL), isOpen(false), isClosed(false),
////			costTotal(MAX), costFromStart(MAX), costToEnd(MAX) {}
////		//생성자로 초기화 할 때 변수는 못넣는데 상수는 가능하네
////		//MAX를 const int 로 만들어야한다
////
////		void calculateTotal() { costTotal = costFromStart + costToEnd; }
////	}NODE;
////
////	//맵의 타일대로 노드 생성
////	//노드는 같은 인덱스번호인 타일의 메모장이라고 보면 된다
////	//부모노드, 열림여부, 닫힘여부, F, G, H
////	NODE node[TILEX][TILEY];
////	//노드 초기화는 멤버 이니셜라이저 사용했으니 생략
////
////	//시작점 노드 초기화
////	node[startIdX][startIdY].parent = NULL;
////	node[startIdX][startIdY].isOpen = true;
////	node[startIdX][startIdY].isClosed = true;
////	node[startIdX][startIdY].costFromStart = 0;
////	node[startIdX][startIdY].costToEnd = getHeuristic(startIdX, startIdY, endIdX, endIdY);
////	node[startIdX][startIdY].calculateTotal();
////
////	//열린목록 생성
////	deque<tile*> vOpen;
////
////	//시작점과 끝점 설정, 현재점은 시작점으로
////	tile* startTile = OBJECTMANAGER->_tile[startIdX][startIdY];
////	tile* endTile = OBJECTMANAGER->_tile[endIdX][endIdY];
////	tile* current = startTile;
////
////	/*----------------------------------------------------------
////	위쪽은 변수선언 및 변수 초기화
////	아래부터 로직
////	----------------------------------------------------------*/
////	bool eightDirection = true;
////
////	//에이스타 길찾는 루프
////	while (true)
////	{
////		if (current == NULL) break;   //안전장치
////
////		int curX = current->getIndexX();
////		int curY = current->getIndexY();
////
////		//현재점 중심 9타일 탐색
////		for (int j = curY - 1; j <= curY + 1; ++j)
////		{
////			if (j < 0 || j >= TILEY) continue;
////			for (int i = curX - 1; i <= curX + 1; ++i)
////			{
////				if (i < 0 || i >= TILEX) continue;
////
////				//자기 자신은 continue
////				if (i == curX && j == curY) continue;
////				
////				//사방을 예외처리해준다
////				if (i == curX || j == curY)continue;
////				
////				//8방향 이동 불가능하면 (4방향 이동만 가능하면) 상하좌우 아니면 continue
////				if (!eightDirection)
////				{
////					if (!(i == curX && j == curY - 1 ||
////						i == curX && j == curY + 1 ||
////						i == curX - 1 && j == curY ||
////						i == curX + 1 && j == curY)) continue;
////				}
////				else   //8방향 이동에서 직선방향 장애물이면 그 옆에 대각선 막아주기
////				{
////					if (curX - 1 >= 0 && !OBJECTMANAGER->_tile[curX - 1][curY]->getIsMovable())
////						if (i == curX - 1 && j != curY) continue;
////					if (curX + 1 < TILEX && !OBJECTMANAGER->_tile[curX + 1][curY]->getIsMovable())
////						if (i == curX + 1 && j != curY) continue;
////					if (curY - 1 >= 0 && !OBJECTMANAGER->_tile[curX][curY - 1]->getIsMovable())
////						if (j == curY - 1 && i != curX) continue;
////					if (curY + 1 < TILEY && !OBJECTMANAGER->_tile[curX][curY + 1]->getIsMovable())
////						if (j == curY + 1 && i != curX) continue;
////
////				}
////
////				//닫혀있으면 continue
////				if (node[i][j].isClosed) continue;
////				//장애물이면 continue
////				if (!OBJECTMANAGER->_tile[i][j]->getIsMovable())
////				{
////					node[i][j].isOpen = true;
////					node[i][j].isClosed = true;
////					continue;
////				}
////
////				//처음 간 노드인 상황
////				if (!node[i][j].isOpen)
////				{
////					node[i][j].isOpen = true;
////					node[i][j].parent = current;
////					node[i][j].costFromStart = node[curX][curY].costFromStart +
////						getDistance(current->_pos.x, current->_pos.y,
////							OBJECTMANAGER->_tile[i][j]->_pos.x, OBJECTMANAGER->_tile[i][j]->_pos.y);
////					node[i][j].costToEnd = getHeuristic(i, j, endIdX, endIdY);
////					node[i][j].calculateTotal();
////
////					vOpen.push_back(OBJECTMANAGER->_tile[i][j]);
////				}
////				//가봤던 노드인 상황
////				else
////				{
////					//휴리스틱 값은 변하지 않는다
////					//costFromStart만 비교해서 더 작으면 작은것
////					int newG = node[curX][curY].costFromStart +
////						getDistance(current->_pos.x, current->_pos.y,
////							OBJECTMANAGER->_tile[i][j]->_pos.x, OBJECTMANAGER->_tile[i][j]->_pos.y);
////					if (newG < node[i][j].costFromStart)
////					{
////						node[i][j].costFromStart = newG;
////						node[i][j].calculateTotal();
////						node[i][j].parent = current;
////					}
////				}
////			}
////		}
////
////		//9타일 탐색은 끝났고 이제 열린목록에서 costTotal 최소인 곳 선택
////		tile* tileMin = NULL;
////		for (int i = 0; i < vOpen.size(); ++i)
////		{
////			if (vOpen[i] == current)
////			{
////				vOpen.erase(vOpen.begin() + i--);
////				continue;
////			}
////			if (tileMin == NULL)
////			{
////				tileMin = vOpen[i];
////				continue;
////			}
////			if (node[vOpen[i]->getIndexX()][vOpen[i]->getIndexY()].costTotal <
////				node[tileMin->getIndexX()][tileMin->getIndexY()].costTotal)
////			{
////				tileMin = vOpen[i];
////			}
////		}
////		//선택타일 닫힘여부 true로 하고 선택타일 열린목록에서 제거
////		if (tileMin != NULL)
////		{
////			node[tileMin->getIndexX()][tileMin->getIndexY()].isClosed = true;
////
////			//선택타일을 다음 current로 설정
////			current = tileMin;
////		}
////		else   //선택타일이 없다 == 열린목록이 비어있다 == 갈 수 있는 길이 없다
////		{
////			break;   //길찾기 루프 종료시킨다
////		}
////
////		// <종료조건> 선택타일이 목표한 끝점인 상황
////		if (tileMin == endTile)
////		{
////			deque<tile*> solution;
////			deque<tile*> solutionReversed;
////			tile* tempTile = current;
////
////			//선택타일부터 부모 따라가면서 solutionReversed에 추가
////			solutionReversed.push_back(tempTile);
////			while (node[tempTile->getIndexX()][tempTile->getIndexY()].parent != NULL)
////			{
////				tempTile = node[tempTile->getIndexX()][tempTile->getIndexY()].parent;
////				solutionReversed.push_back(tempTile);
////			}
////
////			//솔루션리버스 뒤집어서 솔루션에 집어넣고 반환
////			for (int i = solutionReversed.size() - 1; i >= 0; --i)
////			{
////				solution.push_back(solutionReversed[i]);
////			}
////			return solution;
////		}
////		// <>
////	}
////
////	//길찾기 루프가 비정상적으로 종료된 상황
////	//빈 벡터 반환으로 끝냄
////	return deque<tile*>();
////}
////
////
//////=============================================================================
////// ## 8방향 astar ##
//////=============================================================================
////
////deque<tile*> aStarManager::getEightDirectionPath(int startIdX, int startIdY, int endIdX, int endIdY)
////{
////	//시작점 끝점이 같으면 빈 덱을 반환한다. 
////	if (startIdX == endIdX && startIdY == endIdY)return deque<tile*>();
////	//시작점과 끝점이 타일 범위를 벗어나도 빈덱을 반환한다.
////	if (!(0 <= startIdX && startIdX < TILEX && 0 <= startIdY && startIdY < TILEY &&
////		0 <= endIdX && endIdX < TILEX && 0 <= endIdY && endIdY < TILEY))return deque<tile*>();
////	//최댓값 상수 정의 (비용 초기화에 쓰인다) 그렇다고 한다. 
////	const int MAX = 0x0fffffff;
////
////	//노드 구조체 정의
////	typedef struct tagNode
////	{
////		tile *parent;
////		bool isOpen;
////		bool isClosed;
////		int costTotal;
////		int costFromStart;
////		int costToEnd;
////
////		tagNode() : parent(NULL), isOpen(false), isClosed(false),
////			costTotal(MAX), costFromStart(MAX), costToEnd(MAX) {}
////		//생성자로 초기화 할 때 변수는 못넣는데 상수는 가능하네
////		//MAX를 const int 로 만들어야한다
////
////		void calculateTotal() { costTotal = costFromStart + costToEnd; }
////	}NODE;
////
////	//맵의 타일대로 노드 생성
////	//노드는 같은 인덱스번호인 타일의 메모장이라고 보면 된다
////	//부모노드, 열림여부, 닫힘여부, F, G, H
////	NODE node[TILEX][TILEY];
////	//노드 초기화는 멤버 이니셜라이저 사용했으니 생략
////
////	//시작점 노드 초기화
////	node[startIdX][startIdY].parent = NULL;
////	node[startIdX][startIdY].isOpen = true;
////	node[startIdX][startIdY].isClosed = true;
////	node[startIdX][startIdY].costFromStart = 0;
////	node[startIdX][startIdY].costToEnd = getHeuristic(startIdX, startIdY, endIdX, endIdY);
////	node[startIdX][startIdY].calculateTotal();
////
////	//열린목록 생성
////	deque<tile*> vOpen;
////
////	//시작점과 끝점 설정, 현재점은 시작점으로
////	tile* startTile = OBJECTMANAGER->_tile[startIdX][startIdY];
////	tile* endTile = OBJECTMANAGER->_tile[endIdX][endIdY];
////	tile* current = startTile;
////
////	/*----------------------------------------------------------
////	위쪽은 변수선언 및 변수 초기화
////	아래부터 로직
////	----------------------------------------------------------*/
////	bool eightDirection = true;
////
////	//에이스타 길찾는 루프
////	while (true)
////	{
////		if (current == NULL) break;   //안전장치
////
////		int curX = current->getIndexX();
////		int curY = current->getIndexY();
////
////		//현재점 중심 9타일 탐색
////		for (int j = curY - 1; j <= curY + 1; ++j)
////		{
////			if (j < 0 || j >= TILEY) continue;
////			for (int i = curX - 1; i <= curX + 1; ++i)
////			{
////				if (i < 0 || i >= TILEX) continue;
////
////				//자기 자신은 continue
////				if (i == curX && j == curY) continue;
////
////				//8방향 이동 불가능하면 (4방향 이동만 가능하면) 상하좌우 아니면 continue
////				if (!eightDirection)
////				{
////					if (!(i == curX && j == curY - 1 ||
////						i == curX && j == curY + 1 ||
////						i == curX - 1 && j == curY ||
////						i == curX + 1 && j == curY)) continue;
////				}
////				else   //8방향 이동에서 직선방향 장애물이면 그 옆에 대각선 막아주기
////				{
////					if (curX - 1 >= 0 && !OBJECTMANAGER->_tile[curX - 1][curY]->getIsMovable())
////						if (i == curX - 1 && j != curY) continue;
////					if (curX + 1 < TILEX && !OBJECTMANAGER->_tile[curX + 1][curY]->getIsMovable())
////						if (i == curX + 1 && j != curY) continue;
////					if (curY - 1 >= 0 && !OBJECTMANAGER->_tile[curX][curY - 1]->getIsMovable())
////						if (j == curY - 1 && i != curX) continue;
////					if (curY + 1 < TILEY && !OBJECTMANAGER->_tile[curX][curY + 1]->getIsMovable())
////						if (j == curY + 1 && i != curX) continue;
////				}
////
////				//닫혀있으면 continue
////				if (node[i][j].isClosed) continue;
////				//장애물이면 continue
////				if (!OBJECTMANAGER->_tile[i][j]->getIsMovable())
////				{
////					node[i][j].isOpen = true;
////					node[i][j].isClosed = true;
////					continue;
////				}
////
////				//처음 간 노드인 상황
////				if (!node[i][j].isOpen)
////				{
////					node[i][j].isOpen = true;
////					node[i][j].parent = current;
////					node[i][j].costFromStart = node[curX][curY].costFromStart +
////						getDistance(current->_pos.x, current->_pos.y,
////							OBJECTMANAGER->_tile[i][j]->_pos.x, OBJECTMANAGER->_tile[i][j]->_pos.y);
////					node[i][j].costToEnd = getHeuristic(i, j, endIdX, endIdY);
////					node[i][j].calculateTotal();
////
////					vOpen.push_back(OBJECTMANAGER->_tile[i][j]);
////				}
////				//가봤던 노드인 상황
////				else
////				{
////					//휴리스틱 값은 변하지 않는다
////					//costFromStart만 비교해서 더 작으면 작은것
////					int newG = node[curX][curY].costFromStart +
////						getDistance(current->_pos.x, current->_pos.y,
////							OBJECTMANAGER->_tile[i][j]->_pos.x, OBJECTMANAGER->_tile[i][j]->_pos.y);
////					if (newG < node[i][j].costFromStart)
////					{
////						node[i][j].costFromStart = newG;
////						node[i][j].calculateTotal();
////						node[i][j].parent = current;
////					}
////				}
////			}
////		}
////
////		//9타일 탐색은 끝났고 이제 열린목록에서 costTotal 최소인 곳 선택
////		tile* tileMin = NULL;
////		for (int i = 0; i < vOpen.size(); ++i)
////		{
////			if (vOpen[i] == current)
////			{
////				vOpen.erase(vOpen.begin() + i--);
////				continue;
////			}
////			if (tileMin == NULL)
////			{
////				tileMin = vOpen[i];
////				continue;
////			}
////			if (node[vOpen[i]->getIndexX()][vOpen[i]->getIndexY()].costTotal <
////				node[tileMin->getIndexX()][tileMin->getIndexY()].costTotal)
////			{
////				tileMin = vOpen[i];
////			}
////		}
////		//선택타일 닫힘여부 true로 하고 선택타일 열린목록에서 제거
////		if (tileMin != NULL)
////		{
////			node[tileMin->getIndexX()][tileMin->getIndexY()].isClosed = true;
////
////			//열린목록 돌려서 tileMin과 같은거 찾아서 제거하는 과정은
////			//다음 선택 과정에서 하면 불필요한 포문 한번을 없앨 수 있다
////			//for (int i = 0; i < vOpen.size(); ++i)
////			//{
////			//	if (vOpen[i] == tileMin)
////			//	{
////			//		vOpen.erase(vOpen.begin() + i);
////			//		break;
////			//	}
////			//}
////
////			//선택타일을 다음 current로 설정
////			current = tileMin;
////		}
////		else   //선택타일이 없다 == 열린목록이 비어있다 == 갈 수 있는 길이 없다
////		{
////			break;   //길찾기 루프 종료시킨다
////		}
////
////		// <종료조건> 선택타일이 목표한 끝점인 상황
////		if (tileMin == endTile)
////		{
////			deque<tile*> solution;
////			deque<tile*> solutionReversed;
////			tile* tempTile = current;
////
////			//선택타일부터 부모 따라가면서 solutionReversed에 추가
////			solutionReversed.push_back(tempTile);
////			while (node[tempTile->getIndexX()][tempTile->getIndexY()].parent != NULL)
////			{
////				tempTile = node[tempTile->getIndexX()][tempTile->getIndexY()].parent;
////				solutionReversed.push_back(tempTile);
////			}
////
////			//솔루션리버스 뒤집어서 솔루션에 집어넣고 반환
////			for (int i = solutionReversed.size() - 1; i >= 0; --i)
////			{
////				solution.push_back(solutionReversed[i]);
////			}
////			return solution;
////		}
////		// <>
////	}
////
////	//길찾기 루프가 비정상적으로 종료된 상황
////	//빈 벡터 반환으로 끝냄
////	return deque<tile*>();
////
////}