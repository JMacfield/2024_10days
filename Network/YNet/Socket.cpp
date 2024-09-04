#include "Socket.h"

#include <assert.h>
#include <iostream>

namespace YNet {
	Socket::Socket(IPVersion ipversion, SocketHandle handle) :ipversion(ipversion), handle(handle) {
		assert(ipversion == IPVersion::IPv4 || ipversion == IPVersion::IPv6);
	}

	YResult Socket::Create() {
		assert(ipversion == IPVersion::IPv4 || ipversion == IPVersion::IPv6);

		if (handle != INVALID_SOCKET) {
			return YResult::Y_GenericError;
		}

		handle = socket((ipversion == IPVersion::IPv4) ? AF_INET : AF_INET6, SOCK_STREAM, IPPROTO_TCP); //ソケット作成を試みる

		if (handle == INVALID_SOCKET) {
			int error = WSAGetLastError();

			return YResult::Y_GenericError;
		}

		if (SetBlocking(false) != YResult::Y_Success) {
			return YResult::Y_GenericError;
		}

		if (SetSocketOption(SocketOption::TCP_NoDelay, TRUE) != YResult::Y_Success) {
			return YResult::Y_GenericError;
		}

		return YResult::Y_Success;
	}

	YResult Socket::Close() {
		if (handle == INVALID_SOCKET) {
			return YResult::Y_GenericError;
		}

		int result = closesocket(handle);

		if (result != 0) { // ソケットクローズ中にエラーが発生したら
			int error = WSAGetLastError();

			return YResult::Y_GenericError;
		}

		handle = INVALID_SOCKET;

		return YResult::Y_Success;
	}

	YResult Socket::Bind(IPEndpoint endpoint) {
		assert(ipversion == endpoint.GetIPVersion());

		if (ipversion == IPVersion::IPv4) {
			sockaddr_in addr = endpoint.GetSockaddrIPv4();
			int result = bind(handle, (sockaddr*)(&addr), sizeof(sockaddr_in));
			if (result != 0) { // なんらかのエラーが発生したら
				int error = WSAGetLastError();

				return YResult::Y_GenericError;
			}
		} else { //IPv6
			sockaddr_in6 addr = endpoint.GetSockaddrIPv6();
			int result = bind(handle, (sockaddr*)(&addr), sizeof(sockaddr_in6));

			if (result != 0) { //if an error occurred 
				int error = WSAGetLastError();

				return YResult::Y_GenericError;
			}
		}

		return YResult::Y_Success;
	}

	YResult Socket::Listen(IPEndpoint endpoint, int backlog) {
		if (ipversion == IPVersion::IPv6) {
			if (SetSocketOption(SocketOption::IPV6_Only, FALSE) != YResult::Y_Success) {
				return YResult::Y_GenericError;
			}
		}

		if (Bind(endpoint) != YResult::Y_Success) {
			return YResult::Y_GenericError;
		}

		int result = listen(handle, backlog);

		if (result != 0) { // なんらかのエラーが発生したら
			int error = WSAGetLastError();
			return YResult::Y_GenericError;
		}

		return YResult::Y_Success;
	}

	YResult Socket::Accept(Socket& outSocket, IPEndpoint* endpoint) {
		assert(ipversion == IPVersion::IPv4 || ipversion == IPVersion::IPv6);

		if (ipversion == IPVersion::IPv4) {
			sockaddr_in addr = {};
			int len = sizeof(sockaddr_in);

			SocketHandle acceptedConnectionHandle = accept(handle, (sockaddr*)(&addr), &len);

			if (acceptedConnectionHandle == INVALID_SOCKET) {
				int error = WSAGetLastError();

				return YResult::Y_GenericError;
			}

			if (endpoint != nullptr) {
				*endpoint = IPEndpoint((sockaddr*)&addr);
			}

			outSocket = Socket(IPVersion::IPv4, acceptedConnectionHandle);
		} else { // IPv6
			sockaddr_in6 addr = {};
			int len = sizeof(sockaddr_in6);
			
			SocketHandle acceptedConnectionHandle = accept(handle, (sockaddr*)(&addr), &len);
			
			if (acceptedConnectionHandle == INVALID_SOCKET) {
				int error = WSAGetLastError();

				return YResult::Y_GenericError;
			}

			if (endpoint != nullptr) {
				*endpoint = IPEndpoint((sockaddr*)&addr);
			}

			outSocket = Socket(IPVersion::IPv6, acceptedConnectionHandle);
		}

		return YResult::Y_Success;
	}

