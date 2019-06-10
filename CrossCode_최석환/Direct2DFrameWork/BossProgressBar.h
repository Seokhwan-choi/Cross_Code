#pragma once
#include "GameObject.h"
class BossProgressBar :
	public GameObject
{
private:
	Image* BarImage;			// ü�¹� �̹���
	Image* First;				// ó�� �̹���
	Image* Last;				// ������ �̹���
	Image* Middle[2];			// �߰� �̹���

	Image* Line[3];				// ü�¹� Line

	int Width;					// �̹��� ũ��
	int HpWidth;				// ü�¹� ����
	int HpHeight;				// ü�¹� ����
	float MaxHp;				// �ִ� Hp

	RECT WhiteRc;				// �Ͼ� RECT
	int WhiteHpWidth;			// �Ͼ� ü�¹� ����
	int WhiteHpHeight;			// �Ͼ� ü�¹� ����
	float WhiteHp;				// �Ͼ� Hp

	bool isWhite;				// �Ͼ� ü�� ���̱� ����
	bool isShow;				// ���� ü�¹� �����ٲ���!!
	float Alpha;				// ���� ü�¹� Alpha ��
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

