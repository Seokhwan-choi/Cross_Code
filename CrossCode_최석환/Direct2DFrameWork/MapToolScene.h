#pragma once
#include "gameNode.h"
#include "Tileheader.h"

class MapToolScene : public GameNode
{
private:
	class None* CameraPos;						// ī�޶� ����?
	bool Hold;									// ī�޶� ����
	TagTile Tile[XNUM][YNUM];					// Ÿ�� 
	TagTile Sample[SAMPLENUMX][SAMPLENUMY];		// Sample Tile

	RECT Palette;								// Ÿ�� Palette�� RECT
	RECT TilePalette;							// Ÿ�� Palette Move�� RECT
	//RECT _TileRect;							// Ÿ�� ���� RECT ??
	RECT SampleRect;							// Ÿ�� ���� RECT
	bool SampleShow;							// Ÿ�� ���� ������
	bool Show;									// Ÿ�� Palette ������

	bool Drag;									// �巡�� & ��� ����
	RECT MouseRect;								// �巡�� & ��� RECT
	RECT SelectRect;							// �巡�� & ��� ���� RECT
	POINT MRectSize;							// �巡�� & ��� RECT size
	POINT MRectSizeReal;						// �巡�� & ��� ��������
	POINT Start;								// �巡�� & ��� ���� ��ǥ
	POINT End;									// �巡�� & ��� �� ��ǥ
	vector<RECT> AlphaRect;						// Ÿ�Ͽ� �� �̹��� �����ش�.
	vector<RECT> SelectRectList;				// �巡�� & ��� ���� ����ǥ��
	vector<POINT> SelectList;					// �巡�� & ��� ���� ����Ʈ

	bool SampleChoise;							// ���� ����
	bool Select;								// Ÿ�� ����h
	bool DeBug;									// Ȯ�ο� 
	float Speed;								// Palette �ӵ�

	Image* SlimeImage[4];						// ū �����Ӹ� �̹���
	Image* SlimeSampleImage[4];					// ���� �����Ӹ� �̹���

	Image* IceImage[6];							// ū ���̽� ������ �̹���
	Image* IceSampleImage[6];					// ���� ���̽� ������ �̹���

	Image* TitleImage[3];						// Palette Ÿ��Ʋ �̹���
	Image* SystemImage[4];						// �ý��� ���� �̹��� Save, Erase, Load, Reset
	Image* NumberImage[15];						// ���� �̹��� Ÿ�� ������ 1,2,3,4,5 .. Ÿ�� ���� 1,2,3,4,5 .. �� ���� 1,2,3,4,5
	Image* caveImage[10];						// ū �̹��� 0, 1, 2, 3, 4  Terrain / 5, 6, 7, 8, 9 Object
	Image* sampleImage[10];						// ���� �̹��� 0, 1, 2, 3, 4 Terrain / 5, 6, 7, 8, 9 Object


	Image* touchImage;							// ��ġ ����Ʈ �̹���
	Image* cursorImage;							// ���콺 Ŀ�� �̹���
	Image* greenImage;							// ���ÿ��� �̹���
	Image* paletteImage;						// �ȷ�Ʈ �̹���

	Image* BigButton;							// ū ��ư �̹���
	float Bcount[6];							// ū ��ư �̹��� ������ ī��Ʈ
	int Bindex[6];								// ū ��ư �̹��� ������ �ε���

	Image* Button;								// ���� ��ư �̹���
	float count[15];							// ��ư �̹��� ������ ī��Ʈ
	int index[15];								// ��ư �̹��� ������ �ε���

	SceneNum Scene;								// �� �ѹ� Enum
	Order OrderNum;								// Ÿ�� ���� Enum
	PageNum Page;								// ������ �ѹ� Enum
	TileType Type;								// Ÿ�� Ÿ�� Enum

	RECT Title;									// �ȷ�Ʈ Ÿ��Ʋ ��Ʈ
	RECT SelectButton[3];						// Tile SelectButoon RECT
	RECT NumberRect[6];							// NumberButton RECT
	RECT SystemButton[3];						// SaveButton RECT // LoadButton RECT // EraseButton RECT
public:

	void Init();
	void Update();
	void Render();
	void Release();

	void SetImage();							// �̹��� ����
	void SetTile();								// Ÿ�� �����ϱ�

	void KeyHandling();							// Ű �Է� ó��
	void ChangeScene();							// Scene ��ȯ 
	void ChangeOrder();							// ���� ��ȯ

	void MouseButtonSelect();					// ���콺 ��ư ���� �Լ�
	void MouseSampleSelect();					// ���콺 �巡�� ����
	void MouseTileSelect();						// ���콺 Ÿ�� ����

	void SelectRectMake(int x, int y);			// ���콺 �巡�� ���� RECT �����

	void ShowTile();							// Ÿ�� �����ֱ�
	void ShowPalette();							// �ȷ� �����ֱ�
	void DeBugCheck();							// ����� Ȯ�ο�
	void CheckMousePos();						// ���� ���콺 ��ġ

	void Save();
	void Load();
	void Reset();

	MapToolScene() {}
	virtual ~MapToolScene() {}
};

