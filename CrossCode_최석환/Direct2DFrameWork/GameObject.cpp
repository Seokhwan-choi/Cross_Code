#include "stdafx.h"
#include "GameObject.h"

void GameObject::UpdateRect()
{
	_rc = RectMakePivot(_position, _size, _pivot);
}

Vector2 GameObject::GetRelativePos(Vector2 pos)
{
	return Vector2(_position - pos);
}

GameObject::GameObject()
	: _name("None"), _position(Vector2(WINSIZEX/2, WINSIZEY/2)),
	_size(Vector2(64,64)), _pivot(Pivot::Center)
	,_isActive(true), _isLive(true)
{
	this->UpdateRect();
}

GameObject::GameObject(Vector2 pos)
	: _name("None"), _position(pos),
	_size(Vector2(64, 64)), _pivot(Pivot::Center)
	, _isActive(true), _isLive(true)
{
	this->UpdateRect();
}

// ====================================================
// ############ GameObject 변수 초기화 #################
// ====================================================
GameObject::GameObject(const string& name, const Vector2& size, const Vector2& position, const Pivot& pivot)
	: _name(name), _size(size), _position(position), _pivot(pivot), _isActive(true), _isLive(true)
{
	this->UpdateRect();
}

// ====================================================
// ############ Size Set RECT 다시 만듦 ################
// ====================================================
void GameObject::SetSize(const Vector2 & size)
{
	_size = size;
	this->UpdateRect();
}
// ====================================================
// ############ Position Set RECT 다시 만듦 ############
// ====================================================
void GameObject::SetPosition(const Vector2 & position)
{
	_position = position;
	this->UpdateRect();
}
// ====================================================
// ############ Pivot Set RECT 다시 만듦 ###############
// ====================================================
void GameObject::SetPivot(const Pivot & pivot)
{
	_pivot = pivot;
	this->UpdateRect();
}

void GameObject::SetRect(const RECT & rect)
{
	_rc = rect;
}
