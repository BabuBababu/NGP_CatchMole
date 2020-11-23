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
	//�� ���纰�� ������ �Ǿ����� Ȯ���Ѵ�
	int whichMole[9];
	//������ �Ǿ��ٸ� � �δ��� ���� Ȯ���Ѵ�
	int whichContainer[9];
	//� �δ������� Ȯ�εǾ��ٸ� 0,1�� �׸� �����̳� �� ������ �����ΰ�
	int whichFrame[9];
	//������� ������ �� �� �������� �׷��� �ϴ°�
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

