#pragma once
class Animation
{
private:
	bool _isPlay;					// �ִϸ��̼� �������̳� ?
	bool _isLoop;					// �ִϸ��̼� �ݺ��� ���̳� ?

	int _CurrentIndex;				// ���� �ִϸ��̼� ������ �ε���
	float _CurrentTime;				// ���� �ִϸ��̼� ������ �ð�
	float _UpdateTime;				// �����̵Ǵ� ������ �ð�

	vector<POINT> _FrameList;		// ������ ������ �ε��� List
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

