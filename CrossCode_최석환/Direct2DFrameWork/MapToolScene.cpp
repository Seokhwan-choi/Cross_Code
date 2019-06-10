#include "stdafx.h"
#include "MapToolScene.h"
#include "None.h"


void MapToolScene::Init()
{
	this->SetImage();

	Hold = false; Show = false;
	DeBug = false; Select = false;
	SampleChoise = false; SampleShow = false;
	Start = End = { 0, 0 };
	ShowCursor(false);			// 마우스 커서 삭제

	Speed = 0.f;

	//TileRect = RectMake((WINSIZEX - 528), 105, 512, 576);
	TilePalette = RectMake(WINSIZEX, 75, 400, 550);
	SampleRect = RectMake(115 + (WINSIZEX - 450), 185, 256, 288);
	Palette = RectMake(WINSIZEX, 135, 400, 450);

	Title = RectMake(959, 93, 218, 42);
	SelectButton[0] = RectMake(840, 155, 20, 80);
	SelectButton[1] = RectMake(840, 284, 20, 80);
	SelectButton[2] = RectMake(840, 395, 20, 80);

	NumberRect[0] = RectMake(870, 485, 60, 35);
	NumberRect[1] = RectMake(935, 485, 60, 35);
	NumberRect[2] = RectMake(1000, 485, 60, 35);
	NumberRect[3] = RectMake(1065, 485, 60, 35);
	NumberRect[4] = RectMake(1130, 485, 60, 35);
	NumberRect[5] = RectMake(1195, 485, 60, 35);

	SystemButton[0] = RectMake(885, 545, 115, 35);
	SystemButton[1] = RectMake(1009, 545, 115, 35);
	SystemButton[2] = RectMake(1131, 545, 115, 35);

	for (int i = 0; i < 6; ++i) {
		count[i] = 0.f;
		Bcount[i] = 0.f;
		index[i] = 0;
		Bindex[i] = 0;
	}
	CameraPos = new None;
	OBJECTMANAGER->addObject(ObjectType::Object, CameraPos);
	CAMERA->SetTarget(CameraPos);

	MRectSize = { 0,0 };

	Page = PageNum::One;				// 페이지 초기값 One(1페이지)
	Type = TileType::Terrain;			// 타일 초기값 Terrain(지형)
	Scene = SceneNum::Town;				// 씬 초기값 Town(마을)
	OrderNum = Order::Zero;				// 층수 초기값 Zero(0층)
	

}

void MapToolScene::Update()
{
	this->SetTile();					// 타일 셋팅하기
	this->KeyHandling();				// 키 입력 처리
	this->ShowPalette();				// 팔렛 보여주기
	this->MouseButtonSelect();			// 마우스 버튼 선택
	this->MouseSampleSelect();			// 마우스 샘플타일 선택
	this->MouseTileSelect();			// 마우스 타일 선택
	OBJECTMANAGER->Update();			// 오브젝트 매니저 Update()

	if (KEYMANAGER->IsOnceKeyDown(VK_ESCAPE))
	{
		SCENEMANAGER->LoadScene("타이틀");
	}

}

void MapToolScene::Render()
{
	this->ShowTile();					
	this->DeBugCheck();
	this->CheckMousePos();
	OBJECTMANAGER->Render();
}

void MapToolScene::Release()
{
	OBJECTMANAGER->Release();
}

// ===============================================
// ############### 이미지 세팅 함수 ###############
// ===============================================
void MapToolScene::SetImage()
{
	// x사이즈 64, y사이즈 64 마을ㅢ;
	caveImage[0] = IMAGEMANAGER->AddFrameImage("큰마을", L"Image/MapTool/큰마을.png", SAMPLENUMX, SAMPLENUMY);
	caveImage[1] = IMAGEMANAGER->AddFrameImage("큰마을2", L"Image/MapTool/큰마을2.png", SAMPLENUMX, SAMPLENUMY);
	caveImage[2] = IMAGEMANAGER->AddFrameImage("큰마을3", L"Image/MapTool/큰마을3.png", SAMPLENUMX, SAMPLENUMY);
	caveImage[3] = IMAGEMANAGER->AddFrameImage("큰마을4", L"Image/MapTool/큰마을4.png", SAMPLENUMX, SAMPLENUMY);
	caveImage[4] = IMAGEMANAGER->AddFrameImage("큰마을5", L"Image/MapTool/큰마을5.png", SAMPLENUMX, SAMPLENUMY);
	caveImage[5] = IMAGEMANAGER->AddFrameImage("큰마을6", L"Image/MapTool/큰마을6.png", SAMPLENUMX, SAMPLENUMY);
	caveImage[6] = IMAGEMANAGER->AddFrameImage("큰마을7", L"Image/MapTool/큰마을7.png", SAMPLENUMX, SAMPLENUMY);
	caveImage[7] = IMAGEMANAGER->AddFrameImage("큰마을8", L"Image/MapTool/큰마을8.png", SAMPLENUMX, SAMPLENUMY);
	caveImage[8] = IMAGEMANAGER->AddFrameImage("큰마을9", L"Image/MapTool/큰마을9.png", SAMPLENUMX, SAMPLENUMY);

	//  ====== 팔렛 전용 이미지 (마을) ======
	paletteImage = IMAGEMANAGER->AddImage("팔렛", L"Image/MapTool/팔레트2.png");
	// Terrain ( 지형 ) 이미지 
	sampleImage[0] = IMAGEMANAGER->AddFrameImage("마을", L"Image/MapTool/마을.png", SAMPLENUMX, SAMPLENUMY);
	sampleImage[1] = IMAGEMANAGER->AddFrameImage("마을2", L"Image/MapTool/마을2.png", SAMPLENUMX, SAMPLENUMY);
	sampleImage[2] = IMAGEMANAGER->AddFrameImage("마을3", L"Image/MapTool/마을3.png", SAMPLENUMX, SAMPLENUMY);
	// Object ( 오브젝트 ) 이미지 
	sampleImage[3] = IMAGEMANAGER->AddFrameImage("마을4", L"Image/MapTool/마을4.png", SAMPLENUMX, SAMPLENUMY);
	sampleImage[4] = IMAGEMANAGER->AddFrameImage("마을5", L"Image/MapTool/마을5.png", SAMPLENUMX, SAMPLENUMY);
	sampleImage[5] = IMAGEMANAGER->AddFrameImage("마을6", L"Image/MapTool/마을6.png", SAMPLENUMX, SAMPLENUMY);
	sampleImage[6] = IMAGEMANAGER->AddFrameImage("마을7", L"Image/MapTool/마을7.png", SAMPLENUMX, SAMPLENUMY);
	sampleImage[7] = IMAGEMANAGER->AddFrameImage("마을8", L"Image/MapTool/마을8.png", SAMPLENUMX, SAMPLENUMY);
	sampleImage[8] = IMAGEMANAGER->AddFrameImage("마을9", L"Image/MapTool/마을9.png", SAMPLENUMX, SAMPLENUMY);
	//  ==============================

	SlimeImage[0] = IMAGEMANAGER->AddFrameImage("큰슬라임", L"Image/MapTool/큰슬라임맵.png", SAMPLENUMX, SAMPLENUMY);
	SlimeImage[1] = IMAGEMANAGER->AddFrameImage("큰슬라임2", L"Image/MapTool/큰슬라임맵2.png", SAMPLENUMX, SAMPLENUMY);
	SlimeImage[2] = IMAGEMANAGER->AddFrameImage("큰슬라임3", L"Image/MapTool/큰슬라임맵3.png", SAMPLENUMX, SAMPLENUMY);
	SlimeImage[3] = IMAGEMANAGER->AddFrameImage("큰슬라임4", L"Image/MapTool/큰슬라임맵4.png", SAMPLENUMX, SAMPLENUMY);

	SlimeSampleImage[0] = IMAGEMANAGER->AddFrameImage("슬라임", L"Image/MapTool/슬라임맵.png", SAMPLENUMX, SAMPLENUMY);
	SlimeSampleImage[1] = IMAGEMANAGER->AddFrameImage("슬라임2", L"Image/MapTool/슬라임맵2.png", SAMPLENUMX, SAMPLENUMY);
	SlimeSampleImage[2] = IMAGEMANAGER->AddFrameImage("슬라임3", L"Image/MapTool/슬라임맵3.png", SAMPLENUMX, SAMPLENUMY);
	SlimeSampleImage[3] = IMAGEMANAGER->AddFrameImage("슬라임4", L"Image/MapTool/슬라임맵4.png", SAMPLENUMX, SAMPLENUMY);

	IceImage[0] = IMAGEMANAGER->AddFrameImage("큰얼음", L"Image/MapTool/큰얼음맵.png", SAMPLENUMX, SAMPLENUMY);
	IceImage[1] = IMAGEMANAGER->AddFrameImage("큰얼음2", L"Image/MapTool/큰얼음맵2.png", SAMPLENUMX, SAMPLENUMY);
	IceImage[2] = IMAGEMANAGER->AddFrameImage("큰얼음3", L"Image/MapTool/큰얼음맵3.png", SAMPLENUMX, SAMPLENUMY);
	IceImage[3] = IMAGEMANAGER->AddFrameImage("큰얼음4", L"Image/MapTool/큰얼음맵4.png", SAMPLENUMX, SAMPLENUMY);
	IceImage[4] = IMAGEMANAGER->AddFrameImage("큰얼음5", L"Image/MapTool/큰얼음맵5.png", SAMPLENUMX, SAMPLENUMY);
	IceImage[5] = IMAGEMANAGER->AddFrameImage("큰얼음6", L"Image/MapTool/큰얼음맵6.png", SAMPLENUMX, SAMPLENUMY);

	IceSampleImage[0] = IMAGEMANAGER->AddFrameImage("얼음", L"Image/MapTool/얼음맵.png", SAMPLENUMX, SAMPLENUMY);
	IceSampleImage[1] = IMAGEMANAGER->AddFrameImage("얼음2", L"Image/MapTool/얼음맵2.png", SAMPLENUMX, SAMPLENUMY);
	IceSampleImage[2] = IMAGEMANAGER->AddFrameImage("얼음3", L"Image/MapTool/얼음맵3.png", SAMPLENUMX, SAMPLENUMY);
	IceSampleImage[3] = IMAGEMANAGER->AddFrameImage("얼음4", L"Image/MapTool/얼음맵4.png", SAMPLENUMX, SAMPLENUMY);
	IceSampleImage[4] = IMAGEMANAGER->AddFrameImage("얼음5", L"Image/MapTool/얼음맵5.png", SAMPLENUMX, SAMPLENUMY);
	IceSampleImage[5] = IMAGEMANAGER->AddFrameImage("얼음6", L"Image/MapTool/얼음맵6.png", SAMPLENUMX, SAMPLENUMY);

	//  ====== 버튼 이미지 ===========
	BigButton = IMAGEMANAGER->AddFrameImage("큰버튼", L"Image/MapTool/큰버튼2.png",20, 1);
	Button = IMAGEMANAGER->AddFrameImage("버튼", L"Image/MapTool/버튼2.png", 20, 1);
	//  ==============================

	TitleImage[0] = IMAGEMANAGER->AddImage("오브젝트", L"Image/MapTool/오브젝트.png");
	TitleImage[1] = IMAGEMANAGER->AddImage("에너미", L"Image/MapTool/에너미.png");
	TitleImage[2] = IMAGEMANAGER->AddImage("지형", L"Image/MapTool/지형.png");

	NumberImage[0] = IMAGEMANAGER->AddImage("숫자1", L"Image/MapTool/숫자1.png");
	NumberImage[1] = IMAGEMANAGER->AddImage("숫자2", L"Image/MapTool/숫자2.png");
	NumberImage[2] = IMAGEMANAGER->AddImage("숫자3", L"Image/MapTool/숫자3.png");
	NumberImage[3] = IMAGEMANAGER->AddImage("숫자4", L"Image/MapTool/숫자4.png");
	NumberImage[4] = IMAGEMANAGER->AddImage("숫자5", L"Image/MapTool/숫자5.png");
	NumberImage[5] = IMAGEMANAGER->AddImage("숫자6", L"Image/MapTool/숫자6.png");

	SystemImage[0] = IMAGEMANAGER->AddImage("세이브", L"Image/MapTool/세이브.png");
	SystemImage[1] = IMAGEMANAGER->AddImage("지우개", L"Image/MapTool/지우개.png");
	SystemImage[2] = IMAGEMANAGER->AddImage("로드", L"Image/MapTool/로드.png");

	touchImage = IMAGEMANAGER->AddFrameImage("터치1", L"Image/MapTool/터치.png",3, 1);
	greenImage = IMAGEMANAGER->AddImage("선택영역", L"Image/MapTool/선택영역.png");
	cursorImage = IMAGEMANAGER->AddImage("커서", L"Image/MapTool/커서.png");
}
// ===============================================
// ############# 키 입력 처리 함수 ################
// ===============================================
void MapToolScene::KeyHandling()
{
	if (!Hold)
	{
		OBJECTMANAGER->Update();
	}

	if (KEYMANAGER->IsOnceKeyDown('H'))
	{
		Hold = !Hold;
	}

	if (KEYMANAGER->IsOnceKeyDown(VK_RETURN))
	{
		Show = !Show;
	}

	if (KEYMANAGER->IsOnceKeyDown(VK_F1))
	{
		DeBug = !DeBug;
	}

	this->ChangeScene();
	this->ChangeOrder();

}

void MapToolScene::ChangeScene()
{

	if (KEYMANAGER->IsOnceKeyDown('1'))
	{
		this->Save();
		Scene = SceneNum::Town;
		this->Load();
	}

	if (KEYMANAGER->IsOnceKeyDown('2'))
	{
		this->Save();
		Scene = SceneNum::Boss1;
		this->Load();
	}

	if (KEYMANAGER->IsOnceKeyDown('3'))
	{
		this->Save();
		Scene = SceneNum::Boss2;
		this->Load();
	}

	if (KEYMANAGER->IsOnceKeyDown('4'))
	{
		this->Save();
		Scene = SceneNum::Boss3;
		this->Load();
	}
}

void MapToolScene::ChangeOrder()
{
	if (KEYMANAGER->IsOnceKeyDown('P')) 
	{
		if ((int)OrderNum < 3) {
			OrderNum = (Order)((int)OrderNum + 1);
		}

		if ((int)Page < 8) {
			Page = (PageNum)((int)Page + 1);
		}
	}

	if (KEYMANAGER->IsOnceKeyDown('O'))
	{
		if ((int)OrderNum > 0) 
		{
			OrderNum = (Order)((int)OrderNum - 1);
		}

		if ((int)Page > 0) {
			Page = (PageNum)((int)Page - 1);
		}
	}
}

