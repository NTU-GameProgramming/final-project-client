#pragma once
#include "FlyWin32.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <map>

using namespace std;

struct GmObject {
	OBJECTid id;
	int game_id;
	float pos[3];
	float fdir[3];
	float udir[3];
};

struct GmCharacter {
	CHARACTERid id;
	int game_id;
	float pos[3];
	float fdir[3];
	float udir[3];
	bool is_main_actor;
};

struct GmScene {
	SCENEid id;
};

class GmTree {
public:
	GmTree();
	~GmTree();

	GmCharacter& addCharacter(int game_id);
	GmObject& addObject(int game_id);

	map<int, GmObject*>& getObjectNode() {
		return this->objects;
	};

	map<int, GmCharacter*>& getCharacterNode() {
		return this->characters;
	};

private:
	map<int, GmObject*> objects;
	map<int, GmCharacter*> characters;
};
