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

			gFramework->connectingToServ(ipaddr, portNum);
			MainScene * mainscene = new MainScene();
			gFramework->changeScene(mainscene);
		}
	}
}


MainScene::MainScene() : BG(0, 0, 0, 0, 0, 255, "resource/BackGround.jpg"), isRecvInitialPacket(false),
timeFont(500, 0, 30, 0, 0, 0, 0, "textModel.ttf", "60"), myPoint(100, 0, 30, 0, 255, 0, 0, "textModel.ttf", "My Point: 0"),
otherPoint(900, 0, 30, 255, 0, 0, 0, "textModel.ttf", "Other Point: 0"),
watingMsg(100, 300, 100, 255, 0, 0, 0, "textModel.ttf", "Wating Another Player...."),
BGM(("resource/BGM.mp3"),SimpleSDL::SOUNDVOLUME::MEDIUM)
{
	RedHammerAnimation.emplace_back(0, 0, 0, 0,0, 255, "resource/RedHammer.png");
	RedHammerAnimation.emplace_back(0, 0, 0, 0,0, 255, "resource/RedHammer (1).png");
	RedHammerAnimation.emplace_back(0, 0, 0, 0,0, 255, "resource/RedHammer (2).png");
	std::string Zerg[3] = { "Drone", "Zergling", "Hydra" };
	Holes.reserve(9);
	Drones[0].reserve(2);
	Hydras[0].reserve(2);
	Zergs[0].reserve(2);
	Drones[1].reserve(2);
	Hydras[1].reserve(2);
	Zergs[1].reserve(2);
	//��а� ����� ���� ���� 3�������ִ°� 2�� ����.
	int y = -250;

	for (int i = 0; i < HoleNumber; ++i)
	{
		if (i % HolePerLine == 0) y += 250;
		Holes.emplace_back(60 + i % HolePerLine * 500, y, 0, 0, 0, 255, "resource/hole.png");
	}

	for (int i = 0; i < 2; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			Drones[i].emplace_back(0, 0, 0, 0, 0, 0, "resource/Drone" + std::to_string(j + 1) + ".png");
			Hydras[i].emplace_back(0, 0, 0, 0, 0, 0, "resource/Hydra" + std::to_string(j + 1) + ".png");
			Zergs[i].emplace_back(0, 0, 0, 0, 0, 0, "resource/Zergling" + std::to_string(j + 1) + ".png");
		}
	}
	//SDL_ShowCursor(SDL_DISABLE);
	//���콺Ŀ���� �� ���̰� �Ѵ�.
}

MainScene::~MainScene()
{
}

void MainScene::render()
{
	BG.draw();
	//RedHammer.draw();
	

	for (int i = 0; i < Holes.size(); ++i)
		Holes[i].draw();

	for (int i = 0; i < 2; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			Drones[i][j].draw();
			Hydras[i][j].draw();
			Zergs[i][j].draw();
		}
	}
	//�δ��� �̹������� �߼�ȸ�ϸ鼭 �׸���.
	for (int i = 0; i < 2; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			Drones[i][j].setAlphaValue(0);
			Hydras[i][j].setAlphaValue(0);
			Zergs[i][j].setAlphaValue(0);
		}
	}
	//�̹����� �� ���ۿ� �� �׷����� �׸� �̹����� ���İ��� 0����
	//������ ������ �ʵ��� �Ѵ�.
	if (isPlayer1)
		RedHammerAnimation[gFramework->getGameState()->p1HammerFrame].draw();
	else
		RedHammerAnimation[gFramework->getGameState()->p2HammerFrame].draw();
	timeFont.draw();
	myPoint.draw();
	otherPoint.draw();
	if (!isRecvInitialPacket)
		watingMsg.draw();
}