// =================================================
// ############# 팔레트 버튼 선택 함수 #############
// =================================================
void MapToolScene::MouseButtonSelect()
{
	for (int i = 0; i < 6; ++i) {
		// =================================================
		// ############### 타일 속성 선택 ##################
		// =================================================
		if (PtInRect(&SelectButton[i], _ptMouse.ToPoint()) && SampleShow) {
			if (KEYMANAGER->IsOnceKeyDown(VK_LBUTTON)) {
				EFFECT->ShowEffect("터치", _ptMouse, Vector2(50,50));
				switch (i)
				{
				case 0:
					Type = TileType::Object;
					break;
				case 1:
					Type = TileType::Enemy;
					break;
				case 2:
					Type = TileType::Terrain;
					break;
				}
			}
		}

		// =================================================
		// ############ 타일 페이지(숫자) 선택 #############
		// =================================================
		if (PtInRect(&NumberRect[i], _ptMouse.ToPoint()) && SampleShow) {
			count[i] += TIMEMANAGER->getElapedTime();
			if (count[i] > 0.01f) {
				index[i]++;
				if (index[i] > 19) {
					index[i] = 19;
				}
				while (count[i] > 0.01f)
					count[i] -= 0.01f;
			}
			if (KEYMANAGER->IsOnceKeyDown(VK_LBUTTON)) {
				EFFECT->ShowEffect("터치", _ptMouse, Vector2(50,50));
				switch (i)
				{
				case 0:
					Page = PageNum::One;
					break;
				case 1:
					Page = PageNum::Two;
					break;
				case 2:
					Page = PageNum::Three;
					break;
				case 3:
					Page = PageNum::Four;
					break;
				case 4:
					Page = PageNum::Five;
					break;
				case 5:
					Page = PageNum::Six;
					break;
				}
			}
		}
		else {
			count[i] += TIMEMANAGER->getElapedTime();
			if (count[i] > 0.01f) {
				index[i]--;
				if (index[i] < 0) {
					index[i] = 0;
				}
				while (count[i] > 0.01f)
					count[i] -= 0.01f;
			}
		}

		// =================================================
		// ############### 시스템 버튼 선택 ################
		// =================================================
		if (PtInRect(&SystemButton[i], _ptMouse.ToPoint()) && SampleShow) {
			Bcount[i] += TIMEMANAGER->getElapedTime();
			if (Bcount[i] > 0.01f) {
				Bindex[i]++;
				if (Bindex[i] > 19) {
					Bindex[i] = 19;
				}
				while (Bcount[i] > 0.01f)
					Bcount[i] -= 0.01f;
			}
			if (KEYMANAGER->IsOnceKeyDown(VK_LBUTTON)) {
				EFFECT->ShowEffect("터치", _ptMouse, Vector2(50, 50));
				switch (i)
				{
				case 0:
					this->Save();
					break;
				case 1:
					this->Reset();
					break;
				case 2:
					this->Load();
					break;
				}
			}
		}
		else {
			Bcount[i] += TIMEMANAGER->getElapedTime();
			if (Bcount[i] > 0.01f) {
				Bindex[i]--;
				if (Bindex[i] < 0) {
					Bindex[i] = 0;
				}
				while (Bcount[i] > 0.01f)
					Bcount[i] -= 0.01f;
			}
		}
	}
}

// ===============================================
// ########### 마우스 샘플 타일 선택 함수 ##########
// ===============================================
void MapToolScene::MouseSampleSelect()
{
	// Sample 타일 위에서 드래그 & 드롭
	// 시작 좌표(Start), 끝 좌표(End) 결정
	if (PtInRect(&SampleRect, _ptMouse.ToPoint()) && Show)
	{
		if (KEYMANAGER->IsOnceKeyDown(VK_LBUTTON))
		{
			EFFECT->ShowEffect("터치", _ptMouse, Vector2(50, 50));
			Start = _ptMouse.ToPoint();
			SampleChoise = false;
		}

		if (KEYMANAGER->IsStayKeyDown(VK_LBUTTON))
		{
			End = _ptMouse.ToPoint();
			if (End.x > Start.x)
			{
				MRectSize.x = End.x - Start.x;
				MRectSize.y = End.y - Start.y;
				MouseRect = RectMake(Start.x, Start.y, MRectSize.x, MRectSize.y);
			}
			else
			{
				MRectSize.x = Start.x - End.x;
				MRectSize.y = Start.y - End.y;
				MouseRect = RectMake(End.x, End.y, MRectSize.x, MRectSize.y);
			}
		}

		if (KEYMANAGER->IsOnceKeyUp(VK_LBUTTON))
		{
			SelectList.clear();
			SelectRectList.clear();
			EFFECT->ShowEffect("터치", _ptMouse, Vector2(50, 50));

			int stx = (Start.x - (115 + (WINSIZEX - 450))) / 16;
			int sty = (Start.y - 185) / 16;

			int endx = (End.x - (115 + (WINSIZEX - 450))) / 16;
			int endy = (End.y - 185) / 16;

			if (End.x >= Start.x)
			{
				MRectSizeReal.x = endx - stx;
				MRectSizeReal.y = endy - sty;
				for (int i = sty; i <= endy; ++i)
				{
					for (int j = stx; j <= endx; ++j)
					{
						// 범위에 지정된 친구들 리스트에 담아준다.
						SelectList.push_back({ j,i });
						SelectRectList.push_back(Sample[j][i].rc);
					}
				}
			}
			else
			{
				MRectSizeReal.x = stx - endx;
				MRectSizeReal.y = sty - endy;
				for (int i = endy; i <= sty; ++i)
				{
					for (int j = endx; j <= stx; ++j)
					{
						// 범위에 지정된 친구들 리스트에 담아준다.
						SelectList.push_back({ j,i });
					}
				}
			}
			SampleChoise = true;
			this->SelectRectMake(stx, sty);
		}
	}

}

// ===============================================
// ########### 마우스 선택 RECT 만들기 함수 #######
// ===============================================
void MapToolScene::SelectRectMake(int x, int y)
{
	if (SelectRectList.size() != 0) {
		int stx = SelectRectList[0].left;
		int sty = SelectRectList[0].top;

		int edx = SelectRectList[SelectRectList.size() - 1].right;
		int edy = SelectRectList[SelectRectList.size() - 1].bottom;

		SelectRect = RectMake(stx, sty, edx - stx, edy - sty);
	}
	else
	{
		SelectRect = RectMake(Sample[x][y].rc.left, Sample[x][y].rc.top, 16, 16);
	}
}

// ===============================================
// ############# 마우스 타일 선택 함수 #############
// ===============================================
void MapToolScene::MouseTileSelect()
{
	RECT temp;
	int startx = 0;
	int starty = 0;
	int index = 0;
	if (KEYMANAGER->IsStayKeyDown(VK_LBUTTON))
	{
		EFFECT->ShowEffect("터치", _ptMouse, Vector2(50, 50));
		if (!SampleChoise) return;
		if (SelectList.size() == 0) return;
		startx = (_ptMouse.x / CAMERA->GetZoom() + CAMERA->GetCameraRect().left) / 64;
		starty = (_ptMouse.y / CAMERA->GetZoom() + CAMERA->GetCameraRect().top) / 64;
		if (startx < 0 || starty < 0 || starty > YNUM || startx > XNUM) return;
		if (PtInRect(&CAMERA->GetRelativeRect(Tile[startx][starty].rc), _ptMouse.ToPoint())
			&& !IntersectRect(&temp, &CAMERA->GetRelativeRect(Tile[startx][starty].rc), &TilePalette))
		{
			for (int i = starty; i <= starty + MRectSizeReal.y; ++i)
			{
				for (int j = startx; j <= startx + MRectSizeReal.x; ++j)
				{
					if (KEYMANAGER->IsStayKeyDown('E'))
					{
						switch (Type)
						{
						case TileType::Terrain:
							Tile[j][i].Terrainx = -1;
							Tile[j][i].Terrainy = -1;
							Tile[j][i].TerrainNum = PageNum::END;
							Tile[j][i].TerrainOrder = Order::END;
							Tile[j][i].show = true;
							break;
						case TileType::Object:
							Tile[j][i].Objectx = -1;
							Tile[j][i].Objecty = -1;
							Tile[j][i].ObjectNum = PageNum::END;
							Tile[j][i].ObjectOrder = Order::END;
							Tile[j][i].show = true;
							break;
						case TileType::Enemy:
							Tile[j][i].Terrain2x = -1;
							Tile[j][i].Terrain2y = -1;
							Tile[j][i].Terrain2Num = PageNum::END;
							Tile[j][i].Terrain2Order = Order::END;
							Tile[j][i].show = true;
							break;
						}
					}
					else
					{
						switch (Type)
						{
						case TileType::Terrain:
							Tile[j][i].Terrainx = SelectList[index].x;
							Tile[j][i].Terrainy = SelectList[index++].y;
							Tile[j][i].TerrainNum = Page;
							Tile[j][i].TerrainOrder = OrderNum;
							Tile[j][i].show = true;
							break;
						case TileType::Object:
							Tile[j][i].Objectx = SelectList[index].x;
							Tile[j][i].Objecty = SelectList[index++].y;
							Tile[j][i].ObjectNum = Page;
							Tile[j][i].ObjectOrder = OrderNum;
							Tile[j][i].show = true;
							break;
						case TileType::Enemy:
							Tile[j][i].Terrain2x = SelectList[index].x;
							Tile[j][i].Terrain2y = SelectList[index++].y;
							Tile[j][i].Terrain2Num = Page;
							Tile[j][i].Terrain2Order = OrderNum;
							Tile[j][i].show = true;
							break;
						}
					}

				}
			}
		}
	}
	else
	{
		startx = (_ptMouse.x / CAMERA->GetZoom() + CAMERA->GetCameraRect().left) / 64;
		starty = (_ptMouse.y / CAMERA->GetZoom() + CAMERA->GetCameraRect().top) / 64;
		if (startx < 0 || starty < 0) return;
		if (starty > YNUM || startx > XNUM) return;
		if (PtInRect(&CAMERA->GetRelativeRect(Tile[startx][starty].rc), _ptMouse.ToPoint())
			&& !IntersectRect(&temp, &CAMERA->GetRelativeRect(Tile[startx][starty].rc), &TilePalette))
		{
			AlphaRect.clear();
			for (int i = starty; i <= starty + MRectSizeReal.y; ++i)
			{
				for (int j = startx; j <= startx + MRectSizeReal.x; ++j)
				{
					AlphaRect.push_back(Tile[j][i].rc);
				}
			}
		}
	}
	Select = false;
}

// ===============================================
// ##############  타일 셋팅 함수 #################
// ===============================================
void MapToolScene::SetTile()
{
	for (int i = 0; i < YNUM; ++i)
	{
		for (int j = 0; j < XNUM; ++j)
		{
			Tile[j][i].rc = RectMake(j * SIZE, i * SIZE, SIZE, SIZE);
		}
	}

	for (int i = 0; i < SAMPLENUMY; ++i) {
		for (int j = 0; j < SAMPLENUMX; ++j) {
			Sample[j][i].rc = RectMake(115 + (WINSIZEX - 450) + j * SAMPLESIZE,
				185 + i * SAMPLESIZE, SAMPLESIZE, SAMPLESIZE);
		}
	}
}

// ===============================================
// ############ 팔렛 보여주기 함수 ################
// ===============================================
void MapToolScene::ShowPalette()
{
	if (Show && TilePalette.left > WINSIZEX - 450)
	{
		TilePalette.left -= Speed;
		TilePalette.right -= Speed;
		Palette.left -= Speed;
		Palette.right -= Speed;
		Speed += 2.f;
		if (TilePalette.left < WINSIZEX - 450)
		{
			TilePalette.left = WINSIZEX - 450;
			TilePalette.right = WINSIZEX;
			Palette.left = WINSIZEX - 415;
			Palette.right = WINSIZEX - 10;
			SampleShow = true;
			Speed = 0.f;
		}
	}

	if (!Show)
	{
		SampleShow = false;
		TilePalette.left += Speed;
		TilePalette.right += Speed;
		Palette.left += Speed;
		Palette.right += Speed;
		Speed += 2.f;
		if (TilePalette.left > WINSIZEX)
		{
			TilePalette.left = WINSIZEX;
			TilePalette.right = WINSIZEX + 450;
			Palette.left = WINSIZEX + 10;
			Palette.right = WINSIZEX + 415;
			Speed = 0.f;

		}
	}
}

