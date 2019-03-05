
// MyClientDlg.h : ��� ����
//

#pragma once
#include "afxwin.h"

#define BUFSIZE	1024

struct SOCKDATA {
	SOCKET hsocket;
	SOCKADDR_IN servadr;
};

typedef struct SOCKETINFO
{
	WSAOVERLAPPED overlapped;
	WSABUF dataBuffer;
	int receiveBytes;
	int sendBytes;
}PER_HANDLE_DATA, *LPPER_HANDLE_DATA;

// CMyClientDlg ��ȭ ����
class CMyClientDlg : public CDialogEx
{
// �����Դϴ�.
public:
	CMyClientDlg(CWnd* pParent = NULL);	// ǥ�� �������Դϴ�.
	void AddEventString(const wchar_t *ap_string);

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MYCLIENT_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �����Դϴ�.


// �����Դϴ�.
protected:
	HICON m_hIcon;

	// ������ �޽��� �� �Լ�
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
private:
	SOCKET mh_client_socket;
public:
	void SendFrameData(unsigned char a_messageid, const void* ap_data, int a_data_size);
	void ReceiveData(char* ap_buffer, int a_size);
	CListBox m_event_list;
	void ConnectProcess();
	void DestroySocket();
	void ReceiveData();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnClickedConnect();
	afx_msg void OnClickedSend();
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
};
