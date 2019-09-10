#pragma once

#include <Windows.h>
#include <d2d1.h>
#include <d2d1helper.h>
#include <dwrite.h>
#include <wincodec.h>
#include <iostream>
#include <dxgi.h>
#include <map>
#include "resource.h"
#include "FrameClass.h"
using namespace std;


class FrameHandler {
private:
	string currentFrameId;
	HDC hdcSrc = NULL;
	HDC hdcDst = NULL;
	RECT rect;
	D2D1_RECT_F d2d1Rect = { 0 };
	SIZE size;
	HWND window;
	// D2D 工厂
	ID2D1Factory* pD2DFactory = NULL;
	// WIC位图工厂
	IWICImagingFactory* pIWICFactory = NULL;
	// Hwnd句柄 呈现器
	ID2D1HwndRenderTarget* pHWNDtarget = NULL;
	// DC句柄
	ID2D1DCRenderTarget* pDCtarget = NULL;
	// 库存帧（主要用于缓存绘制过的帧，再绘制时无需重新创建)
	// key:frameId
	map<string, ID2D1Bitmap*> frameImageStorge;
	ID2D1Bitmap* CreateBitmap(IStream* pStream1);
	ID2D1Bitmap* mapFrameToImage(SpriteFrame* frame);
public:
	FrameHandler();
	void SetWindowHand(HWND, SIZE);
	void NextFrame(SpriteFrame* frame);
};