// ===============================================
// ############ 타일 보여주기 함수 ################
// ===============================================
void MapToolScene::ShowTile()
{
	for (int i = 0; i < YNUM; ++i) {
		for (int j = 0; j < XNUM; ++j) {
			if (CAMERA->GetCameraRect().left < Tile[j][i].rc.right &&
				CAMERA->GetCameraRect().right > Tile[j][i].rc.left &&
				CAMERA->GetCameraRect().top < Tile[j][i].rc.bottom &&
				CAMERA->GetCameraRect().bottom > Tile[j][i].rc.top)
			{
				if (Tile[j][i].show == true)
				{
					switch (Scene)
					{
					case SceneNum::Town:

						switch (Tile[j][i].TerrainNum)
						{
						case PageNum::One:
							caveImage[0]->SetSize(Vector2(64, 64) * CAMERA->GetZoom());
							caveImage[0]->FrameRender(
								CAMERA->GetRelativeRect(Tile[j][i].rc).left,
								CAMERA->GetRelativeRect(Tile[j][i].rc).top,
								Tile[j][i].Terrainx, Tile[j][i].Terrainy, Pivot::LeftTop);// , CAMERA->GetZoom());
							break;
						case PageNum::Two:
							caveImage[1]->SetSize(Vector2(64, 64) * CAMERA->GetZoom());
							caveImage[1]->FrameRender(
								CAMERA->GetRelativeRect(Tile[j][i].rc).left,
								CAMERA->GetRelativeRect(Tile[j][i].rc).top,
								Tile[j][i].Terrainx, Tile[j][i].Terrainy, Pivot::LeftTop);// , CAMERA->GetZoom());
							break;
						case PageNum::Three:
							caveImage[2]->SetSize(Vector2(64, 64) * CAMERA->GetZoom());
							caveImage[2]->FrameRender(
								CAMERA->GetRelativeRect(Tile[j][i].rc).left,
								CAMERA->GetRelativeRect(Tile[j][i].rc).top,
								Tile[j][i].Terrainx, Tile[j][i].Terrainy, Pivot::LeftTop);// , CAMERA->GetZoom());
							break;
						case PageNum::Four:
							caveImage[3]->SetSize(Vector2(64, 64) * CAMERA->GetZoom());
							caveImage[3]->FrameRender(
								CAMERA->GetRelativeRect(Tile[j][i].rc).left,
								CAMERA->GetRelativeRect(Tile[j][i].rc).top,
								Tile[j][i].Terrainx, Tile[j][i].Terrainy, Pivot::LeftTop);// , CAMERA->GetZoom());
							break;
						case PageNum::Five:
							caveImage[4]->SetSize(Vector2(64, 64) * CAMERA->GetZoom());
							caveImage[4]->FrameRender(
								CAMERA->GetRelativeRect(Tile[j][i].rc).left,
								CAMERA->GetRelativeRect(Tile[j][i].rc).top,
								Tile[j][i].Terrainx, Tile[j][i].Terrainy, Pivot::LeftTop);// , CAMERA->GetZoom());
							break;
						case PageNum::Six:
							caveImage[5]->SetSize(Vector2(64, 64) * CAMERA->GetZoom());
							caveImage[5]->FrameRender(
								CAMERA->GetRelativeRect(Tile[j][i].rc).left,
								CAMERA->GetRelativeRect(Tile[j][i].rc).top,
								Tile[j][i].Terrainx, Tile[j][i].Terrainy, Pivot::LeftTop);// , CAMERA->GetZoom());
							break;
						case PageNum::Seven:
							caveImage[6]->SetSize(Vector2(64, 64) * CAMERA->GetZoom());
							caveImage[6]->FrameRender(
								CAMERA->GetRelativeRect(Tile[j][i].rc).left,
								CAMERA->GetRelativeRect(Tile[j][i].rc).top,
								Tile[j][i].Terrainx, Tile[j][i].Terrainy, Pivot::LeftTop);// , CAMERA->GetZoom());
							break;
						case PageNum::Eight:
							caveImage[7]->SetSize(Vector2(64, 64) * CAMERA->GetZoom());
							caveImage[7]->FrameRender(
								CAMERA->GetRelativeRect(Tile[j][i].rc).left,
								CAMERA->GetRelativeRect(Tile[j][i].rc).top,
								Tile[j][i].Terrainx, Tile[j][i].Terrainy, Pivot::LeftTop);// , CAMERA->GetZoom());
							break;
						case PageNum::Nine:
							caveImage[8]->SetSize(Vector2(64, 64) * CAMERA->GetZoom());
							caveImage[8]->FrameRender(
								CAMERA->GetRelativeRect(Tile[j][i].rc).left,
								CAMERA->GetRelativeRect(Tile[j][i].rc).top,
								Tile[j][i].Terrainx, Tile[j][i].Terrainy, Pivot::LeftTop);// , CAMERA->GetZoom());
							break;
						}

						switch (Tile[j][i].Terrain2Num)
						{
						case PageNum::One:
							caveImage[0]->SetSize(Vector2(64, 64) * CAMERA->GetZoom());
							caveImage[0]->FrameRender(
								CAMERA->GetRelativeRect(Tile[j][i].rc).left,
								CAMERA->GetRelativeRect(Tile[j][i].rc).top,
								Tile[j][i].Terrain2x, Tile[j][i].Terrain2y, Pivot::LeftTop);// , CAMERA->GetZoom());
							break;
						case PageNum::Two:
							caveImage[1]->SetSize(Vector2(64, 64) * CAMERA->GetZoom());
							caveImage[1]->FrameRender(
								CAMERA->GetRelativeRect(Tile[j][i].rc).left,
								CAMERA->GetRelativeRect(Tile[j][i].rc).top,
								Tile[j][i].Terrain2x, Tile[j][i].Terrain2y, Pivot::LeftTop);// , CAMERA->GetZoom());
							break;
						case PageNum::Three:
							caveImage[2]->SetSize(Vector2(64, 64) * CAMERA->GetZoom());
							caveImage[2]->FrameRender(
								CAMERA->GetRelativeRect(Tile[j][i].rc).left,
								CAMERA->GetRelativeRect(Tile[j][i].rc).top,
								Tile[j][i].Terrain2x, Tile[j][i].Terrain2y, Pivot::LeftTop);// , CAMERA->GetZoom());
							break;
						case PageNum::Four:
							caveImage[3]->SetSize(Vector2(64, 64) * CAMERA->GetZoom());
							caveImage[3]->FrameRender(
								CAMERA->GetRelativeRect(Tile[j][i].rc).left,
								CAMERA->GetRelativeRect(Tile[j][i].rc).top,
								Tile[j][i].Terrain2x, Tile[j][i].Terrain2y, Pivot::LeftTop);// , CAMERA->GetZoom());
							break;
						case PageNum::Five:
							caveImage[4]->SetSize(Vector2(64, 64) * CAMERA->GetZoom());
							caveImage[4]->FrameRender(
								CAMERA->GetRelativeRect(Tile[j][i].rc).left,
								CAMERA->GetRelativeRect(Tile[j][i].rc).top,
								Tile[j][i].Terrain2x, Tile[j][i].Terrain2y, Pivot::LeftTop);// , CAMERA->GetZoom());
							break;
						case PageNum::Six:
							caveImage[5]->SetSize(Vector2(64, 64) * CAMERA->GetZoom());
							caveImage[5]->FrameRender(
								CAMERA->GetRelativeRect(Tile[j][i].rc).left,
								CAMERA->GetRelativeRect(Tile[j][i].rc).top,
								Tile[j][i].Terrain2x, Tile[j][i].Terrain2y, Pivot::LeftTop);// , CAMERA->GetZoom());
							break;
						case PageNum::Seven:
							caveImage[6]->SetSize(Vector2(64, 64) * CAMERA->GetZoom());
							caveImage[6]->FrameRender(
								CAMERA->GetRelativeRect(Tile[j][i].rc).left,
								CAMERA->GetRelativeRect(Tile[j][i].rc).top,
								Tile[j][i].Terrain2x, Tile[j][i].Terrain2y, Pivot::LeftTop);// , CAMERA->GetZoom());
							break;
						case PageNum::Eight:
							caveImage[7]->SetSize(Vector2(64, 64) * CAMERA->GetZoom());
							caveImage[7]->FrameRender(
								CAMERA->GetRelativeRect(Tile[j][i].rc).left,
								CAMERA->GetRelativeRect(Tile[j][i].rc).top,
								Tile[j][i].Terrain2x, Tile[j][i].Terrain2y, Pivot::LeftTop);// , CAMERA->GetZoom());
							break;
						case PageNum::Nine:
							caveImage[8]->SetSize(Vector2(64, 64) * CAMERA->GetZoom());
							caveImage[8]->FrameRender(
								CAMERA->GetRelativeRect(Tile[j][i].rc).left,
								CAMERA->GetRelativeRect(Tile[j][i].rc).top,
								Tile[j][i].Terrain2x, Tile[j][i].Terrain2y, Pivot::LeftTop);// , CAMERA->GetZoom());
							break;
						}

						switch (Tile[j][i].ObjectNum)
						{
						case PageNum::One:
							caveImage[0]->SetSize(Vector2(64, 64) * CAMERA->GetZoom());
							caveImage[0]->FrameRender(
								CAMERA->GetRelativeRect(Tile[j][i].rc).left,
								CAMERA->GetRelativeRect(Tile[j][i].rc).top,
								Tile[j][i].Objectx, Tile[j][i].Objecty, Pivot::LeftTop);// , CAMERA->GetZoom());
							break;
						case PageNum::Two:
							caveImage[1]->SetSize(Vector2(64, 64) * CAMERA->GetZoom());
							caveImage[1]->FrameRender(
								CAMERA->GetRelativeRect(Tile[j][i].rc).left,
								CAMERA->GetRelativeRect(Tile[j][i].rc).top,
								Tile[j][i].Objectx, Tile[j][i].Objecty, Pivot::LeftTop);// , CAMERA->GetZoom());
							break;
						case PageNum::Three:
							caveImage[2]->SetSize(Vector2(64, 64) * CAMERA->GetZoom());
							caveImage[2]->FrameRender(
								CAMERA->GetRelativeRect(Tile[j][i].rc).left,
								CAMERA->GetRelativeRect(Tile[j][i].rc).top,
								Tile[j][i].Objectx, Tile[j][i].Objecty, Pivot::LeftTop);// , CAMERA->GetZoom());
							break;
						case PageNum::Four:
							caveImage[3]->SetSize(Vector2(64, 64) * CAMERA->GetZoom());
							caveImage[3]->FrameRender(
								CAMERA->GetRelativeRect(Tile[j][i].rc).left,
								CAMERA->GetRelativeRect(Tile[j][i].rc).top,
								Tile[j][i].Objectx, Tile[j][i].Objecty, Pivot::LeftTop);// , CAMERA->GetZoom());
							break;
						case PageNum::Five:
							caveImage[4]->SetSize(Vector2(64, 64) * CAMERA->GetZoom());
							caveImage[4]->FrameRender(
								CAMERA->GetRelativeRect(Tile[j][i].rc).left,
								CAMERA->GetRelativeRect(Tile[j][i].rc).top,
								Tile[j][i].Objectx, Tile[j][i].Objecty, Pivot::LeftTop);// , CAMERA->GetZoom());
							break;
						case PageNum::Six:
							caveImage[5]->SetSize(Vector2(64, 64) * CAMERA->GetZoom());
							caveImage[5]->FrameRender(
								CAMERA->GetRelativeRect(Tile[j][i].rc).left,
								CAMERA->GetRelativeRect(Tile[j][i].rc).top,
								Tile[j][i].Objectx, Tile[j][i].Objecty, Pivot::LeftTop);// , CAMERA->GetZoom());
							break;
						case PageNum::Seven:
							caveImage[6]->SetSize(Vector2(64, 64) * CAMERA->GetZoom());
							caveImage[6]->FrameRender(
								CAMERA->GetRelativeRect(Tile[j][i].rc).left,
								CAMERA->GetRelativeRect(Tile[j][i].rc).top,
								Tile[j][i].Objectx, Tile[j][i].Objecty, Pivot::LeftTop);// , CAMERA->GetZoom());
							break;
						case PageNum::Eight:
							caveImage[7]->SetSize(Vector2(64, 64) * CAMERA->GetZoom());
							caveImage[7]->FrameRender(
								CAMERA->GetRelativeRect(Tile[j][i].rc).left,
								CAMERA->GetRelativeRect(Tile[j][i].rc).top,
								Tile[j][i].Objectx, Tile[j][i].Objecty, Pivot::LeftTop);// , CAMERA->GetZoom());
							break;
						case PageNum::Nine:
							caveImage[8]->SetSize(Vector2(64, 64) * CAMERA->GetZoom());
							caveImage[8]->FrameRender(
								CAMERA->GetRelativeRect(Tile[j][i].rc).left,
								CAMERA->GetRelativeRect(Tile[j][i].rc).top,
								Tile[j][i].Objectx, Tile[j][i].Objecty, Pivot::LeftTop);// , CAMERA->GetZoom());
							break;
						}


						break;

					// ======================================================================
					// ###################### 슬라임 보스 테마 ###############################
					// ======================================================================
					case SceneNum::Boss1:

						switch (Tile[j][i].TerrainNum)
						{
						case PageNum::One:
							SlimeImage[0]->SetSize(Vector2(64, 64) * CAMERA->GetZoom());
							SlimeImage[0]->FrameRender(
								CAMERA->GetRelativeRect(Tile[j][i].rc).left,
								CAMERA->GetRelativeRect(Tile[j][i].rc).top,
								Tile[j][i].Terrainx, Tile[j][i].Terrainy, Pivot::LeftTop);// , CAMERA->GetZoom());
							break;
						case PageNum::Two:
							SlimeImage[1]->SetSize(Vector2(64, 64) * CAMERA->GetZoom());
							SlimeImage[1]->FrameRender(
								CAMERA->GetRelativeRect(Tile[j][i].rc).left,
								CAMERA->GetRelativeRect(Tile[j][i].rc).top,
								Tile[j][i].Terrainx, Tile[j][i].Terrainy, Pivot::LeftTop);// , CAMERA->GetZoom());
							break;
						case PageNum::Three:
							SlimeImage[2]->SetSize(Vector2(64, 64) * CAMERA->GetZoom());
							SlimeImage[2]->FrameRender(
								CAMERA->GetRelativeRect(Tile[j][i].rc).left,
								CAMERA->GetRelativeRect(Tile[j][i].rc).top,
								Tile[j][i].Terrainx, Tile[j][i].Terrainy, Pivot::LeftTop);// , CAMERA->GetZoom());
							break;
						case PageNum::Four:
							SlimeImage[3]->SetSize(Vector2(64, 64) * CAMERA->GetZoom());
							SlimeImage[3]->FrameRender(
								CAMERA->GetRelativeRect(Tile[j][i].rc).left,
								CAMERA->GetRelativeRect(Tile[j][i].rc).top,
								Tile[j][i].Terrainx, Tile[j][i].Terrainy, Pivot::LeftTop);// , CAMERA->GetZoom());
							break;
						}

						switch (Tile[j][i].Terrain2Num)
						{
						case PageNum::One:
							SlimeImage[0]->SetSize(Vector2(64, 64) * CAMERA->GetZoom());
							SlimeImage[0]->FrameRender(
								CAMERA->GetRelativeRect(Tile[j][i].rc).left,
								CAMERA->GetRelativeRect(Tile[j][i].rc).top,
								Tile[j][i].Terrain2x, Tile[j][i].Terrain2y, Pivot::LeftTop);// , CAMERA->GetZoom());
							break;
						case PageNum::Two:
							SlimeImage[1]->SetSize(Vector2(64, 64) * CAMERA->GetZoom());
							SlimeImage[1]->FrameRender(
								CAMERA->GetRelativeRect(Tile[j][i].rc).left,
								CAMERA->GetRelativeRect(Tile[j][i].rc).top,
								Tile[j][i].Terrain2x, Tile[j][i].Terrain2y, Pivot::LeftTop);// , CAMERA->GetZoom());
							break;
						case PageNum::Three:
							SlimeImage[2]->SetSize(Vector2(64, 64) * CAMERA->GetZoom());
							SlimeImage[2]->FrameRender(
								CAMERA->GetRelativeRect(Tile[j][i].rc).left,
								CAMERA->GetRelativeRect(Tile[j][i].rc).top,
								Tile[j][i].Terrain2x, Tile[j][i].Terrain2y, Pivot::LeftTop);// , CAMERA->GetZoom());
							break;
						case PageNum::Four:
							SlimeImage[3]->SetSize(Vector2(64, 64) * CAMERA->GetZoom());
							SlimeImage[3]->FrameRender(
								CAMERA->GetRelativeRect(Tile[j][i].rc).left,
								CAMERA->GetRelativeRect(Tile[j][i].rc).top,
								Tile[j][i].Terrain2x, Tile[j][i].Terrain2y, Pivot::LeftTop);// , CAMERA->GetZoom());
							break;
						}

						switch (Tile[j][i].ObjectNum)
						{
						case PageNum::One:
							SlimeImage[0]->SetSize(Vector2(64, 64) * CAMERA->GetZoom());
							SlimeImage[0]->FrameRender(
								CAMERA->GetRelativeRect(Tile[j][i].rc).left,
								CAMERA->GetRelativeRect(Tile[j][i].rc).top,
								Tile[j][i].Objectx, Tile[j][i].Objecty, Pivot::LeftTop);// , CAMERA->GetZoom());
							break;
						case PageNum::Two:
							SlimeImage[1]->SetSize(Vector2(64, 64) * CAMERA->GetZoom());
							SlimeImage[1]->FrameRender(
								CAMERA->GetRelativeRect(Tile[j][i].rc).left,
								CAMERA->GetRelativeRect(Tile[j][i].rc).top,
								Tile[j][i].Objectx, Tile[j][i].Objecty, Pivot::LeftTop);// , CAMERA->GetZoom());
							break;
						case PageNum::Three:
							SlimeImage[2]->SetSize(Vector2(64, 64) * CAMERA->GetZoom());
							SlimeImage[2]->FrameRender(
								CAMERA->GetRelativeRect(Tile[j][i].rc).left,
								CAMERA->GetRelativeRect(Tile[j][i].rc).top,
								Tile[j][i].Objectx, Tile[j][i].Objecty, Pivot::LeftTop);// , CAMERA->GetZoom());
							break;
						case PageNum::Four:
							SlimeImage[3]->SetSize(Vector2(64, 64) * CAMERA->GetZoom());
							SlimeImage[3]->FrameRender(
								CAMERA->GetRelativeRect(Tile[j][i].rc).left,
								CAMERA->GetRelativeRect(Tile[j][i].rc).top,
								Tile[j][i].Objectx, Tile[j][i].Objecty, Pivot::LeftTop);// , CAMERA->GetZoom());
							break;
						}








						break;
					// ========================================================================
					// ######################## 얼음 보스 테마 #################################
					// ========================================================================
					case SceneNum::Boss2:

						switch (Tile[j][i].TerrainNum)
						{
						case PageNum::One:
							IceImage[0]->SetSize(Vector2(64, 64) * CAMERA->GetZoom());
							IceImage[0]->FrameRender(
								CAMERA->GetRelativeRect(Tile[j][i].rc).left,
								CAMERA->GetRelativeRect(Tile[j][i].rc).top,
								Tile[j][i].Terrainx, Tile[j][i].Terrainy, Pivot::LeftTop);// , CAMERA->GetZoom());
							break;
						case PageNum::Two:
							IceImage[1]->SetSize(Vector2(64, 64) * CAMERA->GetZoom());
							IceImage[1]->FrameRender(
								CAMERA->GetRelativeRect(Tile[j][i].rc).left,
								CAMERA->GetRelativeRect(Tile[j][i].rc).top,
								Tile[j][i].Terrainx, Tile[j][i].Terrainy, Pivot::LeftTop);// , CAMERA->GetZoom());
							break;
						case PageNum::Three:
							IceImage[2]->SetSize(Vector2(64, 64) * CAMERA->GetZoom());
							IceImage[2]->FrameRender(
								CAMERA->GetRelativeRect(Tile[j][i].rc).left,
								CAMERA->GetRelativeRect(Tile[j][i].rc).top,
								Tile[j][i].Terrainx, Tile[j][i].Terrainy, Pivot::LeftTop);// , CAMERA->GetZoom());
							break;
						case PageNum::Four:
							IceImage[3]->SetSize(Vector2(64, 64) * CAMERA->GetZoom());
							IceImage[3]->FrameRender(
								CAMERA->GetRelativeRect(Tile[j][i].rc).left,
								CAMERA->GetRelativeRect(Tile[j][i].rc).top,
								Tile[j][i].Terrainx, Tile[j][i].Terrainy, Pivot::LeftTop);// , CAMERA->GetZoom());
							break;
						case PageNum::Five:
							IceImage[4]->SetSize(Vector2(64, 64) * CAMERA->GetZoom());
							IceImage[4]->FrameRender(
								CAMERA->GetRelativeRect(Tile[j][i].rc).left,
								CAMERA->GetRelativeRect(Tile[j][i].rc).top,
								Tile[j][i].Terrainx, Tile[j][i].Terrainy, Pivot::LeftTop);// , CAMERA->GetZoom());
							break;
						case PageNum::Six:
							IceImage[5]->SetSize(Vector2(64, 64) * CAMERA->GetZoom());
							IceImage[5]->FrameRender(
								CAMERA->GetRelativeRect(Tile[j][i].rc).left,
								CAMERA->GetRelativeRect(Tile[j][i].rc).top,
								Tile[j][i].Terrainx, Tile[j][i].Terrainy, Pivot::LeftTop);// , CAMERA->GetZoom());
							break;
						}

						switch (Tile[j][i].Terrain2Num)
						{
						case PageNum::One:
							IceImage[0]->SetSize(Vector2(64, 64) * CAMERA->GetZoom());
							IceImage[0]->FrameRender(
								CAMERA->GetRelativeRect(Tile[j][i].rc).left,
								CAMERA->GetRelativeRect(Tile[j][i].rc).top,
								Tile[j][i].Terrain2x, Tile[j][i].Terrain2y, Pivot::LeftTop);// , CAMERA->GetZoom());
							break;
						case PageNum::Two:
							IceImage[1]->SetSize(Vector2(64, 64) * CAMERA->GetZoom());
							IceImage[1]->FrameRender(
								CAMERA->GetRelativeRect(Tile[j][i].rc).left,
								CAMERA->GetRelativeRect(Tile[j][i].rc).top,
								Tile[j][i].Terrain2x, Tile[j][i].Terrain2y, Pivot::LeftTop);// , CAMERA->GetZoom());
							break;
						case PageNum::Three:
							IceImage[2]->SetSize(Vector2(64, 64) * CAMERA->GetZoom());
							IceImage[2]->FrameRender(
								CAMERA->GetRelativeRect(Tile[j][i].rc).left,
								CAMERA->GetRelativeRect(Tile[j][i].rc).top,
								Tile[j][i].Terrain2x, Tile[j][i].Terrain2y, Pivot::LeftTop);// , CAMERA->GetZoom());
							break;
						case PageNum::Four:
							IceImage[3]->SetSize(Vector2(64, 64) * CAMERA->GetZoom());
							IceImage[3]->FrameRender(
								CAMERA->GetRelativeRect(Tile[j][i].rc).left,
								CAMERA->GetRelativeRect(Tile[j][i].rc).top,
								Tile[j][i].Terrain2x, Tile[j][i].Terrain2y, Pivot::LeftTop);// , CAMERA->GetZoom());
							break;
						case PageNum::Five:
							IceImage[4]->SetSize(Vector2(64, 64) * CAMERA->GetZoom());
							IceImage[4]->FrameRender(
								CAMERA->GetRelativeRect(Tile[j][i].rc).left,
								CAMERA->GetRelativeRect(Tile[j][i].rc).top,
								Tile[j][i].Terrain2x, Tile[j][i].Terrain2y, Pivot::LeftTop);// , CAMERA->GetZoom());
							break;
						case PageNum::Six:
							IceImage[5]->SetSize(Vector2(64, 64) * CAMERA->GetZoom());
							IceImage[5]->FrameRender(
								CAMERA->GetRelativeRect(Tile[j][i].rc).left,
								CAMERA->GetRelativeRect(Tile[j][i].rc).top,
								Tile[j][i].Terrain2x, Tile[j][i].Terrain2y, Pivot::LeftTop);// , CAMERA->GetZoom());
							break;
						}

						switch (Tile[j][i].ObjectNum)
						{
						case PageNum::One:
							IceImage[0]->SetSize(Vector2(64, 64) * CAMERA->GetZoom());
							IceImage[0]->FrameRender(
								CAMERA->GetRelativeRect(Tile[j][i].rc).left,
								CAMERA->GetRelativeRect(Tile[j][i].rc).top,
								Tile[j][i].Objectx, Tile[j][i].Objecty, Pivot::LeftTop);// , CAMERA->GetZoom());
							break;
						case PageNum::Two:
							IceImage[1]->SetSize(Vector2(64, 64) * CAMERA->GetZoom());
							IceImage[1]->FrameRender(
								CAMERA->GetRelativeRect(Tile[j][i].rc).left,
								CAMERA->GetRelativeRect(Tile[j][i].rc).top,
								Tile[j][i].Objectx, Tile[j][i].Objecty, Pivot::LeftTop);// , CAMERA->GetZoom());
							break;
						case PageNum::Three:
							IceImage[2]->SetSize(Vector2(64, 64) * CAMERA->GetZoom());
							IceImage[2]->FrameRender(
								CAMERA->GetRelativeRect(Tile[j][i].rc).left,
								CAMERA->GetRelativeRect(Tile[j][i].rc).top,
								Tile[j][i].Objectx, Tile[j][i].Objecty, Pivot::LeftTop);// , CAMERA->GetZoom());
							break;
						case PageNum::Four:
							IceImage[3]->SetSize(Vector2(64, 64) * CAMERA->GetZoom());
							IceImage[3]->FrameRender(
								CAMERA->GetRelativeRect(Tile[j][i].rc).left,
								CAMERA->GetRelativeRect(Tile[j][i].rc).top,
								Tile[j][i].Objectx, Tile[j][i].Objecty, Pivot::LeftTop);// , CAMERA->GetZoom());
							break;
						case PageNum::Five:
							IceImage[4]->SetSize(Vector2(64, 64) * CAMERA->GetZoom());
							IceImage[4]->FrameRender(
								CAMERA->GetRelativeRect(Tile[j][i].rc).left,
								CAMERA->GetRelativeRect(Tile[j][i].rc).top,
								Tile[j][i].Objectx, Tile[j][i].Objecty, Pivot::LeftTop);// , CAMERA->GetZoom());
							break;
						case PageNum::Six:
							IceImage[5]->SetSize(Vector2(64, 64) * CAMERA->GetZoom());
							IceImage[5]->FrameRender(
								CAMERA->GetRelativeRect(Tile[j][i].rc).left,
								CAMERA->GetRelativeRect(Tile[j][i].rc).top,
								Tile[j][i].Objectx, Tile[j][i].Objecty, Pivot::LeftTop);// , CAMERA->GetZoom());
							break;
						}




						break;
					case SceneNum::Boss3:










						break;
					}
					
				}
				else
				{
					D2DRENDERER->DrawRectangle(CAMERA->GetRelativeRect(Tile[j][i].rc),
						D2DRenderer::DefaultBrush::Red);
				}
			}
		}
	}
	// =====================================================================================
	// ################################ 깔려는 타일 보여주기 ###############################
	// =====================================================================================
	// ------------------------------- 알파 랜더 -------------------------------------------
	if (AlphaRect.size() > 0)
	{
		switch (Scene)
		{
		case SceneNum::Town:

			switch (Type)
			{
			case TileType::Terrain:
				switch (Page)
				{
				case PageNum::One:
					caveImage[0]->SetSize(Vector2((MRectSizeReal.x + 1) * 64, (MRectSizeReal.y + 1) * 64) * CAMERA->GetZoom());
					caveImage[0]->SetAlpha(0.4f);
					caveImage[0]->FrameRender2(
						CAMERA->GetRelativeRect(AlphaRect[0]).left,
						CAMERA->GetRelativeRect(AlphaRect[0]).top,
						SelectList[0].x, SelectList[0].y, (MRectSizeReal.x + 1) * 64,
						(MRectSizeReal.y + 1) * 64, Pivot::LeftTop);
					break;
				case PageNum::Two:
					caveImage[1]->SetSize(Vector2((MRectSizeReal.x + 1) * 64, (MRectSizeReal.y + 1) * 64) * CAMERA->GetZoom());
					caveImage[1]->SetAlpha(0.4f);
					caveImage[1]->FrameRender2(
						CAMERA->GetRelativeRect(AlphaRect[0]).left,
						CAMERA->GetRelativeRect(AlphaRect[0]).top,
						SelectList[0].x, SelectList[0].y, (MRectSizeReal.x + 1) * 64,
						(MRectSizeReal.y + 1) * 64, Pivot::LeftTop);
					break;
				case PageNum::Three:
					caveImage[2]->SetSize(Vector2((MRectSizeReal.x + 1) * 64, (MRectSizeReal.y + 1) * 64) * CAMERA->GetZoom());
					caveImage[2]->SetAlpha(0.4f);
					caveImage[2]->FrameRender2(
						CAMERA->GetRelativeRect(AlphaRect[0]).left,
						CAMERA->GetRelativeRect(AlphaRect[0]).top,
						SelectList[0].x, SelectList[0].y, (MRectSizeReal.x + 1) * 64,
						(MRectSizeReal.y + 1) * 64, Pivot::LeftTop);
					break;
				case PageNum::Four:
					caveImage[3]->SetSize(Vector2((MRectSizeReal.x + 1) * 64, (MRectSizeReal.y + 1) * 64) * CAMERA->GetZoom());
					caveImage[3]->SetAlpha(0.4f);
					caveImage[3]->FrameRender2(
						CAMERA->GetRelativeRect(AlphaRect[0]).left,
						CAMERA->GetRelativeRect(AlphaRect[0]).top,
						SelectList[0].x, SelectList[0].y, (MRectSizeReal.x + 1) * 64,
						(MRectSizeReal.y + 1) * 64, Pivot::LeftTop);
					break;
				case PageNum::Five:
					caveImage[4]->SetSize(Vector2((MRectSizeReal.x + 1) * 64, (MRectSizeReal.y + 1) * 64) * CAMERA->GetZoom());
					caveImage[4]->SetAlpha(0.4f);
					caveImage[4]->FrameRender2(
						CAMERA->GetRelativeRect(AlphaRect[0]).left,
						CAMERA->GetRelativeRect(AlphaRect[0]).top,
						SelectList[0].x, SelectList[0].y, (MRectSizeReal.x + 1) * 64,
						(MRectSizeReal.y + 1) * 64, Pivot::LeftTop);
					break;
				case PageNum::Six:
					caveImage[5]->SetSize(Vector2((MRectSizeReal.x + 1) * 64, (MRectSizeReal.y + 1) * 64) * CAMERA->GetZoom());
					caveImage[5]->SetAlpha(0.4f);
					caveImage[5]->FrameRender2(
						CAMERA->GetRelativeRect(AlphaRect[0]).left,
						CAMERA->GetRelativeRect(AlphaRect[0]).top,
						SelectList[0].x, SelectList[0].y, (MRectSizeReal.x + 1) * 64,
						(MRectSizeReal.y + 1) * 64, Pivot::LeftTop);
					break;
				case PageNum::Seven:
					caveImage[6]->SetSize(Vector2((MRectSizeReal.x + 1) * 64, (MRectSizeReal.y + 1) * 64) * CAMERA->GetZoom());
					caveImage[6]->SetAlpha(0.4f);
					caveImage[6]->FrameRender2(
						CAMERA->GetRelativeRect(AlphaRect[0]).left,
						CAMERA->GetRelativeRect(AlphaRect[0]).top,
						SelectList[0].x, SelectList[0].y, (MRectSizeReal.x + 1) * 64,
						(MRectSizeReal.y + 1) * 64, Pivot::LeftTop);
					break;
				case PageNum::Eight:
					caveImage[7]->SetSize(Vector2((MRectSizeReal.x + 1) * 64, (MRectSizeReal.y + 1) * 64) * CAMERA->GetZoom());
					caveImage[7]->SetAlpha(0.4f);
					caveImage[7]->FrameRender2(
						CAMERA->GetRelativeRect(AlphaRect[0]).left,
						CAMERA->GetRelativeRect(AlphaRect[0]).top,
						SelectList[0].x, SelectList[0].y, (MRectSizeReal.x + 1) * 64,
						(MRectSizeReal.y + 1) * 64, Pivot::LeftTop);
					break;
				case PageNum::Nine:
					caveImage[8]->SetSize(Vector2((MRectSizeReal.x + 1) * 64, (MRectSizeReal.y + 1) * 64) * CAMERA->GetZoom());
					caveImage[8]->SetAlpha(0.4f);
					caveImage[8]->FrameRender2(
						CAMERA->GetRelativeRect(AlphaRect[0]).left,
						CAMERA->GetRelativeRect(AlphaRect[0]).top,
						SelectList[0].x, SelectList[0].y, (MRectSizeReal.x + 1) * 64,
						(MRectSizeReal.y + 1) * 64, Pivot::LeftTop);
					break;
				}
				break;
			case TileType::Enemy:
				switch (Page)
				{
				case PageNum::One:
					caveImage[0]->SetSize(Vector2((MRectSizeReal.x + 1) * 64, (MRectSizeReal.y + 1) * 64) * CAMERA->GetZoom());
					caveImage[0]->SetAlpha(0.4f);
					caveImage[0]->FrameRender2(
						CAMERA->GetRelativeRect(AlphaRect[0]).left,
						CAMERA->GetRelativeRect(AlphaRect[0]).top,
						SelectList[0].x, SelectList[0].y, (MRectSizeReal.x + 1) * 64,
						(MRectSizeReal.y + 1) * 64, Pivot::LeftTop);
					break;
				case PageNum::Two:
					caveImage[1]->SetSize(Vector2((MRectSizeReal.x + 1) * 64, (MRectSizeReal.y + 1) * 64) * CAMERA->GetZoom());
					caveImage[1]->SetAlpha(0.4f);
					caveImage[1]->FrameRender2(
						CAMERA->GetRelativeRect(AlphaRect[0]).left,
						CAMERA->GetRelativeRect(AlphaRect[0]).top,
						SelectList[0].x, SelectList[0].y, (MRectSizeReal.x + 1) * 64,
						(MRectSizeReal.y + 1) * 64, Pivot::LeftTop);
					break;
				case PageNum::Three:
					caveImage[2]->SetSize(Vector2((MRectSizeReal.x + 1) * 64, (MRectSizeReal.y + 1) * 64) * CAMERA->GetZoom());
					caveImage[2]->SetAlpha(0.4f);
					caveImage[2]->FrameRender2(
						CAMERA->GetRelativeRect(AlphaRect[0]).left,
						CAMERA->GetRelativeRect(AlphaRect[0]).top,
						SelectList[0].x, SelectList[0].y, (MRectSizeReal.x + 1) * 64,
						(MRectSizeReal.y + 1) * 64, Pivot::LeftTop);
					break;
				case PageNum::Four:
					caveImage[3]->SetSize(Vector2((MRectSizeReal.x + 1) * 64, (MRectSizeReal.y + 1) * 64) * CAMERA->GetZoom());
					caveImage[3]->SetAlpha(0.4f);
					caveImage[3]->FrameRender2(
						CAMERA->GetRelativeRect(AlphaRect[0]).left,
						CAMERA->GetRelativeRect(AlphaRect[0]).top,
						SelectList[0].x, SelectList[0].y, (MRectSizeReal.x + 1) * 64,
						(MRectSizeReal.y + 1) * 64, Pivot::LeftTop);
					break;
				case PageNum::Five:
					caveImage[4]->SetSize(Vector2((MRectSizeReal.x + 1) * 64, (MRectSizeReal.y + 1) * 64) * CAMERA->GetZoom());
					caveImage[4]->SetAlpha(0.4f);
					caveImage[4]->FrameRender2(
						CAMERA->GetRelativeRect(AlphaRect[0]).left,
						CAMERA->GetRelativeRect(AlphaRect[0]).top,
						SelectList[0].x, SelectList[0].y, (MRectSizeReal.x + 1) * 64,
						(MRectSizeReal.y + 1) * 64, Pivot::LeftTop);

					break;
				case PageNum::Six:
					caveImage[5]->SetSize(Vector2((MRectSizeReal.x + 1) * 64, (MRectSizeReal.y + 1) * 64) * CAMERA->GetZoom());
					caveImage[5]->SetAlpha(0.4f);
					caveImage[5]->FrameRender2(
						CAMERA->GetRelativeRect(AlphaRect[0]).left,
						CAMERA->GetRelativeRect(AlphaRect[0]).top,
						SelectList[0].x, SelectList[0].y, (MRectSizeReal.x + 1) * 64,
						(MRectSizeReal.y + 1) * 64, Pivot::LeftTop);
					break;
				case PageNum::Seven:
					caveImage[6]->SetSize(Vector2((MRectSizeReal.x + 1) * 64, (MRectSizeReal.y + 1) * 64) * CAMERA->GetZoom());
					caveImage[6]->SetAlpha(0.4f);
					caveImage[6]->FrameRender2(
						CAMERA->GetRelativeRect(AlphaRect[0]).left,
						CAMERA->GetRelativeRect(AlphaRect[0]).top,
						SelectList[0].x, SelectList[0].y, (MRectSizeReal.x + 1) * 64,
						(MRectSizeReal.y + 1) * 64, Pivot::LeftTop);
					break;
				case PageNum::Eight:
					caveImage[7]->SetSize(Vector2((MRectSizeReal.x + 1) * 64, (MRectSizeReal.y + 1) * 64) * CAMERA->GetZoom());
					caveImage[7]->SetAlpha(0.4f);
					caveImage[7]->FrameRender2(
						CAMERA->GetRelativeRect(AlphaRect[0]).left,
						CAMERA->GetRelativeRect(AlphaRect[0]).top,
						SelectList[0].x, SelectList[0].y, (MRectSizeReal.x + 1) * 64,
						(MRectSizeReal.y + 1) * 64, Pivot::LeftTop);
					break;
				case PageNum::Nine:
					caveImage[8]->SetSize(Vector2((MRectSizeReal.x + 1) * 64, (MRectSizeReal.y + 1) * 64) * CAMERA->GetZoom());
					caveImage[8]->SetAlpha(0.4f);
					caveImage[8]->FrameRender2(
						CAMERA->GetRelativeRect(AlphaRect[0]).left,
						CAMERA->GetRelativeRect(AlphaRect[0]).top,
						SelectList[0].x, SelectList[0].y, (MRectSizeReal.x + 1) * 64,
						(MRectSizeReal.y + 1) * 64, Pivot::LeftTop);
					break;
				}

			case TileType::Object:
				switch (Page)
				{
				case PageNum::One:
					caveImage[0]->SetSize(Vector2((MRectSizeReal.x + 1) * 64, (MRectSizeReal.y + 1) * 64) * CAMERA->GetZoom());
					caveImage[0]->SetAlpha(0.4f);
					caveImage[0]->FrameRender2(
						CAMERA->GetRelativeRect(AlphaRect[0]).left,
						CAMERA->GetRelativeRect(AlphaRect[0]).top,
						SelectList[0].x, SelectList[0].y, (MRectSizeReal.x + 1) * 64,
						(MRectSizeReal.y + 1) * 64, Pivot::LeftTop);
					break;
				case PageNum::Two:
					caveImage[1]->SetSize(Vector2((MRectSizeReal.x + 1) * 64, (MRectSizeReal.y + 1) * 64) * CAMERA->GetZoom());
					caveImage[1]->SetAlpha(0.4f);
					caveImage[1]->FrameRender2(
						CAMERA->GetRelativeRect(AlphaRect[0]).left,
						CAMERA->GetRelativeRect(AlphaRect[0]).top,
						SelectList[0].x, SelectList[0].y, (MRectSizeReal.x + 1) * 64,
						(MRectSizeReal.y + 1) * 64, Pivot::LeftTop);
					break;
				case PageNum::Three:
					caveImage[2]->SetSize(Vector2((MRectSizeReal.x + 1) * 64, (MRectSizeReal.y + 1) * 64) * CAMERA->GetZoom());
					caveImage[2]->SetAlpha(0.4f);
					caveImage[2]->FrameRender2(
						CAMERA->GetRelativeRect(AlphaRect[0]).left,
						CAMERA->GetRelativeRect(AlphaRect[0]).top,
						SelectList[0].x, SelectList[0].y, (MRectSizeReal.x + 1) * 64,
						(MRectSizeReal.y + 1) * 64, Pivot::LeftTop);
					break;
				case PageNum::Four:
					caveImage[3]->SetSize(Vector2((MRectSizeReal.x + 1) * 64, (MRectSizeReal.y + 1) * 64) * CAMERA->GetZoom());
					caveImage[3]->SetAlpha(0.4f);
					caveImage[3]->FrameRender2(
						CAMERA->GetRelativeRect(AlphaRect[0]).left,
						CAMERA->GetRelativeRect(AlphaRect[0]).top,
						SelectList[0].x, SelectList[0].y, (MRectSizeReal.x + 1) * 64,
						(MRectSizeReal.y + 1) * 64, Pivot::LeftTop);
					break;
				case PageNum::Five:
					caveImage[4]->SetSize(Vector2((MRectSizeReal.x + 1) * 64, (MRectSizeReal.y + 1) * 64) * CAMERA->GetZoom());
					caveImage[4]->SetAlpha(0.4f);
					caveImage[4]->FrameRender2(
						CAMERA->GetRelativeRect(AlphaRect[0]).left,
						CAMERA->GetRelativeRect(AlphaRect[0]).top,
						SelectList[0].x, SelectList[0].y, (MRectSizeReal.x + 1) * 64,
						(MRectSizeReal.y + 1) * 64, Pivot::LeftTop);

					break;
				case PageNum::Six:
					caveImage[5]->SetSize(Vector2((MRectSizeReal.x + 1) * 64, (MRectSizeReal.y + 1) * 64) * CAMERA->GetZoom());
					caveImage[5]->SetAlpha(0.4f);
					caveImage[5]->FrameRender2(
						CAMERA->GetRelativeRect(AlphaRect[0]).left,
						CAMERA->GetRelativeRect(AlphaRect[0]).top,
						SelectList[0].x, SelectList[0].y, (MRectSizeReal.x + 1) * 64,
						(MRectSizeReal.y + 1) * 64, Pivot::LeftTop);
					break;
				case PageNum::Seven:
					caveImage[6]->SetSize(Vector2((MRectSizeReal.x + 1) * 64, (MRectSizeReal.y + 1) * 64) * CAMERA->GetZoom());
					caveImage[6]->SetAlpha(0.4f);
					caveImage[6]->FrameRender2(
						CAMERA->GetRelativeRect(AlphaRect[0]).left,
						CAMERA->GetRelativeRect(AlphaRect[0]).top,
						SelectList[0].x, SelectList[0].y, (MRectSizeReal.x + 1) * 64,
						(MRectSizeReal.y + 1) * 64, Pivot::LeftTop);
					break;
				case PageNum::Eight:
					caveImage[7]->SetSize(Vector2((MRectSizeReal.x + 1) * 64, (MRectSizeReal.y + 1) * 64) * CAMERA->GetZoom());
					caveImage[7]->SetAlpha(0.4f);
					caveImage[7]->FrameRender2(
						CAMERA->GetRelativeRect(AlphaRect[0]).left,
						CAMERA->GetRelativeRect(AlphaRect[0]).top,
						SelectList[0].x, SelectList[0].y, (MRectSizeReal.x + 1) * 64,
						(MRectSizeReal.y + 1) * 64, Pivot::LeftTop);
					break;
				case PageNum::Nine:
					caveImage[8]->SetSize(Vector2((MRectSizeReal.x + 1) * 64, (MRectSizeReal.y + 1) * 64) * CAMERA->GetZoom());
					caveImage[8]->SetAlpha(0.4f);
					caveImage[8]->FrameRender2(
						CAMERA->GetRelativeRect(AlphaRect[0]).left,
						CAMERA->GetRelativeRect(AlphaRect[0]).top,
						SelectList[0].x, SelectList[0].y, (MRectSizeReal.x + 1) * 64,
						(MRectSizeReal.y + 1) * 64, Pivot::LeftTop);
					break;
				}
			}

			break;
		case SceneNum::Boss1:

			switch (Type)
			{
			case TileType::Terrain:
				switch (Page)
				{
				case PageNum::One:
					SlimeImage[0]->SetSize(Vector2((MRectSizeReal.x + 1) * 64, (MRectSizeReal.y + 1) * 64) * CAMERA->GetZoom());
					SlimeImage[0]->SetAlpha(0.4f);
					SlimeImage[0]->FrameRender2(
						CAMERA->GetRelativeRect(AlphaRect[0]).left,
						CAMERA->GetRelativeRect(AlphaRect[0]).top,
						SelectList[0].x, SelectList[0].y, (MRectSizeReal.x + 1) * 64,
						(MRectSizeReal.y + 1) * 64, Pivot::LeftTop);
					break;
				case PageNum::Two:
					SlimeImage[1]->SetSize(Vector2((MRectSizeReal.x + 1) * 64, (MRectSizeReal.y + 1) * 64) * CAMERA->GetZoom());
					SlimeImage[1]->SetAlpha(0.4f);
					SlimeImage[1]->FrameRender2(
						CAMERA->GetRelativeRect(AlphaRect[0]).left,
						CAMERA->GetRelativeRect(AlphaRect[0]).top,
						SelectList[0].x, SelectList[0].y, (MRectSizeReal.x + 1) * 64,
						(MRectSizeReal.y + 1) * 64, Pivot::LeftTop);
					break;
				case PageNum::Three:
					SlimeImage[2]->SetSize(Vector2((MRectSizeReal.x + 1) * 64, (MRectSizeReal.y + 1) * 64) * CAMERA->GetZoom());
					SlimeImage[2]->SetAlpha(0.4f);
					SlimeImage[2]->FrameRender2(
						CAMERA->GetRelativeRect(AlphaRect[0]).left,
						CAMERA->GetRelativeRect(AlphaRect[0]).top,
						SelectList[0].x, SelectList[0].y, (MRectSizeReal.x + 1) * 64,
						(MRectSizeReal.y + 1) * 64, Pivot::LeftTop);
					break;
				case PageNum::Four:
					SlimeImage[3]->SetSize(Vector2((MRectSizeReal.x + 1) * 64, (MRectSizeReal.y + 1) * 64) * CAMERA->GetZoom());
					SlimeImage[3]->SetAlpha(0.4f);
					SlimeImage[3]->FrameRender2(
						CAMERA->GetRelativeRect(AlphaRect[0]).left,
						CAMERA->GetRelativeRect(AlphaRect[0]).top,
						SelectList[0].x, SelectList[0].y, (MRectSizeReal.x + 1) * 64,
						(MRectSizeReal.y + 1) * 64, Pivot::LeftTop);
					break;
				}
				break;
			case TileType::Enemy:
				switch (Page)
				{
				case PageNum::One:
					SlimeImage[0]->SetSize(Vector2((MRectSizeReal.x + 1) * 64, (MRectSizeReal.y + 1) * 64) * CAMERA->GetZoom());
					SlimeImage[0]->SetAlpha(0.4f);
					SlimeImage[0]->FrameRender2(
						CAMERA->GetRelativeRect(AlphaRect[0]).left,
						CAMERA->GetRelativeRect(AlphaRect[0]).top,
						SelectList[0].x, SelectList[0].y, (MRectSizeReal.x + 1) * 64,
						(MRectSizeReal.y + 1) * 64, Pivot::LeftTop);
					break;
				case PageNum::Two:
					SlimeImage[1]->SetSize(Vector2((MRectSizeReal.x + 1) * 64, (MRectSizeReal.y + 1) * 64) * CAMERA->GetZoom());
					SlimeImage[1]->SetAlpha(0.4f);
					SlimeImage[1]->FrameRender2(
						CAMERA->GetRelativeRect(AlphaRect[0]).left,
						CAMERA->GetRelativeRect(AlphaRect[0]).top,
						SelectList[0].x, SelectList[0].y, (MRectSizeReal.x + 1) * 64,
						(MRectSizeReal.y + 1) * 64, Pivot::LeftTop);
					break;
				case PageNum::Three:
					SlimeImage[2]->SetSize(Vector2((MRectSizeReal.x + 1) * 64, (MRectSizeReal.y + 1) * 64) * CAMERA->GetZoom());
					SlimeImage[2]->SetAlpha(0.4f);
					SlimeImage[2]->FrameRender2(
						CAMERA->GetRelativeRect(AlphaRect[0]).left,
						CAMERA->GetRelativeRect(AlphaRect[0]).top,
						SelectList[0].x, SelectList[0].y, (MRectSizeReal.x + 1) * 64,
						(MRectSizeReal.y + 1) * 64, Pivot::LeftTop);
					break;
				case PageNum::Four:
					SlimeImage[3]->SetSize(Vector2((MRectSizeReal.x + 1) * 64, (MRectSizeReal.y + 1) * 64) * CAMERA->GetZoom());
					SlimeImage[3]->SetAlpha(0.4f);
					SlimeImage[3]->FrameRender2(
						CAMERA->GetRelativeRect(AlphaRect[0]).left,
						CAMERA->GetRelativeRect(AlphaRect[0]).top,
						SelectList[0].x, SelectList[0].y, (MRectSizeReal.x + 1) * 64,
						(MRectSizeReal.y + 1) * 64, Pivot::LeftTop);
					break;
				}

			case TileType::Object:
				switch (Page)
				{
				case PageNum::One:
					SlimeImage[0]->SetSize(Vector2((MRectSizeReal.x + 1) * 64, (MRectSizeReal.y + 1) * 64) * CAMERA->GetZoom());
					SlimeImage[0]->SetAlpha(0.4f);
					SlimeImage[0]->FrameRender2(
						CAMERA->GetRelativeRect(AlphaRect[0]).left,
						CAMERA->GetRelativeRect(AlphaRect[0]).top,
						SelectList[0].x, SelectList[0].y, (MRectSizeReal.x + 1) * 64,
						(MRectSizeReal.y + 1) * 64, Pivot::LeftTop);
					break;
				case PageNum::Two:
					SlimeImage[1]->SetSize(Vector2((MRectSizeReal.x + 1) * 64, (MRectSizeReal.y + 1) * 64) * CAMERA->GetZoom());
					SlimeImage[1]->SetAlpha(0.4f);
					SlimeImage[1]->FrameRender2(
						CAMERA->GetRelativeRect(AlphaRect[0]).left,
						CAMERA->GetRelativeRect(AlphaRect[0]).top,
						SelectList[0].x, SelectList[0].y, (MRectSizeReal.x + 1) * 64,
						(MRectSizeReal.y + 1) * 64, Pivot::LeftTop);
					break;
				case PageNum::Three:
					SlimeImage[2]->SetSize(Vector2((MRectSizeReal.x + 1) * 64, (MRectSizeReal.y + 1) * 64) * CAMERA->GetZoom());
					SlimeImage[2]->SetAlpha(0.4f);
					SlimeImage[2]->FrameRender2(
						CAMERA->GetRelativeRect(AlphaRect[0]).left,
						CAMERA->GetRelativeRect(AlphaRect[0]).top,
						SelectList[0].x, SelectList[0].y, (MRectSizeReal.x + 1) * 64,
						(MRectSizeReal.y + 1) * 64, Pivot::LeftTop);
					break;
				case PageNum::Four:
					SlimeImage[3]->SetSize(Vector2((MRectSizeReal.x + 1) * 64, (MRectSizeReal.y + 1) * 64) * CAMERA->GetZoom());
					SlimeImage[3]->SetAlpha(0.4f);
					SlimeImage[3]->FrameRender2(
						CAMERA->GetRelativeRect(AlphaRect[0]).left,
						CAMERA->GetRelativeRect(AlphaRect[0]).top,
						SelectList[0].x, SelectList[0].y, (MRectSizeReal.x + 1) * 64,
						(MRectSizeReal.y + 1) * 64, Pivot::LeftTop);
					break;
				}
			}
			break;
		case SceneNum::Boss2:

			switch (Type)
			{
			case TileType::Terrain:
				switch (Page)
				{
				case PageNum::One:
					IceImage[0]->SetSize(Vector2((MRectSizeReal.x + 1) * 64, (MRectSizeReal.y + 1) * 64) * CAMERA->GetZoom());
					IceImage[0]->SetAlpha(0.4f);
					IceImage[0]->FrameRender2(
						CAMERA->GetRelativeRect(AlphaRect[0]).left,
						CAMERA->GetRelativeRect(AlphaRect[0]).top,
						SelectList[0].x, SelectList[0].y, (MRectSizeReal.x + 1) * 64,
						(MRectSizeReal.y + 1) * 64, Pivot::LeftTop);
					break;
				case PageNum::Two:
					IceImage[1]->SetSize(Vector2((MRectSizeReal.x + 1) * 64, (MRectSizeReal.y + 1) * 64) * CAMERA->GetZoom());
					IceImage[1]->SetAlpha(0.4f);
					IceImage[1]->FrameRender2(
						CAMERA->GetRelativeRect(AlphaRect[0]).left,
						CAMERA->GetRelativeRect(AlphaRect[0]).top,
						SelectList[0].x, SelectList[0].y, (MRectSizeReal.x + 1) * 64,
						(MRectSizeReal.y + 1) * 64, Pivot::LeftTop);
					break;
				case PageNum::Three:
					IceImage[2]->SetSize(Vector2((MRectSizeReal.x + 1) * 64, (MRectSizeReal.y + 1) * 64) * CAMERA->GetZoom());
					IceImage[2]->SetAlpha(0.4f);
					IceImage[2]->FrameRender2(
						CAMERA->GetRelativeRect(AlphaRect[0]).left,
						CAMERA->GetRelativeRect(AlphaRect[0]).top,
						SelectList[0].x, SelectList[0].y, (MRectSizeReal.x + 1) * 64,
						(MRectSizeReal.y + 1) * 64, Pivot::LeftTop);
					break;
				case PageNum::Four:
					IceImage[3]->SetSize(Vector2((MRectSizeReal.x + 1) * 64, (MRectSizeReal.y + 1) * 64) * CAMERA->GetZoom());
					IceImage[3]->SetAlpha(0.4f);
					IceImage[3]->FrameRender2(
						CAMERA->GetRelativeRect(AlphaRect[0]).left,
						CAMERA->GetRelativeRect(AlphaRect[0]).top,
						SelectList[0].x, SelectList[0].y, (MRectSizeReal.x + 1) * 64,
						(MRectSizeReal.y + 1) * 64, Pivot::LeftTop);
					break;
				case PageNum::Five:
					IceImage[4]->SetSize(Vector2((MRectSizeReal.x + 1) * 64, (MRectSizeReal.y + 1) * 64) * CAMERA->GetZoom());
					IceImage[4]->SetAlpha(0.4f);
					IceImage[4]->FrameRender2(
						CAMERA->GetRelativeRect(AlphaRect[0]).left,
						CAMERA->GetRelativeRect(AlphaRect[0]).top,
						SelectList[0].x, SelectList[0].y, (MRectSizeReal.x + 1) * 64,
						(MRectSizeReal.y + 1) * 64, Pivot::LeftTop);
					break;
				case PageNum::Six:
					IceImage[5]->SetSize(Vector2((MRectSizeReal.x + 1) * 64, (MRectSizeReal.y + 1) * 64) * CAMERA->GetZoom());
					IceImage[5]->SetAlpha(0.4f);
					IceImage[5]->FrameRender2(
						CAMERA->GetRelativeRect(AlphaRect[0]).left,
						CAMERA->GetRelativeRect(AlphaRect[0]).top,
						SelectList[0].x, SelectList[0].y, (MRectSizeReal.x + 1) * 64,
						(MRectSizeReal.y + 1) * 64, Pivot::LeftTop);
					break;
				}
				break;
			case TileType::Enemy:
				switch (Page)
				{
				case PageNum::One:
					IceImage[0]->SetSize(Vector2((MRectSizeReal.x + 1) * 64, (MRectSizeReal.y + 1) * 64) * CAMERA->GetZoom());
					IceImage[0]->SetAlpha(0.4f);
					IceImage[0]->FrameRender2(
						CAMERA->GetRelativeRect(AlphaRect[0]).left,
						CAMERA->GetRelativeRect(AlphaRect[0]).top,
						SelectList[0].x, SelectList[0].y, (MRectSizeReal.x + 1) * 64,
						(MRectSizeReal.y + 1) * 64, Pivot::LeftTop);
					break;
				case PageNum::Two:
					IceImage[1]->SetSize(Vector2((MRectSizeReal.x + 1) * 64, (MRectSizeReal.y + 1) * 64) * CAMERA->GetZoom());
					IceImage[1]->SetAlpha(0.4f);
					IceImage[1]->FrameRender2(
						CAMERA->GetRelativeRect(AlphaRect[0]).left,
						CAMERA->GetRelativeRect(AlphaRect[0]).top,
						SelectList[0].x, SelectList[0].y, (MRectSizeReal.x + 1) * 64,
						(MRectSizeReal.y + 1) * 64, Pivot::LeftTop);
					break;
				case PageNum::Three:
					IceImage[2]->SetSize(Vector2((MRectSizeReal.x + 1) * 64, (MRectSizeReal.y + 1) * 64) * CAMERA->GetZoom());
					IceImage[2]->SetAlpha(0.4f);
					IceImage[2]->FrameRender2(
						CAMERA->GetRelativeRect(AlphaRect[0]).left,
						CAMERA->GetRelativeRect(AlphaRect[0]).top,
						SelectList[0].x, SelectList[0].y, (MRectSizeReal.x + 1) * 64,
						(MRectSizeReal.y + 1) * 64, Pivot::LeftTop);
					break;
				case PageNum::Four:
					IceImage[3]->SetSize(Vector2((MRectSizeReal.x + 1) * 64, (MRectSizeReal.y + 1) * 64) * CAMERA->GetZoom());
					IceImage[3]->SetAlpha(0.4f);
					IceImage[3]->FrameRender2(
						CAMERA->GetRelativeRect(AlphaRect[0]).left,
						CAMERA->GetRelativeRect(AlphaRect[0]).top,
						SelectList[0].x, SelectList[0].y, (MRectSizeReal.x + 1) * 64,
						(MRectSizeReal.y + 1) * 64, Pivot::LeftTop);
					break;
				case PageNum::Five:
					IceImage[4]->SetSize(Vector2((MRectSizeReal.x + 1) * 64, (MRectSizeReal.y + 1) * 64) * CAMERA->GetZoom());
					IceImage[4]->SetAlpha(0.4f);
					IceImage[4]->FrameRender2(
						CAMERA->GetRelativeRect(AlphaRect[0]).left,
						CAMERA->GetRelativeRect(AlphaRect[0]).top,
						SelectList[0].x, SelectList[0].y, (MRectSizeReal.x + 1) * 64,
						(MRectSizeReal.y + 1) * 64, Pivot::LeftTop);

					break;
				case PageNum::Six:
					IceImage[5]->SetSize(Vector2((MRectSizeReal.x + 1) * 64, (MRectSizeReal.y + 1) * 64) * CAMERA->GetZoom());
					IceImage[5]->SetAlpha(0.4f);
					IceImage[5]->FrameRender2(
						CAMERA->GetRelativeRect(AlphaRect[0]).left,
						CAMERA->GetRelativeRect(AlphaRect[0]).top,
						SelectList[0].x, SelectList[0].y, (MRectSizeReal.x + 1) * 64,
						(MRectSizeReal.y + 1) * 64, Pivot::LeftTop);
					break;
				}
			case TileType::Object:
				switch (Page)
				{
				case PageNum::One:
					IceImage[0]->SetSize(Vector2((MRectSizeReal.x + 1) * 64, (MRectSizeReal.y + 1) * 64) * CAMERA->GetZoom());
					IceImage[0]->SetAlpha(0.4f);
					IceImage[0]->FrameRender2(
						CAMERA->GetRelativeRect(AlphaRect[0]).left,
						CAMERA->GetRelativeRect(AlphaRect[0]).top,
						SelectList[0].x, SelectList[0].y, (MRectSizeReal.x + 1) * 64,
						(MRectSizeReal.y + 1) * 64, Pivot::LeftTop);
					break;
				case PageNum::Two:
					IceImage[1]->SetSize(Vector2((MRectSizeReal.x + 1) * 64, (MRectSizeReal.y + 1) * 64) * CAMERA->GetZoom());
					IceImage[1]->SetAlpha(0.4f);
					IceImage[1]->FrameRender2(
						CAMERA->GetRelativeRect(AlphaRect[0]).left,
						CAMERA->GetRelativeRect(AlphaRect[0]).top,
						SelectList[0].x, SelectList[0].y, (MRectSizeReal.x + 1) * 64,
						(MRectSizeReal.y + 1) * 64, Pivot::LeftTop);
					break;
				case PageNum::Three:
					IceImage[2]->SetSize(Vector2((MRectSizeReal.x + 1) * 64, (MRectSizeReal.y + 1) * 64) * CAMERA->GetZoom());
					IceImage[2]->SetAlpha(0.4f);
					IceImage[2]->FrameRender2(
						CAMERA->GetRelativeRect(AlphaRect[0]).left,
						CAMERA->GetRelativeRect(AlphaRect[0]).top,
						SelectList[0].x, SelectList[0].y, (MRectSizeReal.x + 1) * 64,
						(MRectSizeReal.y + 1) * 64, Pivot::LeftTop);
					break;
				case PageNum::Four:
					IceImage[3]->SetSize(Vector2((MRectSizeReal.x + 1) * 64, (MRectSizeReal.y + 1) * 64) * CAMERA->GetZoom());
					IceImage[3]->SetAlpha(0.4f);
					IceImage[3]->FrameRender2(
						CAMERA->GetRelativeRect(AlphaRect[0]).left,
						CAMERA->GetRelativeRect(AlphaRect[0]).top,
						SelectList[0].x, SelectList[0].y, (MRectSizeReal.x + 1) * 64,
						(MRectSizeReal.y + 1) * 64, Pivot::LeftTop);
					break;
				case PageNum::Five:
					IceImage[4]->SetSize(Vector2((MRectSizeReal.x + 1) * 64, (MRectSizeReal.y + 1) * 64) * CAMERA->GetZoom());
					IceImage[4]->SetAlpha(0.4f);
					IceImage[4]->FrameRender2(
						CAMERA->GetRelativeRect(AlphaRect[0]).left,
						CAMERA->GetRelativeRect(AlphaRect[0]).top,
						SelectList[0].x, SelectList[0].y, (MRectSizeReal.x + 1) * 64,
						(MRectSizeReal.y + 1) * 64, Pivot::LeftTop);

					break;
				case PageNum::Six:
					IceImage[5]->SetSize(Vector2((MRectSizeReal.x + 1) * 64, (MRectSizeReal.y + 1) * 64) * CAMERA->GetZoom());
					IceImage[5]->SetAlpha(0.4f);
					IceImage[5]->FrameRender2(
						CAMERA->GetRelativeRect(AlphaRect[0]).left,
						CAMERA->GetRelativeRect(AlphaRect[0]).top,
						SelectList[0].x, SelectList[0].y, (MRectSizeReal.x + 1) * 64,
						(MRectSizeReal.y + 1) * 64, Pivot::LeftTop);
					break;
				}
			}


			break;
		case SceneNum::Boss3:
			break;
		}
		
	}
	paletteImage->SetSize(Vector2(450, 543));
	paletteImage->Render(TilePalette.left, TilePalette.top, Pivot::LeftTop);

	// =====================================================================================
	// ################################ 샘플 타일 보여주기 #################################
	// =====================================================================================
	if (SampleShow)
	{
		switch (Scene)
		{
		case SceneNum::Town:

			switch (Type)
			{
			case TileType::Terrain:
				switch (Page)
				{
				case PageNum::One:
					sampleImage[0]->SetSize(Vector2(256, 288));
					sampleImage[0]->Render(
						Sample[0][0].rc.left, Sample[0][0].rc.top, Pivot::LeftTop);
					break;
				case PageNum::Two:
					sampleImage[1]->SetSize(Vector2(256, 288));
					sampleImage[1]->Render(
						Sample[0][0].rc.left, Sample[0][0].rc.top, Pivot::LeftTop);
					break;
				case PageNum::Three:
					sampleImage[2]->SetSize(Vector2(256, 288));
					sampleImage[2]->Render(
						Sample[0][0].rc.left, Sample[0][0].rc.top, Pivot::LeftTop);
					break;
				case PageNum::Four:
					sampleImage[3]->SetSize(Vector2(256, 288));
					sampleImage[3]->Render(
						Sample[0][0].rc.left, Sample[0][0].rc.top, Pivot::LeftTop);
					break;
				case PageNum::Five:
					sampleImage[4]->SetSize(Vector2(256, 288));
					sampleImage[4]->Render(
						Sample[0][0].rc.left, Sample[0][0].rc.top, Pivot::LeftTop);
					break;
				case PageNum::Six:
					sampleImage[5]->SetSize(Vector2(256, 288));
					sampleImage[5]->Render(
						Sample[0][0].rc.left, Sample[0][0].rc.top, Pivot::LeftTop);
					break;
				case PageNum::Seven:
					sampleImage[6]->SetSize(Vector2(256, 288));
					sampleImage[6]->Render(
						Sample[0][0].rc.left, Sample[0][0].rc.top, Pivot::LeftTop);
					break;
				case PageNum::Eight:
					sampleImage[7]->SetSize(Vector2(256, 288));
					sampleImage[7]->Render(
						Sample[0][0].rc.left, Sample[0][0].rc.top, Pivot::LeftTop);
					break;
				case PageNum::Nine:
					sampleImage[8]->SetSize(Vector2(256, 288));
					sampleImage[8]->Render(
						Sample[0][0].rc.left, Sample[0][0].rc.top, Pivot::LeftTop);
					break;
				}
				break;
			case TileType::Object:
				switch (Page)
				{
				case PageNum::One:
					sampleImage[0]->SetSize(Vector2(256, 288));
					sampleImage[0]->Render(
						Sample[0][0].rc.left, Sample[0][0].rc.top, Pivot::LeftTop);
					break;
				case PageNum::Two:
					sampleImage[1]->SetSize(Vector2(256, 288));
					sampleImage[1]->Render(
						Sample[0][0].rc.left, Sample[0][0].rc.top, Pivot::LeftTop);
					break;
				case PageNum::Three:
					sampleImage[2]->SetSize(Vector2(256, 288));
					sampleImage[2]->Render(
						Sample[0][0].rc.left, Sample[0][0].rc.top, Pivot::LeftTop);
					break;
				case PageNum::Four:
					sampleImage[3]->SetSize(Vector2(256, 288));
					sampleImage[3]->Render(
						Sample[0][0].rc.left, Sample[0][0].rc.top, Pivot::LeftTop);
					break;
				case PageNum::Five:
					sampleImage[4]->SetSize(Vector2(256, 288));
					sampleImage[4]->Render(
						Sample[0][0].rc.left, Sample[0][0].rc.top, Pivot::LeftTop);
					break;
				case PageNum::Six:
					sampleImage[5]->SetSize(Vector2(256, 288));
					sampleImage[5]->Render(
						Sample[0][0].rc.left, Sample[0][0].rc.top, Pivot::LeftTop);
					break;
				case PageNum::Seven:
					sampleImage[6]->SetSize(Vector2(256, 288));
					sampleImage[6]->Render(
						Sample[0][0].rc.left, Sample[0][0].rc.top, Pivot::LeftTop);
					break;
				case PageNum::Eight:
					sampleImage[7]->SetSize(Vector2(256, 288));
					sampleImage[7]->Render(
						Sample[0][0].rc.left, Sample[0][0].rc.top, Pivot::LeftTop);
					break;
				case PageNum::Nine:
					sampleImage[8]->SetSize(Vector2(256, 288));
					sampleImage[8]->Render(
						Sample[0][0].rc.left, Sample[0][0].rc.top, Pivot::LeftTop);
					break;
				}
				break;
			case TileType::Enemy:
				switch (Page)
				{
				case PageNum::One:
					sampleImage[0]->SetSize(Vector2(256, 288));
					sampleImage[0]->Render(
						Sample[0][0].rc.left, Sample[0][0].rc.top, Pivot::LeftTop);
					break;
				case PageNum::Two:
					sampleImage[1]->SetSize(Vector2(256, 288));
					sampleImage[1]->Render(
						Sample[0][0].rc.left, Sample[0][0].rc.top, Pivot::LeftTop);
					break;
				case PageNum::Three:
					sampleImage[2]->SetSize(Vector2(256, 288));
					sampleImage[2]->Render(
						Sample[0][0].rc.left, Sample[0][0].rc.top, Pivot::LeftTop);
					break;
				case PageNum::Four:
					sampleImage[3]->SetSize(Vector2(256, 288));
					sampleImage[3]->Render(
						Sample[0][0].rc.left, Sample[0][0].rc.top, Pivot::LeftTop);
					break;
				case PageNum::Five:
					sampleImage[4]->SetSize(Vector2(256, 288));
					sampleImage[4]->Render(
						Sample[0][0].rc.left, Sample[0][0].rc.top, Pivot::LeftTop);
					break;
				case PageNum::Six:
					sampleImage[5]->SetSize(Vector2(256, 288));
					sampleImage[5]->Render(
						Sample[0][0].rc.left, Sample[0][0].rc.top, Pivot::LeftTop);
					break;
				case PageNum::Seven:
					sampleImage[6]->SetSize(Vector2(256, 288));
					sampleImage[6]->Render(
						Sample[0][0].rc.left, Sample[0][0].rc.top, Pivot::LeftTop);
					break;
				case PageNum::Eight:
					sampleImage[7]->SetSize(Vector2(256, 288));
					sampleImage[7]->Render(
						Sample[0][0].rc.left, Sample[0][0].rc.top, Pivot::LeftTop);
					break;
				case PageNum::Nine:
					sampleImage[8]->SetSize(Vector2(256, 288));
					sampleImage[8]->Render(
						Sample[0][0].rc.left, Sample[0][0].rc.top, Pivot::LeftTop);
					break;
				}
				break;
			}

			break;
		case SceneNum::Boss1:

			switch (Type)
			{
			case TileType::Terrain:
				switch (Page)
				{
				case PageNum::One:
					SlimeSampleImage[0]->SetSize(Vector2(256, 288));
					SlimeSampleImage[0]->Render(
						Sample[0][0].rc.left, Sample[0][0].rc.top, Pivot::LeftTop);
					break;
				case PageNum::Two:
					SlimeSampleImage[1]->SetSize(Vector2(256, 288));
					SlimeSampleImage[1]->Render(
						Sample[0][0].rc.left, Sample[0][0].rc.top, Pivot::LeftTop);
					break;
				case PageNum::Three:
					SlimeSampleImage[2]->SetSize(Vector2(256, 288));
					SlimeSampleImage[2]->Render(
						Sample[0][0].rc.left, Sample[0][0].rc.top, Pivot::LeftTop);
					break;
				case PageNum::Four:
					SlimeSampleImage[3]->SetSize(Vector2(256, 288));
					SlimeSampleImage[3]->Render(
						Sample[0][0].rc.left, Sample[0][0].rc.top, Pivot::LeftTop);
					break;
				}
				break;
			case TileType::Object:
				switch (Page)
				{
				case PageNum::One:
					SlimeSampleImage[0]->SetSize(Vector2(256, 288));
					SlimeSampleImage[0]->Render(
						Sample[0][0].rc.left, Sample[0][0].rc.top, Pivot::LeftTop);
					break;
				case PageNum::Two:
					SlimeSampleImage[1]->SetSize(Vector2(256, 288));
					SlimeSampleImage[1]->Render(
						Sample[0][0].rc.left, Sample[0][0].rc.top, Pivot::LeftTop);
					break;
				case PageNum::Three:
					SlimeSampleImage[2]->SetSize(Vector2(256, 288));
					SlimeSampleImage[2]->Render(
						Sample[0][0].rc.left, Sample[0][0].rc.top, Pivot::LeftTop);
					break;
				case PageNum::Four:
					SlimeSampleImage[3]->SetSize(Vector2(256, 288));
					SlimeSampleImage[3]->Render(
						Sample[0][0].rc.left, Sample[0][0].rc.top, Pivot::LeftTop);
					break;
				}
				break;
			case TileType::Enemy:
				switch (Page)
				{
				case PageNum::One:
					SlimeSampleImage[0]->SetSize(Vector2(256, 288));
					SlimeSampleImage[0]->Render(
						Sample[0][0].rc.left, Sample[0][0].rc.top, Pivot::LeftTop);
					break;
				case PageNum::Two:
					SlimeSampleImage[1]->SetSize(Vector2(256, 288));
					SlimeSampleImage[1]->Render(
						Sample[0][0].rc.left, Sample[0][0].rc.top, Pivot::LeftTop);
					break;
				case PageNum::Three:
					SlimeSampleImage[2]->SetSize(Vector2(256, 288));
					SlimeSampleImage[2]->Render(
						Sample[0][0].rc.left, Sample[0][0].rc.top, Pivot::LeftTop);
					break;
				case PageNum::Four:
					SlimeSampleImage[3]->SetSize(Vector2(256, 288));
					SlimeSampleImage[3]->Render(
						Sample[0][0].rc.left, Sample[0][0].rc.top, Pivot::LeftTop);
					break;
				}
				break;
			}
			break;
		case SceneNum::Boss2:

			switch (Type)
			{
			case TileType::Terrain:
				switch (Page)
				{
				case PageNum::One:
					IceSampleImage[0]->SetSize(Vector2(256, 288));
					IceSampleImage[0]->Render(
						Sample[0][0].rc.left, Sample[0][0].rc.top, Pivot::LeftTop);
					break;
				case PageNum::Two:
					IceSampleImage[1]->SetSize(Vector2(256, 288));
					IceSampleImage[1]->Render(
						Sample[0][0].rc.left, Sample[0][0].rc.top, Pivot::LeftTop);
					break;
				case PageNum::Three:
					IceSampleImage[2]->SetSize(Vector2(256, 288));
					IceSampleImage[2]->Render(
						Sample[0][0].rc.left, Sample[0][0].rc.top, Pivot::LeftTop);
					break;
				case PageNum::Four:
					IceSampleImage[3]->SetSize(Vector2(256, 288));
					IceSampleImage[3]->Render(
						Sample[0][0].rc.left, Sample[0][0].rc.top, Pivot::LeftTop);
					break;
				case PageNum::Five:
					IceSampleImage[4]->SetSize(Vector2(256, 288));
					IceSampleImage[4]->Render(
						Sample[0][0].rc.left, Sample[0][0].rc.top, Pivot::LeftTop);
					break;
				case PageNum::Six:
					IceSampleImage[5]->SetSize(Vector2(256, 288));
					IceSampleImage[5]->Render(
						Sample[0][0].rc.left, Sample[0][0].rc.top, Pivot::LeftTop);
					break;
				}
				break;
			case TileType::Object:
				switch (Page)
				{
				case PageNum::One:
					IceSampleImage[0]->SetSize(Vector2(256, 288));
					IceSampleImage[0]->Render(
						Sample[0][0].rc.left, Sample[0][0].rc.top, Pivot::LeftTop);
					break;
				case PageNum::Two:
					IceSampleImage[1]->SetSize(Vector2(256, 288));
					IceSampleImage[1]->Render(
						Sample[0][0].rc.left, Sample[0][0].rc.top, Pivot::LeftTop);
					break;
				case PageNum::Three:
					IceSampleImage[2]->SetSize(Vector2(256, 288));
					IceSampleImage[2]->Render(
						Sample[0][0].rc.left, Sample[0][0].rc.top, Pivot::LeftTop);
					break;
				case PageNum::Four:
					IceSampleImage[3]->SetSize(Vector2(256, 288));
					IceSampleImage[3]->Render(
						Sample[0][0].rc.left, Sample[0][0].rc.top, Pivot::LeftTop);
					break;
				case PageNum::Five:
					IceSampleImage[4]->SetSize(Vector2(256, 288));
					IceSampleImage[4]->Render(
						Sample[0][0].rc.left, Sample[0][0].rc.top, Pivot::LeftTop);
					break;
				case PageNum::Six:
					IceSampleImage[5]->SetSize(Vector2(256, 288));
					IceSampleImage[5]->Render(
						Sample[0][0].rc.left, Sample[0][0].rc.top, Pivot::LeftTop);
					break;
				}
				break;
			case TileType::Enemy:
				switch (Page)
				{
				case PageNum::One:
					IceSampleImage[0]->SetSize(Vector2(256, 288));
					IceSampleImage[0]->Render(
						Sample[0][0].rc.left, Sample[0][0].rc.top, Pivot::LeftTop);
					break;
				case PageNum::Two:
					IceSampleImage[1]->SetSize(Vector2(256, 288));
					IceSampleImage[1]->Render(
						Sample[0][0].rc.left, Sample[0][0].rc.top, Pivot::LeftTop);
					break;
				case PageNum::Three:
					IceSampleImage[2]->SetSize(Vector2(256, 288));
					IceSampleImage[2]->Render(
						Sample[0][0].rc.left, Sample[0][0].rc.top, Pivot::LeftTop);
					break;
				case PageNum::Four:
					IceSampleImage[3]->SetSize(Vector2(256, 288));
					IceSampleImage[3]->Render(
						Sample[0][0].rc.left, Sample[0][0].rc.top, Pivot::LeftTop);
					break;
				case PageNum::Five:
					IceSampleImage[4]->SetSize(Vector2(256, 288));
					IceSampleImage[4]->Render(
						Sample[0][0].rc.left, Sample[0][0].rc.top, Pivot::LeftTop);
					break;
				case PageNum::Six:
					IceSampleImage[5]->SetSize(Vector2(256, 288));
					IceSampleImage[5]->Render(
						Sample[0][0].rc.left, Sample[0][0].rc.top, Pivot::LeftTop);
					break;
				}
				break;
			}
			break;

		case SceneNum::Boss3:
			break;
		}
		

		switch (Type)
		{
		case TileType::Terrain:
			IMAGEMANAGER->FindImage("지형")->SetSize(Vector2(218, 42));
			IMAGEMANAGER->FindImage("지형")->Render(Title.left + 5, Title.top + 8, Pivot::LeftTop);
			break;
		case TileType::Object:
			IMAGEMANAGER->FindImage("오브젝트")->SetSize(Vector2(218, 42));
			IMAGEMANAGER->FindImage("오브젝트")->Render(Title.left + 5, Title.top + 8, Pivot::LeftTop);
			break;
		case TileType::Enemy:
			IMAGEMANAGER->FindImage("에너미")->SetSize(Vector2(218, 42));
			IMAGEMANAGER->FindImage("에너미")->Render(Title.left + 5, Title.top + 8, Pivot::LeftTop);
			break;
		case TileType::Erase:
			break;
		}
		IMAGEMANAGER->FindImage("버튼")->SetSize(Vector2(64, 48));
		IMAGEMANAGER->FindImage("버튼")->FrameRender(NumberRect[0].left, NumberRect[0].top, index[0], 0, Pivot::LeftTop);
		IMAGEMANAGER->FindImage("버튼")->SetSize(Vector2(64, 48));
		IMAGEMANAGER->FindImage("버튼")->FrameRender(NumberRect[1].left, NumberRect[1].top, index[1], 0, Pivot::LeftTop);
		IMAGEMANAGER->FindImage("버튼")->SetSize(Vector2(64, 48));
		IMAGEMANAGER->FindImage("버튼")->FrameRender(NumberRect[2].left, NumberRect[2].top, index[2], 0, Pivot::LeftTop);
		IMAGEMANAGER->FindImage("버튼")->SetSize(Vector2(64, 48));
		IMAGEMANAGER->FindImage("버튼")->FrameRender(NumberRect[3].left, NumberRect[3].top, index[3], 0, Pivot::LeftTop);
		IMAGEMANAGER->FindImage("버튼")->SetSize(Vector2(64, 48));
		IMAGEMANAGER->FindImage("버튼")->FrameRender(NumberRect[4].left, NumberRect[4].top, index[4], 0, Pivot::LeftTop);
		IMAGEMANAGER->FindImage("버튼")->SetSize(Vector2(64, 48));
		IMAGEMANAGER->FindImage("버튼")->FrameRender(NumberRect[5].left, NumberRect[5].top, index[5], 0, Pivot::LeftTop);

		IMAGEMANAGER->FindImage("숫자1")->SetSize(Vector2(6, 30));
		IMAGEMANAGER->FindImage("숫자1")->Render(NumberRect[0].left + 28, NumberRect[0].top + 8, Pivot::LeftTop);
		IMAGEMANAGER->FindImage("숫자2")->SetSize(Vector2(20, 30));
		IMAGEMANAGER->FindImage("숫자2")->Render(NumberRect[1].left + 22, NumberRect[1].top + 8, Pivot::LeftTop);
		IMAGEMANAGER->FindImage("숫자3")->SetSize(Vector2(20, 30));
		IMAGEMANAGER->FindImage("숫자3")->Render(NumberRect[2].left + 22, NumberRect[2].top + 8, Pivot::LeftTop);
		IMAGEMANAGER->FindImage("숫자4")->SetSize(Vector2(20, 30));
		IMAGEMANAGER->FindImage("숫자4")->Render(NumberRect[3].left + 28, NumberRect[3].top + 8, Pivot::LeftTop);
		IMAGEMANAGER->FindImage("숫자5")->SetSize(Vector2(20, 30));
		IMAGEMANAGER->FindImage("숫자5")->Render(NumberRect[4].left + 22, NumberRect[4].top + 8, Pivot::LeftTop);
		IMAGEMANAGER->FindImage("숫자6")->SetSize(Vector2(20, 30));
		IMAGEMANAGER->FindImage("숫자6")->Render(NumberRect[5].left + 22, NumberRect[5].top + 8, Pivot::LeftTop);

		IMAGEMANAGER->FindImage("큰버튼")->SetSize(Vector2(121, 37));
		IMAGEMANAGER->FindImage("큰버튼")->FrameRender(SystemButton[0].left, SystemButton[0].top, Bindex[0], 0, Pivot::LeftTop);
		IMAGEMANAGER->FindImage("큰버튼")->SetSize(Vector2(121, 37));
		IMAGEMANAGER->FindImage("큰버튼")->FrameRender(SystemButton[1].left, SystemButton[1].top, Bindex[1], 0, Pivot::LeftTop);
		IMAGEMANAGER->FindImage("큰버튼")->SetSize(Vector2(121, 37));
		IMAGEMANAGER->FindImage("큰버튼")->FrameRender(SystemButton[2].left, SystemButton[2].top, Bindex[2], 0, Pivot::LeftTop);

		IMAGEMANAGER->FindImage("세이브")->SetSize(Vector2(80, 20));
		IMAGEMANAGER->FindImage("세이브")->Render(SystemButton[0].left + 20, SystemButton[0].top + 7, Pivot::LeftTop);
		IMAGEMANAGER->FindImage("지우개")->SetSize(Vector2(80, 20));
		IMAGEMANAGER->FindImage("지우개")->Render(SystemButton[1].left + 20, SystemButton[1].top + 7, Pivot::LeftTop);
		IMAGEMANAGER->FindImage("로드")->SetSize(Vector2(80, 20));
		IMAGEMANAGER->FindImage("로드")->Render(SystemButton[2].left + 20, SystemButton[2].top + 7, Pivot::LeftTop);

		greenImage->SetSize(Vector2(
			(SelectRect.right - SelectRect.left),
			(SelectRect.bottom - SelectRect.top)
		));
		greenImage->SetAlpha(0.4f);
		greenImage->Render(SelectRect.left, SelectRect.top, Pivot::LeftTop);

		D2DRENDERER->DrawRectangle(MouseRect,
			D2DRenderer::DefaultBrush::Red);

	}
	cursorImage->SetSize(Vector2(32,32));
	cursorImage->Render(_ptMouse.x, _ptMouse.y, Pivot::LeftTop);
}

