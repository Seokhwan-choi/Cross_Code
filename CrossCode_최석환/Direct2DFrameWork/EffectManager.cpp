#include "stdafx.h"
#include "Effect.h"

void EffectManager::ShowEffect(string key, Vector2 pos, Vector2 size, bool effect, int frameindex, float effecttime)
{
	EffectContainerIter Iter = Container.find(key);
	if (Iter != Container.end())
	{
		for (int i = 0; i < Iter->second.size(); ++i)
		{
			if (Iter->second[i]->GetShow()) continue;
			Iter->second[i]->Init(pos, size, effect, frameindex, effecttime);					// false => true
			break;
		}
	}
}

void EffectManager::ShowEffect(string key, Vector2 pos, Vector2 size, float angle, float effecttime)
{
	EffectContainerIter Iter = Container.find(key);
	if (Iter != Container.end())
	{
		for (int i = 0; i < Iter->second.size(); ++i)
		{
			if (Iter->second[i]->GetShow()) continue;
			Iter->second[i]->Init(pos, size, angle, effecttime);					// false => true
			break;
		}
	}
}

void EffectManager::Update()
{
	for (EffectContainerIter Iter = Container.begin(); Iter != Container.end(); ++Iter)
	{
		for (int i = 0; i < Iter->second.size(); ++i)
		{
			if (!Iter->second[i]->GetShow()) continue;
			Iter->second[i]->Update();
		}
	}
}

void EffectManager::Render()
{
	for (EffectContainerIter Iter = Container.begin(); Iter != Container.end(); ++Iter)
	{
		for (int i = 0; i < Iter->second.size(); ++i)
		{
			if (!Iter->second[i]->GetShow()) continue;
			if (!Iter->second[i]->GetEffect()) continue;
			Iter->second[i]->Render();
		}
	}
}

void EffectManager::Render2()
{
	for (EffectContainerIter Iter = Container.begin(); Iter != Container.end(); ++Iter)
	{
		for (int i = 0; i < Iter->second.size(); ++i)
		{
			if (!Iter->second[i]->GetShow()) continue;
			if (!Iter->second[i]->GetEffect()) Iter->second[i]->Render();
		}
	}
}

