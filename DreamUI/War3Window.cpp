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
#include "EditBoxButton.h"

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

UINT lastmsg = 0;
WPARAM lastwparam = 0;
LPARAM lastlparam = 0;

LRESULT CALLBACK DreamUI_WarWindow3Proc(int nCode, WPARAM wwParam, LPARAM llParam)
{
	if (nCode < HC_ACTION)
		return CallNextHookEx(hhookSysMsg, nCode, wwParam, llParam);
	if (llParam > 0)
	{
		LPMSG lmsg = (LPMSG)llParam;

		if (lmsg->wParam == lastwparam && lmsg->lParam == lastlparam && lmsg->message == lastmsg)
			return CallNextHookEx(hhookSysMsg, nCode, wwParam, llParam);

		lastmsg = lmsg->message;
		lastwparam = lmsg->wParam;
		lastlparam = lmsg->lParam;

		if (!InitedHook) {
			InitedHook = true;
			HookReady();
		}

		if (lmsg->message == WM_SIZE)
		{
			// (x / y) / (4 / 3 ) = (640 / 480) / ( 4 / 3) = 
			WidthRatioX = ((float)LOWORD(lmsg->lParam) / (float)HIWORD(lmsg->lParam)) / (4.f / 3.f);
			// (y / x) / (3 / 4 ) = (480 / 640) / ( 3 / 4) = 
			WidthRatioY = ((float)HIWORD(lmsg->lParam) / (float)LOWORD(lmsg->lParam)) / (3.f / 4.f);

			GetTimer(0.05, RefreshUICallback)->start();
		}




		if (lmsg->message == WM_LBUTTONDOWN || lmsg->message == WM_RBUTTONDOWN ||
			lmsg->message == WM_MBUTTONDOWN || lmsg->message == WM_LBUTTONUP || lmsg->message == WM_RBUTTONUP
			|| lmsg->message == WM_MBUTTONUP)
		{
			EditBoxButton::StopInput(); 
		}


		if (EditBoxButton::IsAnyEditBoxActive())
		{
			if (lmsg->message == WM_KEYDOWN || lmsg->message == WM_SYSKEYDOWN)
			{
				if (lmsg->wParam == VK_BACK)
				{
					EditBoxButton::RemoveCharacterFromCurrentEditBox(false);
				}
				else if (lmsg->wParam == VK_DELETE)
				{
					EditBoxButton::RemoveCharacterFromCurrentEditBox(true);
				}
				else if (lmsg->wParam == VK_LEFT)
				{
					EditBoxButton::CurrentEditBoxMoveCursorLeft();
				}
				else if (lmsg->wParam == VK_RIGHT)
				{
					EditBoxButton::CurrentEditBoxMoveCursorRight();
				}
				else
				{
					unsigned char _keystate[256];
					WCHAR  _inputbuf[32]{ 0 };
					GetKeyboardState(_keystate);
					if (!(MapVirtualKey(lmsg->wParam, MAPVK_VK_TO_CHAR) >> (sizeof(unsigned int) * 8 - 1) & 1))
					{
						if (ToUnicode(lmsg->wParam, MapVirtualKey(lmsg->wParam, 0),
							_keystate, _inputbuf, 32, 0) >= 1)
						{
							EditBoxButton::EnterTextToCurrentEditBox(_inputbuf);
						}
					}

				}
				return 0;
			}
		}


		CallWndProcs(lmsg->hwnd, lmsg->message, lmsg->wParam, lmsg->lParam);
	}
	return CallNextHookEx(hhookSysMsg, nCode, wwParam, llParam);
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

