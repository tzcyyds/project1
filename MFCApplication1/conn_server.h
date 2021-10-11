#pragma once
#pragma comment (lib,"wsock32.lib")

class m_socket {
public:

	BOOL m_server(u_short port, int cb1);
	void ErrorHandling(char* message);
private:
	SOCKET hSocket;
};