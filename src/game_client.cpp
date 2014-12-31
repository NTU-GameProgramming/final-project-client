#include "game_client.h"





GmClient::GmClient() : Callback() {
	this->status = UNINITIALIZED;
}

GmClient::~GmClient(){
	delete this->json_socket;
}

void GmClient::initialize(string server_ip, string server_port) {
	this->server_ip = server_ip;
	this->server_port = server_port;
	this->json_socket = NULL;
	this->client_id = -1;
	this->status = INITIALIZED;
}

void GmClient::connectServer() {
	if(this->json_socket == NULL) {
		this->json_socket = new JsonSocket(server_ip, server_port);
		Json::Value first_connect_cube;
		first_connect_cube["TYPE"] = "INIT";
		first_connect_cube["EVENT"] = "FIRST_CONNECT";

		(this->json_socket)->setReceiveCallback(this);
		(this->json_socket)->beginConnect();
		this->status = CONNECTING;
		(this->json_socket)->sendJsonMessage(first_connect_cube);
		while (this->status == CONNECTING) {
			(this->json_socket)->receiveData();
		}
	} else {
		printf("[GmClient] JsonSocket is still connected. Please disconnect before reconnect.\n");
	}
}

void GmClient::disconnectServer() {
	if(this->json_socket != NULL) {
		delete this->json_socket;
		this->json_socket = NULL;
	} else {
		printf("[GmClient] JsonSocket is already disconnected.\n");
	}
}

void GmClient::callback(Json::Value &json) {
	Json::FastWriter json_writer;
	std::cout << "Received Callback: " << json_writer.write(json) << endl;
	if(this->status == CONNECTING) {
		if(json["TYPE"] == "INIT" && json["EVENT"] == "FIRST_CONNECT_ACK") {
			// 得到client_id
			cout << "client_id = " << (this->client_id = json["CLIENT_ID"].asInt()) << endl;
			cout << "game_id = " << (this->game_id = json["GAME_ID"].asInt()) << endl;
	/*		Json::Value confirm_cube;
			confirm_cube["TYPE"] = "INIT";
			confirm_cube["EVENT"] = "FIRST_CONNECT_FINAL";
			(this->json_socket)->sendJsonMessage(confirm_cube);*/
		} else if(json["EVENT"] == "READY") {
			this->status = CONNECTED;
		} else if(json["EVENT"] == "ADD_CHARACTER"){
			cout << "Add Character!!" << endl;
			// 得到Gm_id, 新增一個Character
			Json::Value data = json["DATA"];
			int game_id = data["GAME_ID"].asInt();
			GmCharacter& actor = this->game_tree.addCharacter(game_id);
			actor.pos[0] = data["POS"][0].asFloat(); actor.pos[1]=data["POS"][1].asFloat();actor.pos[2]=data["POS"][2].asFloat();
			actor.fdir[0] = data["FDIR"][0].asFloat(); actor.fdir[1] = data["FDIR"][1].asFloat(); actor.fdir[2] = data["FDIR"][2].asFloat();
			actor.udir[0] = data["UDIR"][0].asFloat(); actor.udir[1] = data["UDIR"][1].asFloat(); actor.udir[2] = data["UDIR"][2].asFloat();
			actor.is_main_actor = (this->game_id == game_id);

		} else if(json["EVENT"] == "ADD_OBJECT"){
			Json::Value data = json["DATA"];
			int game_id = data["GAME_ID"].asInt();
			GmObject obj = this->game_tree.addObject(game_id);
			obj.pos[0] = data["POS"][0].asFloat(); obj.pos[1]=data["POS"][1].asFloat();obj.pos[2]=data["POS"][2].asFloat();
		}
	} else if(this->status == CONNECTED) {
		if(json["TYPE"] == "GAME") {
			if(json["EVENT"] == "SYNC_ACK") {
				cout << "Got SYNC_ACK" << endl;
			} else if(json["EVENT"] == "UPDATE_CHARACTER") {
				Json::Value data, jpos;
				float pos[3];
				int game_id;
				data = json["DATA"];
				game_id = data["GAME_ID"].asInt();
				jpos = data["POS"];
				pos[0] = jpos[0].asFloat(); pos[1] = jpos[1].asFloat(); pos[2] = jpos[2].asFloat();

				FnCharacter actor;
				//actor.ID(this->gameid2characterid[game_id]);
				actor.SetPosition(pos);
			}
		}
	}

}

void GmClient::update() {
	this->json_socket->receiveData();
}


void GmClient::updateCharacter(CHARACTERid id) {
	Json::Value cube, data, jpos;
	float pos[3];

	FnCharacter actor;
	actor.ID(id);
	actor.GetPosition(pos);
	
	cube["TYPE"] = "GAME";
	cube["EVENT"] = "UPDATE_CHARACTER";

//	data["GAME_ID"] = this->character_infos[id].game_id;
	jpos.append(pos[0]).append(pos[1]).append(pos[2]);

	data["POS"] = jpos;
	
	cube["DATA"] = data;
	
	(this->json_socket)->sendJsonMessage(cube);
}

void GmClient::sync() {
	Json::Value sync_cube;
	sync_cube["TYPE"] = "GAME";
	sync_cube["EVENT"] = "SYNC";
	(this->json_socket)->sendJsonMessage(sync_cube);
}

/*

void GmClient::registerScene(SCENEid id) {
	(this->scene_info).id = id;
};

void GmClient::registerObject(OBJECTid id) {
	(this->obj_infos)[id].id = id;
};

void GmClient::registerCharacter(CHARACTERid id, DataStrength s) {
	FnCharacter actor;
	Json::Value reg_cube, data;

	reg_cube["TYPE"] = "GAME";
	reg_cube["EVENT"] = "REG_CHARACTER";

	data["ID"] = id;
	reg_cube["DATA"] = data;

	(this->json_socket)->sendJsonMessage(reg_cube);
};

void GmClient::objectPosition(OBJECTid id, float *pos) {
	struct GmObjectInfo *GOI;
	// !test if exists
	GOI = &((this->obj_infos)[id]);
	FnObject obj;
	obj.ID(id);

	if(pos == NULL) { // getter
		obj.GetPosition(GOI->pos);
	} else { // setter
		memcpy (GOI->pos, pos, sizeof(float[3]));
		obj.SetPosition(pos);
	}
};

void GmClient::characterPosition(CHARACTERid id, float *pos) {
	struct GmCharacterInfo *GCI;
	// !test if exists
	GCI = &((this->character_infos)[id]);
	FnObject actor;
	actor.ID(id);

	if(pos == NULL) { // getter
		actor.GetPosition(GCI->pos);
	} else { // setter
		memcpy (GCI->pos, pos, sizeof(float[3]));
		actor.SetPosition(pos);
	}
};
*/