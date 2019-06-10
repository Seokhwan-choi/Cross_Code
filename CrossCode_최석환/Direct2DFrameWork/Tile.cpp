#include "stdafx.h"
//#include "tile.h"
//#include "aStarTest.h"
//
//void tile::Init(int idX, int idY)
//{
//	_color = RGB(250, 150, 0);
//	_brush = CreateSolidBrush(_color);
//	_pen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
//	//3*32+ 32/2  4*32+ 32/2
//	//	96+16=112     128+16 =144
//
//	_center = PointMake(idX * TILEWIDTH + (TILEWIDTH / 2), idY * TILEHEIGHT + (TILEHEIGHT / 2));
//
//	_idX = idX;
//	_idY = idY;
//
//	_rc = RectMakeCenter(_center.x, _center.y, TILEWIDTH, TILEHEIGHT);
//}
//
//void tile::Init()
//{
//}
//
//
//void tile::Release(void)
//{
//	DeleteObject(_brush);
//	DeleteObject(_pen);
//}
//
//void tile::Update(void)
//{
//}
//
//void tile::Render(void)
//{
//	D2DRENDERER->DrawRectangle(_rc, D2DRenderer::DefaultBrush::Green);
//}