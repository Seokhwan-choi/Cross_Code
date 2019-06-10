//#pragma once
//#include "GameObject.h"
//
//enum class STATE : int 
//{
//	LEFT, RIGHT, UP, DOWN, IDLE
//};
//
//class tile : public GameObject
//{
//private:
//	int _idX;
//	int _idY;
//
//	POINT _center;
//	RECT _rc;
//
//	float _totalCost;		//총 비용 : f = g + h
//	float _costFromStart;	//시작점부터 현재 노드까지 경로비용 - g
//	float _costToGoal;		//현재 노드부터 끝점가지의 경로비용 - h
//
//	bool _isOpen;
//
//	tile* _parentNode;
//
//	COLORREF _color;
//	HBRUSH _brush;
//	HPEN _pen;
//
//	string _attribute;
//
//public:
//	void Init(int idX, int idY);
//	void Init();
//	void Release(void);
//	void Update(void);
//	void Render(void);
//
//	//접근자, 설정자(겟터, 셋터)
//	int getIdX(void) { return _idX; }
//	int getIdY(void) { return _idY; }
//
//	void setColor(COLORREF color)
//	{
//		DeleteObject(_brush);
//		_color = color;
//		_brush = CreateSolidBrush(_color);
//	}
//
//	RECT getRect(void) { return _rc; }
//
//	void setCenter(POINT center) { _center = center; }
//	POINT getCenter(void) { return _center; }
//
//	void setAttribute(string str) { _attribute = str; }
//	string getAttribute(void) { return _attribute; }
//
//	void setTotalCost(float totalCost) { _totalCost = totalCost; }
//	float getTotalCost(void) { return _totalCost; }
//
//	void setCostFromStart(float costFromStart) { _costFromStart = costFromStart; }
//	float getCostFromStart(void) { return _costFromStart; }
//
//	void setCostToGoal(float costToGoal) { _costToGoal = costToGoal; }
//	float getCostToGoal(void) { return _costToGoal; }
//
//	void setParentNode(tile* t) { _parentNode = t; }
//	tile* getParentNode(void) { return _parentNode; }
//
//	void setIsOpen(bool isOpen) { _isOpen = isOpen; }
//	bool getIsOpen(void) { return _isOpen; }
//
//
//	tile() : _totalCost(0), _costFromStart(0), _costToGoal(0), _parentNode(NULL), _idX(0), _idY(0) {}
//	~tile() {}
//};