#include "stdafx.h"
#include "Particle.h"

void Particle::Make(Vector2 pos, float Speed, float Gravity, float Angle, float size)
{
	TagParticle temp;
	temp.Pos = pos;
	temp.SavePos = pos;
	temp.Speed = Speed;
	temp.Gravity = Gravity;
	temp.SaveGravity = Gravity;
	temp.Angle = Angle;
	temp.size = size;
	temp.Alpha = 1.f;
	temp.JumpCount = 1;
	temp.life = 400.f;
	temp.FrameCount = 0.f;
	temp.FrameIndex = 0;

	ParticleList.push_back(temp);
}

void Particle::Move()
{
	
	for (int i = 0; i < ParticleList.size(); ++i)
	{
		ParticleList[i].life -= 40.f * TIMEMANAGER->getElapedTime();
		Vector2 DirVec(ParticleList[i].Angle);
		ParticleList[i].Pos += DirVec * ParticleList[i].Speed * TIMEMANAGER->getElapedTime();
		
		ParticleList[i].Gravity += 0.16;
		ParticleList[i].Pos.y += ParticleList[i].Gravity;

		if (Math::Abs((ParticleList[i].SavePos.y + ParticleList[i].size) - ParticleList[i].Pos.y) < 5)
		{
			ParticleList[i].JumpCount++;
			ParticleList[i].SaveGravity = ParticleList[i].SaveGravity / 1.5f;
			ParticleList[i].Speed = Math::Clampf(ParticleList[i].Speed - 45.f, 0, 250.f);
			ParticleList[i].Gravity = ParticleList[i].SaveGravity;
		}

		if (ParticleList[i].JumpCount > 3)
		{
			ParticleList[i].Alpha -= TIMEMANAGER->getElapedTime();
		}

		if (ParticleList[i].Alpha <= 0.f || ParticleList[i].life <= 0)
		{
			ParticleList.erase(ParticleList.begin() + i);
			i--;
		}
	}
}

void Particle::FrameUpdate()
{
	for (int i = 0; i < ParticleList.size(); ++i)
	{
		ParticleList[i].FrameCount += TIMEMANAGER->getElapedTime();
		if (ParticleList[i].FrameCount > 0.01f)
		{
			while (ParticleList[i].FrameCount > 0.01f)
				ParticleList[i].FrameCount -= 0.01f;
			ParticleList[i].FrameIndex++;
			if (ParticleList[i].FrameIndex > 3)
				ParticleList[i].FrameIndex = 0;
		}
	}
}

void Particle::Update()
{
	this->Move();
	this->FrameUpdate();
}

void Particle::Render()
{
	for (int i = 0; i < ParticleList.size(); ++i)
	{
		Vector2 RednerPos = CAMERA->GetRelativeVector2(ParticleList[i].Pos);
		IMAGEMANAGER->FindImage("파편")->SetAlpha(ParticleList[i].Alpha);
		IMAGEMANAGER->FindImage("파편")->SetSize(Vector2(16.f, 16.f));
		IMAGEMANAGER->FindImage("파편")->FrameRender(RednerPos.x, RednerPos.y,
			ParticleList[i].FrameIndex, 0, Pivot::Center);
	}
}

Particle::Particle()
{
}


Particle::~Particle()
{
}
