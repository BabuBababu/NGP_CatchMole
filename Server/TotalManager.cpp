#include "TotalManager.h"
#include <iostream>

int TotalManager::portNumber = 8000;

TotalManager::TotalManager()
{
	threads.resize(2);
	isLogicThreadCompleted = false;
	recvPacketFromClnt[0] = false;
	recvPacketFromClnt[1] = false;
	ctos = new ClientToServer();
	gs = new GameState();

	//�� ������ �ʱ� GameState ������ �ʱ�ȭ���ش�.
	//gs->p1Point = 0;
	//gs->p2Point = 0;
	//...

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
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind(listenSock, (SOCKADDR*)(&servAddr), sizeof(SOCKADDR_IN)) == SOCKET_ERROR)
		std::cout << "listen socket bind failed" << std::endl;
	
	if (listen(listenSock, SOMAXCONN) == SOCKET_ERROR)
		std::cout << "listen socket listen failed" << std::endl;

	int connectedClientNum = 0;
	while (true)
	{
		if (connectedClientNum > 1)
			break;
		SOCKADDR_IN clntAddr;
		int clntAddrSize;
		ZeroMemory(&clntAddr, sizeof(clntAddr));
		clntSock[connectedClientNum] = accept(listenSock, (SOCKADDR*)&clntAddr, &clntAddrSize);
		if (clntSock[connectedClientNum] == INVALID_SOCKET)
			std::cout << "client connection failed" << std::endl;
		threads.emplace_back(TotalManager::clntProcessingThread, connectedClientNum);
		threads[connectedClientNum].join();
		++connectedClientNum;
	}
	//������� ���� Ŭ���̾�Ʈ 2���� ���� ������ �� ���� ���̴�.
	//send(clntSock[0], (char*)gs, sizeof(GameState), 0);
	//send(clntSock[1], (char*)gs, sizeof(GameState), 0);
	//������ �޾����� �� Ŭ���̾�Ʈ���� �ʱ� GameState ������ �����ش�.

	//���ӷ���������� �ڿ������� ���ξ����忡�� ����ȴ�.
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

bool TotalManager::gameLogicThread()
{
	
}

void TotalManager::clntProcessingThread(int threadID)
{

}
