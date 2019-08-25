#pragma once
#include <Windows.h>
#include "resource.h"
#include "FrameHandler.h"
#include "SpriteAnimation.h"
#include "TrayIcon.h"

struct SpriteConfiguration {
	SIZE size;
	POINT position;
	HINSTANCE hand;
};


////////////////////////////////////
//  精灵实例，全局只会存在一个精灵实例  //
////////////////////////////////////
class SpriteInstance
{
private:
	TrayIcon* trayIcon;
	// 拖拽处理 >>>
	bool lMouseButton;
	// 鼠标点击时的坐标点
	POINT	triggerPoint;
	// 拖拽处理 <<<
	const int MAIN_PROGRESS_DELAY = 30;
	const LPCWSTR MAIN_WINDOW_CLASSNAME = L"AMao_Main";
	SpriteConfiguration configuration = { 0 };
	HWND mainWindow = NULL;
	FrameHandler* frameHand = NULL;

	SpriteInstance()
	{
	}

	~SpriteInstance()
	{
		// 关闭托盘图标
		Shell_NotifyIcon(NIM_DELETE, &(trayIcon->trayIcon));
	}

	static LRESULT CALLBACK MainWindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
	void GenerateWindowHand();

public:
	// 简单单例
	static SpriteInstance* GetInstance() {
		static SpriteInstance instance;
		return &instance;
	}
	void Ready(SpriteConfiguration config);
	void Start();
	void Show();

	void Hidden();
	void Shutdown();

	////////////////
	//    拖拽 	  //
	////////////////
	void DragStart(POINT clickPoint);
	void DragIng();
	void DragStop();
};

