#pragma once
#include <Windows.h>
#include <d2d1.h>
#include <d2d1helper.h>
#include <dwrite.h>
#include <wincodec.h>
#include <iostream>
#include <dxgi.h>
#include <map>
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")
#pragma comment(lib, "windowscodecs.lib")
using namespace std;

class SpriteFrame {
	const string id;
public:
	static string CeateFrameId();

	string GetFrameId() {
		return id;
	}

	SpriteFrame() :id(SpriteFrame::CeateFrameId()) {
	}
};

class FrameHandler {
private:
	HDC hdcSrc = NULL;
	HDC hdcDst = NULL;
	RECT clientRect;
	SIZE size;
	HWND window;
	// D2D 工厂
	ID2D1Factory* pD2DFactory;
	// WIC位图工厂
	IWICImagingFactory* pIWICFactory;
	// Hwnd句柄 呈现器
	ID2D1HwndRenderTarget* pHWNDtarget;
	// DC句柄
	ID2D1DCRenderTarget* pDCtarget;
	// 库存帧（主要用于缓存绘制过的帧，再绘制时无需重新创建)
	map<string, SpriteFrame> frameStorge;

public:
	FrameHandler();
	void SetWindowHand(HWND, SIZE);
	// 创建位图
	ID2D1Bitmap* CreateBitmapFromFile(LPCWSTR fileName);
	void NextFrame();


};
