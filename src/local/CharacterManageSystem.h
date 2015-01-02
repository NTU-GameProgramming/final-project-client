#pragma once

#include <string>
#include "Character.h"
#include "FightSystem.h"
#include "net/game_updater_real.h"

class CharacterManageSystem
{
public:
	CharacterManageSystem(GmUpdaterReal *game_updater);
	~CharacterManageSystem(void);

	void update(int skip);

	bool addCharacter(Character &character, bool isLocalPlayer = false);
	void removeCharacter(CHARACTERid characterId);

	void gotAttacked(CHARACTERid characterId,float damage);
	int getCharacterblood(CHARACTERid characterId);

	CHARACTERid getActorID();

	Character* getCameraActor();

	void changActorByTAB();
private:
	void updateCharacterInputs();

private:
	std::map<CHARACTERid, Character*> m_mapCharacterId2Character;
	std::map<CHARACTERid, MotionState> m_mapCharacterId2NewState;
	std::map<std::string, CHARACTERid> m_mapStrName2CharacterId; 
	FightSystem m_FightSystem;
	CHARACTERid m_localPlayerId;
	GmUpdaterReal *game_updater;
};

