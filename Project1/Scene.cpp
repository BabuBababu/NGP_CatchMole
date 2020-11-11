#include "Scene.h"
#include "Communicator.h"
#include "Framework.h"

//�̹��� �ʱ� ���İ� ���� �� �Ұ�. 0���� �ϸ� �� ����.

InitScene::InitScene() :portBox(440, 400, 400, 50, "textModel.ttf"), ipaddrBox(440, 500, 400, 50, "textModel.ttf")
{
	falseButton = SimpleSDL::Image(540, 580, 0, 0, 0, 255, "resource/ButtonFalse.png");
	trueButton = SimpleSDL::Image(540, 580, 0, 0, 0, 255, "resource/ButtonTrue.png");
	BG = SimpleSDL::Image(0, 0, 0, 0, 0, 255, "resource/ReadyBG.jpg");
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

	//std::cout << portNum <<"," << ipaddr << std::endl;



	//Zergling.draw();
	//for (auto& img : Hydra)
	//{
	//	img.draw();
	//}
}

void InitScene::update()
{
	portNum = portBox.getContent();
	ipaddr = ipaddrBox.getContent();
}

void InitScene::handleEvnet(SDL_Event& e)
{
	//�̺κк��� ���� ����������!!! 
	
	if (e.type == SDL_MOUSEBUTTONDOWN)
	{
		int x, y;
		SDL_GetMouseState(&x, &y);
		auto buttonPos = trueButton.getPosition();
		auto buttonSize = trueButton.getTextureSize();
		if ((buttonPos.first < x && x < buttonPos.first+buttonSize.first) && (buttonPos.second < y && y < buttonPos.second+buttonSize.second))
		{
			//���� ������ portNum�� ipaddr ���� �����ְ� mainscene���� �Ѿ�� ���� 
			//Communicator player;
			//player.connectToServ(ipaddr, portNum);
			//�� �κ� Ʋ��.

			gFramework->connectingToServ(ipaddr, portNum);
			MainScene * mainscene = new MainScene();
			gFramework->changeScene(mainscene);
		}
	}
	else if (e.type == SDL_MOUSEBUTTONUP)
	{
	}
}



MainScene::MainScene(): test(0,0,0,0,0,255,"resource/Drone (1).png"), isRecvInitialPacket(false)
{
}

MainScene::~MainScene()
{
}

void MainScene::render()
{
	test.draw();
}

void MainScene::update()
{
	if (!isRecvInitialPacket)
	{
		gFramework->recvInitialPacketFromServer();
	}
	//�ٸ� �÷��̾ ������ �����ߴٸ� ȭ�� ������� ���� ����
}

void MainScene::handleEvnet(SDL_Event& e)
{
}
