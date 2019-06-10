#pragma once
#include "GameObject.h"
class DamageFont : public GameObject
{
struct FontTag
{
	int Damage;			// �󸶳� �޾ҳ�
	float Alpha;		// ��Ʈ ���İ�
	float Speed;		// ��Ʈ �̵��ӵ�
	float Size;			// ��Ʈ ������
	COLORREF Color;		// ��Ʈ ��
	Vector2 Pos;		// ��Ʈ ��ǥ
	bool isFire;		// ��Ʈ ~ing
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

