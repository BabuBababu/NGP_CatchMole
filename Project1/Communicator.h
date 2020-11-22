#pragma once
#pragma comment(lib, "ws2_32")
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WinSock2.h>
#include <chrono>
#include <string>

#pragma pack(1)
struct ClientToServer
{
	ClientToServer()
	{
		clikedHole = INT_MAX;
	}
	int clikedHole;
	std::chrono::high_resolution_clock::time_point clikedTime;
}typedef ClientToServer;
#pragma pack()

#pragma pack(1)
struct GameSate
{
	int remainingTime = 60;
	int p1Point;
	int p2Point;
	int p1HammerFrame;
	int p2HammerFrame;
	bool isSpawned[9];
	//각 구녕별로 스폰이 되었는지 확인한다
	int whichMole[9];
	//스폰이 되었다면 어떤 두더지 인지 확인한다
	int whichContainer[9];
	//어떤 두더지인지 확인되었다면 0,1번 그림 컨테이너 중 무엇을 고를것인가
	int whichFrame[9];
	//여기까지 됐으면 몇 번 프레임을 그려야 하는가
	bool isPlayer1;
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

