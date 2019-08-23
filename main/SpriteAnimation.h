#pragma once
#include <d2d1.h>
#include <d2d1helper.h>
#include <dwrite.h>
#include <wincodec.h>
#include <vector>
#include <dxgi.h>
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")
#pragma comment(lib, "windowscodecs.lib")

using namespace std;



// 动画（系列图片）播放工具类
class SpriteAnimation
{
public:
	SpriteAnimation(HDC* hdcSrc, RECT* rcClient);
	~SpriteAnimation();

	// 获取当前时间 ms单位
	float GetCurrentTick();
	// 播放动画
	bool Play(ID2D1Bitmap* images, float delay);
	// 上次播放完的时间
	float lastPlayTick;
};



