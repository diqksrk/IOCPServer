#include <WinSock2.h>
#include <WS2tcpip.h>
#include "UserData.h"
#include <cstdio>
#include <atlstr.h>
#include <iostream>
#include <list>
#pragma once

#define BUFSIZE 100

class ServerSocket;

typedef struct
{
	SOCKET hClntSock;
	SOCKADDR_IN clntAddr;
} PER_HANDLE_DATA, *LPPER_HANDLE_DATA;

typedef struct
{
	OVERLAPPED overlapped;
	char buffer[BUFSIZE];
	WSABUF wsaBuf;
} PER_IO_DATA, *LPPER_IO_DATA;


class ServerSocket {
protected:
	UserData* p_user;
	SOCKET mh_listen_socket;
	int m_accept_notify_id;
	unsigned short int m_max_user_count;
	UserData **mp_user_list;

public :
	int StartServer(const wchar_t *ap_ip_address, int a_port);
	void UnicodeToAscii(char *ap_dest_ip, wchar_t *ap_src_ip);
	void AsciiToUnicode(wchar_t *ap_dest_ip, char *ap_src_ip);
	void ErrorHandling(char *message);
	void SendFrameData(unsigned char a_messageid, const void* ap_data, int a_data_size);
	SOCKET ProcessToAccept(SOCKADDR_IN &clntAddr, int &addrLen);
	UserData* getUserData() { return p_user; }

	/*int processToAccept();*/
	ServerSocket(unsigned int a_max_user_count, UserData *ap_user_data);
	~ServerSocket();
};

class MyServer :public ServerSocket {
private:

public :
	MyServer() : ServerSocket(50, new UserData) {};
};