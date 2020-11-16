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
	else if (e.type == SDL_MOUSEBUTTONUP)
	{
	}
}



MainScene::MainScene(): BG(0,0,0,0,0,255,"resource/BackGround.jpg"), RedHammer(0,0,0,0,0,255,"resource/RedHammer.png"),isRecvInitialPacket(false)
{
	//�ϵ��ڵ� ������
	std::string Zerg[3] = {"Drone", "Zergling", "Hydra"};
		
	for (int i = 0; i < 3; ++i) {   // ù��° ��
		std::string Selected = Zerg[rand() % 3];
		for (int j = 1; j < 6; ++j) {
			Hole[i].emplace_back(200+i*300, 100, 0, 0, 0, 255, "resource/" + Selected + " (" + std::to_string(j) + ").png");
		}
	}
	for (int i = 0; i < 3; ++i) {  // �ι�° ��
		std::string Selected = Zerg[rand() % 3];
		for (int j = 1; j < 6; ++j) {
			Hole[i+3].emplace_back(200+i*300, 300, 0, 0, 0, 255, "resource/" + Selected + " (" + std::to_string(j) + ").png");
		}
	}
	for (int i = 0; i < 3; ++i) {  // ����° ��
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

	//�δ��� 9���� ���ÿ� �׸���.
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
	//�ٸ� �÷��̾ ������ �����ߴٸ� ȭ�� ������� ���� ����

	int x, y;
	SDL_GetMouseState(&x, &y);
	RedHammer.setPosition(x-100, y-75);
}

void MainScene::handleEvnet(SDL_Event& e)
{
}
