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
	SimpleSDL::Image test;


	bool isRecvInitialPacket;
};