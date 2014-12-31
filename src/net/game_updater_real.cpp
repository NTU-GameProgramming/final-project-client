#include "game_updater_real.h"
#include <iostream>
GmUpdaterReal::GmUpdaterReal () : GmUpdater() {

};

GmUpdaterReal::~GmUpdaterReal() {
};

void GmUpdaterReal::updateCharacterPush(int game_id){
	float pos[3];
	FnCharacter actor;
	CHARACTERid id = (*(this->game2char))[game_id];
	actor.ID(id);
	actor.GetPosition(pos);
	Json::Value data;
	Json::Value jpos;
	jpos[0] = pos[0]; jpos[1] = pos[1]; jpos[2] = pos[2];
	data["GAME_ID"] = game_id;
	data["POS"] = jpos;
	this->update(EVENT::UPDATE_CHARACTER, data);
};

void GmUpdaterReal::updateCharacterPull(int game_id){
};
void GmUpdaterReal::updateObjectPush(int game_id){
};
void GmUpdaterReal::updateObjectPull(int game_id){
};

void GmUpdaterReal::update(enum EVENT evt, Json::Value &json) {
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