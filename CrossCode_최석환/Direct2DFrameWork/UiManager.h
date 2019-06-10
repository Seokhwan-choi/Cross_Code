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

	void addUi(ObjectType type, class GameObject* object);				// ������Ʈ �߰� �Լ�
	class GameObject* FindUi(ObjectType type, string name);				// ������Ʈ ã�� �Լ�
	vector<class GameObject*> FindUis(ObjectType type, string name);	// ���� �̸� ������Ʈ ��� ã�� �Լ�
	vector<class GameObject*>* FindUiList(ObjectType type);				// �ش� Ÿ�� ������Ʈ ��� ã�� �Լ�

	UiManager();
	~UiManager() {}
};





