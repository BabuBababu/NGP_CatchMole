#include "TotalManager.h"
#include <algorithm>
#include <random>
#include <cmath>

int TotalManager::portNumber = 8000;
const int acceptableNum = 2;

SOCKET TotalManager::listenSock;
SOCKET TotalManager::clntSock[2];
std::atomic<bool> TotalManager::isLogicThreadCompleted;
std::atomic<bool> TotalManager::recvPacketFromClnt[2];
GameState* TotalManager::gs;
ClientToServer* TotalManager::ctos[2];
std::atomic<bool> TotalManager::isGameOver = false;
std::vector<std::thread> TotalManager::threads;
std::chrono::high_resolution_clock::time_point TotalManager::startT;
std::chrono::high_resolution_clock::time_point TotalManager::endT;
float TotalManager::elapsedTime = 0.0f;
float TotalManager::accTime = 0.0f;

std::random_device rd;
std::default_random_engine dre(rd());
std::uniform_int_distribution<int> uid(0, 2);

void err_display(const char* msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&lpMsgBuf, 0, NULL);
	std::cout << msg << "\t" << (char*)lpMsgBuf;
	LocalFree(lpMsgBuf);
}

TotalManager::TotalManager()
{
	threads.reserve(2);
	isLogicThreadCompleted = false;
	recvPacketFromClnt[0] = false;
	recvPacketFromClnt[1] = false;
	ctos[0] = new ClientToServer();
	ctos[1] = new ClientToServer();
	gs = new GameState();

	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		std::cout << "WSA Startup error" << std::endl;

	listenSock = socket(AF_INET, SOCK_STREAM, 0);
	if (listenSock == INVALID_SOCKET)
		std::cout << "socket creation failed" << std::endl;

	SOCKADDR_IN servAddr;
	ZeroMemory(&servAddr, sizeof(SOCKADDR_IN));
	servAddr.sin_family = AF_INET;
	servAddr.sin_port = htons(portNumber);
	servAddr.sin_addr.s_addr = htonl(ADDR_ANY);

	if (bind(listenSock, (SOCKADDR*)(&servAddr), sizeof(SOCKADDR_IN)) == SOCKET_ERROR)
		std::cout << "listen socket bind failed" << std::endl;

	if (listen(listenSock, SOMAXCONN) == SOCKET_ERROR)
		std::cout << "listen socket listen failed" << std::endl;

	int connectedClientNum = 0;
	while (true)
	{
		if (connectedClientNum > acceptableNum - 1)
			break;
		SOCKADDR_IN clntAddr;
		int clntAddrSize = sizeof(SOCKADDR_IN);
		ZeroMemory(&clntAddr, sizeof(clntAddr));
		clntSock[connectedClientNum] = accept(listenSock, (SOCKADDR*)&clntAddr, &clntAddrSize);

		if (clntSock[connectedClientNum] == INVALID_SOCKET)
			std::cout << "Connection error" << std::endl;


		++connectedClientNum;
	}
	//여기까지 오면 클라이언트 2개로 부터 연결을 다 받은 것이다.

	for (int i = 0; i < acceptableNum; ++i)
		threads.emplace_back(TotalManager::clntProcessingThread, i);


	threads.emplace_back(TotalManager::gameLogicThread);

	for (int i = 0; i < acceptableNum; ++i)
	{
		if (i == 1)
			gs->isPlayer1 = false;
		//2번 플레이어는 플레이어1이 아니니까 false 로 넘겨준다.
		send(clntSock[i], (char*)gs, sizeof(GameState), 0);
	}
}

TotalManager::~TotalManager()
{
	delete gs;
	delete ctos[0];
	delete ctos[1];
	closesocket(clntSock[0]);
	closesocket(clntSock[1]);
	closesocket(listenSock);
	WSACleanup();
}

