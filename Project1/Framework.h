#pragma once
#include <chrono>

class Scene;
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
	Framework();
	~Framework();
	void render();
	void communicateWithServ();
	void updateScene();
	void handleEvent();
	void update();
	void changeScene();
private:
	Scene* curScene;
	Communicator* communicator;
	ClientToServer* ctos;
	GameSate* gs;
};

