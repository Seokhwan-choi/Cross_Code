#pragma once
#include "SingletonBase.h"
class EventManager : public SingletonBase<EventManager>
{
private:
	vector<class Event*> EventList;
public:

	void Update();
	void AddEvent(class Event* event);
	bool Eventing();					// ���� �̺�Ʈ ���̴�..?

	EventManager();
	~EventManager();
};

