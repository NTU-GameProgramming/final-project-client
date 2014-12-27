#pragma once
#include <iostream>
#include "json/json.h"
#include "json_socket.h"
#include "game_share.h"


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

enum CONNECTION_STATUS {
	BLANK,
	INIT, // After send message
	CONNECTED,
	END,
	ERR
};

class GameClient : public Callback {
public:
	GameClient(string server_ip, string server_port, GameShare *gs);
	~GameClient();
	
	void connectServer();
	void pushData(enum SERVER_INFO s_info, enum SERVER_ACTION s_act);
	void disconnectServer();
	void update();
	void callback(Json::Value &);

	void sync();

private:
	JsonSocket *json_socket;
	string server_ip, server_port;
	GameShare *gs;
	enum CONNECTION_STATUS status;
	int client_id;
};

