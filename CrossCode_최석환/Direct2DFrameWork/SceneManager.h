#pragma once
#include "SingletonBase.h"
//=============================================================
//	## sceneManager ## (æ¿∏≈¥œ¡Æ)
//=============================================================

class SceneManager : public SingletonBase<SceneManager>
{
private:
	typedef map<string,class GameNode*> mSceneList;
	typedef map<string,class GameNode*>::iterator miSceneList;

private:
	mSceneList _mSceneList;			// ∞¢∞¢¿« æ¿(»≠∏È)µÈ¿ª ¥„æ∆µ— ∏ 
	class GameNode* _currentScene;	// «ˆ¿Áæ¿

	SceneState State;				// æ¿ ªÛ≈¬
	float Alpha;					// Fade In / Fade Out æÀ∆ƒ∞™

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

	//æ¿√ﬂ∞°
	class GameNode* AddScene(string sceneName,class GameNode* scene);
	//æ¿∫Ø∞Ê («ÿ¥Áæ¿ ∑ŒµÂ)
	HRESULT LoadScene(string sceneName);
};

