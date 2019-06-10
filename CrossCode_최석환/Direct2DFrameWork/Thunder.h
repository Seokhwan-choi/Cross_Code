#pragma once

struct TagCol
{
	RECT Rc;
	Vector2 Pos;
	Vector2 Size;
	float Life;
	bool fire;
};

class Thunder
{
private:
	vector<TagCol> ThunderList;
	float Alpha;
	float Angle;
	float Time;
	bool Attack;
public:
	
	void Fire(Vector2 pos, Vector2 size, float angle);
	void FireOff();
public:
	void SetThunderList(vector<TagCol> c) { ThunderList = c; }
	vector<TagCol> GetThunderList() { return ThunderList; }
	RECT GetRect(int i) { return ThunderList[i].Rc; }

	void SetAttack(bool a) { Attack = a; }
	bool GetAttack() { return Attack; }

public:
	void Update();
	void Render();
public:
	Thunder();
	~Thunder();
};

