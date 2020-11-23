#pragma once

#include "SimpleSDL.h"

class Scene
{
public:
	Scene() = default;
	virtual ~Scene() = default;
	virtual void render()=0;
	virtual void update()=0;
	virtual void handleEvnet(SDL_Event& e)=0;
};


class InitScene : public Scene
{
public:
	InitScene();
	~InitScene();
	void render() override;
	void update() override;
	void handleEvnet(SDL_Event& e) override;
private:
	//����Ʈ�ڽ� 2���ʿ��ϰ�
	SimpleSDL::EditBox portBox;
	SimpleSDL::EditBox ipaddrBox;
	//������Ʈ�� ip�ּ� �Է��� ���� 2��
	std::string portNum;
	std::string ipaddr;
	// ��ư�� �̹��� 2��, �� ��� 1��
	SimpleSDL::Image BG;
	SimpleSDL::Image falseButton;
	SimpleSDL::Image trueButton;

	std::vector<SimpleSDL::TTF> ttf;
};

class MainScene : public Scene
{

public:
	MainScene();
	~MainScene();
	void render() override;
	void update() override;
	void handleEvnet(SDL_Event& e) override;
private:
	SimpleSDL::Image BG;
	SimpleSDL::Image RedHammer;
	//���� ����� ���ͷ�
	
	std::vector<SimpleSDL::Image> Holes;
	std::vector<SimpleSDL::Image> RedHammerAnimation;
	std::vector<SimpleSDL::Image> Drones[2];
	std::vector<SimpleSDL::Image> Hydras[2];
	std::vector<SimpleSDL::Image> Zergs[2];
	SimpleSDL::TTF timeFont;
	SimpleSDL::TTF myPoint;
	SimpleSDL::TTF otherPoint;
	SimpleSDL::TTF watingMsg;
	bool isRecvInitialPacket;
	bool isPlayer1;
	// ���� �÷��̾�1 ���� Ȯ���Ѵ�.
	const int HoleNumber = 9;
	const int HolePerLine = 3;
};

class EndingScene : public Scene
{

public:
	EndingScene();
	~EndingScene();
	void render() override;
	void update() override;
	void handleEvnet(SDL_Event& e) override;
	void setIsWin(bool var) { isWin = var; }
private:
	bool isWin;
	SimpleSDL::Image winImg;
	SimpleSDL::Image loseImg;
	SimpleSDL::TTF text;
	float remainingTime = 3.0f;
	bool isOver = false;
};