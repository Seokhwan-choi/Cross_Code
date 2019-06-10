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
	// ######################### ����Ʈ ( Effect ) #######################
	// ==================================================================
	// ���� ��ġ ����Ʈ
	Container.insert(make_pair("��ġ", vector<class Effect*>()));
	for (int i = 0; i < 10; ++i)
	{
		Container.find("��ġ")->second.push_back(new Effect("��ġ"));
	}

	// ĳ���� ���� �ܻ�
	Container.insert(make_pair("����", vector<class Effect*>()));
	for (int i = 0; i < 15; ++i)
	{
		Container.find("����")->second.push_back(new Effect("����"));
	}

	// ĳ���� ���� �ܻ�
	Container.insert(make_pair("����2", vector<class Effect*>()));
	for (int i = 0; i < 15; ++i)
	{
		Container.find("����2")->second.push_back(new Effect("����2"));
	}

	// ĳ���� ����
	Container.insert(make_pair("����", vector<class Effect*>()));
	for (int i = 0; i < 15; ++i)
	{
		Container.find("����")->second.push_back(new Effect("����"));
	}

	// ĳ���� �������� 1
	Container.insert(make_pair("��������1", vector<class Effect*>()));
	for (int i = 0; i < 5; ++i)
	{
		Container.find("��������1")->second.push_back(new Effect("��������1"));
	}

	// ĳ���� �������� 2
	Container.insert(make_pair("��������2", vector<class Effect*>()));
	for (int i = 0; i < 5; ++i)
	{
		Container.find("��������2")->second.push_back(new Effect("��������2"));
	}

	// ĳ���� �������� 3 ������ ����
	Container.insert(make_pair("��������3", vector<class Effect*>()));
	for (int i = 0; i < 100; ++i)
	{
		Container.find("��������3")->second.push_back(new Effect("��������3"));
	}

	// ĳ���� ���� ���� ���Ͱ� ����
	Container.insert(make_pair("����", vector<class Effect*>()));
	for (int i = 0; i < 25; ++i)
	{
		Container.find("����")->second.push_back(new Effect("����"));
	}

	// ĳ���� ���Ÿ� ���� ���Ͱ� ����
	Container.insert(make_pair("����2", vector<class Effect*>()));
	for (int i = 0; i < 10; ++i)
	{
		Container.find("����2")->second.push_back(new Effect("����2"));
	}

	// ū ������ ����
	Container.insert(make_pair("����", vector<class Effect*>()));
	for (int i = 0; i < 2; ++i)
	{
		Container.find("����")->second.push_back(new Effect("����"));
	}

	// ū ������ ���� ����1
	Container.insert(make_pair("����", vector<class Effect*>()));
	for (int i = 0; i < 10; ++i)
	{
		Container.find("����")->second.push_back(new Effect("����"));
	}

	// ū ������ ���� ����1
	Container.insert(make_pair("����2", vector<class Effect*>()));
	for (int i = 0; i < 10; ++i)
	{
		Container.find("����2")->second.push_back(new Effect("����2"));
	}

	// ū ������ ���� ����1
	Container.insert(make_pair("��������1", vector<class Effect*>()));
	for (int i = 0; i < 10; ++i)
	{
		Container.find("��������1")->second.push_back(new Effect("��������1"));
	}

	// ū ������ ���� ����2
	Container.insert(make_pair("��������2", vector<class Effect*>()));
	for (int i = 0; i < 10; ++i)
	{
		Container.find("��������2")->second.push_back(new Effect("��������2"));
	}

	// ū ������ ū ����1
	Container.insert(make_pair("ū����1", vector<class Effect*>()));
	for (int i = 0; i < 10; ++i)
	{
		Container.find("ū����1")->second.push_back(new Effect("ū����1"));
	}

	// ū ������ ū ����2
	Container.insert(make_pair("ū����2", vector<class Effect*>()));
	for (int i = 0; i < 10; ++i)
	{
		Container.find("ū����2")->second.push_back(new Effect("ū����2"));
	}

	// ���� ���� 
	Container.insert(make_pair("��������", vector<class Effect*>()));
	for (int i = 0; i < 10; ++i)
	{
		Container.find("��������")->second.push_back(new Effect("��������"));
	}

	// ū ����
	Container.insert(make_pair("ū����", vector<class Effect*>()));
	for (int i = 0; i < 10; ++i)
	{
		Container.find("ū����")->second.push_back(new Effect("ū����"));
	}

	// ���� �ܻ�
	Container.insert(make_pair("�����ܻ�", vector<class Effect*>()));
	for (int i = 0; i < 10; ++i)
	{
		Container.find("�����ܻ�")->second.push_back(new Effect("�����ܻ�"));
	}



	// ===================================================================
	// ######################## ���� �ܻ� ###############################
	// ===================================================================

	// ū ������ �ܻ�
	Container.insert(make_pair("������PageOneIdle", vector<class Effect*>()));
	for (int i = 0; i < 10; ++i)
	{
		Container.find("������PageOneIdle")->second.push_back(new Effect("������PageOneIdle"));
	}

	// ���� ������ �ܻ�
	Container.insert(make_pair("������Hold", vector<class Effect*>()));
	for (int i = 0; i < 30; ++i)
	{
		Container.find("������Hold")->second.push_back(new Effect("������Hold"));
	}
}

EffectManager::EffectManager()
{
	
}


EffectManager::~EffectManager()
{
}
