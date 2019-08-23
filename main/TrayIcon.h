#pragma once
#include <Windows.h>

// 托盘图标
#define AMAOMSG_NOTIFYICON 0x00101
#define AMAO_TRAYICON_ID  0x00110
#define NOTIFYICONUID 0x000110
// 托盘菜单：退出
#define NTI_QUIT 0
// 托盘菜单：作者博客
#define NTI_ABM 0x000233

class TrayIcon
{
private:
public:
	NOTIFYICONDATA trayIcon;
	TrayIcon(HWND wnd);

	void AddNotifyIcon();
};

