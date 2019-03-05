#include "stdafx.h"
#include <iostream>

unsigned int __stdcall CompletionThread(LPVOID pComPort);

std::list<LPPER_HANDLE_DATA> g_UserList;
CRITICAL_SECTION cs;
MyServer serv;
int user_count=1;

#pragma comment(lib, "ws2_32.lib")
#if _WIN32

int main(int argc, char** argv)
{
	HANDLE hCompletionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
	SYSTEM_INFO SystemInfo;
	SOCKET hClntSock;
	LPPER_IO_DATA PerIoData; 
	LPPER_HANDLE_DATA PerHandleData;

	int RecvBytes;
	int i, Flags;

	GetSystemInfo(&SystemInfo);
	for (int i = 0; i<SystemInfo.dwNumberOfProcessors; ++i)
		_beginthreadex(NULL, 0, CompletionThread, (LPVOID)hCompletionPort, 0, NULL);
	serv.StartServer(L"127.0.0.1", 19999);
	while (true)
	{
		SOCKADDR_IN clntAddr;
		int addrLen = sizeof(clntAddr),
		hClntSock =serv.ProcessToAccept(clntAddr, addrLen);
		PerHandleData = (LPPER_HANDLE_DATA)malloc(sizeof(PER_HANDLE_DATA));

		PerHandleData->hClntSock = hClntSock;
		memcpy(&(PerHandleData->clntAddr), &clntAddr, addrLen);

		g_UserList.push_back(PerHandleData);

		CreateIoCompletionPort((HANDLE)hClntSock, hCompletionPort, (DWORD)PerHandleData, 0);
		PerIoData = (LPPER_IO_DATA)malloc(sizeof(PER_IO_DATA));
		memset(&(PerIoData->overlapped), 0, sizeof(OVERLAPPED));
		PerIoData->wsaBuf.len = BUFSIZE;
		PerIoData->wsaBuf.buf = PerIoData->buffer;
		Flags = 0;

		WSARecv(PerHandleData->hClntSock, &(PerIoData->wsaBuf), 1, (LPDWORD)&RecvBytes, (LPDWORD)&Flags, &(PerIoData->overlapped), NULL);
	}
	
	return 0;
}

unsigned int __stdcall CompletionThread(LPVOID pComPort)
{
	setlocale(LC_ALL, "ko_KR.UTF-8");
	std::list<LPPER_HANDLE_DATA>::iterator iter;
	HANDLE hCompletionPort = (HANDLE)pComPort;
	DWORD BytesTransferred;
	CString str;
	LPPER_HANDLE_DATA PerHandleData;
	LPPER_IO_DATA PerIoData;
	DWORD flags;
	char ipv4_str[16];
	char *msg=new char[1024];

	while (1) {
		GetQueuedCompletionStatus(hCompletionPort, &BytesTransferred, (LPDWORD)&PerHandleData, (LPOVERLAPPED*)&PerIoData, INFINITE);

		if (BytesTransferred == 0)
		{
			iter = g_UserList.begin();
			for (int i = 0; i < g_UserList.size(); i++) {
				if ((*iter)->hClntSock == PerHandleData->hClntSock) {
					g_UserList.erase(iter);
					break;
				}
				iter++;
			}
			closesocket(PerHandleData->hClntSock);
			free(PerHandleData);
			free(PerIoData);
			continue;
		}
		inet_ntop(AF_INET, (void *)&PerHandleData->clntAddr.sin_addr, ipv4_str, sizeof(ipv4_str));
		std::wcout << "message received from "<< ipv4_str << std::endl;
		memset(msg, 0, BUFSIZE);
		memcpy(msg, PerIoData->wsaBuf.buf+4, BytesTransferred-4);
		PerIoData->wsaBuf.buf[BytesTransferred-4] = '\0';
		free(PerIoData);

		iter = g_UserList.begin();

		wchar_t temp_str[16];
		inet_ntop(AF_INET, (void *)&PerHandleData->clntAddr.sin_addr, ipv4_str, sizeof(ipv4_str));
		serv.AsciiToUnicode(temp_str, ipv4_str);
		str.Format(L"%s : %s", temp_str, msg);
		for (int i = 0; i < g_UserList.size(); i++) {
			memset(msg, 0, BUFSIZE);

			const void *ap_data = (const wchar_t *)str;
			int a_data_size = (str.GetLength() + 1) * 2;
			unsigned char messageid = 1;
			*msg = 27;
			*(unsigned short int *)(msg + 1) = a_data_size;
			*(msg + 3) = 1;
			memcpy(msg + 4, str, a_data_size);

			PerIoData = (LPPER_IO_DATA)malloc(sizeof(PerIoData));
			memset(&PerIoData->overlapped, 0, sizeof(OVERLAPPED));
			PerIoData->wsaBuf.buf = msg;
			PerIoData->wsaBuf.len = a_data_size+4;
			WSASend((*iter)->hClntSock, &(PerIoData->wsaBuf), 1, NULL, 0, NULL, NULL);
			iter++;
		}
		

		PerIoData = (LPPER_IO_DATA)malloc(sizeof(PER_IO_DATA));
		memset(&(PerIoData->overlapped), 0, sizeof(OVERLAPPED));
		PerIoData->wsaBuf.len = BUFSIZE;
		PerIoData->wsaBuf.buf = PerIoData->buffer;

		flags = 0;

		WSARecv(PerHandleData->hClntSock, &(PerIoData->wsaBuf), 1, NULL, &flags, &(PerIoData->overlapped), NULL);
	}

	return 0;
}

#endif
