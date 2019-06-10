#pragma once

struct TagParticle
{
	Vector2 Pos;
	Vector2 SavePos;
	float Speed;
	float SaveGravity;
	float Gravity;
	float Angle;
	float Alpha;
	float life;
	float size;
	int JumpCount;

	float FrameCount;
	int FrameIndex;
};

class Particle
{
private:
	vector<TagParticle> ParticleList;

public:
	vector<TagParticle> GetList() { return ParticleList; }
	void SetList(vector<TagParticle> t) { ParticleList = t; }

public:
	void Make(Vector2 pos, float Speed, float Gravity, float Angle, float size = 32.f);
	void Move();
	void FrameUpdate();

public:

	void Update();
	void Render();

public:
	Particle();
	~Particle();
};

