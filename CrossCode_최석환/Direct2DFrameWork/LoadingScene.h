#pragma once
#include "GameNode.h"
#include "Loading.h"

class LoadingScene : public GameNode
{
private:
	Loading* _loading; //�ε������� �ε� �ϳ� �̴��ؼ� ���ű⿡ ������ ����
	float per;		   //�ۼ�������

private:
	string NextScene;	// ���� ��

private:
	Image* LoadingImg;
	int FrameIndex;
	float FrameCount;

	Image* BackGroundImg;
	float BackGroundAlpha;

public:
	void Init();
	void Release();
	void Update();
	void Render();

public:
	void SetNextScene(string name) { NextScene = name; }
	string GetNextScene() { return NextScene; }

public:
	
	void LoadingImage(); //�ε��� �̹��� �־�δ� ��
	void LoadingSound(); //���� �ε��ϴ� ��
	void LoadScenes();	 //���� ��Ƽ� �ε��ϴ°�

	LoadingScene() {}
	~LoadingScene() {}
};

