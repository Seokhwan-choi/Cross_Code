#pragma once
#include "GameObject.h"
class ProgressBar :
	public GameObject
{
private:
	class Image* BarImage;		// ���α׷��� �� �̹���
	class Image* RedImage;		// ���α׷��� �� Red
	class Image* YellowImage;	// ���α׷��� �� Yellow
	class Image* WhiteImage;	// ���α׷��� �� White

	int Width;					// �̹��� ũ��
	int HpWidth;				// ü�¹� ����
	int HpHeight;				// ü�¹� ����
	float MaxHp;				// �ִ� Hp

	RECT WhiteRc;				// �Ͼ� RECT
	int WhiteHpWidth;			// �Ͼ� ü�¹� ����
	int WhiteHpHeight;			// �Ͼ� ü�¹� ����
	float WhiteHp;				// �Ͼ� Hp
	
	bool isYellow;				// ��������� ������
	bool isWhite;				// �Ͼ� ü�� ���̱� ����
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

