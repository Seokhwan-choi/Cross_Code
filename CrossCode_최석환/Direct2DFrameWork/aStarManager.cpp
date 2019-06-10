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
//	//������ ������ ������ �� ���� ��ȯ�Ѵ�. 
//	if (startIdX == endIdX && startIdY == endIdY)return deque<tile*>();
//	//�������� ������ Ÿ�� ������ ����� ���� ��ȯ�Ѵ�.
//	if (!(0 <= startIdX && startIdX < 100 && 0 <= startIdY && startIdY < 100 &&
//		0 <= endIdX && endIdX < 100 && 0 <= endIdY && endIdY < 100))return deque<tile*>();
//	//�ִ� ��� ���� (��� �ʱ�ȭ�� ���δ�) �׷��ٰ� �Ѵ�. 
//	const int MAX = 0x0fffffff;
//	
//	//��� ����ü ����
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
//		//�����ڷ� �ʱ�ȭ �� �� ������ ���ִµ� ����� �����ϳ�
//		//MAX�� const int �� �������Ѵ�
//
//		void calculateTotal() { costTotal = costFromStart + costToEnd; }
//	}NODE;
//
//	//���� Ÿ�ϴ�� ��� ����
//	//���� ���� �ε�����ȣ�� Ÿ���� �޸����̶�� ���� �ȴ�
//	//�θ���, ��������, ��������, F, G, H
//	NODE node[100][100];
//	//��� �ʱ�ȭ�� ��� �̴ϼȶ����� ��������� ����
//
//	//������ ��� �ʱ�ȭ
//	node[startIdX][startIdY].parent = NULL;
//	node[startIdX][startIdY].isOpen = true;
//	node[startIdX][startIdY].isClosed = true;
//	node[startIdX][startIdY].costFromStart = 0;
//	node[startIdX][startIdY].costToEnd = getHeuristic(startIdX, startIdY, endIdX, endIdY);
//	node[startIdX][startIdY].calculateTotal();
//	
//	//������� ����
//	deque<tile*> vOpen;
//
//	//�������� ���� ����, �������� ����������
//	tile* startTile = SCENEMANAGER->GetCurrent()->GetTile(startIdX, startIdY);
//	tile* endTile = SCENEMANAGER->GetCurrent()->GetTile(endIdX, endIdY);
//	tile* current = startTile;
//
//	/*----------------------------------------------------------
//	������ �������� �� ���� �ʱ�ȭ
//	�Ʒ����� ����
//	----------------------------------------------------------*/
//	bool eightDirection = false;
//
//	//���̽�Ÿ ��ã�� ����
//	while (true)
//	{
//		if (current == NULL) break;   //������ġ
//
//		int curX = current->getIdX();
//		int curY = current->getIdY();
//
//		//������ �߽� 9Ÿ�� Ž��
//		for (int j = curY - 1; j <= curY + 1; ++j)
//		{
//			if (j < 0 || j >= 100) continue;
//			for (int i = curX - 1; i <= curX + 1; ++i)
//			{
//				if (i < 0 || i >= 100) continue;
//
//				//�ڱ� �ڽ��� continue
//				if (i == curX && j == curY) continue;
//
//				//8���� �̵� �Ұ����ϸ� (4���� �̵��� �����ϸ�) �����¿� �ƴϸ� continue
//				if (!eightDirection)
//				{
//					if (!(i == curX && j == curY - 1 ||
//						i == curX && j == curY + 1 ||
//						i == curX - 1 && j == curY ||
//						i == curX + 1 && j == curY)) continue;
//				}
//				else   //8���� �̵����� �������� ��ֹ��̸� �� ���� �밢�� �����ֱ�
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
//				//���������� continue
//				if (node[i][j].isClosed) continue;
//				//��ֹ��̸� continue
//				if (SCENEMANAGER->GetCurrent()->GetTile(i, j)->getAttribute() == "Wall")
//				{
//					node[i][j].isOpen = true;
//					node[i][j].isClosed = true;
//					continue;
//				}
//
//				//ó�� �� ����� ��Ȳ
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
//				//���ô� ����� ��Ȳ
//				else
//				{
//					//�޸���ƽ ���� ������ �ʴ´�
//					//costFromStart�� ���ؼ� �� ������ ������
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
//		//9Ÿ�� Ž���� ������ ���� ������Ͽ��� costTotal �ּ��� �� ����
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
//		//����Ÿ�� �������� true�� �ϰ� ����Ÿ�� ������Ͽ��� ����
//		if (tileMin != NULL)
//		{
//			node[tileMin->getIdX()][tileMin->getIdY()].isClosed = true;
//
//			//������� ������ tileMin�� ������ ã�Ƽ� �����ϴ� ������
//			//���� ���� �������� �ϸ� ���ʿ��� ���� �ѹ��� ���� �� �ִ�
//			//for (int i = 0; i < vOpen.size(); ++i)
//			//{
//			//	if (vOpen[i] == tileMin)
//			//	{
//			//		vOpen.erase(vOpen.begin() + i);
//			//		break;
//			//	}
//			//}
//
//			//����Ÿ���� ���� current�� ����
//			current = tileMin;
//		}
//		else   //����Ÿ���� ���� == ��������� ����ִ� == �� �� �ִ� ���� ����
//		{
//			break;   //��ã�� ���� �����Ų��
//		}
//
//		// <��������> ����Ÿ���� ��ǥ�� ������ ��Ȳ
//		if (tileMin == endTile)
//		{
//			deque<tile*> solution;
//			deque<tile*> solutionReversed;
//			tile* tempTile = current;
//
//			//����Ÿ�Ϻ��� �θ� ���󰡸鼭 solutionReversed�� �߰�
//			solutionReversed.push_back(tempTile);
//			while (node[tempTile->getIdX()][tempTile->getIdY()].parent != NULL)
//			{
//				tempTile = node[tempTile->getIdX()][tempTile->getIdY()].parent;
//				solutionReversed.push_back(tempTile);
//			}
//
//			//�ַ�Ǹ����� ����� �ַ�ǿ� ����ְ� ��ȯ
//			for (int i = solutionReversed.size() - 1; i >= 0; --i)
//			{
//				solution.push_back(solutionReversed[i]);
//			}
//			return solution;
//		}
//		// <>
//	}
//
//	//��ã�� ������ ������������ ����� ��Ȳ
//	//�� ���� ��ȯ���� ����
//	return deque<tile*>();
//}
//
//
//
////=========================================================================================
//
////## ��ֹ� ������� ���� �������� ã���ش� ##
//
////=========================================================================================
//
////deque<tile*> aStarManager::getShortPath(int startIdX, int startIdY, int endIdX, int endIdY)
////{
////	//������ ������ ������ �� ���� ��ȯ�Ѵ�. 
////	if (startIdX == endIdX && startIdY == endIdY)return deque<tile*>();
////	//�������� ������ Ÿ�� ������ ����� ���� ��ȯ�Ѵ�.
////	if (!(0 <= startIdX && startIdX < TILEX && 0 <= startIdY && startIdY < TILEY &&
////		0 <= endIdX && endIdX < TILEX && 0 <= endIdY && endIdY < TILEY))return deque<tile*>();
////	//�ִ� ��� ���� (��� �ʱ�ȭ�� ���δ�) �׷��ٰ� �Ѵ�. 
////	const int MAX = 0x0fffffff;
////
////	//��� ����ü ����
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
////		//�����ڷ� �ʱ�ȭ �� �� ������ ���ִµ� ����� �����ϳ�
////		//MAX�� const int �� �������Ѵ�
////
////		void calculateTotal() { costTotal = costFromStart + costToEnd; }
////	}NODE;
////
////	//���� Ÿ�ϴ�� ��� ����
////	//���� ���� �ε�����ȣ�� Ÿ���� �޸����̶�� ���� �ȴ�
////	//�θ���, ��������, ��������, F, G, H
////	NODE node[TILEX][TILEY];
////	//��� �ʱ�ȭ�� ��� �̴ϼȶ����� ��������� ����
////
////	//������ ��� �ʱ�ȭ
////	node[startIdX][startIdY].parent = NULL;
////	node[startIdX][startIdY].isOpen = true;
////	node[startIdX][startIdY].isClosed = true;
////	node[startIdX][startIdY].costFromStart = 0;
////	node[startIdX][startIdY].costToEnd = getHeuristic(startIdX, startIdY, endIdX, endIdY);
////	node[startIdX][startIdY].calculateTotal();
////
////	//������� ����
////	deque<tile*> vOpen;
////
////	//�������� ���� ����, �������� ����������
////	tile* startTile = OBJECTMANAGER->_tile[startIdX][startIdY];
////	tile* endTile = OBJECTMANAGER->_tile[endIdX][endIdY];
////	tile* current = startTile;
////
////	/*----------------------------------------------------------
////	������ �������� �� ���� �ʱ�ȭ
////	�Ʒ����� ����
////	----------------------------------------------------------*/
////	bool eightDirection = false;
////
////	//���̽�Ÿ ��ã�� ����
////	while (true)
////	{
////		if (current == NULL) break;   //������ġ
////
////		int curX = current->getIndexX();
////		int curY = current->getIndexY();
////
////		//������ �߽� 9Ÿ�� Ž��
////		for (int j = curY - 1; j <= curY + 1; ++j)
////		{
////			if (j < 0 || j >= TILEY) continue;
////			for (int i = curX - 1; i <= curX + 1; ++i)
////			{
////				if (i < 0 || i >= TILEX) continue;
////
////				//�ڱ� �ڽ��� continue
////				if (i == curX && j == curY) continue;
////
////				//8���� �̵� �Ұ����ϸ� (4���� �̵��� �����ϸ�) �����¿� �ƴϸ� continue
////				if (!eightDirection)
////				{
////					if (!(i == curX && j == curY - 1 ||
////						i == curX && j == curY + 1 ||
////						i == curX - 1 && j == curY ||
////						i == curX + 1 && j == curY)) continue;
////				}
////				else   //8���� �̵����� �������� ��ֹ��̸� �� ���� �밢�� �����ֱ�
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
////				//���������� continue
////				if (node[i][j].isClosed) continue;
////				//��ֹ��̸� continue
////				//if (!OBJECTMANAGER->_tile[i][j]->getIsMovable())
////				//{
////				//	node[i][j].isOpen = true;
////				//	node[i][j].isClosed = true;
////				//	continue;
////				//}
////
////				//ó�� �� ����� ��Ȳ
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
////				//���ô� ����� ��Ȳ
////				else
////				{
////					//�޸���ƽ ���� ������ �ʴ´�
////					//costFromStart�� ���ؼ� �� ������ ������
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
////		//9Ÿ�� Ž���� ������ ���� ������Ͽ��� costTotal �ּ��� �� ����
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
////		//����Ÿ�� �������� true�� �ϰ� ����Ÿ�� ������Ͽ��� ����
////		if (tileMin != NULL)
////		{
////			node[tileMin->getIndexX()][tileMin->getIndexY()].isClosed = true;
////
////		
////
////			//����Ÿ���� ���� current�� ����
////			current = tileMin;
////		}
////		else   //����Ÿ���� ���� == ��������� ����ִ� == �� �� �ִ� ���� ����
////		{
////			break;   //��ã�� ���� �����Ų��
////		}
////
////		// <��������> ����Ÿ���� ��ǥ�� ������ ��Ȳ
////		if (tileMin == endTile)
////		{
////			deque<tile*> solution;
////			deque<tile*> solutionReversed;
////			tile* tempTile = current;
////
////			//����Ÿ�Ϻ��� �θ� ���󰡸鼭 solutionReversed�� �߰�
////			solutionReversed.push_back(tempTile);
////			while (node[tempTile->getIndexX()][tempTile->getIndexY()].parent != NULL)
////			{
////				tempTile = node[tempTile->getIndexX()][tempTile->getIndexY()].parent;
////				solutionReversed.push_back(tempTile);
////			}
////
////			//�ַ�Ǹ����� ����� �ַ�ǿ� ����ְ� ��ȯ
////			for (int i = solutionReversed.size() - 1; i >= 0; --i)
////			{
////				solution.push_back(solutionReversed[i]);
////			}
////			return solution;
////		}
////		// <>
////	}
////
////	//��ã�� ������ ������������ ����� ��Ȳ
////	//�� ���� ��ȯ���� ����
////	return deque<tile*>();
////}
////
//////=========================================================================================
//////##�밢�� ���̽�Ÿ##
//////==========================================================================================
////
////deque<tile*> aStarManager::getCrossPath(int startIdX, int startIdY, int endIdX, int endIdY)
////{
////	//������ ������ ������ �� ���� ��ȯ�Ѵ�. 
////	if (startIdX == endIdX && startIdY == endIdY)return deque<tile*>();
////	//�������� ������ Ÿ�� ������ ����� ���� ��ȯ�Ѵ�.
////	if (!(0 <= startIdX && startIdX < TILEX && 0 <= startIdY && startIdY < TILEY &&
////		0 <= endIdX && endIdX < TILEX && 0 <= endIdY && endIdY < TILEY))return deque<tile*>();
////	//�ִ� ��� ���� (��� �ʱ�ȭ�� ���δ�) �׷��ٰ� �Ѵ�. 
////	const int MAX = 0x0fffffff;
////
////	//��� ����ü ����
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
////		//�����ڷ� �ʱ�ȭ �� �� ������ ���ִµ� ����� �����ϳ�
////		//MAX�� const int �� �������Ѵ�
////
////		void calculateTotal() { costTotal = costFromStart + costToEnd; }
////	}NODE;
////
////	//���� Ÿ�ϴ�� ��� ����
////	//���� ���� �ε�����ȣ�� Ÿ���� �޸����̶�� ���� �ȴ�
////	//�θ���, ��������, ��������, F, G, H
////	NODE node[TILEX][TILEY];
////	//��� �ʱ�ȭ�� ��� �̴ϼȶ����� ��������� ����
////
////	//������ ��� �ʱ�ȭ
////	node[startIdX][startIdY].parent = NULL;
////	node[startIdX][startIdY].isOpen = true;
////	node[startIdX][startIdY].isClosed = true;
////	node[startIdX][startIdY].costFromStart = 0;
////	node[startIdX][startIdY].costToEnd = getHeuristic(startIdX, startIdY, endIdX, endIdY);
////	node[startIdX][startIdY].calculateTotal();
////
////	//������� ����
////	deque<tile*> vOpen;
////
////	//�������� ���� ����, �������� ����������
////	tile* startTile = OBJECTMANAGER->_tile[startIdX][startIdY];
////	tile* endTile = OBJECTMANAGER->_tile[endIdX][endIdY];
////	tile* current = startTile;
////
////	/*----------------------------------------------------------
////	������ �������� �� ���� �ʱ�ȭ
////	�Ʒ����� ����
////	----------------------------------------------------------*/
////	bool eightDirection = true;
////
////	//���̽�Ÿ ��ã�� ����
////	while (true)
////	{
////		if (current == NULL) break;   //������ġ
////
////		int curX = current->getIndexX();
////		int curY = current->getIndexY();
////
////		//������ �߽� 9Ÿ�� Ž��
////		for (int j = curY - 1; j <= curY + 1; ++j)
////		{
////			if (j < 0 || j >= TILEY) continue;
////			for (int i = curX - 1; i <= curX + 1; ++i)
////			{
////				if (i < 0 || i >= TILEX) continue;
////
////				//�ڱ� �ڽ��� continue
////				if (i == curX && j == curY) continue;
////				
////				//����� ����ó�����ش�
////				if (i == curX || j == curY)continue;
////				
////				//8���� �̵� �Ұ����ϸ� (4���� �̵��� �����ϸ�) �����¿� �ƴϸ� continue
////				if (!eightDirection)
////				{
////					if (!(i == curX && j == curY - 1 ||
////						i == curX && j == curY + 1 ||
////						i == curX - 1 && j == curY ||
////						i == curX + 1 && j == curY)) continue;
////				}
////				else   //8���� �̵����� �������� ��ֹ��̸� �� ���� �밢�� �����ֱ�
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
////				//���������� continue
////				if (node[i][j].isClosed) continue;
////				//��ֹ��̸� continue
////				if (!OBJECTMANAGER->_tile[i][j]->getIsMovable())
////				{
////					node[i][j].isOpen = true;
////					node[i][j].isClosed = true;
////					continue;
////				}
////
////				//ó�� �� ����� ��Ȳ
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
////				//���ô� ����� ��Ȳ
////				else
////				{
////					//�޸���ƽ ���� ������ �ʴ´�
////					//costFromStart�� ���ؼ� �� ������ ������
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
////		//9Ÿ�� Ž���� ������ ���� ������Ͽ��� costTotal �ּ��� �� ����
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
////		//����Ÿ�� �������� true�� �ϰ� ����Ÿ�� ������Ͽ��� ����
////		if (tileMin != NULL)
////		{
////			node[tileMin->getIndexX()][tileMin->getIndexY()].isClosed = true;
////
////			//����Ÿ���� ���� current�� ����
////			current = tileMin;
////		}
////		else   //����Ÿ���� ���� == ��������� ����ִ� == �� �� �ִ� ���� ����
////		{
////			break;   //��ã�� ���� �����Ų��
////		}
////
////		// <��������> ����Ÿ���� ��ǥ�� ������ ��Ȳ
////		if (tileMin == endTile)
////		{
////			deque<tile*> solution;
////			deque<tile*> solutionReversed;
////			tile* tempTile = current;
////
////			//����Ÿ�Ϻ��� �θ� ���󰡸鼭 solutionReversed�� �߰�
////			solutionReversed.push_back(tempTile);
////			while (node[tempTile->getIndexX()][tempTile->getIndexY()].parent != NULL)
////			{
////				tempTile = node[tempTile->getIndexX()][tempTile->getIndexY()].parent;
////				solutionReversed.push_back(tempTile);
////			}
////
////			//�ַ�Ǹ����� ����� �ַ�ǿ� ����ְ� ��ȯ
////			for (int i = solutionReversed.size() - 1; i >= 0; --i)
////			{
////				solution.push_back(solutionReversed[i]);
////			}
////			return solution;
////		}
////		// <>
////	}
////
////	//��ã�� ������ ������������ ����� ��Ȳ
////	//�� ���� ��ȯ���� ����
////	return deque<tile*>();
////}
////
////
//////=============================================================================
////// ## 8���� astar ##
//////=============================================================================
////
////deque<tile*> aStarManager::getEightDirectionPath(int startIdX, int startIdY, int endIdX, int endIdY)
////{
////	//������ ������ ������ �� ���� ��ȯ�Ѵ�. 
////	if (startIdX == endIdX && startIdY == endIdY)return deque<tile*>();
////	//�������� ������ Ÿ�� ������ ����� ���� ��ȯ�Ѵ�.
////	if (!(0 <= startIdX && startIdX < TILEX && 0 <= startIdY && startIdY < TILEY &&
////		0 <= endIdX && endIdX < TILEX && 0 <= endIdY && endIdY < TILEY))return deque<tile*>();
////	//�ִ� ��� ���� (��� �ʱ�ȭ�� ���δ�) �׷��ٰ� �Ѵ�. 
////	const int MAX = 0x0fffffff;
////
////	//��� ����ü ����
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
////		//�����ڷ� �ʱ�ȭ �� �� ������ ���ִµ� ����� �����ϳ�
////		//MAX�� const int �� �������Ѵ�
////
////		void calculateTotal() { costTotal = costFromStart + costToEnd; }
////	}NODE;
////
////	//���� Ÿ�ϴ�� ��� ����
////	//���� ���� �ε�����ȣ�� Ÿ���� �޸����̶�� ���� �ȴ�
////	//�θ���, ��������, ��������, F, G, H
////	NODE node[TILEX][TILEY];
////	//��� �ʱ�ȭ�� ��� �̴ϼȶ����� ��������� ����
////
////	//������ ��� �ʱ�ȭ
////	node[startIdX][startIdY].parent = NULL;
////	node[startIdX][startIdY].isOpen = true;
////	node[startIdX][startIdY].isClosed = true;
////	node[startIdX][startIdY].costFromStart = 0;
////	node[startIdX][startIdY].costToEnd = getHeuristic(startIdX, startIdY, endIdX, endIdY);
////	node[startIdX][startIdY].calculateTotal();
////
////	//������� ����
////	deque<tile*> vOpen;
////
////	//�������� ���� ����, �������� ����������
////	tile* startTile = OBJECTMANAGER->_tile[startIdX][startIdY];
////	tile* endTile = OBJECTMANAGER->_tile[endIdX][endIdY];
////	tile* current = startTile;
////
////	/*----------------------------------------------------------
////	������ �������� �� ���� �ʱ�ȭ
////	�Ʒ����� ����
////	----------------------------------------------------------*/
////	bool eightDirection = true;
////
////	//���̽�Ÿ ��ã�� ����
////	while (true)
////	{
////		if (current == NULL) break;   //������ġ
////
////		int curX = current->getIndexX();
////		int curY = current->getIndexY();
////
////		//������ �߽� 9Ÿ�� Ž��
////		for (int j = curY - 1; j <= curY + 1; ++j)
////		{
////			if (j < 0 || j >= TILEY) continue;
////			for (int i = curX - 1; i <= curX + 1; ++i)
////			{
////				if (i < 0 || i >= TILEX) continue;
////
////				//�ڱ� �ڽ��� continue
////				if (i == curX && j == curY) continue;
////
////				//8���� �̵� �Ұ����ϸ� (4���� �̵��� �����ϸ�) �����¿� �ƴϸ� continue
////				if (!eightDirection)
////				{
////					if (!(i == curX && j == curY - 1 ||
////						i == curX && j == curY + 1 ||
////						i == curX - 1 && j == curY ||
////						i == curX + 1 && j == curY)) continue;
////				}
////				else   //8���� �̵����� �������� ��ֹ��̸� �� ���� �밢�� �����ֱ�
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
////				//���������� continue
////				if (node[i][j].isClosed) continue;
////				//��ֹ��̸� continue
////				if (!OBJECTMANAGER->_tile[i][j]->getIsMovable())
////				{
////					node[i][j].isOpen = true;
////					node[i][j].isClosed = true;
////					continue;
////				}
////
////				//ó�� �� ����� ��Ȳ
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
////				//���ô� ����� ��Ȳ
////				else
////				{
////					//�޸���ƽ ���� ������ �ʴ´�
////					//costFromStart�� ���ؼ� �� ������ ������
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
////		//9Ÿ�� Ž���� ������ ���� ������Ͽ��� costTotal �ּ��� �� ����
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
////		//����Ÿ�� �������� true�� �ϰ� ����Ÿ�� ������Ͽ��� ����
////		if (tileMin != NULL)
////		{
////			node[tileMin->getIndexX()][tileMin->getIndexY()].isClosed = true;
////
////			//������� ������ tileMin�� ������ ã�Ƽ� �����ϴ� ������
////			//���� ���� �������� �ϸ� ���ʿ��� ���� �ѹ��� ���� �� �ִ�
////			//for (int i = 0; i < vOpen.size(); ++i)
////			//{
////			//	if (vOpen[i] == tileMin)
////			//	{
////			//		vOpen.erase(vOpen.begin() + i);
////			//		break;
////			//	}
////			//}
////
////			//����Ÿ���� ���� current�� ����
////			current = tileMin;
////		}
////		else   //����Ÿ���� ���� == ��������� ����ִ� == �� �� �ִ� ���� ����
////		{
////			break;   //��ã�� ���� �����Ų��
////		}
////
////		// <��������> ����Ÿ���� ��ǥ�� ������ ��Ȳ
////		if (tileMin == endTile)
////		{
////			deque<tile*> solution;
////			deque<tile*> solutionReversed;
////			tile* tempTile = current;
////
////			//����Ÿ�Ϻ��� �θ� ���󰡸鼭 solutionReversed�� �߰�
////			solutionReversed.push_back(tempTile);
////			while (node[tempTile->getIndexX()][tempTile->getIndexY()].parent != NULL)
////			{
////				tempTile = node[tempTile->getIndexX()][tempTile->getIndexY()].parent;
////				solutionReversed.push_back(tempTile);
////			}
////
////			//�ַ�Ǹ����� ����� �ַ�ǿ� ����ְ� ��ȯ
////			for (int i = solutionReversed.size() - 1; i >= 0; --i)
////			{
////				solution.push_back(solutionReversed[i]);
////			}
////			return solution;
////		}
////		// <>
////	}
////
////	//��ã�� ������ ������������ ����� ��Ȳ
////	//�� ���� ��ȯ���� ����
////	return deque<tile*>();
////
////}