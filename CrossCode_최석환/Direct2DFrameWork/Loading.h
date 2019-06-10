#pragma once
#include "GameNode.h"

//로드아이템 종류
enum class LOAD_KIND
{
	LOAD_KIND_IMAGE,			//일반 이미지
	LOAD_KIND_FRAMEIMAGE,		//프레임 이미지
	LOAD_KIND_SOUND,			//사운드
	LOAD_KIND_SCENE,			//씬
};

//▼이미지 리소스 구조체
struct tagImageResource
{
	std::string keyName;		//이미지 키값
	std::wstring fileName;		//파일이름
	int frameX;					//프레임 이미지의 프레임X 갯수
	int frameY;					//프레임 이미지의 프레임Y 갯수
};

//▼사운드 리소스 구조체
struct tagSoundResource
{
	std::string keyName;		//이미지키값
	const char* fileName;		//파일 이름
	bool bgm;					//브금인지 여부
	bool loop;					//루프인지 여부
};

//▼씬이 가지는 구조체
struct tagSceneResource
{
	std::string sceneName;
	GameNode* scene;
};

//=============================================================
//	## loadItem ## (로드아이템 클래스)
//=============================================================

class LoadItem
{
private:
	LOAD_KIND _kind; //로드 종류
	tagImageResource _imageResource;
	tagSoundResource _soundResource;
	tagSceneResource _sceneResource;

public:
	//일반 이미지
	HRESULT Init(std::string keyName, std::wstring fileName);
	//프레임 이미지 파일 초기화(주사용)
	HRESULT Init(std::string keyName, std::wstring fileName,int frameX, int frameY);
	//사운드 로딩
	HRESULT Init(std::string keyName, const char * fileName, bool bgm, bool loop);
	//씬로딩
	HRESULT Init(std::string sceneName, GameNode * ptr);

	//로딩아이템 종류 가져오기
	LOAD_KIND GetLoadingKind() { return _kind; }
	//이미지 리소스 가져오기
	tagImageResource GetImageResource() { return _imageResource; }
	tagSoundResource GetSoundResource() { return _soundResource; }
	tagSceneResource GetSceneResource() { return _sceneResource; }

	LoadItem() {}
	~LoadItem() {}
};


//=============================================================
//	## loading ## (로딩 클래스)
//=============================================================
class Loading : public GameNode
{
private:
	//▼로딩할 아이템들 담을 벡터 및 이터레이터
	typedef std::vector<LoadItem*> arrLoadItem;
	typedef std::vector<LoadItem*>::iterator arrLoadItemIter;

private:
	arrLoadItem _vLoadItem; //아이템 담을 백터
	int _currentGauge;		//현재 몇개째 로딩했는지 나타낼 게이지

public:
	void Init();			
	void Release();
	void Update();
	void Render();

	//▼이미지 로드
	void LoadImg(std::string keyName, std::wstring fileName);
	//▼프레임 이미지 로드
	void LoadFrameImage(std::string keyName, std::wstring, int frameX, int frameY);
	//▼사운드 로드 함수
	void LoadSound(std::string keyName, const char* fileName, bool bgm = false, bool loop = false);
	//▼씬 로드 
	void LoadScene(std::string sceneName, GameNode * ptr);


	BOOL LoadingDone(); //로딩이 끝났는지 확인하는 함수

	const arrLoadItem &GetLoadItem() { return _vLoadItem; }
	int GetCurrentGauge() { return _currentGauge; }

	Loading() {}
	~Loading() {}
};

