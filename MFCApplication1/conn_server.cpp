
#include "pch.h"
#include "framework.h"
#include "conn_server.h"

#define BUF_SIZE 1024


BOOL m_socket::m_server(u_short port, int cb1)
{
	WSADATA wsaData;
	SOCKET hServSock, hClntSock;
	SOCKADDR_IN servAddr, clntAddr;

	char message[BUF_SIZE] = { 0 };
	int clntAdrSize;
	int strLen, i = 0;


	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		zlog_debug(c, "winsock����ʧ��");
		ErrorHandling("WSAStartup() error!");
	}

	if (cb1 == 0)
		hServSock = socket(PF_INET, SOCK_STREAM, 0);
	else
		hServSock = socket(PF_INET, SOCK_DGRAM, 0);

	if (hServSock == INVALID_SOCKET) {
		zlog_debug(c, "�׽ӿ���Դ����ʧ��");
		ErrorHandling("socket() error!");
	}

	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAddr.sin_port = htons(port);

	if (bind(hServSock, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR) {
		zlog_debug(c, "�׽��ְ�ʧ��");
		ErrorHandling("bind() error!");
	}

	if (cb1 == 0)
		if (listen(hServSock, 5) == SOCKET_ERROR) {
			zlog_debug(c, "�׽��ּ���ʧ��");
			ErrorHandling("listen() error!");
		}

	clntAdrSize = sizeof(clntAddr);

	int cnt = 0;// �ܵ�echo����
	if (cb1 == 0)  // TCP
	{
		zlog_info(c, "TCP���ͣ����Խ�������...");
		hClntSock = accept(hServSock, (SOCKADDR*)&clntAddr, &clntAdrSize);
		if (hClntSock == INVALID_SOCKET) {
			zlog_debug(c, "�׽�������ʧ��");
			ErrorHandling("accept() error!");
		}
		else
			//printf("Connected client %d \n", i + 1);
			zlog_info(c, "�ͻ���������ɹ���������...");
		strLen = recv(hClntSock, message, BUF_SIZE, 0);
		if (strLen != 0) cnt = atoi(message);
		for (int i = 0; i < cnt; i++)
		{
			if ((strLen = recv(hClntSock, message, BUF_SIZE, 0)) != 0)
				//printf("Message from client: %s", message);  //Ϊʲô��������ʱ���ӡ�����Ĳ�ֹmessage
				send(hClntSock, message, strLen, 0);
			//zlog_info(c, "�ɹ��յ������ر���");
		}
	}
	else  // UDP
	{
		zlog_info(c, "UDP���ͣ��ȴ�����...");
		strLen = recvfrom(hServSock, message, BUF_SIZE, 0, (SOCKADDR*)&clntAddr, &clntAdrSize);
		if (strLen != 0) cnt = atoi(message);
		for (int i = 0; i < cnt; i++)
		{
			if ((strLen = recvfrom(hServSock, message, BUF_SIZE, 0, (SOCKADDR*)&clntAddr, &clntAdrSize)) != 0)
				sendto(hServSock, message, strLen, 0, (SOCKADDR*)&clntAddr, sizeof(clntAddr));
			//zlog_info(c, "�ɹ��յ������ر���");
		}
	}

	closesocket(hServSock);
	WSACleanup();
	return true;
}

void m_socket::ErrorHandling(char* message)
{
	//fputs(message, stderr);
	//fputc('\n', stderr);
	//exit(1);
}