	YResult Socket::Connect(IPEndpoint endpoint) {
		assert(ipversion == endpoint.GetIPVersion());

		int result = 0;

		if (ipversion == IPVersion::IPv4) {
			sockaddr_in addr = endpoint.GetSockaddrIPv4();
			result = connect(handle, (sockaddr*)(&addr), sizeof(sockaddr_in));
		} else { //IPv6
			sockaddr_in6 addr = endpoint.GetSockaddrIPv6();
			result = connect(handle, (sockaddr*)(&addr), sizeof(sockaddr_in6));
		}

		if (result != 0) { // なんらかのエラーが発生したら
			int error = WSAGetLastError();

			return YResult::Y_GenericError;
		}

		return YResult::Y_Success;
	}

	YResult Socket::Send(const void* data, int numberOfBytes, int& bytesSent) {
		bytesSent = send(handle, (const char*)data, numberOfBytes, NULL);

		if (bytesSent == SOCKET_ERROR) {
			int error = WSAGetLastError();

			return YResult::Y_GenericError;
		}

		return YResult::Y_Success;
	}

	YResult Socket::Recv(void* destination, int numberOfBytes, int& bytesReceived) {
		bytesReceived = recv(handle, (char*)destination, numberOfBytes, NULL);

		if (bytesReceived == 0) {
			return YResult::Y_GenericError;
		}

		if (bytesReceived == SOCKET_ERROR) {
			int error = WSAGetLastError();

			return YResult::Y_GenericError;
		}

		return YResult::Y_Success;
	}

	YResult Socket::SendAll(const void* data, int numberOfBytes) {
		int totalBytesSent = 0;

		while (totalBytesSent < numberOfBytes) {
			int bytesRemaining = numberOfBytes - totalBytesSent;
			int bytesSent = 0;

			char* bufferOffset = (char*)data + totalBytesSent;
			
			YResult result = Send(bufferOffset, bytesRemaining, bytesSent);
			
			if (result != YResult::Y_Success) {
				return YResult::Y_GenericError;
			}

			totalBytesSent += bytesSent;
		}

		return YResult::Y_Success;
	}

	YResult Socket::RecvAll(void* destination, int numberOfBytes) {
		int totalBytesReceived = 0;

		while (totalBytesReceived < numberOfBytes) {
			int bytesRemaining = numberOfBytes - totalBytesReceived;
			int bytesReceived = 0;
			
			char* bufferOffset = (char*)destination + totalBytesReceived;
			
			YResult result = Recv(bufferOffset, bytesRemaining, bytesReceived);
			
			if (result != YResult::Y_Success) {
				return YResult::Y_GenericError;
			}

			totalBytesReceived += bytesReceived;
		}

		return YResult::Y_Success;
	}

	YResult Socket::Send(Packet& packet) {
		uint16_t encodedPacketSize = htons(packet.buffer.size());
		YResult result = SendAll(&encodedPacketSize, sizeof(uint16_t));
		
		if (result != YResult::Y_Success)
			return YResult::Y_GenericError;

		result = SendAll(packet.buffer.data(), packet.buffer.size());
		
		if (result != YResult::Y_Success)
			return YResult::Y_GenericError;

		return YResult::Y_Success;
	}

	YResult Socket::Recv(Packet& packet) {
		packet.Clear();

		uint16_t encodedSize = 0;
		YResult result = RecvAll(&encodedSize, sizeof(uint16_t));
		
		if (result != YResult::Y_Success)
			return YResult::Y_GenericError;

		uint16_t bufferSize = ntohs(encodedSize);

		if (bufferSize > YNet::g_MaxPacketSize)
			return YResult::Y_GenericError;

		packet.buffer.resize(bufferSize);
		result = RecvAll(&packet.buffer[0], bufferSize);
		
		if (result != YResult::Y_Success)
			return YResult::Y_GenericError;

		return YResult::Y_Success;
	}


	SocketHandle Socket::GetHandle() {
		return handle;
	}

	IPVersion Socket::GetIPVersion() {
		return ipversion;
	}

	YResult Socket::SetBlocking(bool isBlocking) {
		unsigned long nonBlocking = 1;
		unsigned long blocking = 0;

		int result = ioctlsocket(handle, FIONBIO, isBlocking ? &blocking : &nonBlocking);
		
		if (result == SOCKET_ERROR) {
			int error = WSAGetLastError();

			return YResult::Y_GenericError;
		}
		return YResult::Y_Success;
	}

	YResult Socket::SetSocketOption(SocketOption option, BOOL value) {
		int result = 0;

		switch (option) {
		case SocketOption::TCP_NoDelay:
			result = setsockopt(handle, IPPROTO_TCP, TCP_NODELAY, (const char*)&value, sizeof(value));
			break;

		case SocketOption::IPV6_Only:
			result = setsockopt(handle, IPPROTO_IPV6, IPV6_V6ONLY, (const char*)&value, sizeof(value));
			break;

		default:
			return YResult::Y_GenericError;
		}

		if (result != 0) {
			int error = WSAGetLastError();

			return YResult::Y_GenericError;
		}

		return YResult::Y_Success;
	}
}