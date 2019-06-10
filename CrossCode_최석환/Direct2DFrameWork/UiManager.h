#pragma once
#include "SingletonBase.h"
class UiManager :
	public SingletonBase<UiManager>
{
private:
	map<ObjectType, vector<class GameObject*>> UiList;
	map<ObjectType, vector<class GameObject*>>::iterator UiListIter;
public:

	void Init();
	void Update();
	void Render();
	void Render2();
	void Release();

	void addUi(ObjectType type, class GameObject* object);				// 오브젝트 추가 함수
	class GameObject* FindUi(ObjectType type, string name);				// 오브젝트 찾기 함수
	vector<class GameObject*> FindUis(ObjectType type, string name);	// 같은 이름 오브젝트 모두 찾기 함수
	vector<class GameObject*>* FindUiList(ObjectType type);				// 해당 타입 오브젝트 모두 찾기 함수

	UiManager();
	~UiManager() {}
};





