#include "YNet/Network.h"

#include <iostream>

bool YNet::Network::Initialize() {
	WSADATA wsadata;

	int result = WSAStartup(MAKEWORD(2, 2), &wsadata);

	// WinSock APIがスタートアップしなかった場合
	if (result != 0) { 
		std::cerr << "Failed to start up the winsock API." << std::endl;
		
		return false;
	}

	// 受信したWinSockバージョンがリクエストと一致しない場合
	if (LOBYTE(wsadata.wVersion) != 2 || HIBYTE(wsadata.wVersion) != 2) {
		std::cerr << "Could not find a usable version of the winsock api dll." << std::endl;

		return false;
	}

	return true;
}

void YNet::Network::Shutdown() {
	WSACleanup();
}