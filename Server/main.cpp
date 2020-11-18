#include "TotalManager.h"

HANDLE hStdin;
DWORD fdwSaveOldMode;

BOOL onConsoleEvent(DWORD event)
{
	switch (event)
	{
	case CTRL_C_EVENT:
	case CTRL_CLOSE_EVENT:
		TotalManager::isGameOver = true;
		break;
	default:
		break;
	}
	return TRUE;
}

int main()
{
	TotalManager manager;

	while (!TotalManager::isGameOver)
	{
		if (!SetConsoleCtrlHandler(onConsoleEvent, TRUE))
		{
			break;
		}
	}
	return 0;
}