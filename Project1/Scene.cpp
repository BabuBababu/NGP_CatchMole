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
	//이부분부터 절대 지우지마셈!!! 
	
	if (e.type == SDL_MOUSEBUTTONDOWN)
	{
		int x, y;
		SDL_GetMouseState(&x, &y);
		auto buttonPos = trueButton.getPosition();
		auto buttonSize = trueButton.getTextureSize();
		if ((buttonPos.first < x && x < buttonPos.first+buttonSize.first) && (buttonPos.second < y && y < buttonPos.second+buttonSize.second))
		{
			//먼저 서버에 portNum과 ipaddr 값을 보내주고 mainscene으로 넘어가기 구현 
			//Communicator player;
			//player.connectToServ(ipaddr, portNum);
			//이 부분 틀림.

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
	//다른 플레이어도 접속을 성공했다면 화면 밝아지고 게임 시작
}

void MainScene::handleEvnet(SDL_Event& e)
{
}
