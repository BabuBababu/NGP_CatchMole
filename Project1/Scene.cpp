#include "Scene.h"



InitScene::InitScene() :portBox(600, 300, 400, 50, "test1.ttf"), ipaddrBox(600, 400, 400, 50, "test1.ttf")
{//√ ±‚»≠ EditBox(int x,int y, int w,int h, std::string fontLoc):container(x, y + h / 8, h / 2, 0, 0, 0, 255, fontLoc.c_str(), "")
	upButton = SimpleSDL::Image(640, 550, 0, 0, 0, 255, "resource/Hydra (1).png");
	downButton = SimpleSDL::Image(640, 550, 0, 0, 0, 0, "resource/Hydra (4).png");
}

InitScene::~InitScene()
{
}

void InitScene::render()
{
	portBox.draw();
	ipaddrBox.draw();
	upButton.draw();
	downButton.draw();
	std::cout << portNum <<"," << ipaddr << std::endl;



	//Zergling.draw();
	//for (auto& img : Hydra)
	//{
	//	img.draw();
	//}
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
		if ((640 < x < 740) && (550 < y < 650))
		{
			upButton.setAlphaValue(0);
			upButton.enableAlphaBlending();
			portNum = portBox.getContent();
			ipaddr = ipaddrBox.getContent();
		}
	}
	else if (e.type == SDL_MOUSEBUTTONUP)
	{
	}
}
