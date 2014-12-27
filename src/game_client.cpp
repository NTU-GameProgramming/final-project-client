#include "game_client.h"

GameClient::GameClient(string server_ip, string server_port, GameShare *gs) : Callback() {
	this->server_ip = server_ip;
	this->server_port = server_port;
	this->gs = gs;
	this->json_socket = NULL;
	this->status = BLANK;
	this->client_id = -1;

}

GameClient::~GameClient(){
	delete this->json_socket;
}

void GameClient::connectServer() {
	if(this->json_socket == NULL) {
		this->json_socket = new JsonSocket(server_ip, server_port);
		Json::Value first_connect_cube;
		first_connect_cube["TYPE"] = "NET";
		first_connect_cube["EVENT"] = "FIRST_CONNECT";

		(this->json_socket)->setReceiveCallback(this);
		(this->json_socket)->beginConnect();
		this->status = INIT;
		(this->json_socket)->sendJsonMessage(first_connect_cube);
	} else {
		printf("[GameClient] JsonSocket is still connected. Please disconnect before reconnect.\n");
	}
}

void GameClient::disconnectServer() {
	if(this->json_socket != NULL) {
		delete this->json_socket;
		this->json_socket = NULL;
	} else {
		printf("[GameClient] JsonSocket is already disconnected.\n");
	}
}

void GameClient::callback(Json::Value &json) {
	Json::FastWriter json_writer;
	std::cout << json_writer.write(json) << endl;
	if(this->status == INIT && json["TYPE"] == "NET" && json["EVENT"] == "FIRST_CONNECT_ACK") {

		cout << "Client_id = " << (this->client_id = json["CLIENT_ID"].asInt()) << endl;

		Json::Value confirm_cube;
		confirm_cube["TYPE"] = "NET";
		confirm_cube["EVENT"] = "FIRST_CONNECT_FINAL";
		(this->json_socket)->sendJsonMessage(confirm_cube);
		this->status = CONNECTED;
		
	} else if(this->status == CONNECTED) {
		if(json["TYPE"] == "GAME") {
			if(json["EVENT"] == "SYNC_ACK") {
				cout << "Got SYNC_ACK" << endl;
			}
		}
	}

}

void GameClient::update() {
	this->json_socket->receiveData();
}


void GameClient::sync() {
	Json::Value sync_cube;
	sync_cube["TYPE"] = "GAME";
	sync_cube["EVENT"] = "SYNC";
	(this->json_socket)->sendJsonMessage(sync_cube);
}