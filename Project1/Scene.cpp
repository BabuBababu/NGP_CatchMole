#include "Scene.h"

TestScene::TestScene():musik("test.mp3", SimpleSDL::SOUNDVOLUME::MEDIUM), Zergling(0, 0, 0, 0, 0, 0, "resource/Zergling (1).png")
{
	tests.emplace_back("test.wav", SimpleSDL::SOUNDVOLUME::MEDIUM);
	
	for (int i = 1; i < 5; ++i)
	{
		std::string temp{ "resource/Hydra (" };
		temp += std::to_string(i) + ").png";
		Hydra.emplace_back(i*50, 0, 0, 0, 0, 0, temp);
	}
	

}
TestScene::~TestScene()
{

}

void TestScene::render()
{
	Zergling.draw();
	for (auto & img : Hydra)
	{
		img.draw();
	}
}

void TestScene::update()
{
	
	
}

void TestScene::handleEvnet(SDL_Event& e)
{
}



InitScene::InitScene():portBox(600,300,400,50,"test1.ttf"),ipaddrBox(600,400,400,50,"test1.ttf")
{//ÃÊ±âÈ­ EditBox(int x,int y, int w,int h, std::string fontLoc):container(x, y + h / 8, h / 2, 0, 0, 0, 255, fontLoc.c_str(), "")
	upButton = SimpleSDL::Image(640, 550, 0, 0, 0, 0,"resource/Hydra (1).png");
	downButton = SimpleSDL::Image(640, 550, 0, 0, 0,0, "resource/Hydra (4).png");

}

InitScene::~InitScene()
{
}

void InitScene::render()
{
	/*portBox.draw();
	ipaddrBox.draw();*/
	upButton.draw();
	downButton.draw();
}

void InitScene::update()
{
}

void InitScene::handleEvnet(SDL_Event& e)
{
	if (e.type == SDL_MOUSEBUTTONDOWN)
	{
		int x, y;
		SDL_GetMouseState(&x, &y);
	}
}
