#pragma once
//=============================================================
//	## commomMacroFunction ##
//=============================================================

//POINT
inline POINT PointMake(int x, int y)
{
	POINT pt = { x, y };
	return pt;
}

//�� �׸���
inline void LineMake(HDC hdc, int startX, int startY, int endX, int endY)
{
	MoveToEx(hdc, startX, startY, NULL);
	LineTo(hdc, endX, endY);
}

//RECT �����
inline RECT RectMake(int x, int y, int width, int height)
{
	RECT rc = { x, y, x + width, y + height };
	return rc;
}

//RECT �߽���ǥ�� �����
inline RECT RectMakeCenter(int x, int y, int width, int height)
{
	RECT rc = { x - width / 2, y - height / 2, x + width / 2, y + height / 2 };
	return rc;
}

//���ʱ��Լ� �̿��ؼ� �簢 �׸���
inline void RectagleMake(HDC hdc, int x, int y, int width, int height)
{
	Rectangle(hdc, x, y, x + width, y + height);
}

//�ϸ����Լ� �̿��ؼ� Ÿ�� �׸���
inline void EllipseMake(HDC hdc, int x, int y, int width, int height)
{
	Ellipse(hdc, x, y, x + width, y + height);
}

//�ϸ����Լ� �̿��ؼ� �������� Ÿ�� �׸���
inline void EllipseMakeCenter(HDC hdc, int x, int y, int width, int height)
{
	Ellipse(hdc, x - width / 2, y - height / 2, x + width / 2, y + height / 2);
}

//�����Ƽ� Rectangle()�Լ� �����ε����� ������
inline void Rectangle(HDC hdc, RECT rc)
{
	Rectangle(hdc, rc.left, rc.top, rc.right, rc.bottom);
}
inline void Ellipse(HDC hdc, RECT rc)
{
	Ellipse(hdc, rc.left, rc.top, rc.right, rc.bottom);
}

//�����ӷ�Ʈ �Լ� �����ε�(����ִ� ��Ʈ �����)
inline void FrameRect(HDC hdc, RECT rc, COLORREF color)
{
	HBRUSH brush = CreateSolidBrush(color);
	FrameRect(hdc, &rc, brush);
	DeleteObject(brush);
}

inline RECT RectMakePivot(Vector2 pos, Vector2 size, Pivot pivot)
{
	RECT result;
	switch (pivot)
	{
	case Pivot::LeftTop:
		result.left = pos.x;
		result.top = pos.y;
		result.right = pos.x + size.x;
		result.bottom = pos.y + size.y;
		return result;
	case Pivot::Center:
		result.left = pos.x - size.x / 2.f;
		result.top = pos.y - size.y / 2.f;
		result.right = pos.x + size.x / 2.f;
		result.bottom = pos.y + size.y / 2.f;
		return result;
	case Pivot::Bottom:
		result.left = pos.x - size.x / 2.f;
		result.top = pos.y - size.y;
		result.right = pos.x + size.x / 2.f;
		result.bottom = pos.y;
		return result;
	}
	return result;
}