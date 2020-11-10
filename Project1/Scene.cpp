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
	//�����ȵ�
	BG.draw();
	portBox.draw();
	ipaddrBox.draw();
	if (portNum == "" || ipaddr == "")  // ��� �ڽ��ȿ� �۾��� �ϳ��� ���������� ���
	{
		//��ư false�� �����ش�.
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
{ //�̺κ� ���� ���� ����������!!! 
	portNum = portBox.getContent();
	ipaddr = ipaddrBox.getContent();
	
	if (e.type == SDL_MOUSEBUTTONDOWN)
	{
		int x, y;
		SDL_GetMouseState(&x, &y);
		if ((540 < x < 640) && (580 < y < 630))
		{
			//mainscene���� �Ѿ�� ����
			
			
		}
	}
	else if (e.type == SDL_MOUSEBUTTONUP)
	{
	}
}
