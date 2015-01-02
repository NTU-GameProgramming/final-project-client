#pragma once
#include "FlyWin32.h"
#include <string>
#include <map>
#include <json/json.h>
#include "game_updater.h"

using namespace std;

enum EVENT {
	UPDATE_CHARACTER,
	UPDATE_OBJECT,
	UPDATE_MOTION_STATE,
	UPDATE_ATTACK
};


class GmUpdaterReal: public GmUpdater {
public:
	GmUpdaterReal();
	~GmUpdaterReal();
	
	void initialize(map<CHARACTERid, MotionState> *char2ms);

	void updateCharacterPushPosition(CHARACTERid game_id);
	void updateCharacterPullPosition(int game_id, float *pos);
	void updateObjectPush(int game_id);
	void updateObjectPull(int game_id);

	void updateCharacterAttackPush(CHARACTERid id);
	void updateCharacterAttackPull(int game_id);
	
	void updateCharacterMotionStatePush(CHARACTERid id, MotionState ms);
	void updateCharacterMotionStatePull(int game_id, MotionState ms);
	
	void update(enum EVENT, Json::Value &);
private:
	map<CHARACTERid, MotionState> *char2ms;
	bool if_initialized;
};