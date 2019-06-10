#include "stdafx.h"
#include "ObjectManager.h"
#include "GameObject.h"

// ==========================================================================
// ######################## ObjectList Init() ###############################
// ==========================================================================
void ObjectManager::Init()
{
	// �ݺ��ڸ� ����Ͽ� map�� Ž���Ѵ�.
	ObjectListIter = ObjectList.begin();
	for (ObjectListIter; ObjectListIter != ObjectList.end(); ++ObjectListIter)
	{
		// map->second = vector �ϳ��ϳ� Ž���ϸ�
		for (int i = 0; i < ObjectListIter->second.size(); ++i) 
		{
			// object Init() ���ش�.
			ObjectListIter->second[i]->Init();
		}
	}
}

// ==========================================================================
// ####################### ObjectList Update() ##############################
// ==========================================================================
void ObjectManager::Update()
{
	// �ݺ��ڸ� ����Ͽ� map�� Ž���Ѵ�.
	ObjectListIter = ObjectList.begin();
	for (ObjectListIter; ObjectListIter != ObjectList.end(); ++ObjectListIter)
	{
		// map->second = vector �ϳ��ϳ� Ž���ϸ�
		for (int i = 0; i < ObjectListIter->second.size(); ++i)
		{
			// object�� Ȱ��ȭ(isActive)�� Update() ���ش�.
			if(ObjectListIter->second[i]->GetisActive())
				ObjectListIter->second[i]->Update();

			// object�� isLive�� false�̸� release() ���ְ�
			// SAFE_DELETE ȣ���Ͽ� �޸� ���� ���ش�.
			if (!ObjectListIter->second[i]->GetisLive())
			{
				ObjectListIter->second[i]->Release();
				SAFE_DELETE(ObjectListIter->second[i]);
				// �޸� ���� �� ���ͳ� null ������Ʈ ��ü ����
				ObjectListIter->second.erase(ObjectListIter->second.begin() + i);
				// ���� ��ü�� ���ŉ�⿡ ���� ��ü�� �ε����� ���� ��ü�� �����Ƿ�
				// ���� ��ü�� �ε����� �����ϱ����� i �� ����
				i--;
			}
		}
	}
}

// ==========================================================================
// ####################### ObjectList Render() ##############################
// ==========================================================================
void ObjectManager::Render()
{
	// �ݺ��ڸ� ����Ͽ� map�� Ž���Ѵ�.
	ObjectListIter = ObjectList.begin();
	for (ObjectListIter; ObjectListIter != ObjectList.end(); ++ObjectListIter)
	{
		if (ObjectListIter == ObjectList.find(ObjectType::UI)) continue;
		// map->second = vector �ϳ��ϳ� Ž���ϸ�
		for (int i = 0; i < ObjectListIter->second.size(); ++i)
		{
			// object�� Ȱ��ȭ(isActive)�� Render() ���ش�.
			if (ObjectListIter->second[i]->GetisActive())
				ObjectListIter->second[i]->Render();
		}
	}
}

// ==========================================================================
// ####################### ObjectList Release() #############################
// ==========================================================================
void ObjectManager::Release()
{
	// �ݺ��ڸ� ����Ͽ� map�� Ž���Ѵ�.
	ObjectListIter = ObjectList.begin();
	for (ObjectListIter; ObjectListIter != ObjectList.end(); ++ObjectListIter)
	{
		// map->second = vector �ϳ��ϳ� Ž���ϸ�
		for (int i = 0; i < ObjectListIter->second.size(); ++i)
		{
			// object �ϳ��ϳ� release(), SAFE_DELETE
			// �޸𸮸� ���� ���ش�.
			ObjectListIter->second[i]->Release();
			SAFE_DELETE(ObjectListIter->second[i]);
		}
		// �޸� ������ ���� vector�� ����ش�.
		ObjectListIter->second.clear();
	}
	// �޸� ������ ��� ������ map�� ����ش�.
	//ObjectList.clear();
}

