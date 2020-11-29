#include "Scene.h"
#include "Communicator.h"
#include "Framework.h"
//이미지 초기 알파값 설정 잘 할것. 0으로 하면 안 보임.

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

	//std::cout << portNum <<"," << ipaddr << std::endl;



}

void InitScene::update()
{
	portNum = portBox.getContent();
	ipaddr = ipaddrBox.getContent();
}

void InitScene::handleEvnet(SDL_Event& e)
{
	//이부분부터 절대 지우지마셈!!! 
	
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
	//드론과 히드라 저그 몬스터 3프레임있는걸 2개 쓴다.
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
	//마우스커서를 안 보이게 한다.
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
	//두더지 이미지들을 쭉순회하면서 그린다.
	for (int i = 0; i < 2; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			Drones[i][j].setAlphaValue(0);
			Hydras[i][j].setAlphaValue(0);
			Zergs[i][j].setAlphaValue(0);
		}
	}
	//이미지를 백 버퍼에 다 그렸으면 그린 이미지는 알파값을 0으로
	//설정해 보이지 않도록 한다.
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
	//망치를 그리고
	GameState* curGameState = gFramework->getGameState();

	for (int i = 0; i < 9; ++i)
	{
		if (curGameState->isSpawned[i])//만약 현재 체크하는 구멍이 스폰이 된 상태라면
		{
			auto coord = Holes[i].getPosition();
			switch (curGameState->whichMole[i])// i번째 구멍에 어떤 두더지인가?
			{
			case 0:
				Drones[curGameState->whichContainer[i]][curGameState->whichFrame[i]].setAlphaValue(255);
				Drones[curGameState->whichContainer[i]][curGameState->whichFrame[i]].setPosition(coord.first, coord.second);
				//[i]번 컨테이너의 [n]번 프레임의 알파값을 255로 설정해서 보이게한다.
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
			//case0은 드론, 1은 히드라, 2는 저글링
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
		//내가 플레이어1이면 p1의 점수가 내 점수가 된다.
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
			//비긴경우 모두 승리처리.
		}
		else
		{
			if (gFramework->getGameState()->p1Point > gFramework->getGameState()->p2Point)
				scene->setIsWin(false);
			else if (gFramework->getGameState()->p1Point < gFramework->getGameState()->p2Point)
				scene->setIsWin(true);
			else
				scene->setIsWin(true);
			//비긴경우 모두 승리처리.
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
		//구멍 너비는 70
		//길이는 120
		for (int i = 0; i < Holes.size(); ++i)
		{
			auto coord = Holes[i].getPosition();
			if ((coord.first-HoleWidth < x && x < coord.first + HoleWidth) && (coord.second - HoleHeight < y && y < coord.second + HoleHeight))
			{
				gFramework->setClientToServer(i, std::chrono::high_resolution_clock::now());
				//구멍이 선택이 됐다면 서버에게 보낼 ctos구조체에 정보를 넣어준다.
				isSelected = true;
			}
		}
	}
	if (!isSelected)
		gFramework->setClientToServer(INT_MAX, std::chrono::high_resolution_clock::now());
	//만약 구멍을 선택하지 않았다면 INT_MAX값을 채워서 서버에게 넘겨준다.
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
