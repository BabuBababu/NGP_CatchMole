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
	std::chrono::high_resolution_clock::time_point clikedTime;
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
	bool isSpawned[9];
	//각 구녕별로 스폰이 되었는지 확인한다
	int whichMole[9];
	//스폰이 되었다면 어떤 두더지 인지 확인한다
	int whichContainer[9];
	//어떤 두더지인지 확인되었다면 0,1번 그림 컨테이너 중 무엇을 고를것인가
	int whichFrame[9];
	//여기까지 됐으면 몇 번 프레임을 그려야 하는가
}typedef GameState;
#pragma pack()

class TotalManager
{
public:
	TotalManager();
	~TotalManager();

	static void gameLogicThread();
	static void clntProcessingThread(int threadID);
	//이 함수들은 나중에 구현할 예정임.
	static std::vector<std::thread> threads;
	static std::atomic<bool> isGameOver;
	//두 쓰레드에서 각기 사용할 deadTime.
private:
	static SOCKET listenSock;
	static SOCKET clntSock[2];
	static std::atomic<bool> isLogicThreadCompleted;
	static std::atomic<bool> recvPacketFromClnt[2];
	static GameState* gs;
	static ClientToServer* ctos;
	static int portNumber;

};