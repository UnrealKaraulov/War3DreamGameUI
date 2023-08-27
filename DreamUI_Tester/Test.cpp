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
#include <Input.h>
#include <EditBoxButton.h>

Button* TestButton1 = NULL;

std::string TestEditBoxString;

EditBoxButton* TestEditBox;


void TestButtonChangeState(Button* btn)
{
	OutputScreen(15.0f, "Press TestButton1");

	float newposx = (rand() % (int)(wc3_max_x * 100)) / 100.0f;

	if (newposx + TestButton1->getFrame()->width() + 0.01 > wc3_max_x)
		newposx -= TestButton1->getFrame()->width();

	float newposy = (rand() % (int)(wc3_max_y * 100)) / 100.0f;

	if (newposy + TestButton1->getFrame()->height() + 0.01 > wc3_max_y)
		newposy -= TestButton1->getFrame()->height();

	TestButton1->setAbsolutePosition(UILayoutFrame::POSITION_BOTTOM_RIGHT,
		newposx, newposy);


	TestEditBox->setRelativePosition(
		UILayoutFrame::POSITION_TOP_LEFT,
		TestButton1->_backdrop,
		UILayoutFrame::POSITION_TOP_LEFT,
		-(TestEditBox->getFrame()->width() + 0.002), 0.0f);
}

void EditBoxCallback(Button* button)
{
	OutputScreen(15.0f, "Called EditBoxCallback");
}

void TestGameUI()
{
	TestButton1 = new Button(
		UISimpleFrame::GetGameSimpleConsole(),
		0.09f, 0.027f,
		UISimpleButton::MOUSEBUTTON_LEFT,
		UISimpleButton::STATE_ENABLED,
		&TestButtonChangeState,
		"TestButton1 ToolTip |nTestButton1 ToolTip|nTestButton1 ToolTip|nTestButton1 ToolTip",
		/*  Set to false to cleanup it after game end */
		false,
		false,
		0.011f
	);

	TestButton1->setText("TestButton1");
	TestButton1->setAbsolutePosition(UILayoutFrame::POSITION_BOTTOM_RIGHT,
		0.48f, 0.35f);

	TestButton1->_backdrop->show();


	TestEditBox = new EditBoxButton(
		TestButton1->_backdrop,
		0.05f, 0.03f,
		&TestEditBoxString,
		"TEST ME", EditBoxCallback);


	TestEditBox->setRelativePosition(
		UILayoutFrame::POSITION_TOP_LEFT,
		TestButton1->_backdrop,
		UILayoutFrame::POSITION_TOP_LEFT,
		-(TestEditBox->getFrame()->width() + 0.002), 0.0f);

}

void CleanupTestGameUI()
{
	// NO NEED CLEANUP WITH 'DELETE', BECAUSE BUTTONS.CPP DO CLEAN ALL BUTTONS AFTER GAME END
	TestButton1 = NULL;
	TestEditBox = NULL;
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