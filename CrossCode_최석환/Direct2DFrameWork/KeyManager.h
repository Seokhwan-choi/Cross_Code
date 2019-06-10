#pragma once
#include "SingletonBase.h"
#include <bitset> // 0,1 �� ���� �� �ִ� ��Ʈ
				  // bool�� �����ϸ� �ξ� ������.
#define KEYMAX 256
class KeyManager : public SingletonBase<KeyManager>
{
private:
	bitset<KEYMAX> keyPast;				// ������ Ű ���¸� ����
	bitset<KEYMAX> keyCurrent;			// ������ Ű ���¸� ����

	bool _keyDown[KEYMAX];
public:
	void Update();
	//Ű�� �ѹ��� ���ȳ�?
	bool IsOnceKeyDown(int key);
	//Ű�� �ѹ� ������ �����?
	bool IsOnceKeyUp(int key);
	//Ű�� ��� ���� �ֳ�?
	bool IsStayKeyDown(int key);
	//���Ű��?
	bool IsToggleKey(int key);

	KeyManager() {
		keyPast.reset();
		keyCurrent.reset();
	}
	~KeyManager() {}
};

