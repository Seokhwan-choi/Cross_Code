// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 또는 프로젝트 관련 포함 파일이
// 들어 있는 포함 파일입니다.
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용은 Windows 헤더에서 제외합니다.
// Windows 헤더 파일:
#include <windows.h>

// C 런타임 헤더 파일입니다.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <string>
#include <time.h>
#include <assert.h>
#include <math.h>
#include <vector>
#include <deque>
#include <map>
#include <unordered_map>
#include <functional>
#include <stack>
#include <iostream>

using namespace std;
/**********************************************************************
## Library  ##
***********************************************************************/
//다이렉트2D 라이브러리
#pragma comment(lib, "d2d1.lib")
#include <D2D1.h>
#include <d2d1helper.h>

//다이렉트 Write 라이브러리 (텍스트)
#pragma comment(lib, "dwrite.lib")
#include <dwrite.h>

//WIC 라이브러리
#pragma comment(lib, "WindowsCodecs.lib")
#include <wincodec.h>

/**********************************************************************
## 공용 enum stuct ##
***********************************************************************/
enum class Pivot : int
{
	LeftTop,Center,Bottom
};

// ObjectManager에서 사용
enum class ObjectType : int
{
	Tile, Object, Player, Enemy, UI, END
};

enum class SceneState : int
{
	None, FadeIn, FadeOut, Load, END
};

/**********************************************************************
## 유틸 ##
***********************************************************************/
#include "GameNode.h"
#include "Vector2.h"
#include "CommonMacroFunction.h"
#include "StringHelper.h"
#include "Math.h"
#include "Image.h"

/**********************************************************************
## 매니져 ##
***********************************************************************/
#include "D2DRenderer.h"
#include "RandomFunction.h"
#include "ImageManager.h"
#include "CameraManager.h"
#include "KeyManager.h"
#include "ObjectManager.h"
#include "SceneManager.h"
#include "SoundManager.h"
#include "TimeManager.h"
#include "EffectManager.h"
#include "UiManager.h"
#include "EventManager.h"

#define D2DRENDERER D2DRenderer::Get()
#define RND RandomFunction::Get()
#define IMAGEMANAGER ImageManager::Get()
#define CAMERA CameraManager::Get()
#define KEYMANAGER KeyManager::Get()
#define OBJECTMANAGER ObjectManager::Get()
#define SCENEMANAGER SceneManager::Get()
#define SOUNDMANAGER SoundManager::Get()
#define TIMEMANAGER TimeManager::Get()
#define EFFECT EffectManager::Get()
#define UIMANAGER UiManager::Get()
#define EVENT EventManager::Get()
#define ASTAR aStarManager::Get()

/**********************************************************************
## Window정보 ##
***********************************************************************/
#define WINNAME			L"D2DFrameWork"
#define WINSTARTX		100
#define WINSTARTY		100
#define WINSIZEX		1280
#define WINSIZEY		720
#define WINSTYLE		WS_CAPTION | WS_SYSMENU

/**********************************************************************
## 전처리기 ##
***********************************************************************/
#define SAFE_DELETE(p)		{if(p) {delete (p); (p) = NULL;}}
#define SAFE_DELETE_ARRAY(p)	{if(p) {delete[] (p); (p) = NULL;}}
#define SAFE_RELEASE(p){if(p){(p)->Release();(p) = NULL;}}
/**********************************************************************
## 전역 변수 ##
***********************************************************************/
extern HINSTANCE	_hInstance;
extern HWND			_hWnd;
extern Vector2		_ptMouse;
extern bool			_isDebug;

#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console") 