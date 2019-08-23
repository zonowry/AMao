#pragma once
#include<Windows.h>
#include "resources.h"
LRESULT TrayIconHandle(LPARAM param, HWND hwnd);
LRESULT MenuHandle(WPARAM param);

HMENU ntiMenu = CreatePopupMenu();

LRESULT CALLBACK SpriteWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	POINT pt = { 0 };
	switch (message)
	{
	case WM_CREATE:
		// 托盘弹出菜单
		AppendMenu(ntiMenu, MF_STRING, NTI_ABM, "作者博客");
		AppendMenu(ntiMenu, MF_STRING, NTI_QUIT, "退出");
		return 0;
	case AMAOMSG_NOTIFYICON:
		return TrayIconHandle(lParam, hwnd);
	case WM_COMMAND:
		return MenuHandle(wParam);
	default:
		return DefWindowProc(hwnd, message, wParam, lParam);
	}
}

LRESULT TrayIconHandle(LPARAM param, HWND wnd) {

	switch (param)
	{
	case WM_RBUTTONDOWN:
		POINT pt;
		GetCursorPos(&pt);
		// 激活窗口，以便响应事件，关闭弹出菜单
		SetForegroundWindow(wnd);
		TrackPopupMenu(ntiMenu, TPM_RIGHTBUTTON, pt.x, pt.y, NULL, wnd, NULL);
		return 0;
	}
}

LRESULT MenuHandle(WPARAM param) {
	switch (param)
	{
	case NTI_ABM:
		ShellExecute(NULL, "open", "https://kaakira.com", NULL, NULL, SW_SHOWNORMAL);
		break;
	case NTI_QUIT:
		PostQuitMessage(0);
		break;
	}
	return 0;
}