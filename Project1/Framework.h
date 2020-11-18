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
	bool communicateWithServ();
	void updateScene();
	bool recvInitialPacketFromServer();
	bool connectingToServ(const std::string&servAddr,const std::string& portNum );
	bool handleEvent();
	bool update();
	void changeScene(Scene* scene);
	void setClientToServer(int clickedHole, std::chrono::high_resolution_clock::time_point tp)
	{
		ctos->clikedHole = clickedHole;
		ctos->clikedTime = tp;
	}
	GameSate* getGameState() { return gs; }
private:
	Scene* curScene;
	Communicator* communicator;
	ClientToServer* ctos;
	GameSate* gs;
	bool isGameStart;
};

extern Framework* gFramework;
