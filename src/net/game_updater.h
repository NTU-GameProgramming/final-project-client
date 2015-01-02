#pragma once
#include "FlyWin32.h"
#include <string>
#include <iostream>
#include <map>
#include "json_socket.h"

#include "enum/motion_state.h"


using namespace std;

class GmUpdater {
public:
	GmUpdater() {this->if_initialized = false;};
	~GmUpdater() {};
	
	void initialize(JsonSocket *json_socket) {
		this->json_socket = json_socket;
		this->if_initialized = true;
	};

	virtual void updateCharacterPushPosition(CHARACTERid id) = 0;
	virtual void updateCharacterPullPosition(int game_id, float *pos) = 0;

	//virtual void updateCharacterPushDirection(CHARACTERid id) = 0;
	//virtual void updateCharacterPullDirection(int game_id, float *pos) = 0;

	virtual void updateObjectPush(int game_id) = 0;
	virtual void updateObjectPull(int game_id) = 0;

	virtual void updateCharacterAttackPush(CHARACTERid id) = 0;
	virtual void updateCharacterAttackPull(int game_id) = 0;

	virtual void updateCharacterMotionStatePush(CHARACTERid id, MotionState ms) = 0;
	virtual void updateCharacterMotionStatePull(int game_id, MotionState ms) = 0;

	virtual

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

	bool if_initialized;
};