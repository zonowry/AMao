#include "TrayIcon.h"

TrayIcon::TrayIcon(HWND wnd)
{
	HICON ntfIcon = (HICON)LoadImage(NULL, L"icon.ico", IMAGE_ICON, 0, 0, LR_LOADFROMFILE);
	trayIcon = { 0 };
	trayIcon.hWnd = wnd;
	trayIcon.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;
	trayIcon.cbSize = sizeof(NOTIFYICONDATA);
	trayIcon.uCallbackMessage = AMAOMSG_NOTIFYICON;
	trayIcon.uID = AMAO_TRAYICON_ID;
	trayIcon.hIcon = ntfIcon;
	lstrcpy(trayIcon.szTip, L"=£þ¦Ø£þ=");
}

void TrayIcon::AddNotifyIcon()
{
	Shell_NotifyIcon(NIM_ADD, &trayIcon);
}

