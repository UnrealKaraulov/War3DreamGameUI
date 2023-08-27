#include "stdafx.h"

#include "DreamUI.h"

#include <process.h>
#include <new>
#include <time.h>
#include <stdlib.h>
#include "Tools.h"
#include "Offsets.h"
#include "Hook.h"
#include "Storm.h"
#include "Input.h"
#include "War3Window.h"
#include "RefreshManager.h"
#include "Version.h"
#include "SystemTools.h"
#include "Game.h"
#include "ObjectHookManager.h"
void* ModuleGame;		//Game.dll 模块
void* ModuleStorm;		//Storm.dll 模块
void* ModuleDream;		//自身模块
unsigned long VersionGame;	//Game.dll版本
bool IsInitedDreamUI = false;			//标记是否需要清理

char RootPath[MAX_PATH];

int GetGameVersion() { return (int)VersionGame; }

void InitDreamUI(void* _GameDll, void * _StormDll, unsigned long version) {

	if (_GameDll != NULL && _StormDll != NULL && version != 0) {
		VersionGame = version;
		ModuleStorm = _StormDll;
		ModuleGame = _GameDll;

		if (SystemTools::Init())
		{
			if (Offset_Init(VersionGame, (unsigned long)ModuleGame))
			{
				srand((unsigned int)(time(NULL)));

				DreamStorm::Init(ModuleStorm);

				ObjectHookManager_Init();

				Tools_Init();

				War3Window_Init((HINSTANCE)_GameDll);

				Input_Init();

				IsInitedDreamUI = true;
			}
		}
	}
}

void CleanupDreamUI() {

	if (IsInitedDreamUI) {
		DreamStorm::Cleanup();
		ObjectHookManager_Cleanup();
		War3Window_Cleanup();

		delete GameEventDispatcher;
		GameEventDispatcher = NULL;
	}

}

extern "C" unsigned int GetRevision() {
	return VERSION.revision;
}