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
	//에디트박스 2개필요하고
	SimpleSDL::EditBox portBox;
	SimpleSDL::EditBox ipaddrBox;
	//서버포트랑 ip주소 입력할 변수 2개
	std::string portNum;
	std::string ipaddr;
	// 버튼용 이미지 2개, 뒷 배경 1개
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
	SimpleSDL::Image test;


	bool isRecvInitialPacket;
};