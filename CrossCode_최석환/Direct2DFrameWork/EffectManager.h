#pragma once
#include "singletonBase.h"
class EffectManager : public SingletonBase<EffectManager>
{
private:
	typedef map<string, vector<class Effect*>> EffectContainer;
	typedef map<string, vector<class Effect*>>::iterator EffectContainerIter;

	EffectContainer Container;
	//EffectContainerIter Iter;
public:
	
	void ShowEffect(string key, Vector2 pos, Vector2 size, bool effect = true, int frameindex = 0, float effecttime = 0.01f);
	void ShowEffect(string key, Vector2 pos, Vector2 size, float angle, float effecttime = 0.01f);
	void Update();
	void Render();
	void Render2();
	void Init();

	EffectManager();
	~EffectManager();
};

