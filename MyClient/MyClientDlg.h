
// MyClientDlg.h : 헤더 파일
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

// CMyClientDlg 대화 상자
class CMyClientDlg : public CDialogEx
{
// 생성입니다.
public:
	CMyClientDlg(CWnd* pParent = NULL);	// 표준 생성자입니다.
	void AddEventString(const wchar_t *ap_string);

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MYCLIENT_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
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
