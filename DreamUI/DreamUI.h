#include "stdafx.h"
#ifndef FOUNDATION_H_INCLUDED
#define FOUNDATION_H_INCLUDED

//#ifdef _VMP
//#pragma comment(lib, "../Release/DreamProtect.lib")
//#else
//#pragma comment(lib, "../Debug/DreamProtect.lib")
//#endif

extern unsigned long VersionGame;	//Game.dll°æ±¾
extern bool IsInitedDreamUI;

extern char RootPath[MAX_PATH];

void InitDreamUI(void* _GameDll, void* _StormDll, unsigned long version);
void CleanupDreamUI();

int GetGameVersion();

#endif