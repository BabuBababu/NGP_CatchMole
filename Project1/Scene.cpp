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



MainScene::MainScene(): BG(0,0,0,0,0,255,"resource/BackGround.jpg"), RedHammer(0,0,0,0,0,255,"resource/RedHammer.png"),isRecvInitialPacket(false)
{
	//int x, int y, int z, int w, int h, int alpha, std::string filePath
	//�ϵ��ڵ� ������
	std::string Zerg[3] = {"Drone", "Zergling", "Hydra"};
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
		Holes.emplace_back(60 + i%HolePerLine * 500, y, 0, 0, 0, 255, "resource/hole.png");
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
	SDL_ShowCursor(SDL_DISABLE);
}

MainScene::~MainScene()
{
}

void MainScene::render()
{
	BG.draw();
	RedHammer.draw();

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
}

void MainScene::update()
{
	int x, y;
	SDL_GetMouseState(&x, &y);
	RedHammer.setPosition(x-100, y-75);
	//��ġ�� �׸���
	GameState* curGameState = gFramework->getGameState();

	for (int i = 0; i < 9; ++i)
	{
		if (curGameState->isSpawned[i])//���� ���� üũ�ϴ� ������ ������ �� ���¶��
		{
			switch (curGameState->whichMole[i])
			{
			case 0:
				Drones[curGameState->whichContainer[i]][curGameState->whichFrame[i]].setAlphaValue(255);
				auto coord = Holes[i].getPosition();
				Drones[curGameState->whichContainer[i]][curGameState->whichFrame[i]].setPosition(coord.first, coord.second);
				//[i]�� �����̳��� [n]�� �������� ���İ��� 255�� �����ؼ� ���̰��Ѵ�.
				break;
			case 1:
				Hydras[curGameState->whichContainer[i]][curGameState->whichFrame[i]].setAlphaValue(255);
				auto coord = Holes[i].getPosition();
				Hydras[curGameState->whichContainer[i]][curGameState->whichFrame[i]].setPosition(coord.first, coord.second);
				break;
			case 2:
				Zergs[curGameState->whichContainer[i]][curGameState->whichFrame[i]].setAlphaValue(255);
				auto coord = Holes[i].getPosition();
				Zergs[curGameState->whichContainer[i]][curGameState->whichFrame[i]].setPosition(coord.first, coord.second);
				break;
			default:
				break;
			}
			//case0�� ���, 1�� ���̵��, 2�� ���۸�
		}
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
			if ((coord.first < x && x < coord.first + HoleWidth) && (coord.second < y && y < coord.second + HoleHeight))
			{
				gFramework->setClientToServer(i, std::chrono::high_resolution_clock::now());
				isSelected = true;
			}
		}
	}
	if (!isSelected)
		gFramework->setClientToServer(INT_MAX, std::chrono::high_resolution_clock::now());
	//���� ������ �������� �ʾҴٸ� INT_MAX���� ä���� �������� �Ѱ��ش�.
}
