#include "ServerSocket.h"

ServerSocket::ServerSocket(unsigned int a_max_user_count, UserData *ap_user_data) {
	m_max_user_count = a_max_user_count;
	mh_listen_socket = INVALID_SOCKET;
	mp_user_list = new UserData*[m_max_user_count];
	for (int i = 0; i < m_max_user_count; i++) {
		mp_user_list[i] = ap_user_data->CreateObject();
	}
	delete ap_user_data;
}

ServerSocket::~ServerSocket() {
	if (mh_listen_socket != INVALID_SOCKET) closesocket(mh_listen_socket);
	for (int i = 0; i < m_max_user_count; i++) {
		delete mp_user_list[i];
	}

	delete[] mp_user_list;
}

void ServerSocket::SendFrameData(unsigned char a_messageid, const void* ap_data, int a_data_size) {
	char *send_data = new char[a_data_size + 4];
	*send_data = 27;
	*(unsigned short int *)(send_data + 1) = a_data_size;
	*(send_data + 3) = a_messageid;

	memcpy(send_data + 4, ap_data, a_data_size);

	int sendBytes = send(mh_listen_socket, send_data, a_data_size + 4, 0);

	if (sendBytes < 0) {
		printf("send error\n");
	}
	delete[] send_data;
}


int ServerSocket::StartServer(const wchar_t *ap_ip_address, int a_port) {
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrorHandling("WSAStartup() error!");
	struct sockaddr_in serv_addr;
	mh_listen_socket= WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (mh_listen_socket < 0)return -1;
	char temp_ip_address[16];
	UnicodeToAscii(temp_ip_address, (wchar_t *)ap_ip_address);
	memset((char *)&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	InetPton(AF_INET, (PCWSTR)temp_ip_address, &serv_addr.sin_addr);
	serv_addr.sin_port = htons((short unsigned int)a_port);

	if (bind(mh_listen_socket, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {	
		ErrorHandling("bind() error!");
		closesocket(mh_listen_socket);
		mh_listen_socket = INVALID_SOCKET;
		return -2;
	}
	listen(mh_listen_socket, 5);
	return 1;
}

void ServerSocket::UnicodeToAscii(char *ap_dest_ip, wchar_t *ap_src_ip) {
	int ip_length = wcslen(ap_src_ip) + 1;
	for (int i = 0; i < ip_length; i++) {
		ap_dest_ip[i] = (char)ap_src_ip[i];
	}
}

void ServerSocket::AsciiToUnicode(wchar_t *ap_dest_ip, char *ap_src_ip) {
	int ip_length = strlen(ap_src_ip) + 1;
	memset(ap_dest_ip, 0, ip_length << 1);
	for (int i = 0; i < ip_length; i++) {
		ap_dest_ip[i] = ap_src_ip[i];
	}
}

SOCKET ServerSocket::ProcessToAccept(SOCKADDR_IN &clntAddr, int &addrLen) {
	int i;
	char ipv4_str[16];
	SOCKET hClntSock = accept(mh_listen_socket, (SOCKADDR*)&clntAddr, &addrLen);
	if (hClntSock == INVALID_SOCKET) return -1;
	else {
		p_user = (UserData*)malloc(sizeof(UserData));
		wchar_t temp_ip_address[16];
		inet_ntop(AF_INET, (void *)&clntAddr.sin_addr, ipv4_str, sizeof(ipv4_str));
		std::cout << ipv4_str << "가 접속했습니다" << std::endl;
		AsciiToUnicode(temp_ip_address, ipv4_str);
		for (i = 0; i < m_max_user_count; i++) {
			p_user = mp_user_list[i];
			if (p_user->GetHandle() == INVALID_SOCKET) {
				p_user->SetHandle(hClntSock);
				p_user->SetIP(temp_ip_address);
				UnicodeToAscii(ipv4_str, temp_ip_address);
				break;
			}
		}
		if (i == m_max_user_count) {
			printf("cant afford for user : %s\n", temp_ip_address);
			closesocket(hClntSock);
			return -2;
		}

	}
	return hClntSock;
}

void ServerSocket::ErrorHandling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
