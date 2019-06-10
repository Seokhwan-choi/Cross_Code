#pragma once
#include "SingletonBase.h"
class ObjectManager :
	public SingletonBase<ObjectManager>
{
private:
	map<ObjectType, vector<class GameObject*>> ObjectList;
	map<ObjectType, vector<class GameObject*>>::iterator ObjectListIter;
public:

	void Init();
	void Update();
	void Render();
	void Release();

	void addObject(ObjectType type, class GameObject* object);				// 오브젝트 추가 함수
	class GameObject* FindObject(ObjectType type, string name);				// 오브젝트 찾기 함수
	vector<class GameObject*> FindObjects(ObjectType type, string name);	// 같은 이름 오브젝트 모두 찾기 함수
	vector<class GameObject*> FindObjectList(ObjectType type);				// 해당 타입 오브젝트 모두 찾기 함수

	ObjectManager();
	~ObjectManager() {}
};



