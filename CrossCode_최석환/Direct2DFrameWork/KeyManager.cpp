#include "stdafx.h"
#include "KeyManager.h"

void KeyManager::Update()
{
	keyPast = keyCurrent;
}

// ########### 키를 한번 눌렀을 때 ###########
// int key : 누른 키
// return bool : KeyDown 여부
// ##########################################
bool KeyManager::IsOnceKeyDown(int key)
{
	if (GetAsyncKeyState(key) & 0x8000)
	{
		//그 키가 눌려있지 않았다면
		if (!keyPast[key])
		{
			// 현재키를 눌린 상태로 바꾸고 return true
			keyCurrent.set(key, true);
			return true;
		}
	}
	// 해당 키를 누르지 않은 상태
	else
	{
		// 현재키를 누르지 않은 생태로 바꿈
		keyCurrent.set(key, false);
	}

	return false;
}

// ######### 키를 한번 눌렀다 땠을 때 #########
// int key : 누른 키
// return bool : KeyUp 여부
// ##########################################
bool KeyManager::IsOnceKeyUp(int key)
{
	// 해당키를 누른 상태 현재키를 눌린 상태로 바꿈
	if (GetAsyncKeyState(key) & 0x8000)
	{
		keyCurrent.set(key, true);
	}
	// 해당키를 누르지 않은 생태
	else
	{
		// 그 키가 눌려있었다면
		if (keyPast[key])
		{
			// 현재키를 누르지 않은 생태로 바꾸고 return true
			keyCurrent.set(key, false);
			return true;
		}
	}

	return false;
}

// ######### 키를 계속 누르고 있을 때 #########
// int key : 누른 키
// return bool : Staykey 여부
// ##########################################
bool KeyManager::IsStayKeyDown(int key)
{
	if (GetAsyncKeyState(key) & 0x8000)
	{
		return true;
	}
	return false;
}

// ######### 키 토글키 활성 화 여부  ##########
// int key : 누른 키
// return bool : ToggleKey 여부
// ##########################################
bool KeyManager::IsToggleKey(int key)
{
	if (GetKeyState(key) & 0x0001)
	{
		return true;
	}

	return false;
}
