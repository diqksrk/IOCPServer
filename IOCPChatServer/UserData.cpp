#include "UserData.h"

wchar_t* UserData::GetIP() {
	return m_ip_address;
}
void UserData::SetIP(const wchar_t *ap_ip_address) {
	wcscpy_s(m_ip_address, ap_ip_address);
}

UserData::UserData() {
	mh_socket = INVALID_SOCKET;
	m_ip_address[0] = 0;
}

UserData::~UserData() {
	if (mh_socket != INVALID_SOCKET) {
		if (mh_socket != INVALID_SOCKET) closesocket(mh_socket);
	}
}

wchar_t *UserAccount::GetID() {
	return id;
}
wchar_t *UserAccount::GetPassword() {
	return password;
}
void UserAccount::SetID(const wchar_t *ap_id) {
	wcscpy_s(id, ap_id);
}
void UserAccount::SetPassword(const wchar_t *ap_password) {
	wcscpy_s(password, ap_password);
}


