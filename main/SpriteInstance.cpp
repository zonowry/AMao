#include "SpriteInstance.h"


// 托盘弹出菜单
HMENU ntiMenu;
// 测试帧
// 从文件中读取
//SpriteFrame* testFrame = new SpriteFrameFile(L"frame/test.png");
// 从资源中读取
SpriteFrame* testFrame = new SpriteFrameResource(FRAME_TEST2);

void PopupNotifyMenu(HWND hwnd) {

	// HRSRC hRsrc = ::FindResource(hInst, MAKEINTRESOURCE(FRAME_TEST), lpType);
	POINT pt;
	GetCursorPos(&pt);
	// 激活窗口，以便响应事件，关闭弹出菜单
	SetForegroundWindow(hwnd);
	TrackPopupMenu(ntiMenu, TPM_RIGHTBUTTON, pt.x, pt.y, NULL, hwnd, NULL);
}

LRESULT SpriteInstance::MainWindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	POINT pt = { 0 };
	switch (message)
	{
	case WM_CREATE:
		ntiMenu = CreatePopupMenu();
		// 托盘弹出菜单
		AppendMenu(ntiMenu, MF_STRING, NTI_ABM, L"py交易");
		AppendMenu(ntiMenu, MF_STRING, NTI_QUIT, L"退出");
		return 0;

	case WM_RBUTTONDOWN:
		PopupNotifyMenu(hwnd);
		break;
		/// 窗口拖拽处理 >>>
	case WM_LBUTTONDOWN:
		SpriteInstance::GetInstance()->DragStart({ LOWORD(lParam), HIWORD(lParam) });
		break;
	case WM_MOUSEMOVE:
		// 拖拽
		SpriteInstance::GetInstance()->DragIng();
		break;
	case WM_LBUTTONUP:
		SpriteInstance::GetInstance()->DragStop();
		break;
		/// 窗口拖拽处理 <<<

	case AMAOMSG_NOTIFYICON:
		switch (lParam) {
		case  WM_RBUTTONDOWN:
			PopupNotifyMenu(hwnd);
			break;
		}
		break;
	case WM_COMMAND:
		switch (wParam)
		{
			/// 托盘图标菜单 >>>
		case NTI_QUIT:
			PostQuitMessage(0);
			return 0;
		case NTI_ABM:
			ShellExecute(NULL, TEXT("open"), TEXT("https://kaakira.com"), NULL, NULL, SW_SHOWNORMAL);
			break;
			/// 托盘图标菜单 <<<
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
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
		L"",
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
	this->frameHand->SetWindowHand(this->mainWindow, this->configuration.size);
	HICON ntfIcon = ::LoadIcon(this->configuration.hand, MAKEINTRESOURCE(ICON_APP));
	this->trayIcon = new TrayIcon(mainWindow, ntfIcon);
	this->trayIcon->AddNotifyIcon();

}

void SpriteInstance::Start()
{
	this->Show();
	this->Running();
}

void SpriteInstance::Running() {

	MSG msg = { 0 };
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		// TODO: 通过行为树获取frame帧
		SpriteFrame* frame = this->behavior.Excute();
		// 更新帧（重绘）
		frameHand->NextFrame(testFrame);

		// TODO: 动态调节性能
		if (!leftMouseButton) {
			Sleep(MAIN_PROGRESS_DELAY);
		}
	}
}

void SpriteInstance::Show()
{
	ShowWindow(this->mainWindow, 10);
}


void SpriteInstance::DragStart(POINT clickPoint)
{
	leftMouseButton = true;
	// 优化针对鼠标事件捕获（独占） ,需要与ReleaseCapture()成对出现
	SetCapture(this->mainWindow);
	triggerPoint = clickPoint;
}



void SpriteInstance::DragIng()
{
	if (leftMouseButton)
	{

		POINT cursor;
		GetCursorPos(&cursor);
		cursor.x -= this->triggerPoint.x;
		cursor.y -= this->triggerPoint.y;
		SetWindowPos(this->mainWindow, NULL, cursor.x, cursor.y, NULL, NULL, SWP_NOREDRAW | SWP_NOSIZE | SWP_NOZORDER);
	}
}

void SpriteInstance::DragStop()
{
	//currentState = c
	leftMouseButton = false;
	ReleaseCapture();
}