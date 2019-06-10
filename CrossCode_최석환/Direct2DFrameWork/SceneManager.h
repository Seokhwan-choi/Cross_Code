#pragma once
#include "SingletonBase.h"
//=============================================================
//	## sceneManager ## (���Ŵ���)
//=============================================================

class SceneManager : public SingletonBase<SceneManager>
{
private:
	typedef map<string,class GameNode*> mSceneList;
	typedef map<string,class GameNode*>::iterator miSceneList;

private:
	mSceneList _mSceneList;			// ������ ��(ȭ��)���� ��Ƶ� ��
	class GameNode* _currentScene;	// �����

	SceneState State;				// �� ����
	float Alpha;					// Fade In / Fade Out ���İ�

public:
	SceneManager();
	~SceneManager();

	void Update(void);
	void Render(void);

public:
	void SetSceneState(SceneState state) { this->State = state; }
	void SetAlpha(float alpha) { this->Alpha = alpha; }
	float GetAlpha() { return this->Alpha; }

public:

	GameNode* GetCurrent() { return _currentScene; }
	GameNode* FindScene(string sceneName);

	//���߰�
	class GameNode* AddScene(string sceneName,class GameNode* scene);
	//������ (�ش�� �ε�)
	HRESULT LoadScene(string sceneName);
};

