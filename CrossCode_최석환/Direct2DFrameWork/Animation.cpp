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
		// 프레임 시간에 DeltaTime 값을 더해 준다.
		_CurrentTime += TIMEMANAGER->getElapedTime();
		if (_CurrentTime >= _UpdateTime)
		{
			// 기준 시간 보다 프레임 시간이 커지면
			// 현재 프레임 인덱스를 증가 시킨다.
			_CurrentIndex++;
			while (_CurrentTime >= _UpdateTime)
				// 프레임 시간을 초기화 해준다.
				_CurrentTime -= _UpdateTime;
		}
		// 만약 현재 프레임 인덱스가 
		// 프레임 리스트의 size 보다 커진다면 ( 프레임 랜더 마지막에 도달 )
		if (_CurrentIndex >= static_cast<int>(_FrameList.size()))
		{

			// 루프(Loop) 반복한다면
			// 프레임 인덱스 0(처음) 으로 초기화
			if (_isLoop)
			{
				_CurrentIndex = 0;
			}
			// 반복하지 않는다면
			// 프레임 인덱스를 -- 프레임 마지막에 그대로 둔고
			// _isPlay를 꺼준다.
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
	// 시작 x,y 끝 x,y를 기준으로 
	// FrameList에 프레임 담아준다.
	for (int i = startY; i <  endY + 1; ++i)
	{
		for (int j = startX; j < endX + 1; ++j)
		{
			_FrameList.push_back({ j,i });
		}
	}

	// 만약 되돌아오기(reverse)가 true 라면
	// FrameList에 역순으로 프레임 담아준다.
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

