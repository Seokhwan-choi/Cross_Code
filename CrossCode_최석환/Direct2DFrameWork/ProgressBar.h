#pragma once
#include "GameObject.h"
class ProgressBar :
	public GameObject
{
private:
	class Image* BarImage;		// 프로그래스 바 이미지
	class Image* RedImage;		// 프로그래스 바 Red
	class Image* YellowImage;	// 프로그래스 바 Yellow
	class Image* WhiteImage;	// 프로그래스 바 White

	int Width;					// 이미지 크기
	int HpWidth;				// 체력바 길이
	int HpHeight;				// 체력바 높이
	float MaxHp;				// 최대 Hp

	RECT WhiteRc;				// 하양 RECT
	int WhiteHpWidth;			// 하양 체력바 길이
	int WhiteHpHeight;			// 하양 체력바 높이
	float WhiteHp;				// 하양 Hp
	
	bool isYellow;				// 노랑색으로 쓸꺼야
	bool isWhite;				// 하양 체력 깍이기 시작
	bool isShow;

public:

	void Init() override;
	void Init(float maxhp, float hpwidth, float hpheight, bool isyellow = false);
	void Update() override;
	void Render() override;
	void Release() override;

public:

	void SetisWhite(bool w) { isWhite = w; }
	bool GetisWhite() { return isWhite; }

	void SetHpWidth(int w) { HpWidth = w; }
	void SetWhiteHpWidth(int w) { WhiteHpWidth = w; }

	void SetisShow(bool s) { isShow = isShow; }
	bool GetisShow() { return isShow; }

public:

	void WhiteSetGauge(int currentHp, int maxHp);
	void SetGauge(int currentHp, int maxHp);
	void SetPosition(Vector2 pos);

public:
	ProgressBar(string name, Vector2 size, Vector2 pos, Pivot pivot = Pivot::LeftTop);
	~ProgressBar();
};

