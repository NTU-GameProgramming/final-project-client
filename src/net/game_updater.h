#pragma once
#include "FlyWin32.h"
#include <string>
#include <iostream>
#include <map>
#include "json_socket.h"

#include 


using namespace std;

class GmUpdater {
public:
	GmUpdater() {};
	~GmUpdater() {};
	
	void initialize(JsonSocket *json_socket) {
		this->json_socket = json_socket;
	};

	virtual void updateCharacterPush(CHARACTERid id) = 0;
	virtual void updateCharacterPullPosition(int game_id, float *pos) = 0;
	virtual void updateObjectPush(int game_id) = 0;
	virtual void updateObjectPull(int game_id) = 0;

	virtual void updateCharacterAttackPush(CHARACTERid id) = 0;
	virtual void updateCharacterAttackPull(int game_id) = 0;

	virtual void updateCharacterMotionState(CHARACTERid id) = 0;
	virtual void updateCharacterMotionState(int game_id) = 0;

	void GmUpdater::registerCharacter(int game_id, CHARACTERid actor_id) {
		cout << game_id << " <---> " << actor_id << endl;
		this->char2game[actor_id] = game_id;
		this->game2char[game_id] = actor_id;
	}

protected:
	JsonSocket *json_socket;
	
	map<CHARACTERid, int> char2game;
	map<int, CHARACTERid> game2char;

	map<OBJECTid, int> obj2game;
	map<int, OBJECTid> game2obj;
};