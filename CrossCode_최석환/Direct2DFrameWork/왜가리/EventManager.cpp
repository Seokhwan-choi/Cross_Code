#include "stdafx.h"
#include "EventManager.h"
#include "Event.h"

// =================================================
// ########### Update ( 이벤트 호출 ) ###############d
// EventList의 맨 처음 Event만 Update 시작
// Update가 모두 완료가 되면 true를 반환 
// true를 반환하면 EventList에서 제거하면
// 바로 다음 Event가 Update 시작된다.
// =================================================
void EventManager::Update()
{
	// EventList가 텅텅 비어있으면
	// 이벤트 동작하지 않는다.
	if (EventList.size() != 0)
	{
		// 맨 처음 이벤트가 Update 모두 완료되면
		if (EventList[0]->UpdateEvent() == true)
		{
			// 맨 처음 이벤트를 삭제한다.
			SAFE_DELETE(EventList[0]);
			EventList.erase(EventList.begin());
			// EventList의 size가 하나 줄었기 때문에
			// EventList가 현재 비어있는지 다시 확인
			if (EventList.size() != 0)
			{
				EventList[0]->StartEvent();
			}
		}
	}
}


// =================================================
// ########### AddEvent ( 이벤트 추가 ) #############
// 이벤트를 이벤트 매니저에 추가한다
// 맨 처음 들어오는 이벤트라면 StartEvent()를 호출한다.
// 그 뒤에 들어오는 이벤트들은 그냥 push_back 해준다.
// =================================================
void EventManager::AddEvent(Event* event)
{
	if (EventList.size() == 0)
	{
		event->StartEvent();
	}
	EventList.push_back(event);
}

bool EventManager::Eventing()
{
	if (EventList.size() == 0)
	{
		return false;
	}
	else
	{
		return true;
	}
}

EventManager::EventManager()
{
}


EventManager::~EventManager()
{
}
