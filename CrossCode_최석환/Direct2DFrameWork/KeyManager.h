#pragma once
#include "SingletonBase.h"
#include <bitset> // 0,1 을 담을 수 있는 비트
				  // bool과 동일하며 훨씬 빠르다.
#define KEYMAX 256
class KeyManager : public SingletonBase<KeyManager>
{
private:
	bitset<KEYMAX> keyPast;				// 이전의 키 상태를 저장
	bitset<KEYMAX> keyCurrent;			// 현재의 키 상태를 저장

	bool _keyDown[KEYMAX];
public:
	void Update();
	//키가 한번만 눌렸냐?
	bool IsOnceKeyDown(int key);
	//키가 한번 눌렀다 띄었냐?
	bool IsOnceKeyUp(int key);
	//키가 계속 눌려 있냐?
	bool IsStayKeyDown(int key);
	//토글키냐?
	bool IsToggleKey(int key);

	KeyManager() {
		keyPast.reset();
		keyCurrent.reset();
	}
	~KeyManager() {}
};

