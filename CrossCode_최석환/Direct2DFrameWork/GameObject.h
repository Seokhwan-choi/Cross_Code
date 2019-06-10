#pragma once
#include "GameNode.h"
class GameObject : public GameNode
{
protected:
	RECT _rc;					// ������Ʈ RECT
	string _name;				// ������Ʈ �̸�
	Vector2 _size;				// ������Ʈ ũ�� width, height
	Vector2 _position;			// ������Ʈ ��ǥ x, y
	bool _isActive;				// ������Ʈ Ȱ��ȭ ����
	bool _isLive;				// ������Ʈ Live ����
	Pivot _pivot;				// ������Ʈ RECT ���� ��

public:
	virtual void Init() = 0 ;
	virtual void Update() = 0;
	virtual void Render() = 0;
	virtual void Release() = 0;

	// =============================================================
	// ############### ���� ������Ʈ Get / Set ######################
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
	// ############### ���� ������Ʈ Get / Set ######################
	// =============================================================

	void UpdateRect();
	void Destroy() { _isLive = false; }
	Vector2 GetRelativePos(Vector2 pos);

	GameObject();
	GameObject(Vector2 pos);
	GameObject(const string& name, const Vector2& size, const Vector2& position, const Pivot& pivot);
	virtual ~GameObject() {}
};