// ===============================================
// ############ 디버그 확인용 함수 ################
// ===============================================
void MapToolScene::DeBugCheck()
{
	if (DeBug)
	{
		D2DRENDERER->DrawRectangle(Palette, D2DRenderer::DefaultBrush::Red);
		D2DRENDERER->DrawRectangle(TilePalette, D2DRenderer::DefaultBrush::Red);

		if (SampleShow) {

			D2DRENDERER->DrawRectangle(Title, D2DRenderer::DefaultBrush::Red);
			D2DRENDERER->DrawRectangle(SelectButton[0], D2DRenderer::DefaultBrush::Red);
			D2DRENDERER->DrawRectangle(SelectButton[1], D2DRenderer::DefaultBrush::Red);
			D2DRENDERER->DrawRectangle(SelectButton[2], D2DRenderer::DefaultBrush::Red);

			D2DRENDERER->DrawRectangle(NumberRect[0], D2DRenderer::DefaultBrush::Red);
			D2DRENDERER->DrawRectangle(NumberRect[1], D2DRenderer::DefaultBrush::Red);
			D2DRENDERER->DrawRectangle(NumberRect[2], D2DRenderer::DefaultBrush::Red);
			D2DRENDERER->DrawRectangle(NumberRect[3], D2DRenderer::DefaultBrush::Red);
			D2DRENDERER->DrawRectangle(NumberRect[4], D2DRenderer::DefaultBrush::Red);
			D2DRENDERER->DrawRectangle(NumberRect[5], D2DRenderer::DefaultBrush::Red);

			D2DRENDERER->DrawRectangle(SystemButton[0], D2DRenderer::DefaultBrush::Red);
			D2DRENDERER->DrawRectangle(SystemButton[1], D2DRenderer::DefaultBrush::Red);
			D2DRENDERER->DrawRectangle(SystemButton[2], D2DRenderer::DefaultBrush::Red);

		}
	}



	//char str[256];
	//sprintf_s(str, "시작 좌표 : %d, %d, 끝 좌표 : %d, %d",
	//	Start.x, Start.y, End.x, End.y);
	//TextOut(getMemDC(), _ptMouse.x + 10, _ptMouse.y, str, strlen(str));
	//TextOut(getMemDC(), _ptMouse.x + 10, _ptMouse.y+20, (to_string(SelectList.size())).c_str(), to_string(SelectList.size()).size());
	//TextOut(getMemDC(), _ptMouse.x + 10, _ptMouse.y, (to_string(CAMERA->GetZoom())).c_str(), to_string(CAMERA->GetZoom()).size());
	//TextOut(getMemDC(), _ptMouse.x + 10, _ptMouse.y, ("X 좌표 : " + to_string(_ptMouse.x) + "Y 좌표 : " + to_string(_ptMouse.y)).c_str(),
	//	("X 좌표 : " + to_string(_ptMouse.x) + "Y 좌표 : " + to_string(_ptMouse.y)).size());
}

