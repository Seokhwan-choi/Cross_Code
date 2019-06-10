#pragma once
#include "GameObject.h"
class BossProgressBar :
	public GameObject
{
private:
	Image* BarImage;			// 체력바 이미지
	Image* First;				// 처음 이미지
	Image* Last;				// 마지막 이미지
	Image* Middle[2];			// 중간 이미지

	Image* Line[3];				// 체력바 Line

	int Width;					// 이미지 크기
	int HpWidth;				// 체력바 길이
	int HpHeight;				// 체력바 높이
	float MaxHp;				// 최대 Hp

	RECT WhiteRc;				// 하양 RECT
	int WhiteHpWidth;			// 하양 체력바 길이
	int WhiteHpHeight;			// 하양 체력바 높이
	float WhiteHp;				// 하양 Hp

	bool isWhite;				// 하양 체력 깍이기 시작
	bool isShow;				// 보스 체력바 보여줄꺼야!!
	float Alpha;				// 보스 체력바 Alpha 값
public:

	void Init() override;
	void Update() override;
	void Render() override;
	void Release() override;

public:

	void SetisWhite(bool s) { isWhite = s; }
	bool GetisWhite() { return isWhite; }
	void SetisShow(bool s) { isShow = s; }
	bool GetisShow() { return isShow; }

public:

	bool BarDone(int maxhp);
	void WhiteSetGauge(int currentHp, int maxHp);
	void SetGauge(int currentHp, int maxHp);

	BossProgressBar(string name, Vector2 size, Vector2 pos, Pivot pivot = Pivot::Center);
	~BossProgressBar();
};

