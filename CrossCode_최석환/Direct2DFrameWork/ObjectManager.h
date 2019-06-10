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

	void addObject(ObjectType type, class GameObject* object);				// ������Ʈ �߰� �Լ�
	class GameObject* FindObject(ObjectType type, string name);				// ������Ʈ ã�� �Լ�
	vector<class GameObject*> FindObjects(ObjectType type, string name);	// ���� �̸� ������Ʈ ��� ã�� �Լ�
	vector<class GameObject*> FindObjectList(ObjectType type);				// �ش� Ÿ�� ������Ʈ ��� ã�� �Լ�

	ObjectManager();
	~ObjectManager() {}
};