void MapToolScene::CheckMousePos()
{
	int X = (_ptMouse.x / CAMERA->GetZoom() + CAMERA->GetCameraRect().left) / SIZE;
	int Y = (_ptMouse.y / CAMERA->GetZoom() + CAMERA->GetCameraRect().top) / SIZE;
	D2DRENDERER->RenderText(_ptMouse.x + 5, _ptMouse.y - 25,
		(L"X: " + to_wstring(X) + L", Y: " + to_wstring(Y) + L", 층수 : " + to_wstring((int)OrderNum)).c_str(), 20,
		D2DRenderer::DefaultBrush::Green);
}

void MapToolScene::Save()
{
	HANDLE file;
	DWORD write;
	
	switch (Scene)
	{
	case SceneNum::Town:
		file = CreateFile(L"Townsave.map", GENERIC_WRITE, 0, NULL, CREATE_ALWAYS,
			FILE_ATTRIBUTE_NORMAL, NULL);
		break;
	case SceneNum::Boss1:
		file = CreateFile(L"Boss1save.map", GENERIC_WRITE, 0, NULL, CREATE_ALWAYS,
			FILE_ATTRIBUTE_NORMAL, NULL);
		break;
	case SceneNum::Boss2:
		file = CreateFile(L"Boss2save.map", GENERIC_WRITE, 0, NULL, CREATE_ALWAYS,
			FILE_ATTRIBUTE_NORMAL, NULL);
		break;
	case SceneNum::Boss3:
		file = CreateFile(L"Boss3save.map", GENERIC_WRITE, 0, NULL, CREATE_ALWAYS,
			FILE_ATTRIBUTE_NORMAL, NULL);
		break;
	}

	WriteFile(file, Tile, sizeof(Tile), &write, NULL);
	CloseHandle(file);

}

