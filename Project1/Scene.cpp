#include "Scene.h"

TestScene::TestScene():musik("test.mp3", SimpleSDL::SOUNDVOLUME::MEDIUM)
{
	tests.emplace_back("test.wav", SimpleSDL::SOUNDVOLUME::MEDIUM);
}
TestScene::TestScene() : Zergling(0, 0, 0, 0, 0, 0, "Zergling(1).png")
{

}

TestScene::~TestScene()
{

}

void TestScene::render()
{
	Zergling.draw();
}

void TestScene::update()
{
	
	
}

void TestScene::handleEvnet(SDL_Event& e)
{
}
