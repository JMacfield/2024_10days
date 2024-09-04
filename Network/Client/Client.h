#pragma once

#include <YNet/IncludeMe.h>

using namespace YNet;

class Client {
public:
	bool Connect(IPEndpoint ip);
	bool IsConnected();
	bool Frame();

private:
	bool isConnected = false;
	Socket socket;
};