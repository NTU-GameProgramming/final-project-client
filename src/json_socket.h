#pragma once
#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "8976"

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>
#include <json/json.h>

#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")


using namespace std;



class JsonSocket {
public:
	JsonSocket(string &ip, string &port);
	~JsonSocket();

	void onReceive(void (*cb_on_receive)(Json::Value &));
	void onError(void (*cb_on_receive)(string &));
	int beginConnect();


private:

	SOCKET ConnectSocket;
	string ip, port;
	void (*cb_on_receive)(Json::Value &);
	void (*cb_on_error)(string &);
	char *sendbuf;
    char recvbuf[DEFAULT_BUFLEN];
	int recvsbuflen;
};