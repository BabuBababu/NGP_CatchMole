#include "Scene.h"



InitScene::InitScene() :portBox(440, 400, 400, 50, "textModel.ttf"), ipaddrBox(440, 500, 400, 50, "textModel.ttf")
{
	upButton = SimpleSDL::Image(640, 550, 0, 0, 0, 255, "resource/ButtonFalse.png");
	downButton = SimpleSDL::Image(640, 550, 0, 0, 0, 0, "resource/ButtonTrue.png");
	BG = SimpleSDL::Image(0, 0, 0, 0, 0, 0, "resource/ReadyBG.jpg");
}

InitScene::~InitScene()
{
}

void InitScene::render()
{
	BG.draw();
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
