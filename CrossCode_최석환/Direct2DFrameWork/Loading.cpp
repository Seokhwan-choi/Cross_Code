#include "stdafx.h"
#include "Loading.h"
//=============================================================
//	## loadItem ## (�ε������ Ŭ����)
//=============================================================

//���Ϲ� �̹��� �ʱ�ȭ
HRESULT LoadItem::Init(std::string keyName, std::wstring fileName)
{
	//������, ������ �ʱ�ȭ
	_kind = LOAD_KIND::LOAD_KIND_IMAGE;
	_imageResource.keyName = keyName;
	_imageResource.fileName = fileName;

	return S_OK;
}

//�������� �̹��� �ʱ�ȭ
HRESULT LoadItem::Init(std::string keyName, std::wstring fileName,int frameX, int frameY)
{
	//������, ������ �ʱ�ȭ
	_kind = LOAD_KIND::LOAD_KIND_FRAMEIMAGE;
	_imageResource.keyName = keyName;
	_imageResource.fileName = fileName;
	_imageResource.frameX = frameX;
	_imageResource.frameY = frameY;

	return S_OK;
}

//����� �ʱ�ȭ
HRESULT LoadItem::Init(std::string keyName, const char * fileName, bool bgm, bool loop)
{
	//������, ������ �ʱ�ȭ
	_kind = LOAD_KIND::LOAD_KIND_SOUND;
	_soundResource.keyName = keyName;
	_soundResource.fileName = fileName;
	_soundResource.bgm = bgm;
	_soundResource.loop = loop;

	return S_OK;
}

//��� �ʱ�ȭ
HRESULT LoadItem::Init(std::string sceneName, GameNode * ptr)
{
	//������, ������ �ʱ�ȭ
	_kind = LOAD_KIND::LOAD_KIND_SCENE;
	_sceneResource.sceneName = sceneName;
	_sceneResource.scene = ptr;

	return S_OK;
}


//=============================================================
//	## loading ## (�ε� Ŭ����)
//=============================================================

//�庻���� �߰��ϰ� �ʹٸ� ���⿡ "�̹����Ŵ���"�� ���� �ε��� �� ��׶��� �� �̹��� �߰�
void Loading::Init()
{
	
	//���� IMAGEMANAGER->addImage("loadingText", "image/UI/LoadingText.bmp", 245, 75);
	
	
	_currentGauge = 0; //���� ������. 0�� �ε������� �����ϰ� ��
}

//�常�� ���Ҵ� �����͵��� �����ϸ� Ǯ����
void Loading::Release()
{
	for (auto &a : _vLoadItem) SAFE_DELETE(a);

}

//��ε��� ���� �������̹����� ������Ʈ�� �ʿ��Ѱ͵��� �ִٸ� ������
void Loading::Update()
{
	

}

//��ε�ȭ�鿡 ��׶��峪 �����̴� ������ ���Ÿ� ���⼭ ����
void Loading::Render()
{
	

}

//���Ϲ� �̹��� �ε��ϱ� ���� �̴ֽ�Ű�� ��Ƶ�
void Loading::LoadImg(std::string keyName, std::wstring fileName)
{
	LoadItem* item = new LoadItem;
	item->Init(keyName, fileName);
	_vLoadItem.push_back(item);
}

//�������� �̹��� �ε��ϱ� ���� �̴ֽ�Ű�� ��Ƶ�
void Loading::LoadFrameImage(std::string keyName, std::wstring fileName,int frameX, int frameY)
{
	LoadItem* item = new LoadItem;
	item->Init(keyName, fileName,frameX, frameY);
	_vLoadItem.push_back(item);
}

//����� �ε��ϱ� ���� �̴ֽ�Ű�� ��Ƶ�
void Loading::LoadSound(std::string keyName, const char * fileName, bool bgm, bool loop)
{
	LoadItem * item = new LoadItem;
	item->Init(keyName, fileName, bgm, loop);
	_vLoadItem.push_back(item);
}

//��� �ε��ϱ� ���� �̴ֽ�Ű�� ��Ƶ�
void Loading::LoadScene(std::string sceneName, GameNode * ptr)
{
	LoadItem * item = new LoadItem;
	item->Init(sceneName, ptr);
	_vLoadItem.push_back(item);
}

BOOL Loading::LoadingDone()
{
	//�ε��Ϸ��
	if (_currentGauge >= _vLoadItem.size())
	{
		return TRUE;
	}

	LoadItem* item = _vLoadItem[_currentGauge];

	switch (item->GetLoadingKind())
	{
	case LOAD_KIND::LOAD_KIND_IMAGE:
	{
		tagImageResource img = item->GetImageResource();
		IMAGEMANAGER->AddImage(img.keyName, img.fileName);
	}
	break;
	case LOAD_KIND::LOAD_KIND_FRAMEIMAGE:
	{
		tagImageResource img = item->GetImageResource();
		IMAGEMANAGER->AddFrameImage(img.keyName, img.fileName,img.frameX, img.frameY);
	}
	break;
	case LOAD_KIND::LOAD_KIND_SOUND:
	{
		tagSoundResource snd = item->GetSoundResource();
		SOUNDMANAGER->addSound(snd.keyName, snd.fileName, snd.bgm, snd.loop);
	}
	break;
	case LOAD_KIND::LOAD_KIND_SCENE:
	{
		tagSceneResource scene = item->GetSceneResource();
		SCENEMANAGER->AddScene(scene.sceneName, scene.scene);
	}
	break;


	}

	//���� ������ ����
	_currentGauge++;

	return 0;
}


