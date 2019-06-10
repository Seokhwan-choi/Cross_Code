#pragma once
#include "GameNode.h"
class GameObject : public GameNode
{
protected:
	RECT _rc;					// 오브젝트 RECT
	string _name;				// 오브젝트 이름
	Vector2 _size;				// 오브젝트 크기 width, height
	Vector2 _position;			// 오브젝트 좌표 x, y
	bool _isActive;				// 오브젝트 활성화 여부
	bool _isLive;				// 오브젝트 Live 여부
	Pivot _pivot;				// 오브젝트 RECT 기준 점

public:
	virtual void Init() = 0 ;
	virtual void Update() = 0;
	virtual void Render() = 0;
	virtual void Release() = 0;

	// =============================================================
	// ############### 게임 오브젝트 Get / Set ######################
	// =============================================================
	void SetName(const string& name) { _name = name; }
	void SetisActive(const bool& isActive) { _isActive = isActive; }
	void SetisLive(const bool& isLive) { _isLive = isLive; }
	void SetSize(const Vector2& size);
	void SetPosition(const Vector2& position);
	void SetPivot(const Pivot& pivot);
	void SetRect(const RECT& rect);

	 string GetName() const { return _name; }
	 Vector2 GetSize() const { return _size; }
	 Vector2 GetPosition() const { return _position; }
	 bool GetisActive() const { return _isActive; }
	 bool GetisLive() const { return _isLive; }
	 Pivot GetPivot() const { return _pivot; }
	 RECT GetRect() const { return _rc; }
	// =============================================================
	// ############### 게임 오브젝트 Get / Set ######################
	// =============================================================

	void UpdateRect();
	void Destroy() { _isLive = false; }
	Vector2 GetRelativePos(Vector2 pos);

	GameObject();
	GameObject(Vector2 pos);
	GameObject(const string& name, const Vector2& size, const Vector2& position, const Pivot& pivot);
	virtual ~GameObject() {}
};

