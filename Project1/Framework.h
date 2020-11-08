#pragma once
#include <chrono>
#include "SimpleSDL.h"
#include "Scene.h"

class Communicator;

struct ClientToServer
{
	int clikedHole;
	std::chrono::steady_clock::time_point clikedTime;
};

struct GameSate
{
	int remainingTime;
	int p1Point;
	int p2Point;
	int p1HammerFrame;
	int p2HammerFrame;
	std::pair<int, float> molespos[9];
};

class Framework
{
public:
	Framework(Scene* initScene);
	~Framework();
	void render();
	void communicateWithServ();
	void updateScene();
	bool handleEvent();
	bool update();
	void changeScene(Scene* scene);
private:
	Scene* curScene;
	Communicator* communicator;
	ClientToServer* ctos;
	GameSate* gs;
};

