#include <FlyWin32.h>
#include "game_client.h"
#include "game_updater_pseudo.h"

#include <iostream>
#include <map>

GmClient game_client;

map<CHARACTERid, GmCharacter *> actors;
GmUpdaterPseudo game_updater(&actors);
CHARACTERid main_actor_id;

void FyMain(int argc, char **argv) {
	AllocConsole();
	freopen("CONOUT$", "w", stdout);
	game_client.initialize("127.0.0.1", "8976", &game_updater);
	game_client.connectServer();

	
	map<int, GmCharacter*> charNode = game_client.getGmTree().getCharacterNode();
	
	cout << "Number of Character: " << charNode.size() << endl;
    for (map<int, GmCharacter*>::iterator it = charNode.begin(); it != charNode.end(); it++) {
		cout << it->second->game_id << ", is_main_character?" << it->second->is_main_actor << endl;
		CHARACTERid cid = static_cast<CHARACTERid> (it->second->game_id + 100);
		actors[cid] = new GmCharacter();
		actors[cid]->game_id = it->second->game_id;
		actors[cid]->id = cid;
		
		memcpy(actors[cid]->pos, it->second->pos, 3 * sizeof(float));
		memcpy(actors[cid]->fdir, it->second->fdir, 3 * sizeof(float));
		memcpy(actors[cid]->udir, it->second->udir, 3 * sizeof(float));

		cout << "position: " << it->second->pos[0] << ","<< it->second->pos[1] << ","<< it->second->pos[2]  << endl;
		actors[cid]->is_main_actor = it->second->is_main_actor;
		game_updater.registerCharacter(it->second->game_id, cid);
		if(it->second->is_main_actor) {
			main_actor_id = cid;
		}
    }

	while(true) {
		actors[main_actor_id]->pos[1] += 5.0f;
		Sleep(500);
		game_updater.updateCharacterPush(main_actor_id);
		game_client.update();

	}
}

