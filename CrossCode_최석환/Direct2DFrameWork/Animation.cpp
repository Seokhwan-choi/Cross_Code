#include "stdafx.h"
#include "Animation.h"


Animation::Animation()
	: _isPlay(false), _isLoop(true), _CurrentIndex(0)
	, _CurrentTime(0.f), _UpdateTime(0.1f)
{

}


Animation::~Animation()
{
	_FrameList.clear();
}


void Animation::Stop()
{
	_isPlay = false;
	_CurrentIndex = 0;
	_CurrentTime = 0.f;
}

void Animation::Pause()
{
	_isPlay = false;
}

void Animation::Play()
{
	_isPlay = true;
}

void Animation::UpdateFrame()
{
	if (_isPlay)
	{
		// ������ �ð��� DeltaTime ���� ���� �ش�.
		_CurrentTime += TIMEMANAGER->getElapedTime();
		if (_CurrentTime >= _UpdateTime)
		{
			// ���� �ð� ���� ������ �ð��� Ŀ����
			// ���� ������ �ε����� ���� ��Ų��.
			_CurrentIndex++;
			while (_CurrentTime >= _UpdateTime)
				// ������ �ð��� �ʱ�ȭ ���ش�.
				_CurrentTime -= _UpdateTime;
		}
		// ���� ���� ������ �ε����� 
		// ������ ����Ʈ�� size ���� Ŀ���ٸ� ( ������ ���� �������� ���� )
		if (_CurrentIndex >= static_cast<int>(_FrameList.size()))
		{

			// ����(Loop) �ݺ��Ѵٸ�
			// ������ �ε��� 0(ó��) ���� �ʱ�ȭ
			if (_isLoop)
			{
				_CurrentIndex = 0;
			}
			// �ݺ����� �ʴ´ٸ�
			// ������ �ε����� -- ������ �������� �״�� �а�
			// _isPlay�� ���ش�.
			else 
			{
				_CurrentIndex--;
				_isPlay = false;
			}
		}
	}
}

void Animation::SetFrameList(int startX, int startY, int endX, int endY, bool reverse)
{
	_FrameList.clear();
	// ���� x,y �� x,y�� �������� 
	// FrameList�� ������ ����ش�.
	for (int i = startY; i <  endY + 1; ++i)
	{
		for (int j = startX; j < endX + 1; ++j)
		{
			_FrameList.push_back({ j,i });
		}
	}

	// ���� �ǵ��ƿ���(reverse)�� true ���
	// FrameList�� �������� ������ ����ش�.
	if (reverse)
	{
		for (int i = endY; i + 1 > startY; --i)
		{
			for (int j = endX; j + 1 > startX; --j)
			{
				_FrameList.push_back({ j,i });
			}
		}
	}
}

