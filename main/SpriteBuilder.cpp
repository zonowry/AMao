#include "SpriteBuilder.h"


void SpriteBuilder::Configuration(HINSTANCE  hInstance)
{
	// TODO: 配置文件读取配置，目前写死
	int width = 128, height = 128, x = 500, y = 500;
	SpriteConfiguration config = {
		SIZE{width,height},
		POINT{x,y},
		hInstance
	};
	// 读取精灵配置文件
	SpriteInstance::GetInstance()->Ready(config);


}


SpriteBuilder::SpriteBuilder()
{
}

SpriteBuilder::~SpriteBuilder()
{
}


void SpriteBuilder::Run()
{

	SpriteInstance::GetInstance()->Start();

	///// 托盘图标 >>>
	//TrayIcon trayIcon(wnd);
	//trayIcon.AddNotifyIcon();
	///// 托盘图标 <<<

	//// 显示窗体
	//ShowWindow(wnd, cmdShow);

	//MSG msg = { 0 };
	//while (msg.message != WM_QUIT)
	//{
	//	if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
	//	{
	//		TranslateMessage(&msg);
	//		DispatchMessage(&msg);
	//	}
	//	mainWindow.RedrawWndFrame();
	//	// 更新窗口信息
	//	Sleep(DELAY_TIME);
	//}
	//// 托盘图标消失
	//Shell_NotifyIcon(NIM_DELETE, mainWindow.trayIcon);
	//return static_cast<int>(msg.wParam);
}

