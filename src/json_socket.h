#pragma once
#define DEFAULT_BUFLEN 128
#define DEFAULT_PORT "8976"

#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>
#include <string.h>
#include <json/json.h>
#include <time.h>

#include "callback.h"

#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")


using namespace std;



class JsonSocket {
public:
	JsonSocket(string &ip, string &port);
	~JsonSocket();

	void setReceiveCallback(Callback*);
	void setErrorCallback(Callback*);
	int beginConnect();
	void sendJsonMessage(Json::Value &);
	string formatJsonString(Json::Value &);
	void parseData(string &);
	void receiveData();
	void JsonSocket::handleData(char const *buf, int bufsize, bool is_recursive);
private:

	SOCKET ConnectSocket;
	string ip, port;
	string received_data;
	Callback *cb_on_receive;
	Callback *cb_on_error;
	char recvbuf[DEFAULT_BUFLEN];
	int recvsbuflen;

	size_t json_length;


	Json::FastWriter json_writer; 
	Json::Reader json_reader;  
	Json::Value json_object;
};