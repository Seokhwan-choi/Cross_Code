#pragma once
#include "SingletonBase.h"
class EventManager : public SingletonBase<EventManager>
{
private:
	vector<class Event*> EventList;
public:

	void Update();
	void AddEvent(class Event* event);
	bool Eventing();					// 현재 이벤트 중이니..?

	EventManager();
	~EventManager();
};

