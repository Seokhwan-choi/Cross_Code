#include "stdafx.h"
#include "testScene.h"

void testScene::Init()
{
	SOUNDMANAGER->play("testSound");
	randomBeat = 1;
	counter = 0;
}

void testScene::Release()
{
}

void testScene::Update()
{
	counter++;
	if (counter % 50 == 0)
		randomBeat = RND->Range(0.5f, 2.f);
}

void testScene::Render()
{
	IMAGEMANAGER->FindImage("testImg")->SetScale(randomBeat);
	IMAGEMANAGER->FindImage("testImg")->Render(WINSIZEX / 2, WINSIZEY / 2);
}
