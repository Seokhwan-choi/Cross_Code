#pragma once
class Animation
{
private:
	bool _isPlay;					// 애니메이션 실행중이냐 ?
	bool _isLoop;					// 애니메이션 반복할 것이냐 ?

	int _CurrentIndex;				// 현재 애니메이션 프레임 인덱스
	float _CurrentTime;				// 현재 애니메이션 프레임 시간
	float _UpdateTime;				// 기준이되는 프레임 시간

	vector<POINT> _FrameList;		// 랜더할 프레임 인덱스 List
public:

	void Stop();
	void Pause();
	void Play();
	void UpdateFrame();
	void SetFrameList(int startX, int startY, int endX, int endY, bool reverse);

	int GetFrameX() { return _FrameList[_CurrentIndex].x; }
	int GetFrameY() { return _FrameList[_CurrentIndex].y; }
	
	void SetCurrentIndex(int index) { _CurrentIndex = index; }
	void SetCurrentTime(float time) { _CurrentTime = time; }
	void SetUpdateTime(float time) { _UpdateTime = time; }
	void SetisLoop(bool loop) { _isLoop = loop; }

	int GetCurrentIndex() { return _CurrentIndex; }
	float GetUpdateTime() { return _UpdateTime; }

	Animation();
	~Animation();
};

