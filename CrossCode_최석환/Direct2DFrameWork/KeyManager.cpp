#include "stdafx.h"
#include "KeyManager.h"

void KeyManager::Update()
{
	keyPast = keyCurrent;
}

// ########### Ű�� �ѹ� ������ �� ###########
// int key : ���� Ű
// return bool : KeyDown ����
// ##########################################
bool KeyManager::IsOnceKeyDown(int key)
{
	if (GetAsyncKeyState(key) & 0x8000)
	{
		//�� Ű�� �������� �ʾҴٸ�
		if (!keyPast[key])
		{
			// ����Ű�� ���� ���·� �ٲٰ� return true
			keyCurrent.set(key, true);
			return true;
		}
	}
	// �ش� Ű�� ������ ���� ����
	else
	{
		// ����Ű�� ������ ���� ���·� �ٲ�
		keyCurrent.set(key, false);
	}

	return false;
}

// ######### Ű�� �ѹ� ������ ���� �� #########
// int key : ���� Ű
// return bool : KeyUp ����
// ##########################################
bool KeyManager::IsOnceKeyUp(int key)
{
	// �ش�Ű�� ���� ���� ����Ű�� ���� ���·� �ٲ�
	if (GetAsyncKeyState(key) & 0x8000)
	{
		keyCurrent.set(key, true);
	}
	// �ش�Ű�� ������ ���� ����
	else
	{
		// �� Ű�� �����־��ٸ�
		if (keyPast[key])
		{
			// ����Ű�� ������ ���� ���·� �ٲٰ� return true
			keyCurrent.set(key, false);
			return true;
		}
	}

	return false;
}

// ######### Ű�� ��� ������ ���� �� #########
// int key : ���� Ű
// return bool : Staykey ����
// ##########################################
bool KeyManager::IsStayKeyDown(int key)
{
	if (GetAsyncKeyState(key) & 0x8000)
	{
		return true;
	}
	return false;
}

// ######### Ű ���Ű Ȱ�� ȭ ����  ##########
// int key : ���� Ű
// return bool : ToggleKey ����
// ##########################################
bool KeyManager::IsToggleKey(int key)
{
	if (GetKeyState(key) & 0x0001)
	{
		return true;
	}

	return false;
}
