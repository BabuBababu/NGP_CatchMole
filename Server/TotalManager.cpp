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
	//������� ���� Ŭ���̾�Ʈ 2���� ���� ������ �� ���� ���̴�.

	for (int i = 0; i < acceptableNum; ++i)
		threads.emplace_back(TotalManager::clntProcessingThread, i);


	threads.emplace_back(TotalManager::gameLogicThread);

	for (int i = 0; i < acceptableNum; ++i)
	{
		if (i == 1)
			gs->isPlayer1 = false;
		//2�� �÷��̾�� �÷��̾�1�� �ƴϴϱ� false �� �Ѱ��ش�.
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
	//���� �δ��� ���� �ð��̴�. �����̸� ��ȯ�Ѵ�.(�Ʒ� �� ����)
	float maxMoleSpawnTime = 0.5f;
	//�δ����� 0.5�ʿ� �� ���� ���´�.(������ �´ٸ�)
	bool whichMoleAndContainer[3][2] = { {false,false}, {false,false},{false,false} };
	//���� ��� �� �迭�� [0][0]�� false ��� �� ����� 1�� �̹��� �����̳ʰ� ��밡���ϴ� �Ҹ��̴�.
	//[1][1]�� false �̸� ������� 2�� �̹��� �����̳ʰ� ��밡���ϴ� �Ҹ���.
	//[2][0]�� false �̸� ���۸��� 1�� �̹��� �����̳ʰ� ��밡���ϴ� �Ҹ���.

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
		//���� p1�� p2�� INT_MAX�� �ƴϸ鼭 ������ ������ ����ٸ� ���� �� ���� ������Ȯ���ϰ� ������ �ش�.
		//�׷��� �ʴٸ� ���� ���� ���ۿ� ���� ������ �ش�.
		//�̶� ������ ���� �δ����� ������ �ƴ��� �ȵƴ����� Ȯ���ؾ��Ѵٴ� ���̴�.
		//###################################################################################################
		//������� ������ ���� ��Ŷ�� ������� �δ����� ��� �����̴�.

		//###################################################################################################
		//�̰��� �ִϸ��̼��� �����ϴ� �ڵ带 �����Ѵ�.
		for (int i = 0; i < 9; ++i)
		{
			if (gs->isSpawned[i])
			{
				//i��° ���ۿ� �δ����� ������ ���¶�� �ִϸ��̼��� �����Ų��.
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
		//��ȯ�� �δ����� �ִϸ��̼��� �����Ų��.

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
			//�ĺ����� �̴´�.(������ �ȵ� ������ �ĺ����̴�.)
			std::random_shuffle(candidates.begin(), candidates.end());
			int selectedHole;
			selectedHole = candidates[0];
			//�������� ���� �ĺ������� �տ� 2���� �̾� �ش� ���ۿ� �δ����� ��ȯ�� ���̴�.

			int spawnedNum = 0;

			int pickedMole = uid(dre);

			bool isSpawned = false;
			if (!whichMoleAndContainer[pickedMole][0])
			{
				gs->isSpawned[selectedHole] = true;
				gs->whichMole[selectedHole] = pickedMole;
				gs->whichContainer[selectedHole] = 0;
				gs->whichFrame[selectedHole] = 0;
				//������ ������ �ش� ���� ���� true!
				//�׸��� pickedMole�� �ش� ���ۿ� �����Ǵ� �δ���!
				//�׸��� �̹��� �����̳ʴ� 0�̵ȴ�.
				//�ִϸ��̼� �������� 0���� �����Ѵ�!
				whichMoleAndContainer[pickedMole][0] = true;
				//�ش� �̹��� �����̳ʴ� ���Ǵ� true�� ������ش�.
				spawnedNum += 1;
				isSpawned = true;
			}
			else if (!whichMoleAndContainer[pickedMole][1])
			{
				gs->isSpawned[selectedHole] = true;
				gs->whichMole[selectedHole] = pickedMole;
				gs->whichContainer[selectedHole] = 1;
				gs->whichFrame[selectedHole] = 0;
				//������ ������ �ش� ���� ���� true!
				//�׸��� pickedMole�� �ش� ���ۿ� �����Ǵ� �δ���!
				//�׸��� �̹��� �����̳ʴ� 1�̵ȴ�.
				//�ִϸ��̼� �������� 0���� �����Ѵ�!
				whichMoleAndContainer[pickedMole][1] = true;
				//�ش� �̹��� �����̳ʴ� ���Ǵ� true�� ������ش�.
				spawnedNum += 1;
				isSpawned = true;
			}
			//if (spawnedNum == 1)break;
			//�� ���� �� ��ȯ������ ��ȯ�� ��ģ��.

			if (isSpawned)
			{
				currentMole += 1;
				/*std::cout << "Mole Spawned one at: " << selectedHole << "and mole species is: " << gs->whichMole[selectedHole] <<" and cont: "<<gs->whichContainer[selectedHole] <<std::endl;*/
			}
		}
		//���� �δ����� ���ڰ� 6���� �̸��̰� �δ��� ��ȯ �ð��� �ƴٸ� ��ȯ�Ѵ�.
		//###################################################################################################
		//�̰����� �δ����� ��ȯ�Ѵ�.

		//Sleep(50);

		//���ӷ���ó����� �켱 ������ �־��� ���� �κ��� ���ӷ������� ��ü�Ұ���.
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
		//���ӽð��� ���ҽ�Ų��.
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
		//Ŭ���̾�Ʈ���� �����͸� �ް�
		recvPacketFromClnt[threadID] = true;
		//���� ��Ŷ�� �޾Ҵٴ� �÷��׸� true�� �����.
		while (!isLogicThreadCompleted);
		//���������尡 �Ϸ�ƴٴ� �÷��װ� �ɶ����� ����Ѵ�.
		recvPacketFromClnt[threadID] = false;
		while (true)
		{
			if (!recvPacketFromClnt[0]) break;//&& !recvPacketFromClnt[1]) break;
		}
		if (!isGameOver)
			send(clntSock[threadID], (char*)gs, sizeof(GameState), 0);
		isLogicThreadCompleted = false;

		//std::cout << "send to clnt" << std::endl;
		//���� �����尡 �Ϸ�Ǿ��ٸ� �����¿��� Ǯ���� Ŭ���̾�Ʈ�� ���� ���񽺸� �簳�Ѵ�.
		//startT = std::chrono::high_resolution_clock::now();
		if (isGameOver)return;
		//Ŭ���̾�Ʈ �� �� ����ǵ� ���ᰡ�ȴ�.
	}
}