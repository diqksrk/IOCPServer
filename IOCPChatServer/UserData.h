#pragma once

#include <winsock2.h>
#include <string.h>

typedef class UserData {
protected:
	SOCKET mh_socket;
	wchar_t m_ip_address[16];
public:
	UserData();
	~UserData();
	inline SOCKET GetHandle() { return mh_socket; }
	inline void SetHandle(SOCKET ah_socket) { mh_socket = ah_socket; }

	wchar_t* GetIP();
	void SetIP(const wchar_t *ap_ip_address);
	virtual UserData *CreateObject() { return new UserData; }
}*LUserData;

class UserAccount : public UserData{
protected:
	wchar_t id[32];
	wchar_t password[32];
public:
	wchar_t *GetID();
	wchar_t *GetPassword();
	void SetID(const wchar_t *ap_id);
	void SetPassword(const wchar_t *ap_password);
	virtual UserAccount *CreateObject() { return new UserAccount; }
};
