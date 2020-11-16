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
	else if (e.type == SDL_MOUSEBUTTONUP)
	{
	}
}



MainScene::MainScene(): BG(0,0,0,0,0,255,"resource/BackGround.jpg"), RedHammer(0,0,0,0,0,255,"resource/RedHammer.png"),isRecvInitialPacket(false)
{
	//하드코딩 양해좀
	std::string Zerg[3] = {"Drone", "Zergling", "Hydra"};
		
	for (int i = 0; i < 3; ++i) {   // 첫번째 줄
		std::string Selected = Zerg[rand() % 3];
		for (int j = 1; j < 6; ++j) {
			Hole[i].emplace_back(200+i*300, 100, 0, 0, 0, 255, "resource/" + Selected + " (" + std::to_string(j) + ").png");
		}
	}
	for (int i = 0; i < 3; ++i) {  // 두번째 줄
		std::string Selected = Zerg[rand() % 3];
		for (int j = 1; j < 6; ++j) {
			Hole[i+3].emplace_back(200+i*300, 300, 0, 0, 0, 255, "resource/" + Selected + " (" + std::to_string(j) + ").png");
		}
	}
	for (int i = 0; i < 3; ++i) {  // 세번째 줄
		std::string Selected = Zerg[rand() % 3];
		for (int j = 1; j < 6; ++j) {
			Hole[i+6].emplace_back(200+i*300, 500, 0, 0, 0, 255, "resource/" + Selected + " (" + std::to_string(j) + ").png");
		}
	}
}

MainScene::~MainScene()
{
}

void MainScene::render()
{
	BG.draw();
	RedHammer.draw();

	//두더지 9마리 동시에 그린다.
	for (int i = 0; i < 9; ++i) {
		for (auto& img : Hole[i])
		{
			img.draw();
		}
	}
	
}

void MainScene::update()
{
	if (!isRecvInitialPacket)
	{
		isRecvInitialPacket = gFramework->recvInitialPacketFromServer();
	}
	//다른 플레이어도 접속을 성공했다면 화면 밝아지고 게임 시작

	int x, y;
	SDL_GetMouseState(&x, &y);
	RedHammer.setPosition(x-100, y-75);
}

void MainScene::handleEvnet(SDL_Event& e)
{
}
