#include "stdafx.h"
#include "Loading.h"
//=============================================================
//	## loadItem ## (로드아이템 클래스)
//=============================================================

//▼일반 이미지 초기화
HRESULT LoadItem::Init(std::string keyName, std::wstring fileName)
{
	//▼종류, 상세정보 초기화
	_kind = LOAD_KIND::LOAD_KIND_IMAGE;
	_imageResource.keyName = keyName;
	_imageResource.fileName = fileName;

	return S_OK;
}

//▼프레임 이미지 초기화
HRESULT LoadItem::Init(std::string keyName, std::wstring fileName,int frameX, int frameY)
{
	//▼종류, 상세정보 초기화
	_kind = LOAD_KIND::LOAD_KIND_FRAMEIMAGE;
	_imageResource.keyName = keyName;
	_imageResource.fileName = fileName;
	_imageResource.frameX = frameX;
	_imageResource.frameY = frameY;

	return S_OK;
}

//▼사운드 초기화
HRESULT LoadItem::Init(std::string keyName, const char * fileName, bool bgm, bool loop)
{
	//▼종류, 상세정보 초기화
	_kind = LOAD_KIND::LOAD_KIND_SOUND;
	_soundResource.keyName = keyName;
	_soundResource.fileName = fileName;
	_soundResource.bgm = bgm;
	_soundResource.loop = loop;

	return S_OK;
}

//▼씬 초기화
HRESULT LoadItem::Init(std::string sceneName, GameNode * ptr)
{
	//▼종류, 상세정보 초기화
	_kind = LOAD_KIND::LOAD_KIND_SCENE;
	_sceneResource.sceneName = sceneName;
	_sceneResource.scene = ptr;

	return S_OK;
}


//=============================================================
//	## loading ## (로딩 클래스)
//=============================================================

//▼본인이 추가하고 싶다면 여기에 "이미지매니저"를 통해 로딩에 쓸 백그라운드 및 이미지 추가
void Loading::Init()
{
	
	//예시 IMAGEMANAGER->addImage("loadingText", "image/UI/LoadingText.bmp", 245, 75);
	
	
	_currentGauge = 0; //현재 게이지. 0번 인덱스부터 시작하게 함
}

//▼만약 뉴할당 받은것들이 존재하면 풀어줌
void Loading::Release()
{
	for (auto &a : _vLoadItem) SAFE_DELETE(a);

}

//▼로딩에 만약 프레임이미지등 업데이트가 필요한것들이 있다면 시켜줌
void Loading::Update()
{
	

}

//▼로딩화면에 백그라운드나 움직이는 사진들 띄울거면 여기서 랜더
void Loading::Render()
{
	

}

//▼일반 이미지 로딩하기 위해 이닛시키고 담아둠
void Loading::LoadImg(std::string keyName, std::wstring fileName)
{
	LoadItem* item = new LoadItem;
	item->Init(keyName, fileName);
	_vLoadItem.push_back(item);
}

//▼프레임 이미지 로딩하기 위해 이닛시키고 담아둠
void Loading::LoadFrameImage(std::string keyName, std::wstring fileName,int frameX, int frameY)
{
	LoadItem* item = new LoadItem;
	item->Init(keyName, fileName,frameX, frameY);
	_vLoadItem.push_back(item);
}

//▼사운드 로딩하기 위해 이닛시키고 담아둠
void Loading::LoadSound(std::string keyName, const char * fileName, bool bgm, bool loop)
{
	LoadItem * item = new LoadItem;
	item->Init(keyName, fileName, bgm, loop);
	_vLoadItem.push_back(item);
}

//▼씬 로딩하기 위해 이닛시키고 담아둠
void Loading::LoadScene(std::string sceneName, GameNode * ptr)
{
	LoadItem * item = new LoadItem;
	item->Init(sceneName, ptr);
	_vLoadItem.push_back(item);
}

BOOL Loading::LoadingDone()
{
	//로딩완료됨
	if (_currentGauge >= _vLoadItem.size())
	{
		return TRUE;
	}

	LoadItem* item = _vLoadItem[_currentGauge];

	switch (item->GetLoadingKind())
	{
	case LOAD_KIND::LOAD_KIND_IMAGE:
	{
		tagImageResource img = item->GetImageResource();
		IMAGEMANAGER->AddImage(img.keyName, img.fileName);
	}
	break;
	case LOAD_KIND::LOAD_KIND_FRAMEIMAGE:
	{
		tagImageResource img = item->GetImageResource();
		IMAGEMANAGER->AddFrameImage(img.keyName, img.fileName,img.frameX, img.frameY);
	}
	break;
	case LOAD_KIND::LOAD_KIND_SOUND:
	{
		tagSoundResource snd = item->GetSoundResource();
		SOUNDMANAGER->addSound(snd.keyName, snd.fileName, snd.bgm, snd.loop);
	}
	break;
	case LOAD_KIND::LOAD_KIND_SCENE:
	{
		tagSceneResource scene = item->GetSceneResource();
		SCENEMANAGER->AddScene(scene.sceneName, scene.scene);
	}
	break;


	}

	//현재 게이지 증가
	_currentGauge++;

	return 0;
}


