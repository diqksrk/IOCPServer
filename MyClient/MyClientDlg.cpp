
// MyClientDlg.cpp : ���� ����
//
#include "stdafx.h"
#include "afxdialogex.h"
#include "MyClientDlg.h"
#include "MyClient.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define BUF_SIZE 1024
// CMyClientDlg ��ȭ ����

CMyClientDlg::CMyClientDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_MYCLIENT_DIALOG, pParent)
{
	
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMyClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_event_list);
}

BEGIN_MESSAGE_MAP(CMyClientDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDCANCEL, &CMyClientDlg::OnBnClickedCancel)
	ON_BN_CLICKED(Connect_BTN, &CMyClientDlg::OnClickedConnect)
	ON_BN_CLICKED(SND_BUTTON, &CMyClientDlg::OnClickedSend)
END_MESSAGE_MAP()

// CMyClientDlg �޽��� ó����

BOOL CMyClientDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// �� ��ȭ ������ �������� �����մϴ�.  ���� ���α׷��� �� â�� ��ȭ ���ڰ� �ƴ� ��쿡��
	//  �����ӿ�ũ�� �� �۾��� �ڵ����� �����մϴ�.
	SetIcon(m_hIcon, TRUE);			// ū �������� �����մϴ�.
	SetIcon(m_hIcon, FALSE);		// ���� �������� �����մϴ�.
	/*_beginthreadex(NULL, 0, (_beginthreadex_proc_type)ServerThreadMain, (LPVOID)sockData, 0, NULL);*/
	
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	

	return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
}

// ��ȭ ���ڿ� �ּ�ȭ ���߸� �߰��� ��� �������� �׸�����
//  �Ʒ� �ڵ尡 �ʿ��մϴ�.  ����/�� ���� ����ϴ� MFC ���� ���α׷��� ��쿡��
//  �����ӿ�ũ���� �� �۾��� �ڵ����� �����մϴ�.

void CMyClientDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �׸��⸦ ���� ����̽� ���ؽ�Ʈ�Դϴ�.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Ŭ���̾�Ʈ �簢������ �������� ����� ����ϴ�.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �������� �׸��ϴ�.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}



void CMyClientDlg::AddEventString(const wchar_t *a_eventString) {
	int index = m_event_list.InsertString(-1, a_eventString);
	m_event_list.SetCurSel(index);
}

// ����ڰ� �ּ�ȭ�� â�� ���� ���ȿ� Ŀ���� ǥ�õǵ��� �ý��ۿ���
//  �� �Լ��� ȣ���մϴ�.
HCURSOR CMyClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMyClientDlg::OnBnClickedCancel()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CDialogEx::OnCancel();
}

void err_display(char *msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	MessageBox(NULL, (LPCTSTR)lpMsgBuf,(LPWSTR)msg, MB_ICONERROR);
	LocalFree(lpMsgBuf);
}

void UnicodeToAscii(char *ap_dest_ip, wchar_t *ap_src_ip) {
	int ip_length = wcslen(ap_src_ip) + 1;
	for (int i = 0; i < ip_length; i++) ap_dest_ip[i] = (char)ap_src_ip[i];
}

void AsciiToUnicode(wchar_t *ap_dest_ip, char *ap_src_ip) {
	int ip_length = strlen(ap_src_ip) + 1;
	memset(ap_dest_ip, 0, ip_length<<1);
	for (int i = 0; i < ip_length; i++) ap_dest_ip[i] = ap_src_ip[i];
}

void CMyClientDlg::OnClickedConnect()
{
	ConnectProcess();
}

