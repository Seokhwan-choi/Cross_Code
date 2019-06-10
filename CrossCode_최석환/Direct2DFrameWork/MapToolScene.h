#pragma once
#include "gameNode.h"
#include "Tileheader.h"

class MapToolScene : public GameNode
{
private:
	class None* CameraPos;						// 카메라 중점?
	bool Hold;									// 카메라 중지
	TagTile Tile[XNUM][YNUM];					// 타일 
	TagTile Sample[SAMPLENUMX][SAMPLENUMY];		// Sample Tile

	RECT Palette;								// 타일 Palette용 RECT
	RECT TilePalette;							// 타일 Palette Move용 RECT
	//RECT _TileRect;							// 타일 샘플 RECT ??
	RECT SampleRect;							// 타일 샘플 RECT
	bool SampleShow;							// 타일 샘플 보여줌
	bool Show;									// 타일 Palette 보여줌

	bool Drag;									// 드래그 & 드롭 시작
	RECT MouseRect;								// 드래그 & 드롭 RECT
	RECT SelectRect;							// 드래그 & 드롭 선택 RECT
	POINT MRectSize;							// 드래그 & 드롭 RECT size
	POINT MRectSizeReal;						// 드래그 & 드롭 갯수저장
	POINT Start;								// 드래그 & 드롭 시작 좌표
	POINT End;									// 드래그 & 드롭 끝 좌표
	vector<RECT> AlphaRect;						// 타일에 깔 이미지 보여준다.
	vector<RECT> SelectRectList;				// 드래그 & 드롭 선택 영역표시
	vector<POINT> SelectList;					// 드래그 & 드롭 선택 리스트

	bool SampleChoise;							// 샘플 선택
	bool Select;								// 타일 선택h
	bool DeBug;									// 확인용 
	float Speed;								// Palette 속도

	Image* SlimeImage[4];						// 큰 슬라임맵 이미지
	Image* SlimeSampleImage[4];					// 작은 슬라임맵 이미지

	Image* IceImage[6];							// 큰 아이스 보스맵 이미지
	Image* IceSampleImage[6];					// 작은 아이스 보스맵 이미지

	Image* TitleImage[3];						// Palette 타이틀 이미지
	Image* SystemImage[4];						// 시스템 글자 이미지 Save, Erase, Load, Reset
	Image* NumberImage[15];						// 숫자 이미지 타일 페이지 1,2,3,4,5 .. 타일 층수 1,2,3,4,5 .. 씬 구분 1,2,3,4,5
	Image* caveImage[10];						// 큰 이미지 0, 1, 2, 3, 4  Terrain / 5, 6, 7, 8, 9 Object
	Image* sampleImage[10];						// 작은 이미지 0, 1, 2, 3, 4 Terrain / 5, 6, 7, 8, 9 Object


	Image* touchImage;							// 터치 이펙트 이미지
	Image* cursorImage;							// 마우스 커서 이미지
	Image* greenImage;							// 선택영역 이미지
	Image* paletteImage;						// 팔레트 이미지

	Image* BigButton;							// 큰 버튼 이미지
	float Bcount[6];							// 큰 버튼 이미지 프레임 카운트
	int Bindex[6];								// 큰 버튼 이미지 프레임 인덱스

	Image* Button;								// 작은 버튼 이미지
	float count[15];							// 버튼 이미지 프레임 카운트
	int index[15];								// 버튼 이미지 프레임 인덱스

	SceneNum Scene;								// 씬 넘버 Enum
	Order OrderNum;								// 타일 층수 Enum
	PageNum Page;								// 페이지 넘버 Enum
	TileType Type;								// 타일 타입 Enum

	RECT Title;									// 팔레트 타이틀 렉트
	RECT SelectButton[3];						// Tile SelectButoon RECT
	RECT NumberRect[6];							// NumberButton RECT
	RECT SystemButton[3];						// SaveButton RECT // LoadButton RECT // EraseButton RECT
public:

	void Init();
	void Update();
	void Render();
	void Release();

	void SetImage();							// 이미지 셋팅
	void SetTile();								// 타일 세팅하기

	void KeyHandling();							// 키 입력 처리
	void ChangeScene();							// Scene 전환 
	void ChangeOrder();							// 층수 변환

	void MouseButtonSelect();					// 마우스 버튼 선택 함수
	void MouseSampleSelect();					// 마우스 드래그 선택
	void MouseTileSelect();						// 마우스 타일 선택

	void SelectRectMake(int x, int y);			// 마우스 드래그 선택 RECT 만들기

	void ShowTile();							// 타일 보여주기
	void ShowPalette();							// 팔렛 보여주기
	void DeBugCheck();							// 디버그 확인용
	void CheckMousePos();						// 현재 마우스 위치

	void Save();
	void Load();
	void Reset();

	MapToolScene() {}
	virtual ~MapToolScene() {}
};

