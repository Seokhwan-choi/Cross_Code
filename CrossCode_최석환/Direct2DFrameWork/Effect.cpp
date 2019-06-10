#include "stdafx.h"
#include "Effect.h"

Effect::Effect(string name)
{
	// 1100, 50
	_EffectImage = IMAGEMANAGER->FindImage(name);
	_FrameCount = 0.f;
	_FrameIndex = 0;
	_EffectAlpha = 1.f;
	_Pos = Vector2(0.f, 0.f);
	_Size = Vector2(0.f, 0.f);
	_Show = false;
	_Effect = true;
}

void Effect::Init(Vector2 pos, Vector2 size, bool effect, int frameindex, float effectTime)
{
	_Effect = effect;
	_Pos = pos;
	_Size = size;
	_FrameCount = 0.f;
	_Show = true;
	_EffectAlpha = 1.f;
	_FrameIndex = frameindex;
	_EffectTime = effectTime;
}

void Effect::Init(Vector2 pos, Vector2 size, float angle, float effectTime)
{
	_Pos = pos;
	_Size = size;
	_FrameCount = 0.f;
	_Show = true;
	_EffectAlpha = 1.f;
	_EffectAngle = angle;
	_FrameIndex = 0;
	_EffectTime = effectTime;
}

void Effect::Init(void)
{

}

void Effect::Release(void)
{

}

void Effect::Update(void)
{
	// deltaTime을 더해준다.
	_FrameCount += TIMEMANAGER->getElapedTime();

	if (_FrameCount > _EffectTime)
	{
		while (_FrameCount > _EffectTime)
			_FrameCount -= _EffectTime;
		if (_Effect)
		{
			_FrameIndex++;
			if (_FrameIndex == _EffectImage->GetMaxFrameX()) _Show = false;
		}
		else {
			_EffectAlpha -= 0.03f;
			if (_EffectAlpha <= 0) _Show = false;
		}
	}
}

void Effect::Render(void)
{
	Vector2 TempPos = CAMERA->GetRelativeVector2(_Pos);
	if (_Show)
	{
		_EffectImage->SetSize(_Size);
		_EffectImage->SetAngle(_EffectAngle);
		_EffectImage->SetAlpha(_EffectAlpha);
		_EffectImage->FrameRender((TempPos.x), (TempPos.y), _FrameIndex, 0, Pivot::Center);
	}
		
}
