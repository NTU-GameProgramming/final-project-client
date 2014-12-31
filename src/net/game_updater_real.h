#pragma once
#include "FlyWin32.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <map>
#include "game_updater.h"
#include <json/json.h>

using namespace std;

enum EVENT {
	UPDATE_CHARACTER,
	UPDATE_OBJECT
};


class GmUpdaterReal: public GmUpdater {
public:
	GmUpdaterReal();
	~GmUpdaterReal();
	
	void updateCharacterPush(CHARACTERid game_id);
	void updateCharacterPullPosition(int game_id, float *pos);
	void updateObjectPush(int game_id);
	void updateObjectPull(int game_id);

	void updateCharacterAttackPush(CHARACTERid id);
	void updateCharacterAttackPull(int game_id);

	void update(enum EVENT, Json::Value &);
private:
	


};