#pragma once
#include <Windows.h>
#include "resources.h"

class TrayIcon
{
private:
public:
	NOTIFYICONDATA trayIcon;
	TrayIcon(HWND wnd);

	void AddNotifyIcon();
};

