#include "TotalManager.h"

int TotalManager::portNumber = 8000;
const int acceptableNum = 1;
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
	servAddr.sin_addr.s_addr = htonl(ADDR_ANY);
	
	if (bind(listenSock, (SOCKADDR*)(&servAddr), sizeof(SOCKADDR_IN)) == SOCKET_ERROR)
		std::cout << "listen socket bind failed" << std::endl;
	
	if (listen(listenSock, SOMAXCONN) == SOCKET_ERROR)
		std::cout << "listen socket listen failed" << std::endl;

	int connectedClientNum = 0;
	while (true)
	{
		if (connectedClientNum > acceptableNum-1)
			break;
		SOCKADDR_IN clntAddr;
		int clntAddrSize=sizeof(SOCKADDR_IN);
		ZeroMemory(&clntAddr, sizeof(clntAddr));
		clntSock[connectedClientNum] = accept(listenSock, (SOCKADDR*)&clntAddr, &clntAddrSize);

		if (clntSock[connectedClientNum] == INVALID_SOCKET)
			std::cout << "Connection error" << std::endl;
		
		++connectedClientNum;
	}
	//������� ���� Ŭ���̾�Ʈ 2���� ���� ������ �� ���� ���̴�.

	for (int i = 0; i < acceptableNum; ++i)
	{
		threads.emplace_back(TotalManager::clntProcessingThread, i);
		threads[i].join();
	}

	gs->p1Point = 100;
	gs->p2Point = 100;
	gs->remainingTime = 30;

	for (int i = 0; i < acceptableNum; ++i)
	{
		send(clntSock[i], (char*)gs, sizeof(GameState), 0);
	}
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
	return true;
}

void TotalManager::clntProcessingThread(int threadID)
{
	std::cout << "ThreadID is:" << threadID << std::endl;
	/*SOCKADDR_IN addr;
	int len;
	getpeername(clntSock[0], (SOCKADDR*)&addr, &len);
	std::cout << "client from: " << inet_ntoa(addr.sin_addr) << "port num: " << ntohs(addr.sin_port) << std::endl;*/
}
