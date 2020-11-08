#pragma once
#include "Scene.h"
#include "Communicator.h"
#include "SimpleSDL.h"

class Framework
{
public:
	Framework(Scene* initScene);
	~Framework();
	void render();
	void communicateWithServ();
	void updateScene();
	bool connectingToServ(const std::string&servAddr,const std::string& portNum );
	bool handleEvent();
	bool update();
	void changeScene(Scene* scene);
private:
	Scene* curScene;
	Communicator* communicator;
	ClientToServer* ctos;
	GameSate* gs;
};

extern Framework* gFramework;