void CMyClientDlg::ConnectProcess() {
	CString str;
	GetDlgItemText(IDC_EDIT2, str);
	sockaddr_in srv_addr;
	mh_client_socket = WSASocket(PF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (mh_client_socket == INVALID_SOCKET) {
		err_display("korea");
	}
	char temp_ip_address[16];
	UnicodeToAscii(temp_ip_address, (wchar_t *)(const wchar_t *)str);
	srv_addr.sin_family = AF_INET;
	srv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	srv_addr.sin_port = htons(atoi("19999"));

	SOCKDATA* sockData = new SOCKDATA();
	sockData->hsocket = mh_client_socket;
	sockData->servadr = srv_addr;

	if (connect(mh_client_socket, (SOCKADDR*)&srv_addr, sizeof(srv_addr)) == SOCKET_ERROR) {
		if (WSAGetLastError() != WSA_IO_PENDING) {
			err_display("korea");
			AddEventString(CString(L"connect function failed with error = ") + temp_ip_address);
			exit(1);
		}
	}
	AddEventString(L"������ ���Ӊ���ϴ�.");
	WSAAsyncSelect(mh_client_socket, m_hWnd, 27002, FD_READ | FD_CLOSE);
}

void CMyClientDlg::SendFrameData(unsigned char a_messageid,const void* ap_data, int a_data_size) {
	char *send_data = new char[a_data_size + 4];
	*send_data = 27;\
	*(unsigned short int *)(send_data+1) = a_data_size;
	*(send_data + 3) = a_messageid;
	
	memcpy(send_data + 4, ap_data, a_data_size);

	int sendBytes=send(mh_client_socket, send_data, a_data_size + 4, 0);

	if (sendBytes < 0) {
		err_display("korea");
		AddEventString(L"send function error.");
	}
	delete[] send_data;
}

void CMyClientDlg::OnClickedSend()
{
	CString str;
	GetDlgItemText(INPUT_EDIT, str);
	int str_length = str.GetLength();
	if (str_length == 0) {
		AddEventString(L"������ �Է����ּ���.");
	}
	else {
		/*char* temp_contents = new char[str_length];
		UnicodeToAscii(temp_contents, (wchar_t *)(const wchar_t *)str);*/
		SendFrameData(1,str, (str.GetLength()+1)*2);
	}
}

//char s1[100];
//sprintf(s1, "%d", sizeof(sendbuffer));
//wchar_t *temp_buffer = new wchar_t[sizeof(sendbuffer)];
//AsciiToUnicode(temp_buffer, s1);
//AddEventString(temp_buffer);
//err_display("korea");

void CMyClientDlg::DestroySocket() {
	LINGER temp_linger = { true, 0 };
	setsockopt(mh_client_socket, SOL_SOCKET, SO_LINGER, (char *)&temp_linger, sizeof(temp_linger));

	closesocket(mh_client_socket);
	mh_client_socket = INVALID_SOCKET;
}

void CMyClientDlg::ReceiveData(char* ap_buffer, int a_size) {
	int current_size, total_size = 0, retry_count = 0;

	while (total_size < a_size) {
		current_size = recv(mh_client_socket, ap_buffer + total_size, a_size-total_size,0);

		if (current_size == SOCKET_ERROR) {
			retry_count++;
			Sleep(50);
			if (retry_count > 5) break;
		}
		else {
			retry_count = 0;
			total_size = total_size + current_size;
		}
	}
}

void CMyClientDlg::ReceiveData() {
	char key, message_id;
	recv(mh_client_socket, &key, 1, 0);
	if (key == 27) {
		unsigned short int body_size = 0;
		recv(mh_client_socket, (char *)&body_size, 2, 0);
		recv(mh_client_socket, &message_id, 1, 0);
		if (body_size > 0) {
			char *p_body_data = new char[body_size];

			ReceiveData(p_body_data, body_size);
			AddEventString((wchar_t *)p_body_data);

			delete[] p_body_data;
		}
	}
	
	/*char *receiveBuffer = new char[4];
	int receiveBytes = recv(mh_client_socket, receiveBuffer, 4, 0);
	AddEventString(L"123");*/
	/*int receiveBytes = recv(mh_client_socket, receiveBuffer, sizeof(7),0);
	char s1[100];
	if (receiveBytes > 0) {
		sprintf(s1, "%d", receiveBytes);
		wchar_t *temp_buffer = new wchar_t[receiveBytes];
		AsciiToUnicode(temp_buffer, s1);
		AddEventString(temp_buffer);
		
		AsciiToUnicode(temp_buffer, receiveBuffer);
		AddEventString(temp_buffer);
	}
	delete[] receiveBuffer;*/
}

LRESULT CMyClientDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == 27002) {
		if (WSAGETSELECTEVENT(lParam) == FD_READ) {
			WSAAsyncSelect(mh_client_socket, m_hWnd, 27002, FD_CLOSE);
			ReceiveData();
			WSAAsyncSelect(mh_client_socket, m_hWnd, 27002, FD_READ | FD_CLOSE);
		}
		else {
			DestroySocket();
			err_display("korea");
			AddEventString(L"������ �����Ҽ� �����ϴ�.");
		}
	}
	return CDialogEx::WindowProc(message, wParam, lParam);
}
