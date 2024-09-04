#pragma once

namespace YNet {
	enum SocketOption {
		TCP_NoDelay, // trueの時アルゴリズムを無効化
		IPV6_Only, // true IPv4 IPv6両方で接続可能
	};
}