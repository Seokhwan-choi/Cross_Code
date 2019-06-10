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
	Image* Line;			// 타이틀 하양 라인
	Vector2 LineSize;
	bool LineEnd;			

	Image* WhiteRect[2];	// 타이틀 하양 마름모
	Vector2 WhiteRectSize[2];
	Vector2 WhiteRectPos[2];

	Image* BlueRect;		// 타이틀 파랑 마름모
	Vector2 BlueRectSize;	
	float BlueAlpha;		
	bool BlueEnd;

	Image* Cloud[2];		// 타이틀 구름1, 구름2
	Image* Sky;				// 타이틀 하늘
	Image* Ground;			// 타이틀 바닥
	Image* Planet;			// 타이틀 행성

	Image* Logo[2];			// 타이틀 로고, 로고 배경
	Vector2 LogoPos;		
	float LogoAlpha;		
	bool LogoEnd;

	Image* Bridge;			// 타이틀 난간
	Vector2 BridgePos;		

	Image* Lea;				// 타이틀 주인공 ( Lea )
	Vector2 LeaPos;	
	float FrameCount;		// 타이틀 주인공 프레임 카운트
	int FrameIndex;			// 타이틀 주인공 프레임 인덱스
	bool LeaEnd;		

	Image* Black;			// 타이틀 검은 배경
	float BlackAlpha;		// 타이틀 검은 알파값

	Image* Start;			// 타이틀 시작하려면 누르시오.
	RECT StartRect;			// Start 누르기용 RECT
	bool isTouch;			// Start 눌렀냐
	float AlphaTime;		// 알파 변경 시간
	float StartAlpha;		// 알파 값
	bool AlphaChange;		// 알파 변경 여부


	ButtonTag Button[3];	// 시작, 맵툴, 종료
	float ButtonDelay;		// 버튼 등장 딜레이

	Image* Cursor;			// 마우스 커서
	bool hovor;				// 커서 올라왔니?
	bool hovorSound;
	float hovorTime;		// 커서 타임
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

