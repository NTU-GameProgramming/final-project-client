#pragma once
#include <iostream>
#include "json/json.h"
#include "json_socket.h"


enum SERVER_INFO {
	POSITION,
	FDIR,
	POSE,
	ACTION,
	BLOOD
};

enum SERVER_ACTION {
	POST,
	GET
};

class GameClient : public Callback {
public:
	GameClient(string server_ip, string server_port);
	~GameClient();
	
	void connectServer();
	void pushData(enum SERVER_INFO s_info, enum SERVER_ACTION s_act);
	void disconnectServer();
	void connectServerCallback(Json::Value &json_object);
	void callback(Json::Value &);
	
private:
	JsonSocket *json_socket;
	string server_ip;
	string server_port;
};

