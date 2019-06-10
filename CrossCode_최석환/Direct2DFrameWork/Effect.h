#pragma once
#include "gameNode.h"
class Effect : public GameNode
{
private:
	Image* _EffectImage;		// ����Ʈ �̹���
	int _FrameIndex;			// ����Ʈ ������ �ε���
	float _FrameCount;			// ����Ʈ ������ ī��Ʈ
	float _EffectAlpha;			// ����Ʈ ���İ�
	float _EffectAngle;			// ����Ʈ ����
	float _EffectTime;			// ����Ʈ ������
	Vector2 _Pos;				// ����Ʈ ��ġ
	Vector2 _Size;				// ����Ʈ ������
	bool _Show;					// ����Ʈ �����ֳ�?
	bool _Effect;				// ����Ʈ��..?

public:

	void Init(Vector2 pos, Vector2 size, bool effect = true, int frameindex = 0, float effectTime = 0.01f);
	void Init(Vector2 pos, Vector2 size, float angle = 0.f, float effectTime = 0.01f);
	void Init(void);
	void Release(void);
	void Update(void);
	void Render(void);

public:

	void SetShow(bool show) { _Show = show; }
	bool GetShow() { return _Show; }

	void SetEffect(bool effect) { _Effect = effect; }
	bool GetEffect() { return _Effect; }

public:

	Effect(string name);
	~Effect() {}
};

