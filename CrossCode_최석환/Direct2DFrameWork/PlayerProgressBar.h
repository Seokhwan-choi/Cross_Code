#pragma once
#include "GameObject.h"
class PlayerProgressBar : public GameObject
{
private:
	int HpWidth;
	int WHpWidth;
	int Width;
	float WHp;

	int SaveHp;
	int ShowHp;

	RECT WhiteRc;
	bool isWhite;
public:
	void Init() override;
	void Update() override;
	void Render() override;
	void Release() override;

public:

	void SetisWhite(bool w) { isWhite = w; }
	bool GetisWhite() { return isWhite; }

public:
	void WhiteSetGauge(int currentHp, int maxHp);
	void SetGauge(int currentHp, int maxHp);
	void ShowNumber();

	PlayerProgressBar(string name, Vector2 size, Vector2 pos, Pivot pivot = Pivot::LeftTop);
	~PlayerProgressBar();
};

