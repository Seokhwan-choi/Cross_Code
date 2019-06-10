#include "stdafx.h"
#include "ObjectManager.h"
#include "GameObject.h"

// ==========================================================================
// ######################## ObjectList Init() ###############################
// ==========================================================================
void ObjectManager::Init()
{
	// 반복자를 사용하여 map을 탐색한다.
	ObjectListIter = ObjectList.begin();
	for (ObjectListIter; ObjectListIter != ObjectList.end(); ++ObjectListIter)
	{
		// map->second = vector 하나하나 탐색하며
		for (int i = 0; i < ObjectListIter->second.size(); ++i) 
		{
			// object Init() 해준다.
			ObjectListIter->second[i]->Init();
		}
	}
}

// ==========================================================================
// ####################### ObjectList Update() ##############################
// ==========================================================================
void ObjectManager::Update()
{
	// 반복자를 사용하여 map을 탐색한다.
	ObjectListIter = ObjectList.begin();
	for (ObjectListIter; ObjectListIter != ObjectList.end(); ++ObjectListIter)
	{
		// map->second = vector 하나하나 탐색하며
		for (int i = 0; i < ObjectListIter->second.size(); ++i)
		{
			// object가 활성화(isActive)면 Update() 해준다.
			if(ObjectListIter->second[i]->GetisActive())
				ObjectListIter->second[i]->Update();

			// object가 isLive가 false이면 release() 해주고
			// SAFE_DELETE 호출하여 메모리 해제 해준다.
			if (!ObjectListIter->second[i]->GetisLive())
			{
				ObjectListIter->second[i]->Release();
				SAFE_DELETE(ObjectListIter->second[i]);
				// 메모리 해제 후 벡터내 null 오브젝트 객체 제거
				ObjectListIter->second.erase(ObjectListIter->second.begin() + i);
				// 현재 객체가 제거됬기에 다음 객체의 인덱스가 현재 객체와 같으므로
				// 다음 객체의 인덱스에 접근하기위해 i 값 감소
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
	// 반복자를 사용하여 map을 탐색한다.
	ObjectListIter = ObjectList.begin();
	for (ObjectListIter; ObjectListIter != ObjectList.end(); ++ObjectListIter)
	{
		if (ObjectListIter == ObjectList.find(ObjectType::UI)) continue;
		// map->second = vector 하나하나 탐색하며
		for (int i = 0; i < ObjectListIter->second.size(); ++i)
		{
			// object가 활성화(isActive)면 Render() 해준다.
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
	// 반복자를 사용하여 map을 탐색한다.
	ObjectListIter = ObjectList.begin();
	for (ObjectListIter; ObjectListIter != ObjectList.end(); ++ObjectListIter)
	{
		// map->second = vector 하나하나 탐색하며
		for (int i = 0; i < ObjectListIter->second.size(); ++i)
		{
			// object 하나하나 release(), SAFE_DELETE
			// 메모리를 해제 해준다.
			ObjectListIter->second[i]->Release();
			SAFE_DELETE(ObjectListIter->second[i]);
		}
		// 메모리 해제가 끝난 vector는 비워준다.
		ObjectListIter->second.clear();
	}
	// 메모리 해제가 모두 끝나면 map을 비워준다.
	//ObjectList.clear();
}

// ==========================================================================
// ######################### Object 추가 함수 ################################
// ==========================================================================
void ObjectManager::addObject(ObjectType type, GameObject * object)
{
	// 해당 타입이 존재하는지 확인한다.
	ObjectListIter = ObjectList.find(type);
	// 해당 타입을 찾지 못하면 ObjectListIter == ObjectList.end() 가 된다.
	// 해당 타입을 찾았다면
	if (ObjectListIter != ObjectList.end())
	{
		// 해당 타입의 vector에 object를 추가 해준다.
		ObjectListIter->second.push_back(object);
	}
}

// ==========================================================================
// ######################### Object 찾기 함수 ################################
// ==========================================================================
GameObject* ObjectManager::FindObject(ObjectType type, string name)
{
	// 해당 타입이 존재하는지 확인한다.
	ObjectListIter = ObjectList.find(type);
	// 해당 타입을 찾지 못하면 ObjectListIter == ObjectList.end() 가 된다.
	// 해당 타입을 찾았다면
	if (ObjectListIter != ObjectList.end())
	{
		// 해당 타입의 vector를 탐색하여
		for (int i = 0; i < ObjectListIter->second.size(); ++i)
		{
			// 같은 name(key)를 가지는 object를 반환해준다.
			if (name == ObjectListIter->second[i]->GetName())
			{
				return ObjectListIter->second[i];
			}
		}
	}
	return nullptr;
}

// ==========================================================================
// ###################### 같은 이름 Object 찾기 함수 #########################
// ==========================================================================
vector<class GameObject*> ObjectManager::FindObjects(ObjectType type, string name)
{
	// 임시 vector하나를 생성한다.
	vector<class GameObject*> temp;
	// 해당 타입이 존재하는지 확인한다.
	ObjectListIter = ObjectList.find(type);
	// 해당 타입을 찾지 못하면 ObjectListIter == ObjectList.end() 가 된다.
	// 해당 타입을 찾았다면
	if (ObjectListIter != ObjectList.end()) 
	{
		// 해당 타입의 vector를 탐색하여
		for (int i = 0; i < ObjectListIter->second.size(); ++i)
		{
			if (name == ObjectListIter->second[i]->GetName())
			{
				// 같은 name(key)를 가지는 object들을 임시 vector에 담는다.
				temp.push_back(ObjectListIter->second[i]);
			}
		}

	}
	// 탐색이 완료되면 임시 vector를 반환해준다.
	return vector<class GameObject*>(temp);
}

// ==========================================================================
// ###################### 해당 Type Object 찾기 함수 #########################
// ==========================================================================
vector<class GameObject*> ObjectManager::FindObjectList(ObjectType type)
{
	// 해당 타입이 존재하는지 확인한다.
	ObjectListIter = ObjectList.find(type);
	// 해당 타입을 찾지 못하면 ObjectListIter == ObjectList.end() 가 된다.
	// 해당 타입을 찾았다면
	if (ObjectListIter != ObjectList.end())
	{
		// 해당 타입에 있는 vector를 반환해준다.
		return vector<class GameObject*>(ObjectListIter->second);
	}
}

// ==========================================================================
// #################### ObjectList 빈 vector 추가 ############################
// ==========================================================================
ObjectManager::ObjectManager()
{
	// addObject 하기 위해서 ObjectList 해당 타입마다 빈 vector를 생성해둔다. 
	for (int i = 0; i < static_cast<int>(ObjectType::END); ++i) 
	{
		ObjectList.insert(make_pair((ObjectType)i, vector<class GameObject*>()));
	}
}