void TotalManager::gameLogicThread()
{
	int currentMole = 0;
	float curMoleSpawnTime = -1.0f;
	//현재 두더지 스폰 시간이다. 음수이면 소환한다.(아래 또 나옴)
	float maxMoleSpawnTime = 0.5f;
	//두더지가 0.5초에 한 번씩 나온다.(조건이 맞다면)
	bool whichMoleAndContainer[3][2] = { {false,false}, {false,false},{false,false} };
	//예를 들어 위 배열의 [0][0]이 false 라는 건 드론의 1번 이미지 컨테이너가 사용가능하단 소리이다.
	//[1][1]이 false 이면 히드라의 2번 이미지 컨테이너가 사용가능하단 소리다.
	//[2][0]이 false 이면 저글링의 1번 이미지 컨테이너가 사용가능하단 소리다.

	const float TIME_PER_ACTION = 0.5f;
	const float ACTION_PER_TIME = 1.0f / TIME_PER_ACTION;
	const int FRAMES_PER_ACTION = 3;

	float moleFrames[9];
	ZeroMemory(moleFrames, sizeof(float) * 9);
	bool isBackAnim[9];
	ZeroMemory(isBackAnim, sizeof(bool) * 9);

	while (1)
	{
		startT = std::chrono::high_resolution_clock::now();
		while (1)
		{
			if (recvPacketFromClnt[0]) break;// && recvPacketFromClnt[1]) break;
		}

		//###################################################################################################
		int p1ClickedHole = p1ClickedHole = ctos[0]->clikedHole;
		int p2ClickedHole = p2ClickedHole = ctos[1]->clikedHole;

		if (p1ClickedHole != INT_MAX && (p1ClickedHole == p2ClickedHole) && gs->isSpawned[p1ClickedHole])
		{
			if (ctos[0]->clikedTime < ctos[1]->clikedTime)
				gs->p1Point += 1;
			else
				gs->p2Point += 1;

			gs->isSpawned[p1ClickedHole] = false;
			whichMoleAndContainer[gs->whichMole[p1ClickedHole]][gs->whichFrame[p1ClickedHole]] = false;
			gs->whichMole[p1ClickedHole] = INT_MAX;
			gs->whichFrame[p1ClickedHole] = 0;
			currentMole -= 1;
			moleFrames[p1ClickedHole] = 0;
			isBackAnim[p1ClickedHole] = false;
			std::cout << "mole die" << std::endl;
		}
		else
		{
			if (p1ClickedHole != INT_MAX && gs->isSpawned[p1ClickedHole])
			{
				//std::cout << "mole die at" << p1ClickedHole << " and mole sp: " << gs->whichMole[p1ClickedHole] << " and cont: " << gs->whichContainer[p1ClickedHole] << std::endl;
				gs->p1Point += 1;
				gs->isSpawned[p1ClickedHole] = false;
				whichMoleAndContainer[gs->whichMole[p1ClickedHole]][gs->whichContainer[p1ClickedHole]] = false;
				gs->whichMole[p1ClickedHole] = INT_MAX;
				gs->whichFrame[p1ClickedHole] = 0;
				gs->whichContainer[p1ClickedHole] = INT_MAX;
				currentMole -= 1;
				moleFrames[p1ClickedHole] = 0;
				isBackAnim[p1ClickedHole] = false;
			}
			if (p2ClickedHole != INT_MAX && gs->isSpawned[p2ClickedHole])
			{
				//std::cout << "mole die at" << p1ClickedHole << " and mole sp: " << gs->whichMole[p1ClickedHole] << " and cont: " << gs->whichContainer[p1ClickedHole] << std::endl;
				gs->p2Point += 1;
				gs->isSpawned[p2ClickedHole] = false;
				whichMoleAndContainer[gs->whichMole[p2ClickedHole]][gs->whichContainer[p2ClickedHole]] = false;
				gs->whichMole[p2ClickedHole] = INT_MAX;
				gs->whichFrame[p2ClickedHole] = 0;
				currentMole -= 1;
				moleFrames[p1ClickedHole] = 0;
				isBackAnim[p1ClickedHole] = false;
			}
		}
		//만약 p1과 p2가 INT_MAX가 아니면서 동일한 구멍을 골랐다면 누가 더 빨리 눌럿나확인하고 점수를 준다.
		//그렇지 않다면 각자 누른 구멍에 따라 점수를 준다.
		//이때 주의할 점은 두더지가 스폰이 됐는지 안됐는지를 확인해야한다는 점이다.
		//###################################################################################################
		//여기까지 유저가 보낸 패킷을 기반으로 두더지를 잡는 로직이다.

		//###################################################################################################
		//이곳에 애니메이션을 진행하는 코드를 삽입한다.
		for (int i = 0; i < 9; ++i)
		{
			if (gs->isSpawned[i])
			{
				//i번째 구멍에 두더지가 스폰된 상태라면 애니메이션을 진행시킨다.
				if (!isBackAnim[i])
				{
					moleFrames[i] += (FRAMES_PER_ACTION * ACTION_PER_TIME * elapsedTime);
					if (moleFrames[i] >= 2.4)
					{
						isBackAnim[i] = true;
						moleFrames[i] = 2.0f;
					}
					gs->whichFrame[i] = int(floorf(moleFrames[i]));
				}
				else
				{
					moleFrames[i] -= (FRAMES_PER_ACTION * ACTION_PER_TIME * elapsedTime);
					if (int(moleFrames[i]) <= 0)
					{
						moleFrames[i] = 0.0f;
						gs->isSpawned[i] = false;
						whichMoleAndContainer[gs->whichMole[i]][gs->whichContainer[i]] = false;
						gs->whichMole[i] = INT_MAX;
						gs->whichFrame[i] = 0;
						gs->whichContainer[i] = INT_MAX;
						currentMole -= 1;
						isBackAnim[i] = false;
						moleFrames[i] = 0;
					}
					gs->whichFrame[i] = int(floorf(moleFrames[i]));
				}
			}
		}
		//###################################################################################################
		//소환된 두더지의 애니메이션을 진행시킨다.

		//###################################################################################################
		if (currentMole < 6 && curMoleSpawnTime < 0.0f)
		{
			std::vector<int>candidates;
			candidates.reserve(9);
			curMoleSpawnTime = maxMoleSpawnTime;
			for (int i = 0; i < 9; ++i)
			{
				if (!gs->isSpawned[i])
					candidates.emplace_back(i);
			}
			//후보군을 뽑는다.(스폰이 안된 구멍이 후보군이다.)
			std::random_shuffle(candidates.begin(), candidates.end());
			int selectedHole;
			selectedHole = candidates[0];
			//랜덤으로 섞인 후보군에서 앞에 2개를 뽑아 해당 구멍에 두더지를 소환할 것이다.

			int spawnedNum = 0;

			int pickedMole = uid(dre);

			bool isSpawned = false;
			if (!whichMoleAndContainer[pickedMole][0])
			{
				gs->isSpawned[selectedHole] = true;
				gs->whichMole[selectedHole] = pickedMole;
				gs->whichContainer[selectedHole] = 0;
				gs->whichFrame[selectedHole] = 0;
				//스폰이 됐으니 해당 구멍 스폰 true!
				//그리고 pickedMole이 해당 구멍에 스폰되는 두더지!
				//그리고 이미지 컨테이너는 0이된다.
				//애니메이션 프레임은 0으로 설정한다!
				whichMoleAndContainer[pickedMole][0] = true;
				//해당 이미지 컨테이너는 사용되니 true로 만들어준다.
				spawnedNum += 1;
				isSpawned = true;
			}
			else if (!whichMoleAndContainer[pickedMole][1])
			{
				gs->isSpawned[selectedHole] = true;
				gs->whichMole[selectedHole] = pickedMole;
				gs->whichContainer[selectedHole] = 1;
				gs->whichFrame[selectedHole] = 0;
				//스폰이 됐으니 해당 구멍 스폰 true!
				//그리고 pickedMole이 해당 구멍에 스폰되는 두더지!
				//그리고 이미지 컨테이너는 1이된다.
				//애니메이션 프레임은 0으로 설정한다!
				whichMoleAndContainer[pickedMole][1] = true;
				//해당 이미지 컨테이너는 사용되니 true로 만들어준다.
				spawnedNum += 1;
				isSpawned = true;
			}
			//if (spawnedNum == 1)break;
			//두 마리 다 소환됐으면 소환을 마친다.

			if (isSpawned)
			{
				currentMole += 1;
				/*std::cout << "Mole Spawned one at: " << selectedHole << "and mole species is: " << gs->whichMole[selectedHole] <<" and cont: "<<gs->whichContainer[selectedHole] <<std::endl;*/
			}
		}
		//현재 두더지의 숫자가 6마리 미만이고 두더지 소환 시간이 됐다면 소환한다.
		//###################################################################################################
		//이곳에서 두더지를 소환한다.

		//Sleep(50);

		//게임로직처리대신 우선 슬립을 넣었음 슬립 부분을 게임로직으로 대체할것임.
		/*auto delay = 1000.0f/600.0f - std::chrono::duration<float, std::milli>((startT - std::chrono::high_resolution_clock::now())).count();
		if(delay>0)
			Sleep(delay);*/
		endT = std::chrono::high_resolution_clock::now();
		std::chrono::duration<float, std::milli> dur = endT - startT;
		TotalManager::elapsedTime = dur.count() / 1000.0f;
		TotalManager::accTime += TotalManager::elapsedTime;
		curMoleSpawnTime -= TotalManager::elapsedTime;
		std::cout << 1.0f / elapsedTime << std::endl;
		//float delay = 1.0f / 10.0f - elapsedTime;
		//std::cout << delay*1000 << std::endl;

		/*if (delay > 0.0f)
		{
			Sleep(delay * 1000.0f);
		}*/
		/*std::cout << 1.0f / (TotalManager::elapsedTime + delay * 0.1f) << std::endl;*/
		//###################################################################################################
		if (accTime >= 1.0f)
		{
			gs->remainingTime -= 1;
			accTime = 0.0f;
		}
		//###################################################################################################
		//게임시간을 감소시킨다.
		isLogicThreadCompleted = true;
		if (isGameOver)
		{
			std::cout << "gameLogicThreadOver" << std::endl;
			return;
		}
	}
}

