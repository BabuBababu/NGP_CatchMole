#include "Communicator.h"
#include <iostream>

Communicator::Communicator()
{
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2),&wsaData) != 0)
	{
		std::cout << "Winsock init startup error" << std::endl;
		exit(1);
	}
}

Communicator::~Communicator()
{
	closesocket(clntSock);
	WSACleanup();
}

void Communicator::sendData(ClientToServer* obj)
{
	send(clntSock, (char*)obj, sizeof(ClientToServer), 0);
}

bool Communicator::recvData(GameState* obj)
{
	if (recv(clntSock, (char*)obj, sizeof(GameState), 0) == 0)
		return false;
	return true;
}

bool Communicator::connectToServ(const std::string& servAddr, const std::string& portNum)
{
	clntSock = socket(PF_INET, SOCK_STREAM, 0);
	SOCKADDR_IN addr;
	ZeroMemory(&addr, sizeof(addr));
	addr.sin_family = PF_INET;
	addr.sin_addr.s_addr = inet_addr(servAddr.c_str());
	addr.sin_port = htons(std::stoi(portNum));
	
	if (connect(clntSock, (SOCKADDR*)&addr, sizeof(addr)) == SOCKET_ERROR)
	{
		std::cout << "Connection Error" << std::endl;
		return false;
	}
	return true;
}

bool Communicator::recvInitialGameState(GameState* gs)
{
	if (recv(clntSock, (char*)gs, sizeof(GameState), 0) > 0) 
		return true;
	return false;
}
