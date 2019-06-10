#pragma once
class TitleScene : public GameNode
{
	struct ButtonTag
	{
		Image* Image;
		Vector2 Pos;
		Vector2 Size;
		RECT Rc;
		float Alpha;
		float FrameCount;
		int FrameIndex;
		bool AlphaChange;
	};
private:
	Image* Line;			// Ÿ��Ʋ �Ͼ� ����
	Vector2 LineSize;
	bool LineEnd;			

	Image* WhiteRect[2];	// Ÿ��Ʋ �Ͼ� ������
	Vector2 WhiteRectSize[2];
	Vector2 WhiteRectPos[2];

	Image* BlueRect;		// Ÿ��Ʋ �Ķ� ������
	Vector2 BlueRectSize;	
	float BlueAlpha;		
	bool BlueEnd;

	Image* Cloud[2];		// Ÿ��Ʋ ����1, ����2
	Image* Sky;				// Ÿ��Ʋ �ϴ�
	Image* Ground;			// Ÿ��Ʋ �ٴ�
	Image* Planet;			// Ÿ��Ʋ �༺

	Image* Logo[2];			// Ÿ��Ʋ �ΰ�, �ΰ� ���
	Vector2 LogoPos;		
	float LogoAlpha;		
	bool LogoEnd;

	Image* Bridge;			// Ÿ��Ʋ ����
	Vector2 BridgePos;		

	Image* Lea;				// Ÿ��Ʋ ���ΰ� ( Lea )
	Vector2 LeaPos;	
	float FrameCount;		// Ÿ��Ʋ ���ΰ� ������ ī��Ʈ
	int FrameIndex;			// Ÿ��Ʋ ���ΰ� ������ �ε���
	bool LeaEnd;		

	Image* Black;			// Ÿ��Ʋ ���� ���
	float BlackAlpha;		// Ÿ��Ʋ ���� ���İ�

	Image* Start;			// Ÿ��Ʋ �����Ϸ��� �����ÿ�.
	RECT StartRect;			// Start ������� RECT
	bool isTouch;			// Start ������
	float AlphaTime;		// ���� ���� �ð�
	float StartAlpha;		// ���� ��
	bool AlphaChange;		// ���� ���� ����


	ButtonTag Button[3];	// ����, ����, ����
	float ButtonDelay;		// ��ư ���� ������

	Image* Cursor;			// ���콺 Ŀ��
	bool hovor;				// Ŀ�� �ö�Դ�?
	bool hovorSound;
	float hovorTime;		// Ŀ�� Ÿ��
	bool blue;
	
public:

	void Init() override;
	void Update() override;
	void Render() override;
	void Release() override;

public:

	void FrameUpdate();
	void TitleRender();
	void ButtonRender();

public:

	void LineUpdate();
	void BlueUpdate();
	void LogoUpdate();
	void LeaUpdate();
	void StartUpdate();

	void ButtonMove();
	void ButtonUpdate();
	void ButtonClick();

	TitleScene() {}
	~TitleScene() {}
};

