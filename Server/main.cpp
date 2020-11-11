#include "TotalManager.h"

int main()
{
	TotalManager manager;
	for (auto& thread : TotalManager::threads)
		thread.join();
	return 0;
}