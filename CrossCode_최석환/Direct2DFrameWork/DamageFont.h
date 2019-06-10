#pragma once
#include "GameObject.h"
class DamageFont : public GameObject
{
struct FontTag
{
	int Damage;			// 얼마나 달았냐
	float Alpha;		// 폰트 알파값
	float Speed;		// 폰트 이동속도
	float Size;			// 폰트 사이즈
	COLORREF Color;		// 폰트 색
	Vector2 Pos;		// 폰트 좌표
	bool isFire;		// 폰트 ~ing
};

private:
	vector<FontTag> FontList;
	Vector2 Pos;
	bool isDamage;
	float Alpha;
	float DamageTime;
	int SumDamage;
	int ShowDamage;
	bool show;
public:

	void Show(Vector2 Pos, int damage, float size = 25.f, bool show = false, COLORREF color = RGB(255,255,255));
	void DamageSum();
	void FontMove();

	void Init() override;
	void Update() override;
	void Render() override;
	void Release() override;

	DamageFont();
	~DamageFont() {}
};

