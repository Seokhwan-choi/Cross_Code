#include "stdafx.h"
#include "UiManager.h"
#include "GameObject.h"

// ==========================================================================
// ######################## ObjectList Init() ###############################
// ==========================================================================
void UiManager::Init()
{
	// �ݺ��ڸ� ����Ͽ� map�� Ž���Ѵ�.
	UiListIter = UiList.begin();
	for (UiListIter; UiListIter != UiList.end(); ++UiListIter)
	{
		// map->second = vector �ϳ��ϳ� Ž���ϸ�
		for (int i = 0; i < UiListIter->second.size(); ++i)
		{
			// object Init() ���ش�.
			UiListIter->second[i]->Init();
		}
	}
}

// ==========================================================================
// ####################### ObjectList Update() ##############################
// ==========================================================================
void UiManager::Update()
{
	// �ݺ��ڸ� ����Ͽ� map�� Ž���Ѵ�.
	UiListIter = UiList.begin();
	for (UiListIter; UiListIter != UiList.end(); ++UiListIter)
	{
		// map->second = vector �ϳ��ϳ� Ž���ϸ�
		for (int i = 0; i < UiListIter->second.size(); ++i)
		{
			// object�� Ȱ��ȭ(isActive)�� Update() ���ش�.
			if (UiListIter->second[i]->GetisActive())
				UiListIter->second[i]->Update();

			// object�� isLive�� false�̸� release() ���ְ�
			// SAFE_DELETE ȣ���Ͽ� �޸� ���� ���ش�.
			if (!UiListIter->second[i]->GetisLive())
			{
				UiListIter->second[i]->Release();
				SAFE_DELETE(UiListIter->second[i]);
				// �޸� ���� �� ���ͳ� null ������Ʈ ��ü ����
				UiListIter->second.erase(UiListIter->second.begin() + i);
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
void UiManager::Render()
{
	// �ݺ��ڸ� ����Ͽ� map�� Ž���Ѵ�.
	UiListIter = UiList.find(ObjectType::UI);
	// map->second = vector �ϳ��ϳ� Ž���ϸ�
	for (int i = 0; i < UiListIter->second.size(); ++i)
	{
		// object�� Ȱ��ȭ(isActive)�� Render() ���ش�.
		if (UiListIter->second[i]->GetisActive())
			UiListIter->second[i]->Render();
	}
}

void UiManager::Render2()
{
	// �ݺ��ڸ� ����Ͽ� map�� Ž���Ѵ�.
	UiListIter = UiList.find(ObjectType::Object);
	// map->second = vector �ϳ��ϳ� Ž���ϸ�
	for (int i = 0; i < UiListIter->second.size(); ++i)
	{
		// object�� Ȱ��ȭ(isActive)�� Render() ���ش�.
		if (UiListIter->second[i]->GetisActive())
			UiListIter->second[i]->Render();
	}
}

// ==========================================================================
// ####################### ObjectList Release() #############################
// ==========================================================================
void UiManager::Release()
{
	// �ݺ��ڸ� ����Ͽ� map�� Ž���Ѵ�.
	UiListIter = UiList.begin();
	for (UiListIter; UiListIter != UiList.end(); ++UiListIter)
	{
		// map->second = vector �ϳ��ϳ� Ž���ϸ�
		for (int i = 0; i < UiListIter->second.size(); ++i)
		{
			// object �ϳ��ϳ� release(), SAFE_DELETE
			// �޸𸮸� ���� ���ش�.
			UiListIter->second[i]->Release();
			SAFE_DELETE(UiListIter->second[i]);
		}
		// �޸� ������ ���� vector�� ����ش�.
		UiListIter->second.clear();
	}
	// �޸� ������ ��� ������ map�� ����ش�.
	//UiList.clear();
}

// ==========================================================================
// ######################### Object �߰� �Լ� ################################
// ==========================================================================
void UiManager::addUi(ObjectType type, GameObject* object)
{
	// �ش� Ÿ���� �����ϴ��� Ȯ���Ѵ�.
	UiListIter = UiList.find(type);
	// �ش� Ÿ���� ã�� ���ϸ� ObjectListIter == ObjectList.end() �� �ȴ�.
	// �ش� Ÿ���� ã�Ҵٸ�
	if (UiListIter != UiList.end())
	{
		// �ش� Ÿ���� vector�� object�� �߰� ���ش�.
		UiListIter->second.push_back(object);
	}
}

// ==========================================================================
// ######################### Object ã�� �Լ� ################################
// ==========================================================================
GameObject* UiManager::FindUi(ObjectType type, string name)
{
	// �ش� Ÿ���� �����ϴ��� Ȯ���Ѵ�.
	UiListIter = UiList.find(type);
	// �ش� Ÿ���� ã�� ���ϸ� ObjectListIter == ObjectList.end() �� �ȴ�.
	// �ش� Ÿ���� ã�Ҵٸ�
	if (UiListIter != UiList.end())
	{
		// �ش� Ÿ���� vector�� Ž���Ͽ�
		for (int i = 0; i < UiListIter->second.size(); ++i)
		{
			// ���� name(key)�� ������ object�� ��ȯ���ش�.
			if (name == UiListIter->second[i]->GetName())
			{
				return UiListIter->second[i];
			}
		}
	}
	return nullptr;
}

// ==========================================================================
// ###################### ���� �̸� Object ã�� �Լ� #########################
// ==========================================================================
vector<class GameObject*> UiManager::FindUis(ObjectType type, string name)
{
	// �ӽ� vector�ϳ��� �����Ѵ�.
	vector<class GameObject*> temp;
	// �ش� Ÿ���� �����ϴ��� Ȯ���Ѵ�.
	UiListIter = UiList.find(type);
	// �ش� Ÿ���� ã�� ���ϸ� ObjectListIter == ObjectList.end() �� �ȴ�.
	// �ش� Ÿ���� ã�Ҵٸ�
	if (UiListIter != UiList.end())
	{
		// �ش� Ÿ���� vector�� Ž���Ͽ�
		for (int i = 0; i < UiListIter->second.size(); ++i)
		{
			if (name == UiListIter->second[i]->GetName())
			{
				// ���� name(key)�� ������ object���� �ӽ� vector�� ��´�.
				temp.push_back(UiListIter->second[i]);
			}
		}

	}
	// Ž���� �Ϸ�Ǹ� �ӽ� vector�� ��ȯ���ش�.
	return vector<class GameObject*>(temp);
}

// ==========================================================================
// ###################### �ش� Type Object ã�� �Լ� #########################
// ==========================================================================
vector<class GameObject*>* UiManager::FindUiList(ObjectType type)
{
	// �ش� Ÿ���� �����ϴ��� Ȯ���Ѵ�.
	UiListIter = UiList.find(type);
	// �ش� Ÿ���� ã�� ���ϸ� ObjectListIter == ObjectList.end() �� �ȴ�.
	// �ش� Ÿ���� ã�Ҵٸ�
	if (UiListIter != UiList.end())
	{
		// �ش� Ÿ�Կ� �ִ� vector�� ��ȯ���ش�.
		return &UiListIter->second;
	}
}

// ==========================================================================
// #################### ObjectList �� vector �߰� ############################
// ==========================================================================
UiManager::UiManager()
{
	// addObject �ϱ� ���ؼ� ObjectList �ش� Ÿ�Ը��� �� vector�� �����صд�. 
	for (int i = 0; i < static_cast<int>(ObjectType::END); ++i)
	{
		UiList.insert(make_pair((ObjectType)i, vector<class GameObject*>()));
	}
}

