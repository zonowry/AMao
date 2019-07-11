#pragma once
#include <Windows.h>
#include "resources.h"

class SpriteWindowTray
{
private:
	NOTIFYICONDATA trayIcon;
public:
	SpriteWindowTray(HWND wnd);

	void AddNotifyIcon();
};

