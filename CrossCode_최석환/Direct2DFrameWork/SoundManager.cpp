#include "stdafx.h"
#include "SoundManager.h"

HRESULT SoundManager::Init(void)
{
	//FMOD �ý����� �����Ѵ�
	System_Create(&_system);

	//����, ä�μ� ����
	_system->init(SOUNDBUFFER, FMOD_INIT_NORMAL, 0);

	//����, ä�� Ŭ���� �����Ҵ�
	_sound = new Sound*[SOUNDBUFFER];
	_channel = new Channel*[SOUNDBUFFER];

	//�޸� �ʱ�ȭ
	memset(_sound, 0, sizeof(Sound*) * SOUNDBUFFER);
	memset(_channel, 0, sizeof(Channel*) * SOUNDBUFFER);

	isFadingIn = false;
	isFadingOut = false;

	totalVolume = 1.f;
	prevVolume = totalVolume;
	return S_OK;
}

void SoundManager::Release(void)
{
	//���� �� ä�� ����
	if (_sound != NULL || _channel != NULL)
	{
		for (int i = 0; i < SOUNDBUFFER; i++)
		{
			if (_channel != NULL)
			{
				if (_channel[i]) _channel[i]->stop();
			}
			if (_sound != NULL)
			{
				if (_sound[i]) _sound[i]->release();
			}
		}
	}

	//�޸� �����
	SAFE_DELETE_ARRAY(_sound);
	SAFE_DELETE_ARRAY(_channel);

	//�ý��� �ݱ�
	if (_system != NULL)
	{
		_system->release();
		_system->close();
	}
}

void SoundManager::Update(void)
{
	//���ΰ��ӿ��� ����Ŵ����� ������Ʈ ����� �Ѵ�
	//���� ���� �Ѱ������ �Ѵ�
	//���� �ý����� ������ ����ǰų�, ����� ������ ���带 �����ϴµ�
	//���� ���������� ������ �̷����� �ﰢ������ ó�����ش�
	_system->update();

	//�����̵���, �ƿ� ���¿� ���� ������Ʈ ����
	if(isFadingIn) FadeIn();
	else if (isFadingOut) FadeOut();

}

void SoundManager::addSound(std::string keyName, std::string soundName, bool bgm, bool loop)
{
	if (loop)//�Ϲ������� BGM
	{
		if (bgm)
		{
			//���� ������ �̸� ���ε��صΰ� ����Ѵ�
			//�Ѱ��� ���ϸ� �������
			_system->createStream(soundName.c_str(), FMOD_LOOP_NORMAL, 0, &_sound[_mTotalSound.size()]);
		}
		else//���� ������
		{
			//���� ������ �׶��׶� �ǽð����� �ε��Ѵ�
			//���� ȿ������ ���� ũ�Ⱑ Ŭ��� ���۰� �����
			//�������� ���� ������� �����ϱ⶧���� ȿ������ �����ϴ�
			_system->createSound(soundName.c_str(), FMOD_LOOP_NORMAL, 0, &_sound[_mTotalSound.size()]);
		}
	}
	else //�Ϲ������� ȿ����
	{
		_system->createSound(soundName.c_str(), FMOD_DEFAULT, 0, &_sound[_mTotalSound.size()]);
	}

	//�ʿ� ���带 Ű���� �Բ� ��Ƶд�
	_mTotalSound.insert(make_pair(keyName, &_sound[_mTotalSound.size()]));

}

void SoundManager::play(std::string keyName, float volume)
{
	int count = 0;
	arrSoundIter iter = _mTotalSound.begin();
	for (iter; iter != _mTotalSound.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			//���� �÷���
			_system->playSound(FMOD_CHANNEL_FREE, *iter->second, false, &_channel[count]);
			//��������
			_channel[count]->setVolume(volume);
		}
	}
}

void SoundManager::stop(std::string keyName)
{
	int count = 0;
	arrSoundIter iter = _mTotalSound.begin();
	for (iter; iter != _mTotalSound.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			//���� ����
			_channel[count]->stop();
		}
	}
}

void SoundManager::pause(std::string keyName)
{
	int count = 0;
	arrSoundIter iter = _mTotalSound.begin();
	for (iter; iter != _mTotalSound.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			//���� �Ͻ�����
			_channel[count]->setPaused(true);
			break;
		}
	}
}

void SoundManager::resume(std::string keyName)
{
	int count = 0;
	arrSoundIter iter = _mTotalSound.begin();
	for (iter; iter != _mTotalSound.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			//���� �ٽ����
			_channel[count]->setPaused(false);
			break;
		}
	}
}

void SoundManager::SetVolume(string keyName, float volume)
{
	int count = 0;
	arrSoundIter iter = _mTotalSound.begin();
	for (iter; iter != _mTotalSound.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			_channel[count]->setVolume(volume);
		}
	}
}

bool SoundManager::isPlaySound(std::string keyName)
{
	bool isPlay = false;
	int count = 0;
	arrSoundIter iter = _mTotalSound.begin();
	for (iter; iter != _mTotalSound.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			//���� �÷������̳�?
			_channel[count]->isPlaying(&isPlay);
			break;
		}
	}

	return isPlay;
}

bool SoundManager::isPauseSound(std::string keyName)
{
	bool isPause = false;
	int count = 0;
	arrSoundIter iter = _mTotalSound.begin();
	for (iter; iter != _mTotalSound.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			//���� �÷������̳�?
			_channel[count]->isPlaying(&isPause);
			break;
		}
	}

	return isPause;
}

//��BGM�� ���� ����Ǵ°͵��� ������ �ٲ��ִ� ���ÿ� ���� ������� BGM�� ���� ���� �ٲپ��ش�
void SoundManager::setTotalVolume(float volume)
{
	totalVolume = volume;

	int count = 0;
	arrSoundIter iter = _mTotalSound.begin();

	for (iter; iter != _mTotalSound.end(); ++iter, count++)
	{
		_channel[count]->setVolume(volume);
	}
}

void SoundManager::setFadeOut()
{
	prevVolume = totalVolume; //���̵� �ƿ� ��Ű�� ���� ������ ���� ����
	isFadingOut = true;
}

void SoundManager::FadeOut()
{
	if(totalVolume>0) totalVolume -= 0.03f;
	else if (totalVolume >= 0)
	{
		totalVolume = 0;
		isFadingOut = false;
		isFadingIn = false;
	}
	setTotalVolume(totalVolume);
}

void SoundManager::setFadeIn()
{
	isFadingOut = false;
	totalVolume = 0;
	isFadingIn = true;
}

void SoundManager::FadeIn()
{
	if (totalVolume < prevVolume) totalVolume += 0.01f;
	else if (totalVolume >= prevVolume)
	{
		totalVolume = prevVolume;
		isFadingIn = false;
	}
	setTotalVolume(totalVolume);
}

