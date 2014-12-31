#include "game_updater_real.h"
#include <iostream>
GmUpdaterReal::GmUpdaterReal () : GmUpdater() {

};

GmUpdaterReal::~GmUpdaterReal() {
};

void GmUpdaterReal::updateCharacterPush(CHARACTERid id){
	float pos[3];
	FnCharacter actor;
	actor.ID(id);
	actor.GetPosition(pos);
	Json::Value data;
	Json::Value jpos;
	//cout << "(" << pos[0] << ", " << pos[1] << ", " << pos[2] << ")" << endl;
	jpos[0] = pos[0]; jpos[1] = pos[1]; jpos[2] = pos[2];
	data["GAME_ID"] = this->char2game[id];
	data["POS"] = jpos;
	this->update(UPDATE_CHARACTER, data);
};

void GmUpdaterReal::updateCharacterPullPosition(int game_id, float *pos){
	FnCharacter actor;
	actor.ID(this->game2char[game_id]);
	actor.SetPosition(pos);
};

void GmUpdaterReal::updateObjectPush(int game_id){
};
void GmUpdaterReal::updateObjectPull(int game_id){
};

void GmUpdaterReal::updateCharacterAttackPush(CHARACTERid id){
	float pos[3], fdir[3];
	FnCharacter actor;
	actor.ID(id);
	actor.GetPosition(pos);
	actor.GetDirection(fdir, NULL);
	Json::Value data, jpos, jfdir;
	jpos[0] = pos[0]; jpos[1] = pos[1]; jpos[2] = pos[2];
	jfdir[0] = fdir[0];	jfdir[1] = fdir[1];	jfdir[2] = fdir[2];
	data["GAME_ID"] = this->char2game[id];
	data["POS"] = jpos; data["FDIR"] = jfdir;
	this->update(UPDATE_CHARACTER, data);
};

void GmUpdaterReal::updateCharacterAttackPull(int game_id){
};


void GmUpdaterReal::update(enum EVENT evt, Json::Value &json) {
	string evt_str;
	switch(evt) {
	case UPDATE_CHARACTER:
		evt_str = "UPDATE_CHARACTER";
		break;
	case UPDATE_OBJECT:
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

void GmUpdaterReal::updateCharacterAttack(int game_id){
	float pos[3], fdir[3];
	FnCharacter actor;
	CHARACTERid id = (this->game2char)[game_id];
	actor.ID(id);
	actor.GetPosition(pos);
	actor.GetDirection(fdir, NULL);
	Json::Value data;
	Json::Value jpos;
	jpos[0] = pos[0]; jpos[1] = pos[1]; jpos[2] = pos[2];
	data["GAME_ID"] = game_id;
	data["POS"] = jpos;
	this->update(UPDATE_CHARACTER, data);
};