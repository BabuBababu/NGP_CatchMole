#include "Scene.h"

TestScene::TestScene():musik("test.mp3", SimpleSDL::SOUNDVOLUME::MEDIUM)
{
	tests.emplace_back("test.wav", SimpleSDL::SOUNDVOLUME::MEDIUM);
}

TestScene::~TestScene()
{

}

void TestScene::render()
{
	
}

void TestScene::update()
{
	
}

void TestScene::handleEvnet(SDL_Event& e)
{
}
