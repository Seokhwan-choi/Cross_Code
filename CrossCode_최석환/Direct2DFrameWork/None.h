#pragma once
#include "GameObject.h"
class None :
	public GameObject
{
private:
	bool _isFree;
public:

	virtual void Init() override;
	virtual void Update() override;
	virtual void Render() override;
	virtual void Release() override;

	None();
	~None();
};

