#include "stdafx.h"
#include "EventManager.h"
#include "Event.h"

// =================================================
// ########### Update ( �̺�Ʈ ȣ�� ) ###############d
// EventList�� �� ó�� Event�� Update ����
// Update�� ��� �Ϸᰡ �Ǹ� true�� ��ȯ 
// true�� ��ȯ�ϸ� EventList���� �����ϸ�
// �ٷ� ���� Event�� Update ���۵ȴ�.
// =================================================
void EventManager::Update()
{
	// EventList�� ���� ���������
	// �̺�Ʈ �������� �ʴ´�.
	if (EventList.size() != 0)
	{
		// �� ó�� �̺�Ʈ�� Update ��� �Ϸ�Ǹ�
		if (EventList[0]->UpdateEvent() == true)
		{
			// �� ó�� �̺�Ʈ�� �����Ѵ�.
			SAFE_DELETE(EventList[0]);
			EventList.erase(EventList.begin());
			// EventList�� size�� �ϳ� �پ��� ������
			// EventList�� ���� ����ִ��� �ٽ� Ȯ��
			if (EventList.size() != 0)
			{
				EventList[0]->StartEvent();
			}
		}
	}
}


// =================================================
// ########### AddEvent ( �̺�Ʈ �߰� ) #############
// �̺�Ʈ�� �̺�Ʈ �Ŵ����� �߰��Ѵ�
// �� ó�� ������ �̺�Ʈ��� StartEvent()�� ȣ���Ѵ�.
// �� �ڿ� ������ �̺�Ʈ���� �׳� push_back ���ش�.
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