void MapToolScene::Load()
{
	HANDLE file;
	DWORD read;

	switch (Scene)
	{
	case SceneNum::Town:
		file = CreateFile(L"Townsave.map", GENERIC_READ, 0, NULL, OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL, NULL);
		break;
	case SceneNum::Boss1:
		file = CreateFile(L"Boss1save.map", GENERIC_READ, 0, NULL, OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL, NULL);
		break;
	case SceneNum::Boss2:
		file = CreateFile(L"Boss2save.map", GENERIC_READ, 0, NULL, OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL, NULL);
		break;
	case SceneNum::Boss3:
		file = CreateFile(L"Boss3save.map", GENERIC_READ, 0, NULL, OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL, NULL);
		break;
	}
	
	ReadFile(file, Tile, sizeof(Tile), &read, NULL);
	CloseHandle(file);
}

void MapToolScene::Reset()
{
	//for (int i = 0; i < YNUM; ++i)
	//{
	//	for (int j = 0; j < XNUM; ++j)
	//	{
	//		Tile[j][i].Objectx = -1;
	//		Tile[j][i].Objecty = -1;
	//		Tile[j][i].Terrainx = -1;
	//		Tile[j][i].Terrainy = -1;
	//		if (Tile[j][i].show) Tile[j][i].show = false;
	//	}
	//}
}
