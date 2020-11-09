#include "Scene.h"

TestScene::TestScene():musik("test.mp3", SimpleSDL::SOUNDVOLUME::MEDIUM), Zergling(0, 0, 0, 0, 0, 0, "resource/Zergling (1).png")
{
	tests.emplace_back("test.wav", SimpleSDL::SOUNDVOLUME::MEDIUM);
	//Hydra.emplace_back(0, 0, 0, 0, 0, 0, "resource/Zergling (1).png");
}
TestScene::~TestScene()
{

}

void TestScene::render()
{
	//Zergling.draw();
	/*for (auto & img : Hydra)
	{
		img.draw();
	}*/
}

void TestScene::update()
{
	
	
}

void TestScene::handleEvnet(SDL_Event& e)
{
}
