#include "stdafx.h"
#include "War3Window.h"
#include "Event.h"
#include "Game.h"
#include "Tools.h"
#include "Input.h"
#include "Offsets.h"
#include "GameEventObserver.h"
#include "UI.h"
#include "Version.h"
#include "IUIObject.h"
#include "Timer.h"

constexpr unsigned int MAX_WNDPROC = 32;
static unsigned int WndProcCount = 0;
static CustomWndProc WndProcs[MAX_WNDPROC] = { nullptr };

static void CallWndProcs(HWND win, unsigned int message, WPARAM wParam, LPARAM lParam) {
	for (unsigned int i = 0; i < MAX_WNDPROC && WndProcs[i]; ++i) {
		if (WndProcs[i](win, message, wParam, lParam))
			break;
	}
}

void AddWindowProc(CustomWndProc proc) {
	if (WndProcCount < MAX_WNDPROC)
		WndProcs[WndProcCount++] = proc;
}

static float WidthRatioX = 1.0f;
static float WidthRatioY = 1.0f;

float GetWindowSizeX()
{
	return *(float*)Offset(WINDOW_X);
}
float GetWindowSizeY()
{
	return *(float*)Offset(WINDOW_Y);
}
float GetMousePosY()
{
	return *(float*)Offset(MOUSE_X);
}
float GetMousePosX()
{
	return *(float*)Offset(MOUSE_Y);
}

void HookReady();

void RefreshUICallback(Timer* tm)
{
	UI_Refresh();
}

bool InitedHook = false;

HHOOK hhookSysMsg;

LRESULT CALLBACK DreamUI_WarWindow3Proc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode < HC_ACTION)
		return CallNextHookEx(hhookSysMsg, nCode, wParam, lParam);
	if (lParam > 0)
	{
		LPMSG lmsg = (LPMSG)lParam;

		if (!InitedHook) {
			InitedHook = true;
			HookReady();
		}

		if (lmsg->message == WM_SIZE)
		{
			// (x / y) / (4 / 3 ) = (640 / 480) / ( 4 / 3) = 
			WidthRatioX = ((float)LOWORD(lParam) / (float)HIWORD(lParam)) / (4.f / 3.f);
			// (y / x) / (3 / 4 ) = (480 / 640) / ( 3 / 4) = 
			WidthRatioY = ((float)HIWORD(lParam) / (float)LOWORD(lParam)) / (3.f / 4.f);

			GetTimer(0.05, RefreshUICallback)->start();
		}


		CallWndProcs(lmsg->hwnd, lmsg->message, lmsg->wParam, lmsg->lParam);
	}
	return CallNextHookEx(hhookSysMsg, nCode, wParam, lParam);
}

void War3Window_Init(HINSTANCE hGameDll) {
	hhookSysMsg = SetWindowsHookExW(WH_GETMESSAGE, DreamUI_WarWindow3Proc, hGameDll, GetCurrentThreadId());
}

void War3Window_Cleanup() {
	InitedHook = false;
	GameEventObserver_Cleanup();
	if (hhookSysMsg)
		UnhookWindowsHookEx(hhookSysMsg);
}

void HookReady() {
	GameEventObserver_Init();
}

float War3WindowRatioX()
{
	//TODO
	return WidthRatioX ? WidthRatioX : 1.f;
}

float War3WindowRatioY()
{
	//TODO
	return WidthRatioY ? WidthRatioY : 1.f;
}

