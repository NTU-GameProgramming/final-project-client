#include "game_tree.h"
#include <iostream>

GmTree::GmTree() {
};

GmTree::~GmTree() {
	for(map<int, GmCharacter*>::iterator it=this->characters.begin(); it != this->characters.begin(); ++it) {
		delete it->second;
	}

	for(map<int, GmObject*>::iterator it=this->objects.begin(); it != this->objects.begin(); ++it) {
		delete it->second;
	}
};

GmCharacter& GmTree::addCharacter(int game_id){
	this->characters[game_id] = new struct GmCharacter();
	this->characters[game_id]->game_id = game_id;
	std::cout << "[GmCharacter] position = " << this->characters[game_id] << std::endl;
	return *(this->characters[game_id]);
};

GmObject& GmTree::addObject(int game_id) {
	this->objects[game_id] = new struct GmObject();
	this->objects[game_id]->game_id = game_id;
	return *(this->objects[game_id]);
};