void EffectManager::Init()
{
	// ==================================================================
	// ######################### 이펙트 ( Effect ) #######################
	// ==================================================================
	// 맵툴 터치 이펙트
	Container.insert(make_pair("터치", vector<class Effect*>()));
	for (int i = 0; i < 10; ++i)
	{
		Container.find("터치")->second.push_back(new Effect("터치"));
	}

	// 캐릭터 먼지 잔상
	Container.insert(make_pair("먼지", vector<class Effect*>()));
	for (int i = 0; i < 15; ++i)
	{
		Container.find("먼지")->second.push_back(new Effect("먼지"));
	}

	// 캐릭터 먼지 잔상
	Container.insert(make_pair("먼지2", vector<class Effect*>()));
	for (int i = 0; i < 15; ++i)
	{
		Container.find("먼지2")->second.push_back(new Effect("먼지2"));
	}

	// 캐릭터 폭발
	Container.insert(make_pair("폭발", vector<class Effect*>()));
	for (int i = 0; i < 15; ++i)
	{
		Container.find("폭발")->second.push_back(new Effect("폭발"));
	}

	// 캐릭터 근접공격 1
	Container.insert(make_pair("근접공격1", vector<class Effect*>()));
	for (int i = 0; i < 5; ++i)
	{
		Container.find("근접공격1")->second.push_back(new Effect("근접공격1"));
	}

	// 캐릭터 근접공격 2
	Container.insert(make_pair("근접공격2", vector<class Effect*>()));
	for (int i = 0; i < 5; ++i)
	{
		Container.find("근접공격2")->second.push_back(new Effect("근접공격2"));
	}

	// 캐릭터 근접공격 3 마지막 공격
	Container.insert(make_pair("근접공격3", vector<class Effect*>()));
	for (int i = 0; i < 100; ++i)
	{
		Container.find("근접공격3")->second.push_back(new Effect("근접공격3"));
	}

	// 캐릭터 근접 공격 몬스터가 맞음
	Container.insert(make_pair("맞음", vector<class Effect*>()));
	for (int i = 0; i < 25; ++i)
	{
		Container.find("맞음")->second.push_back(new Effect("맞음"));
	}

	// 캐릭터 원거리 공격 몬스터가 맞음
	Container.insert(make_pair("맞음2", vector<class Effect*>()));
	for (int i = 0; i < 10; ++i)
	{
		Container.find("맞음2")->second.push_back(new Effect("맞음2"));
	}

	// 큰 슬라임 충전
	Container.insert(make_pair("충전", vector<class Effect*>()));
	for (int i = 0; i < 2; ++i)
	{
		Container.find("충전")->second.push_back(new Effect("충전"));
	}

	// 큰 슬라임 작은 전기1
	Container.insert(make_pair("번개", vector<class Effect*>()));
	for (int i = 0; i < 10; ++i)
	{
		Container.find("번개")->second.push_back(new Effect("번개"));
	}

	// 큰 슬라임 작은 전기1
	Container.insert(make_pair("번개2", vector<class Effect*>()));
	for (int i = 0; i < 10; ++i)
	{
		Container.find("번개2")->second.push_back(new Effect("번개2"));
	}

	// 큰 슬라임 작은 전기1
	Container.insert(make_pair("작은전기1", vector<class Effect*>()));
	for (int i = 0; i < 10; ++i)
	{
		Container.find("작은전기1")->second.push_back(new Effect("작은전기1"));
	}

	// 큰 슬라임 작은 전기2
	Container.insert(make_pair("작은전기2", vector<class Effect*>()));
	for (int i = 0; i < 10; ++i)
	{
		Container.find("작은전기2")->second.push_back(new Effect("작은전기2"));
	}

	// 큰 슬라임 큰 전기1
	Container.insert(make_pair("큰전기1", vector<class Effect*>()));
	for (int i = 0; i < 10; ++i)
	{
		Container.find("큰전기1")->second.push_back(new Effect("큰전기1"));
	}

	// 큰 슬라임 큰 전기2
	Container.insert(make_pair("큰전기2", vector<class Effect*>()));
	for (int i = 0; i < 10; ++i)
	{
		Container.find("큰전기2")->second.push_back(new Effect("큰전기2"));
	}

	// 작은 폭파 
	Container.insert(make_pair("작은폭파", vector<class Effect*>()));
	for (int i = 0; i < 10; ++i)
	{
		Container.find("작은폭파")->second.push_back(new Effect("작은폭파"));
	}

	// 큰 폭파
	Container.insert(make_pair("큰폭파", vector<class Effect*>()));
	for (int i = 0; i < 10; ++i)
	{
		Container.find("큰폭파")->second.push_back(new Effect("큰폭파"));
	}

	// 죽음 잔상
	Container.insert(make_pair("죽음잔상", vector<class Effect*>()));
	for (int i = 0; i < 10; ++i)
	{
		Container.find("죽음잔상")->second.push_back(new Effect("죽음잔상"));
	}



	// ===================================================================
	// ######################## 몬스터 잔상 ###############################
	// ===================================================================

	// 큰 슬라임 잔상
	Container.insert(make_pair("슬라임PageOneIdle", vector<class Effect*>()));
	for (int i = 0; i < 10; ++i)
	{
		Container.find("슬라임PageOneIdle")->second.push_back(new Effect("슬라임PageOneIdle"));
	}

	// 작은 슬라임 잔상
	Container.insert(make_pair("슬라임Hold", vector<class Effect*>()));
	for (int i = 0; i < 30; ++i)
	{
		Container.find("슬라임Hold")->second.push_back(new Effect("슬라임Hold"));
	}
}

EffectManager::EffectManager()
{
	
}


EffectManager::~EffectManager()
{
}
