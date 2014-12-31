#pragma once
#include "FlyWin32.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <map>
#include <game_updater.h>
#include <json/json.h>
#include "game_tree.h"

using namespace std;

enum EVENT {
	UPDATE_CHARACTER,
	UPDATE_OBJECT
};


class GmUpdaterPseudo: public GmUpdater {
public:
	GmUpdaterPseudo(map<CHARACTERid, GmCharacter *> *id2char);
	~GmUpdaterPseudo();
	
	void updateCharacterPush(int game_id);
	void updateCharacterPull(int game_id);
	void updateObjectPush(int game_id);
	void updateObjectPull(int game_id);

	void update(enum EVENT, Json::Value &);
private:
	map<CHARACTERid, GmCharacter *> *id2char;
};