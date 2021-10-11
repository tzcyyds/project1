#pragma once

class m_socket {
	public:

		BOOL m_client(char ip[], u_short port, int cb1);
		void ErrorHandling(char* message);
		double* send_ehco(int times, int points);
	private:
		SOCKET hSocket;
};