void TotalManager::clntProcessingThread(int threadID)
{
	std::cout << "ThreadID is:" << threadID << std::endl;
	while (1)
	{
		if (isGameOver)return;
		int temp = 0;
		if (recv(clntSock[threadID], (char*)ctos[threadID], sizeof(ClientToServer), 0) <= 0)
			isGameOver = true;
		//std::cout << "recv from clnt" << std::endl;
		//클라이언트에게 데이터를 받고
		recvPacketFromClnt[threadID] = true;
		//내가 패킷을 받았다는 플래그를 true로 만든다.
		while (!isLogicThreadCompleted);
		//로직쓰레드가 완료됐다는 플래그가 될때까지 대기한다.
		recvPacketFromClnt[threadID] = false;
		while (true)
		{
			if (!recvPacketFromClnt[0]) break;//&& !recvPacketFromClnt[1]) break;
		}
		if (!isGameOver)
			send(clntSock[threadID], (char*)gs, sizeof(GameState), 0);
		isLogicThreadCompleted = false;

		//std::cout << "send to clnt" << std::endl;
		//로직 쓰레드가 완료되었다면 대기상태에서 풀려나 클라이언트를 위한 서비스를 재개한다.
		//startT = std::chrono::high_resolution_clock::now();
		if (isGameOver)return;
		//클라이언트 한 명만 종료되도 종료가된다.
	}
}