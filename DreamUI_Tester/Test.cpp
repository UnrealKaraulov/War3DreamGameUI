//#define DEBUG
#define _WIN32_WINNT 0x0501 
#define WINVER 0x0501 
#define NTDDI_VERSION 0x05010000
//#define BOTDEBUG
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

// Thanks @thewisp and @fluxxu for original DreamDota https://github.com/DreamHacks/dreamdota

#include <DreamUI.h>
#pragma comment(lib, "DreamUI.lib")

#include <EventData.h>
#include <Game.h>

#include <Tools.h>

#include <Button.h>


Button* TestButton1 = NULL;


void TestButtonChangeState(Button* btn)
{
	OutputScreen(15.0f, "Press TestButton1");
}


void TestGameUI()
{
	TestButton1 = new Button(
		UISimpleFrame::GetGameSimpleConsole(),
		0.09f, 0.027f,
		UISimpleButton::MOUSEBUTTON_LEFT,
		UISimpleButton::STATE_ENABLED,
		&TestButtonChangeState,
		NULL,
		false,
		false,
		0.011f
	);

	TestButton1->setText("Show dota config");
	TestButton1->setAbsolutePosition(UILayoutFrame::POSITION_BOTTOM_RIGHT,
		0.79f, 0.155f);

	TestButton1->_backdrop->show();
}

void CleanupTestGameUI()
{
	// NO NEED CLEANUP WITH 'DELETE', BECAUSE BUTTONS.CPP DO CLEAN ALL BUTTONS AFTER GAME END
	TestButton1 = NULL;
}

void TestGameStart(const Event*)
{
	MessageBox(0, "GAME START MESSAGE", "TEST DREAM UI", 0);

	TestGameUI();
}

void TestGameEnd(const Event*)
{
	MessageBox(0, "GAME END MESSAGE", "TEST DREAM UI", 0);

	CleanupTestGameUI();
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