#include "pch.h"

#define BUF_SIZE 1024

BOOL m_socket::m_client(char ip[], u_short port, int cb1)
{
	WSADATA wsaData;
	SOCKADDR_IN servAddr;

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		ErrorHandling("WSAStartup() error!");
		zlog_debug(c, "winsock����ʧ��");
	}
	if (cb1 == 0)
	{
		hSocket = socket(PF_INET, SOCK_STREAM, 0);
		zlog_info(c, "�����ֽ��������׽���...");
	}
	else
	{
		hSocket = socket(PF_INET, SOCK_DGRAM, 0);
		zlog_info(c, "�������ݰ������׽���...");
	}

	if (hSocket == INVALID_SOCKET) {
		zlog_debug(c, "�׽ӿ���Դ����ʧ��");
		ErrorHandling("hSocket() error");
	}

	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	//servAddr.sin_addr.s_addr = inet_addr(ip);
	inet_pton(AF_INET, ip, &(servAddr.sin_addr.s_addr));
	servAddr.sin_port = htons(port);

	if (connect(hSocket, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
		ErrorHandling("connect() error!");
	else
		return true;

	return false;
}



void m_socket::ErrorHandling(char* message)
{
	//fputs(message, stderr);
	//fputc('\n', stderr);
	//exit(1);
}

double* m_socket::send_ehco(int times, int points)
{
	// ��¼ʱ��Ļ����ͻ���
	char message[BUF_SIZE] = "Hello world!";
	//std::vector<double> v(65535, 0);
	//double sum = 0;
	double* sum;
	sum = (double*)calloc(times, sizeof(double));
	if (sum == NULL) {

	}
	else {
		int str_len, recv_len, recv_cnt;
		char cnt[BUF_SIZE] = {0};// ��¼�ܵ�echo����
		_itoa_s(times * points, cnt, 10);
		str_len = send(hSocket, cnt, strlen(message), 0);

		LARGE_INTEGER nFreq = {0};

		LARGE_INTEGER t1;

		LARGE_INTEGER t2;

		double dt;

		QueryPerformanceFrequency(&nFreq);

		for (int i = 0;i < times;i++)
		{
			for (int j = 0; j < points; j++)
			{
				//printf("test%d: ", i);

				str_len = send(hSocket, message, strlen(message), 0);
				QueryPerformanceCounter(&t1);
				recv_len = 0;
				/*while (recv_len < str_len)
				{
					recv_cnt = recv(hSocket, message, BUF_SIZE, 0);
					if (recv_cnt == -1)

						ErrorHandling("read() error!");
					recv_len += recv_cnt;
				}*/
				recv_cnt = recv(hSocket, message, BUF_SIZE, 0);
				if (recv_cnt == -1) {
					zlog_error(c, "û�гɹ����ӣ������Ƿ������ͻ���Э�����Ͳ�һ��");
				}
				QueryPerformanceCounter(&t2);

				dt = (t2.QuadPart - t1.QuadPart) / (double)nFreq.QuadPart;
				sum[i] += dt;
				//printf("%.1f us\n", dt * 1000000);

			}
			zlog_info(c,"��%d�β��ԣ�%.1f us",i+1,sum[i] * 1000000);
		}
		//printf("Average: %.1f us\n", sum * 1000000 / 30);

		closesocket(hSocket);
		zlog_info(c, "������ϣ��ر��׽���");
		WSACleanup();
		//free(sum);
	}

	//system("pause");
	return sum;

}
