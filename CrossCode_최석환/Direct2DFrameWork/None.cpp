#include "stdafx.h"
#include "None.h"


void None::Init()
{
}

void None::Update()
{
	if (KEYMANAGER->IsStayKeyDown('A'))
	{
		_position.x -= 300.f * TIMEMANAGER->getElapedTime();
		//CAMERA->SetPosition(_position);
	}

	if (KEYMANAGER->IsStayKeyDown('D'))
	{
		_position.x += 300.f * TIMEMANAGER->getElapedTime();
		//CAMERA->SetPosition(_position);
	}

	if (KEYMANAGER->IsStayKeyDown('W'))
	{
		_position.y -= 300.f * TIMEMANAGER->getElapedTime();
		//CAMERA->SetPosition(_position);
	}

	if (KEYMANAGER->IsStayKeyDown('S'))
	{
		_position.y += 300.f * TIMEMANAGER->getElapedTime();
		//CAMERA->SetPosition(_position);
	}

	/*_position.x  = _ptMouse.x + (CAMERA->GetCameraRect().left * CAMERA->GetZoom());
	_position.y = _ptMouse.y + (CAMERA->GetCameraRect().top * CAMERA->GetZoom());*/

	//_position.x = _ptMouse.x + CAMERA->GetCameraRect().left;
	//_position.y = _ptMouse.y + CAMERA->GetCameraRect().top;

	//if (KEYMANAGER->IsOnceKeyDown('F'))
	//{
	//	_isFree = !_isFree;
	//	if (_isFree)
	//	{
	//		CAMERA->SetFreeCamera();
	//	}
	//	else
	//	{
	//		CAMERA->SetNoneCamera();
	//	}
	//}

	if (KEYMANAGER->IsOnceKeyDown('S'))
	{
		//CAMERA->Shake();
	}

	_rc = RectMakePivot(_position, _size, _pivot);
}

void None::Render()
{
	//D2DRENDERER->FillRectangle(CAMERA->GetRelativeRect(this->_rc), D2DRenderer::DefaultBrush::White);
}

void None::Release()
{

}

None::None()
	: GameObject()
{
	_isFree = false;
}


None::~None()
{
}