void MainScene::update()
{
	if (!isRecvInitialPacket)
	{
		isRecvInitialPacket = gFramework->recvInitialPacketFromServer();
		if (isRecvInitialPacket)
		{
			isPlayer1 = gFramework->getGameState()->isPlayer1;
		}
	}
	int x, y;
	SDL_GetMouseState(&x, &y);
	if (isPlayer1)
		RedHammerAnimation[gFramework->getGameState()->p1HammerFrame].setPosition(x - 100, y - 75);
	else
		RedHammerAnimation[gFramework->getGameState()->p2HammerFrame].setPosition(x - 100,y -75);
	//RedHammer.setPosition(x - 100, y - 75);
	//��ġ�� �׸���
	GameState* curGameState = gFramework->getGameState();

	for (int i = 0; i < 9; ++i)
	{
		if (curGameState->isSpawned[i])//���� ���� üũ�ϴ� ������ ������ �� ���¶��
		{
			auto coord = Holes[i].getPosition();
			switch (curGameState->whichMole[i])// i��° ���ۿ� � �δ����ΰ�?
			{
			case 0:
				Drones[curGameState->whichContainer[i]][curGameState->whichFrame[i]].setAlphaValue(255);
				Drones[curGameState->whichContainer[i]][curGameState->whichFrame[i]].setPosition(coord.first, coord.second);
				//[i]�� �����̳��� [n]�� �������� ���İ��� 255�� �����ؼ� ���̰��Ѵ�.
				break;
			case 1:
				Hydras[curGameState->whichContainer[i]][curGameState->whichFrame[i]].setAlphaValue(255);
				Hydras[curGameState->whichContainer[i]][curGameState->whichFrame[i]].setPosition(coord.first, coord.second);
				break;
			case 2:
				Zergs[curGameState->whichContainer[i]][curGameState->whichFrame[i]].setAlphaValue(255);
				Zergs[curGameState->whichContainer[i]][curGameState->whichFrame[i]].setPosition(coord.first, coord.second);
				break;
			default:
				break;
			}
			//case0�� ���, 1�� �����, 2�� ���۸�
		}
	}

	if (gFramework->getGameState()->remainingTime > 0)
	{
		timeFont.changeText("Remaining Time: " + std::to_string(gFramework->getGameState()->remainingTime));
		if (isPlayer1)
		{
			myPoint.changeText("My Point" + std::to_string(gFramework->getGameState()->p1Point));
			otherPoint.changeText("Other Point" + std::to_string(gFramework->getGameState()->p2Point));
		}
		//���� �÷��̾�1�̸� p1�� ������ �� ������ �ȴ�.
		else
		{
			myPoint.changeText("My Point" + std::to_string(gFramework->getGameState()->p2Point));
			otherPoint.changeText("Other Point" + std::to_string(gFramework->getGameState()->p1Point));
		}
	}
	else
	{
		EndingScene* scene = new EndingScene();
		if (isPlayer1)
		{
			if (gFramework->getGameState()->p1Point > gFramework->getGameState()->p2Point)
				scene->setIsWin(true);
			else if (gFramework->getGameState()->p1Point < gFramework->getGameState()->p2Point)
				scene->setIsWin(false);
			else
				scene->setIsWin(true);
			//����� ��� �¸�ó��.
		}
		else
		{
			if (gFramework->getGameState()->p1Point > gFramework->getGameState()->p2Point)
				scene->setIsWin(false);
			else if (gFramework->getGameState()->p1Point < gFramework->getGameState()->p2Point)
				scene->setIsWin(true);
			else
				scene->setIsWin(true);
			//����� ��� �¸�ó��.
		}
		gFramework->changeScene(scene);
	}

}

void MainScene::handleEvnet(SDL_Event& e)
{
	bool isSelected = false;
	if (e.type == SDL_MOUSEBUTTONUP)
	{
		int x, y;
		SDL_GetMouseState(&x, &y);
		int HoleWidth = 150;
		int HoleHeight = 120;
		//���� �ʺ�� 70
		//���̴� 120
		for (int i = 0; i < Holes.size(); ++i)
		{
			auto coord = Holes[i].getPosition();
			if ((coord.first-HoleWidth < x && x < coord.first + HoleWidth) && (coord.second - HoleHeight < y && y < coord.second + HoleHeight))
			{
				gFramework->setClientToServer(i, std::chrono::high_resolution_clock::now());
				//������ ������ �ƴٸ� �������� ���� ctos����ü�� ������ �־��ش�.
				isSelected = true;
			}
		}
	}
	if (!isSelected)
		gFramework->setClientToServer(INT_MAX, std::chrono::high_resolution_clock::now());
	//���� ������ �������� �ʾҴٸ� INT_MAX���� ä���� �������� �Ѱ��ش�.
}


EndingScene::EndingScene() :winImg(300, 0, 0, 700, 600, 255, "resource/Win.png"),
loseImg(300, 0, 0, 700, 600, 255, "resource/Lose.png"),
text(400, 650, 50, 0, 0, 0, 0, "textModel.ttf", "Press Any Key To Quit")
{
}

EndingScene::~EndingScene()
{
}

void EndingScene::render()
{
	if (isWin)
		winImg.draw();

	else
		loseImg.draw();
	text.draw();
}

void EndingScene::update()
{

}

void EndingScene::handleEvnet(SDL_Event& e)
{
	if (e.type == SDL_KEYDOWN)
	{
		gFramework->setIsGameEnd(true);
	}
}
