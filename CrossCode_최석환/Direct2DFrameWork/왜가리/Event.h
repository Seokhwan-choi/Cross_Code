#pragma once
class Event
{
public:
	virtual void StartEvent() = 0;
	virtual bool UpdateEvent() = 0;

	Event() {}
	~Event() {}
};

// ==================================================
// ############# Event Class ########################
// ==================================================

class Start : public Event
{
public:
	virtual void StartEvent() override;
	virtual bool UpdateEvent() override;

	Start() {}
	~Start() {}
};


// ==================================================
// ################ Camera Move #####################
// ==================================================
class CameraMove : public Event
{
private:
	class GameObject* Target;
public:
	virtual void StartEvent() override;
	virtual bool UpdateEvent() override;

	CameraMove(class GameObject* object);
	~CameraMove() {}
};

// ==================================================
// ################ Boss Move #######################
// ������ ������ �Ʒ��� �����´�.
// ������ ������ �Ʒ��� ��� ��������
// �������� ������ ��ġ�� ���� ü�°������� ���δ�.
// ==================================================
class BossMove : public Event
{
private:
	class GameObject* Boss;
public:
	virtual void StartEvent() override;
	virtual bool UpdateEvent() override;

	BossMove(class GameObject* boss);
	~BossMove() {}
};

// ===================================================
// ################ Boss Gather ######################
// ################ Boss Gather ######################
// ===================================================
class BossGather : public Event
{
private:
	class GameObject* Boss;

public:
	virtual void StartEvent() override;
	virtual bool UpdateEvent() override;

	BossGather(class GameObject* boss);
	~BossGather() {}
};

// ===================================================
// ################ Boss Break #######################
// ################ Boss Break #######################
// ===================================================
class BossBreak : public Event
{
private:
	class GameObject* Boss;

public:
	virtual void StartEvent() override;
	virtual bool UpdateEvent() override;

	BossBreak(class GameObject* boss);
	~BossBreak() {}
};


// =====================================================
// ################ Boss BarDone #######################
// ���� ���α׷����� ä������ Event
// ################ Boss BarDone #######################
// =====================================================
class BossBar : public Event
{
public:
	virtual void StartEvent() override;
	virtual bool UpdateEvent() override;

	BossBar() {}
	~BossBar() {}
};

// ===================================================
// ################ Camera Zoom ######################
// ===================================================
class CameraZoom : public Event
{

public:
	virtual void StartEvent() override;
	virtual bool UpdateEvent() override;

	CameraZoom() {}
	~CameraZoom() {}
};

// ====================================================
// ################ Camera Shake ######################
// ====================================================
class CameraShake : public Event
{
	virtual void StartEvent() override;
	virtual bool UpdateEvent() override;

	CameraShake() {}
	~CameraShake() {}
};

// ====================================================
// ################ Camera Change ######################
// ====================================================
class CameraChange : public Event
{
private:
	class GameObject* CameraTarget;
public:
	virtual void StartEvent() override;
	virtual bool UpdateEvent() override;

	CameraChange(class GameObject* target);
	~CameraChange() {}
};


// ====================================================
// ################ Event Delay #######################
// ====================================================
class Delay : public Event
{
private:
	float Current;
	float time;

public:
	virtual void StartEvent() override;
	virtual bool UpdateEvent() override;

	Delay(float time);
	~Delay() {}
};

class FadeIn : public Event
{
public:
	virtual void StartEvent() override;
	virtual bool UpdateEvent() override;

	FadeIn() {}
	~FadeIn() {}
};

class ChangeScene : public Event
{
private:
	string Name;
public:
	virtual void StartEvent() override;
	virtual bool UpdateEvent() override;

	ChangeScene(string name);
	~ChangeScene() {}

};

class End : public Event
{
public:
	virtual void StartEvent() override;
	virtual bool UpdateEvent() override;

	End() {}
	~End() {}
};
