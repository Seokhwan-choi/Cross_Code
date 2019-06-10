#pragma once
#include "gameNode.h"
class Effect : public GameNode
{
private:
	Image* _EffectImage;		// 이펙트 이미지
	int _FrameIndex;			// 이펙트 프레임 인덱스
	float _FrameCount;			// 이펙트 프레임 카운트
	float _EffectAlpha;			// 이펙트 알파값
	float _EffectAngle;			// 이펙트 각도
	float _EffectTime;			// 이펙트 딜레이
	Vector2 _Pos;				// 이펙트 위치
	Vector2 _Size;				// 이펙트 사이즈
	bool _Show;					// 이펙트 보여주냐?
	bool _Effect;				// 이펙트냐..?

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

