#pragma once
#include "FlyWin32.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <map>
#include "json_socket.h"

using namespace std;

class GmUpdater {
public:
	GmUpdater() {};
	~GmUpdater() {};
	
	void initialize(
		JsonSocket *json_socket,
		map<CHARACTERid, int> *char2game, 
		map<int, CHARACTERid> *game2char,
		map<OBJECTid, int> *obj2game,
		map<int, OBJECTid> *game2obj) {

			this->json_socket = json_socket;
			this->char2game = char2game;
			this->game2char = game2char;
			this->obj2game = obj2game;
			this->game2obj = game2obj;
	};

	virtual void updateCharacterPush(int game_id) = 0;
	virtual void updateCharacterPull(int game_id) = 0;
	virtual void updateObjectPush(int game_id) = 0;
	virtual void updateObjectPull(int game_id) = 0;

protected:
	JsonSocket *json_socket;
	
	map<CHARACTERid, int> *char2game;
	map<int, CHARACTERid> *game2char;

	map<OBJECTid, int> *obj2game;
	map<int, OBJECTid> *game2obj;
};