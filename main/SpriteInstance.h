#pragma once
#include <Windows.h>

struct SpriteConfiguration {
	SIZE size;
	POINT position;
	HINSTANCE hand;
};

class FrameHandler {
private:
	HDC hdcSrc = NULL;
	HDC hdcDst = NULL;
	SIZE size;
	HWND window;

public:
	FrameHandler();
	void SetWindowHand(HWND, SIZE);
	void NextFrame();
};


// 精灵实例，全局只会存在一个精灵实例
class SpriteInstance
{
private:
	const LPCSTR MAIN_WINDOW_CLASSNAME = "AMao_Main";
	SpriteConfiguration configuration = { 0 };
	HWND mainWindow = NULL;
	FrameHandler* frameHand = NULL;

	SpriteInstance()
	{
	}

	~SpriteInstance()
	{
	}

	static LRESULT CALLBACK MainWindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
	void GenerateWindowHand();

public:
	static SpriteInstance* GetInstance() {
		static SpriteInstance instance;
		return &instance;
	}
	void Ready(SpriteConfiguration config);
	void Start();
	void Show();
	void Hidden();
	void Shutdown();
};

