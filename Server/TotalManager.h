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
	int clikedHole;
	std::chrono::steady_clock::time_point clikedTime;
}typedef ClientToServer;
#pragma pack()

#pragma pack(1)
struct GameState
{
	int remainingTime;
	int p1Point;
	int p2Point;
	int p1HammerFrame;
	int p2HammerFrame;
	std::pair<int, float> molespos[9];
}typedef GameState;
#pragma pack()

class TotalManager
{
public:
	TotalManager();
	~TotalManager();

	static bool gameLogicThread();
	static void clntProcessingThread(int threadID);
	//이 함수들은 나중에 구현할 예정임.
private:
	SOCKET listenSock;
	SOCKET clntSock[2];
	std::vector<std::thread> threads;
	std::atomic<bool> isLogicThreadCompleted;
	std::atomic<bool> recvPacketFromClnt[2];
	GameState* gs;
	ClientToServer* ctos;
	static int portNumber;
};