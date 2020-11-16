#include "TotalManager.h"

int TotalManager::portNumber = 8000;
const int acceptableNum = 1;

SOCKET TotalManager::listenSock;
SOCKET TotalManager::clntSock[2];
std::atomic<bool> TotalManager::isLogicThreadCompleted;
std::atomic<bool> TotalManager::recvPacketFromClnt[2];
GameState* TotalManager::gs;
ClientToServer* TotalManager::ctos;
std::atomic<bool> TotalManager::isGameOver = false;
std::vector<std::thread> TotalManager::threads;


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
	ctos = new ClientToServer();
	gs = new GameState();


	gs->p1Point = 100;
	gs->p2Point = 100;
	gs->remainingTime = 30;

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

	for(int i=0;i<acceptableNum;++i)
		send(clntSock[i], (char*)gs, sizeof(GameState), 0);
}

TotalManager::~TotalManager()
{
	delete gs;
	delete ctos;
	closesocket(clntSock[0]);
	closesocket(clntSock[1]);
	closesocket(listenSock);
	WSACleanup();
}

void TotalManager::gameLogicThread()
{
	while (1)
	{
		while (!recvPacketFromClnt[0] && !recvPacketFromClnt[1]);
		std::cout << "finishing" << std::endl;
		Sleep(100);
		//���ӷ���ó����� �켱 ������ �־��� ���� �κ��� ���ӷ������� ��ü�Ұ���.
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
		auto start = std::chrono::high_resolution_clock::now();
		if (isGameOver)return;
		int temp = 0;
		if (recv(clntSock[threadID], (char*)ctos, sizeof(ClientToServer), 0) <= 0)
			isGameOver = true;
		std::cout << "recv from clnt" << std::endl;
		//�������� �����͸� �ް�
		recvPacketFromClnt[threadID] = true;
		//���� ��Ŷ�� �޾Ҵٴ� �÷��׸� true�� �����.
		while (!isLogicThreadCompleted);
		//���������尡 �Ϸ�ƴٴ� �÷��װ� �ɶ����� ����Ѵ�.
		recvPacketFromClnt[threadID] = false;
		if (!recvPacketFromClnt[0] && !recvPacketFromClnt[1])
			isLogicThreadCompleted = false;
		if(!isGameOver)
			send(clntSock[threadID], (char*)gs, sizeof(GameState), 0);
		std::cout << "send to clnt" << std::endl;
		//���� �����尡 �Ϸ�Ǿ��ٸ� �����¿��� Ǯ���� Ŭ���̾�Ʈ�� ���� ���񽺸� �簳�Ѵ�.
		if (isGameOver)return;
		//Ŭ���̾�Ʈ �� �� ����ǵ� ���ᰡ�ȴ�.
		auto end = std::chrono::high_resolution_clock::now();
		std::chrono::duration<float> fsec;
		fsec = end - start;
		/*TotalManager::deadTime[threadID] += fsec.count();
		if (deadTime[threadID] > 1.5f)
			isGameOver = true;*/
	}
}