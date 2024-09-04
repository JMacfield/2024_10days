#pragma once

#include <YNet/IncludeMe.h>

using namespace YNet;

class Server {
public:
	bool Initialize(IPEndpoint ip);
	void Frame();

private:
	Socket listeningSocket;
	std::vector<TCPConnection> connections;
	std::vector<WSAPOLLFD> master_fd;
};