// ==========================================================================
// ######################### Object �߰� �Լ� ################################
// ==========================================================================
void ObjectManager::addObject(ObjectType type, GameObject * object)
{
	// �ش� Ÿ���� �����ϴ��� Ȯ���Ѵ�.
	ObjectListIter = ObjectList.find(type);
	// �ش� Ÿ���� ã�� ���ϸ� ObjectListIter == ObjectList.end() �� �ȴ�.
	// �ش� Ÿ���� ã�Ҵٸ�
	if (ObjectListIter != ObjectList.end())
	{
		// �ش� Ÿ���� vector�� object�� �߰� ���ش�.
		ObjectListIter->second.push_back(object);
	}
}

// ==========================================================================
// ######################### Object ã�� �Լ� ################################
// ==========================================================================
GameObject* ObjectManager::FindObject(ObjectType type, string name)
{
	// �ش� Ÿ���� �����ϴ��� Ȯ���Ѵ�.
	ObjectListIter = ObjectList.find(type);
	// �ش� Ÿ���� ã�� ���ϸ� ObjectListIter == ObjectList.end() �� �ȴ�.
	// �ش� Ÿ���� ã�Ҵٸ�
	if (ObjectListIter != ObjectList.end())
	{
		// �ش� Ÿ���� vector�� Ž���Ͽ�
		for (int i = 0; i < ObjectListIter->second.size(); ++i)
		{
			// ���� name(key)�� ������ object�� ��ȯ���ش�.
			if (name == ObjectListIter->second[i]->GetName())
			{
				return ObjectListIter->second[i];
			}
		}
	}
	return nullptr;
}

// ==========================================================================
// ###################### ���� �̸� Object ã�� �Լ� #########################
// ==========================================================================
vector<class GameObject*> ObjectManager::FindObjects(ObjectType type, string name)
{
	// �ӽ� vector�ϳ��� �����Ѵ�.
	vector<class GameObject*> temp;
	// �ش� Ÿ���� �����ϴ��� Ȯ���Ѵ�.
	ObjectListIter = ObjectList.find(type);
	// �ش� Ÿ���� ã�� ���ϸ� ObjectListIter == ObjectList.end() �� �ȴ�.
	// �ش� Ÿ���� ã�Ҵٸ�
	if (ObjectListIter != ObjectList.end()) 
	{
		// �ش� Ÿ���� vector�� Ž���Ͽ�
		for (int i = 0; i < ObjectListIter->second.size(); ++i)
		{
			if (name == ObjectListIter->second[i]->GetName())
			{
				// ���� name(key)�� ������ object���� �ӽ� vector�� ��´�.
				temp.push_back(ObjectListIter->second[i]);
			}
		}

	}
	// Ž���� �Ϸ�Ǹ� �ӽ� vector�� ��ȯ���ش�.
	return vector<class GameObject*>(temp);
}

// ==========================================================================
// ###################### �ش� Type Object ã�� �Լ� #########################
// ==========================================================================
vector<class GameObject*> ObjectManager::FindObjectList(ObjectType type)
{
	// �ش� Ÿ���� �����ϴ��� Ȯ���Ѵ�.
	ObjectListIter = ObjectList.find(type);
	// �ش� Ÿ���� ã�� ���ϸ� ObjectListIter == ObjectList.end() �� �ȴ�.
	// �ش� Ÿ���� ã�Ҵٸ�
	if (ObjectListIter != ObjectList.end())
	{
		// �ش� Ÿ�Կ� �ִ� vector�� ��ȯ���ش�.
		return vector<class GameObject*>(ObjectListIter->second);
	}
}

// ==========================================================================
// #################### ObjectList �� vector �߰� ############################
// ==========================================================================
ObjectManager::ObjectManager()
{
	// addObject �ϱ� ���ؼ� ObjectList �ش� Ÿ�Ը��� �� vector�� �����صд�. 
	for (int i = 0; i < static_cast<int>(ObjectType::END); ++i) 
	{
		ObjectList.insert(make_pair((ObjectType)i, vector<class GameObject*>()));
	}
}
