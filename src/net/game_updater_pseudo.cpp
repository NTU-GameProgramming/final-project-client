#include "game_updater_pseudo.h"
#include <iostream>

GmUpdaterPseudo::GmUpdaterPseudo (map<CHARACTERid, GmCharacter *> *id2char) : GmUpdater() {
	this->id2char = id2char;
};

GmUpdaterPseudo::~GmUpdaterPseudo() {
};

void GmUpdaterPseudo::updateCharacterPush(int game_id){

	FnCharacter actor;
	CHARACTERid id = (*(this->game2char))[game_id];
	cout << "id: " << id << endl;
	float *pos = (*(this->id2char))[id]->pos;
	Json::Value data;
	Json::Value jpos;
	jpos[0] = pos[0]; jpos[1] = pos[1]; jpos[2] = pos[2];
	data["GAME_ID"] = game_id;
	data["POS"] = jpos;
	this->update(EVENT::UPDATE_CHARACTER, data);
};

void GmUpdaterPseudo::updateCharacterPull(int game_id){

};
void GmUpdaterPseudo::updateObjectPush(int game_id){
};
void GmUpdaterPseudo::updateObjectPull(int game_id){
};

void GmUpdaterPseudo::update(enum EVENT evt, Json::Value &json) {
	string evt_str;
	switch(evt) {
	case EVENT::UPDATE_CHARACTER:
		evt_str = "UPDATE_CHARACTER";
		break;
	case EVENT::UPDATE_OBJECT:
		evt_str = "UPDATE_OBJECT";
		break;
	};
	Json::Value cube;
	Json::FastWriter fw;
	cube[0] = "GAME";
	cube[1] = evt_str;
	cube[2] = json;
	//cout << "cube is made " << fw.write(cube) << endl; system("pause");
	this->json_socket->sendJsonMessage(cube);
};