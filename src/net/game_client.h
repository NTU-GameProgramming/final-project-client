#pragma once
#include <iostream>
#include <map>
#include "FlyWin32.h"
#include "json/json.h"
#include "json_socket.h"
#include "game_tree.h"
#include "game_updater.h"

enum CONNECTION_STATUS {
	UNINITIALIZED,
	INITIALIZED,
	CONNECTING, // After send message
	CONNECTED,
	END,
	ERR
};

enum SERVER_INFO {
	POSITION,
	FDIR,
	POSE,
	ACTION,
	BLOOD
};

class GmClient : public Callback {
public:
	GmClient();
	~GmClient();
	void initialize(string server_ip, string server_port, GmUpdater *game_updater);
	void connectServer();
	void pushData(enum SERVER_INFO s_info, enum SERVER_ACTION s_act);
	void disconnectServer();
	void update();
	void callback(Json::Value &);

	GmTree& getGmTree() { return this->game_tree; };
	void sync();
	
	void objectPosition(OBJECTid, float *pos = NULL);
	void characterPosition(CHARACTERid, float *pos = NULL);

private:
	JsonSocket *json_socket;
	GmUpdater *game_updater;

	string server_ip, server_port;
	enum CONNECTION_STATUS status;
	int client_id, game_id;
	GmTree game_tree;
};

