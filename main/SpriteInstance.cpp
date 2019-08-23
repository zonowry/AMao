#include "SpriteInstance.h"





LRESULT SpriteInstance::MainWindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	POINT pt = { 0 };
	switch (message)
	{
	case WM_CREATE:
		// 托盘弹出菜单
		/*AppendMenu(ntiMenu, MF_STRING, NTI_ABM, "作者博客");
		AppendMenu(ntiMenu, MF_STRING, NTI_QUIT, "退出");*/
		return 0;
		/*case AMAOMSG_NOTIFYICON:
			return TrayIconHandle(lParam, hwnd);
		case WM_COMMAND:
			return MenuHandle(wParam);*/
	default:
		return DefWindowProc(hwnd, message, wParam, lParam);
	}

}

void SpriteInstance::GenerateWindowHand()
{
	WNDCLASSEX wndClass = { 0 };
	wndClass.cbSize = sizeof(WNDCLASSEX);
	wndClass.style = CS_HREDRAW | CS_VREDRAW;
	wndClass.lpfnWndProc = MainWindowProc;
	wndClass.hInstance = this->configuration.hand;
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hbrBackground = (HBRUSH)(COLOR_WINDOW);
	wndClass.lpszMenuName = NULL;
	wndClass.lpszClassName = MAIN_WINDOW_CLASSNAME;

	// 注册
	RegisterClassEx(&wndClass);
	// 创建
	mainWindow = CreateWindowEx(
		// 窗口扩展样式
		WS_EX_TOPMOST | WS_EX_TOOLWINDOW | WS_EX_LAYERED,
		// 窗体类名
		MAIN_WINDOW_CLASSNAME,
		// 窗口标题
		"",
		// 窗口样式
		WS_POPUP | WS_VISIBLE,
		// 窗口初始屏幕X坐标
		configuration.position.x,
		// 窗口初始屏幕Y坐标
		configuration.position.y,
		// 窗口初始宽度
		configuration.size.cx,
		// 窗口初始高度
		configuration.size.cx,
		// 父窗体句柄
		NULL,
		// 菜单的句柄或是子窗口的标识符
		NULL,
		// 应用程序实例的句柄
		configuration.hand,
		// 指向窗口的创建数据
		this
	);


}

void SpriteInstance::Ready(SpriteConfiguration config)
{
	this->configuration = config;
	this->GenerateWindowHand();
	this->frameHand = new FrameHandler();
}

void SpriteInstance::Start()
{
	this->Show();
	// TODO 托盘图标
	MSG msg = { 0 };
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		// 更新帧（重绘）
		frameHand->NextFrame();
		Sleep(30);
	}
	// ����ͼ����ʧ
	// Shell_NotifyIcon(NIM_DELETE, mainWindow.trayIcon);

}

void SpriteInstance::Show()
{
	ShowWindow(this->mainWindow, 0);

	//SpriteWindowTray trayIcon(wnd);
	//trayIcon.AddNotifyIcon();
}
