#pragma once
/****************************************************************************
## Image ##
@@ Author : ±Ç¼ø¿ì , Date : 2019.03.20
*****************************************************************************/
class Image final
{
public:
	struct TagLoadedImageInfo
	{
		string key;
		wstring directory;

		TagLoadedImageInfo() {}
		TagLoadedImageInfo(const string& key, const wstring& directory)
			:key(key), directory(directory) {}
	};
	struct FrameRect
	{
		float x;
		float y;
		float width;
		float height;
	};
private:
	ID2D1Bitmap*		mBitmap;
	float				mAlpha;

	Vector2				mSize;
	float				mScale;
	float				mAngle;

	TagLoadedImageInfo	mLoadInfo;

	vector<Image::FrameRect>	mFrameInfo;
	int							mMaxFrameX;
	int							mMaxFrameY;

	bool						mIsReverseAxisX;
	bool						mIsReverseAxisY;
private:
	friend class ImageManager;
	Image() = delete;
	Image(ID2D1Bitmap*const bitmap, const TagLoadedImageInfo& loadinfo);
	Image(ID2D1Bitmap* bitmap, const TagLoadedImageInfo& loadinfo, const int& maxFrameX, const int& maxFrameY);
	virtual ~Image();
	Image operator = (const Image& image) {}
public:
	void Render(int x,  int y,  Pivot pivot = Pivot::Center);
	void FrameRender(float x, float y,  int frameX,  int frameY,  Pivot pivot = Pivot::Center);
	void FrameRender2(int x, int y, int frameX, int frameY, int width, int height, Pivot pivot = Pivot::Center);

	void ResetRenderOption();

	void SetWidth(const float width) { this->mSize.x = width; }
	void SetHeight(const float height) { this->mSize.y = height; }
	void SetSize(const Vector2& vec) { this->mSize = vec; }
	void SetAlpha(const float& alpha) { this->mAlpha = alpha; }
	void SetScale(const float& scale) { this->mScale = scale; }
	void SetAngle(const float& angle) { this->mAngle = angle; }
	void SetReverseX(const bool& x) { this->mIsReverseAxisX = x; }
	void SetReverseY(const bool& y) { this->mIsReverseAxisY = y; }

	 int GetMaxFrameX()const { return mMaxFrameX; }
	 int GetMaxFrameY()const { return mMaxFrameY; }
	 string GetKey() const { return this->mLoadInfo.key; }
	 UINT GetWidth() const { return mBitmap->GetPixelSize().width; }
	 UINT GetHeight()const { return mBitmap->GetPixelSize().height; }
	 Vector2 GetSize()const { return this->mSize; }
	 Vector2 GetFrameSize(const int& frame = 0) const { return { mFrameInfo[frame].width, mFrameInfo[frame].height }; }
	 TagLoadedImageInfo GetLoadInfo() const { return mLoadInfo; }
private:
	const Vector2 GetPivotPosition( int x,  int y, Pivot pivot);
};

