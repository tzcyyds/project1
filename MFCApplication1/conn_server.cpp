
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
		zlog_debug(c, "winsock启动失败");
		ErrorHandling("WSAStartup() error!");
	}

	if (cb1 == 0)
		hServSock = socket(PF_INET, SOCK_STREAM, 0);
	else
		hServSock = socket(PF_INET, SOCK_DGRAM, 0);

	if (hServSock == INVALID_SOCKET) {
		zlog_debug(c, "套接口资源分配失败");
		ErrorHandling("socket() error!");
	}

	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAddr.sin_port = htons(port);

	if (bind(hServSock, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR) {
		zlog_debug(c, "套接字绑定失败");
		ErrorHandling("bind() error!");
	}

	if (cb1 == 0)
		if (listen(hServSock, 5) == SOCKET_ERROR) {
			zlog_debug(c, "套接字监听失败");
			ErrorHandling("listen() error!");
		}

	clntAdrSize = sizeof(clntAddr);

	int cnt = 0;// 总的echo次数
	if (cb1 == 0)  // TCP
	{
		zlog_info(c, "TCP类型，尝试建立连接...");
		hClntSock = accept(hServSock, (SOCKADDR*)&clntAddr, &clntAdrSize);
		if (hClntSock == INVALID_SOCKET) {
			zlog_debug(c, "套接字连接失败");
			ErrorHandling("accept() error!");
		}
		else
			//printf("Connected client %d \n", i + 1);
			zlog_info(c, "客户与服务器成功建立连接...");
		strLen = recv(hClntSock, message, BUF_SIZE, 0);
		if (strLen != 0) cnt = atoi(message);
		for (int i = 0; i < cnt; i++)
		{
			if ((strLen = recv(hClntSock, message, BUF_SIZE, 0)) != 0)
				//printf("Message from client: %s", message);  //为什么服务器有时候打印出来的不止message
				send(hClntSock, message, strLen, 0);
			//zlog_info(c, "成功收到并返回报文");
		}
	}
	else  // UDP
	{
		zlog_info(c, "UDP类型，等待报文...");
		strLen = recvfrom(hServSock, message, BUF_SIZE, 0, (SOCKADDR*)&clntAddr, &clntAdrSize);
		if (strLen != 0) cnt = atoi(message);
		for (int i = 0; i < cnt; i++)
		{
			if ((strLen = recvfrom(hServSock, message, BUF_SIZE, 0, (SOCKADDR*)&clntAddr, &clntAdrSize)) != 0)
				sendto(hServSock, message, strLen, 0, (SOCKADDR*)&clntAddr, sizeof(clntAddr));
			//zlog_info(c, "成功收到并返回报文");
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