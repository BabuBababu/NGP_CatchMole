#include "Scene.h"



InitScene::InitScene() :portBox(440, 400, 400, 50, "textModel.ttf"), ipaddrBox(440, 500, 400, 50, "textModel.ttf")
{
	falseButton = SimpleSDL::Image(540, 580, 0, 0, 0, 0, "resource/ButtonFalse.png");
	trueButton = SimpleSDL::Image(540, 580, 0, 0, 0, 0, "resource/ButtonTrue.png");
	BG = SimpleSDL::Image(0, 0, 0, 0, 0, 0, "resource/ReadyBG.jpg");
}

InitScene::~InitScene()
{
}

void InitScene::render()
{
	//지우면안됨
	BG.draw();
	portBox.draw();
	ipaddrBox.draw();
	if (portNum == "" || ipaddr == "")  // 모든 박스안에 글씨가 하나라도 안적혀있을 경우
	{
		//버튼 false를 보여준다.
		falseButton.draw();
	}
	else 
	{
		trueButton.draw();
	}

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
{ //이부분 부터 절대 지우지마셈!!! 
	portNum = portBox.getContent();
	ipaddr = ipaddrBox.getContent();
	
	if (e.type == SDL_MOUSEBUTTONDOWN)
	{
		int x, y;
		SDL_GetMouseState(&x, &y);
		if ((540 < x < 640) && (580 < y < 630))
		{
			//mainscene으로 넘어가기 구현
			
			
		}
	}
	else if (e.type == SDL_MOUSEBUTTONUP)
	{
	}
}
