#pragma once
#pragma comment(lib, "ws2_32")
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WinSock2.h>
#include <vector>
#include <thread>
#include <atomic>
#include <chrono>
#include <iostream>

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
struct GameState
{
	GameState()
	{
		remainingTime = 30;
		p1Point = 0;
		p2Point = 0;
		p1HammerFrame = 0;
		p2HammerFrame = 0;
		for (int i = 0; i < 9; ++i)
		{
			isSpawned[i] = false;
			whichMole[i] = INT_MAX;
			whichContainer[i] = INT_MAX;
			whichFrame[i] = INT_MAX;
		}
		isPlayer1 = true;
	}
	int remainingTime;
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

class TotalManager
{
public:
	TotalManager();
	~TotalManager();

	static void gameLogicThread();
	static void clntProcessingThread(int threadID);
	//�� �Լ����� ���߿� ������ ������.
	static std::vector<std::thread> threads;
	static std::atomic<bool> isGameOver;
	//�� �����忡�� ���� ����� deadTime.
private:
	static SOCKET listenSock;
	static SOCKET clntSock[2];
	static std::atomic<bool> isLogicThreadCompleted;
	static std::atomic<bool> recvPacketFromClnt[2];
	static GameState* gs;
	static ClientToServer* ctos[2];
	static int portNumber;
	static std::chrono::high_resolution_clock::time_point startT;
	static std::chrono::high_resolution_clock::time_point endT;
	static float elapsedTime;
	static float accTime;
	//�� ������
};