#include "stdafx.h"
#include "SceneManager.h"
#include "LoadingScene.h"

SceneManager::SceneManager()
{
	Alpha = 0.f;
	_currentScene = nullptr;
	State = SceneState::None;
}

SceneManager::~SceneManager()
{
	//�������ι��
	miSceneList iter = _mSceneList.begin();
	for (; iter != _mSceneList.end();)
	{
		if (iter->second != NULL)
		{
			if (iter->second == _currentScene) iter->second->Release();
			SAFE_DELETE(iter->second);
			iter = _mSceneList.erase(iter);
		}
		else
		{
			++iter;
		}
	}
	_mSceneList.clear();
}

void SceneManager::Update(void)
{
	//������� �����ϸ� �ش��ϴ� ������� ������Ʈ�ض�
	if (_currentScene) _currentScene->Update();

	switch (State)
	{
	case SceneState::None:
		break;
	case SceneState::FadeOut:
		Alpha += 1.0f * TIMEMANAGER->getElapedTime();
		if (Alpha > 1.f)
		{
			Alpha = 1.f;
			State = SceneState::None;
		}
		break;

	case SceneState::FadeIn:
		Alpha -= 0.5f * TIMEMANAGER->getElapedTime();
		if (Alpha <= 0.f)
		{
			Alpha = 0.f;
			State = SceneState::None;
		}
		break;
	case SceneState::Load:
		break;
	case SceneState::END:
		break;
	default:
		break;
	}
}

void SceneManager::Render(void)
{
	//������� �����ϸ� �ش��ϴ� ������� �����ض�
	if (_currentScene) _currentScene->Render();

	if (Alpha != 0.f)
	{
		D2DRENDERER->FillRectangle({ 0,0, WINSIZEX, WINSIZEY }, D2D1::ColorF::Black, Alpha);
	}
}

GameNode* SceneManager::FindScene(string sceneName)
{
	miSceneList find = _mSceneList.find(sceneName);

	//��ã�Ҵٸ� E_FAIL
	if (find != _mSceneList.end())
	{
		return find->second;
	}
}

GameNode * SceneManager::AddScene(string sceneName, GameNode * scene)
{
	//���� ������ �׳� �� ����
	if (!scene) return NULL;

	//���� ������ �ʿ� ���
	_mSceneList.insert(make_pair(sceneName, scene));
	//mSceneList.insert(pair<string, gameNode*>(sceneName, scene));

	return scene;
}

HRESULT SceneManager::LoadScene(string sceneName)
{
	miSceneList find = _mSceneList.find(sceneName);

	//��ã�Ҵٸ� E_FAIL
	if (find == _mSceneList.end()) return E_FAIL;

	//�ٲٷ��� ���� ������� ���ٸ� E_FAIL
	if (find->second == _currentScene) return E_FAIL;

	if (_currentScene != nullptr)
	{
		_currentScene->Release();
		_currentScene = find->second;
		_currentScene->Init();

		return S_OK;
	}
	else
	{
		_currentScene = find->second;
		_currentScene->Init();

		return S_OK;
	}
}
