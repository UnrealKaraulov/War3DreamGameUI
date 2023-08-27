//#define DEBUG
#define _WIN32_WINNT 0x0501 
#define WINVER 0x0501 
#define NTDDI_VERSION 0x05010000
//#define BOTDEBUG
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <DreamUI.h>
#pragma comment(lib, "DreamUI.lib")

#include <EventData.h>
#include <Game.h>

#include <Tools.h>


// Thanks @thewisp and @fluxxu for original DreamDota https://github.com/DreamHacks/dreamdota

void TestGameStart(const Event*)
{
	MessageBox(0, "GAME START MESSAGE", "TEST DREAM UI", 0);
}

void TestGameEnd(const Event*)
{
	MessageBox(0, "GAME END MESSAGE", "TEST DREAM UI", 0);
}


int WINAPI DllMain(HINSTANCE hDLL, int reason, LPVOID reserved)
{
	if (reason == DLL_PROCESS_ATTACH)
	{
		InitDreamUI(GetModuleHandle("Game.dll"), GetModuleHandle("Storm.dll"), 6401);
		MainDispatcher()->listen(EVENT_GAME_START, TestGameStart);
		MainDispatcher()->listen(EVENT_GAME_END, TestGameEnd);
	}
	else if (reason == DLL_PROCESS_DETACH)
	{
		CleanupDreamUI();
	}
	return TRUE;
}