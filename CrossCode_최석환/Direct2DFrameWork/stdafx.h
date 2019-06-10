// stdafx.h : ���� ��������� ���� ��������� �ʴ�
// ǥ�� �ý��� ���� ���� �Ǵ� ������Ʈ ���� ���� ������
// ��� �ִ� ���� �����Դϴ�.
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // ���� ������ �ʴ� ������ Windows ������� �����մϴ�.
// Windows ��� ����:
#include <windows.h>

// C ��Ÿ�� ��� �����Դϴ�.
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
//���̷�Ʈ2D ���̺귯��
#pragma comment(lib, "d2d1.lib")
#include <D2D1.h>
#include <d2d1helper.h>

//���̷�Ʈ Write ���̺귯�� (�ؽ�Ʈ)
#pragma comment(lib, "dwrite.lib")
#include <dwrite.h>

//WIC ���̺귯��
#pragma comment(lib, "WindowsCodecs.lib")
#include <wincodec.h>

/**********************************************************************
## ���� enum stuct ##
***********************************************************************/
enum class Pivot : int
{
	LeftTop,Center,Bottom
};

// ObjectManager���� ���
enum class ObjectType : int
{
	Tile, Object, Player, Enemy, UI, END
};

enum class SceneState : int
{
	None, FadeIn, FadeOut, Load, END
};

/**********************************************************************
## ��ƿ ##
***********************************************************************/
#include "GameNode.h"
#include "Vector2.h"
#include "CommonMacroFunction.h"
#include "StringHelper.h"
#include "Math.h"
#include "Image.h"

/**********************************************************************
## �Ŵ��� ##
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
## Window���� ##
***********************************************************************/
#define WINNAME			L"D2DFrameWork"
#define WINSTARTX		100
#define WINSTARTY		100
#define WINSIZEX		1280
#define WINSIZEY		720
#define WINSTYLE		WS_CAPTION | WS_SYSMENU

/**********************************************************************
## ��ó���� ##
***********************************************************************/
#define SAFE_DELETE(p)		{if(p) {delete (p); (p) = NULL;}}
#define SAFE_DELETE_ARRAY(p)	{if(p) {delete[] (p); (p) = NULL;}}
#define SAFE_RELEASE(p){if(p){(p)->Release();(p) = NULL;}}
/**********************************************************************
## ���� ���� ##
***********************************************************************/
extern HINSTANCE	_hInstance;
extern HWND			_hWnd;
extern Vector2		_ptMouse;
extern bool			_isDebug;

#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console") 