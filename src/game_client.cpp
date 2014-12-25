#include "game_client.h"

GameClient::GameClient(string server_ip, string server_port) : Callback() {
	this->server_ip = server_ip;
	this->server_port = server_port;
	this->json_socket = NULL;
}

GameClient::~GameClient(){
	delete this->json_socket;
}

void GameClient::connectServer() {
	if(this->json_socket == NULL) {
		this->json_socket = new JsonSocket(server_ip, server_port);
		Json::Value first_connect_cube;
		first_connect_cube["typ"] = "net";
		first_connect_cube["evt"] = "FIRST_CONNECT";

		(this->json_socket)->setReceiveCallback(this);
		(this->json_socket)->beginConnect();
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

void GameClient::connectServerCallback(Json::Value &json_object) {


}

void GameClient::callback(Json::Value &json) {
	Json::FastWriter json_writer;
	
	std::cout << json_writer.write(json) << endl;
}