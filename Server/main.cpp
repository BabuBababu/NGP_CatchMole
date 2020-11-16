#include "TotalManager.h"

HANDLE hStdin;
DWORD fdwSaveOldMode;

int main()
{
	TotalManager manager;
	/*for (auto& thread :TotalManager::threads)
	{
		thread.join();
	}*/

	DWORD cNumRead, fdwMode, i;
	INPUT_RECORD irInBuf[4];

	hStdin = GetStdHandle(STD_INPUT_HANDLE);

	fdwMode = ENABLE_WINDOW_INPUT;

	while (!TotalManager::isGameOver)
	{
		if (!ReadConsoleInput(hStdin, irInBuf, 4, &cNumRead))
			std::cout << "Error" << std::endl;

		switch (irInBuf[0].EventType)
		{
		case KEY_EVENT:
			std::cout << "KeyEventOccure" << std::endl;
			TotalManager::isGameOver = true;
			break;
		default:
			break;
		}
		
	}
	SetConsoleMode(hStdin, fdwSaveOldMode);
	return 0;
}