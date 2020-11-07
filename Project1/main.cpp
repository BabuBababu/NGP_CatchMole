#include "Framework.h"

int main(int argc, char* args[])
{
	SimpleSDL::init(600, 600);
	TestScene* initScene = new TestScene();
	Framework *framework=new Framework(initScene);
	while (!framework->update());
	delete framework;
	SimpleSDL::quit();
	return 0;
}