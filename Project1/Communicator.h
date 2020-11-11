#pragma once
#pragma comment(lib, "ws2_32")
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WinSock2.h>
#include <chrono>
#include <string>

#pragma pack(1)
struct ClientToServer
{
	int clikedHole;
	std::chrono::steady_clock::time_point clikedTime;
}typedef ClientToServer;
#pragma pack()

#pragma pack(1)
struct GameSate
{
	int remainingTime;
	int p1Point;
	int p2Point;
	int p1HammerFrame;
	int p2HammerFrame;
	std::pair<int, float> molespos[9];
}typedef GameState;
#pragma pack()

class Communicator
{
public:
	Communicator();
	~Communicator();
	void sendData(ClientToServer* obj);
	bool recvData(GameState* obj);
	bool connectToServ(const std::string& servAddr, const std::string& portNum);
	bool recvInitialGameState(GameState* gs);
private:
	SOCKET clntSock;
};

