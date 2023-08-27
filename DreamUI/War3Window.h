#include "stdafx.h"
#ifndef CURSOR_LOCK_H_INCLUDED_
#define CURSOR_LOCK_H_INCLUDED_

typedef bool(*CustomWndProc)(HWND win, unsigned int message, WPARAM wParam, LPARAM lParam);

void AddWindowProc(CustomWndProc proc);
void War3Window_Init(HINSTANCE hGameDll);
void War3Window_Cleanup();
bool KeyIsDown(const unsigned int keyCode);
float War3WindowRatioX();
float War3WindowRatioY();
float GetWindowSizeY();
float GetWindowSizeX();

float GetMousePosY();
float GetMousePosX();
#endif