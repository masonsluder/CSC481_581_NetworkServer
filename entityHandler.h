#pragma once
#include <list>
#include <SDL.h>
#include <iostream>

#include "../CSC481-581-GameEngine/player.h"
#include "../CSC481-581-GameEngine/movingEntity.h"
#include "../CSC481-581-GameEngine/timeline.h"

#pragma once
#ifndef ENTITY_HANDLER_H
#define ENTITY_HANDLER_H

/**
* Manages and updates physics and functions of all entities
*/
class EntityHandler {
private:
	/** The list of all players */
	std::map<int, Entities::Player>* m_players;
	/** The list of all entities */
	std::map<int, Entities::MovingEntity>* m_movingEntities;
	/** Timeline used for movement calculations */
	Timeline m_timeline;

public:
	/**
	* The constructor for EntityHandler
	*/
	EntityHandler();

	/**
	* Updates each MovingEntity's position
	*/
	void updateEntities();

	/**
	* Deserializes a string and adds those entities into the entity controller
	*
	* @param entityListString
	*/
	void updatePlayerByString(std::string playerString);

	/**
	* Returns the entities map
	*/
	std::map<int, Entities::Player>* getPlayers();

	/**
	* Adds the given entity to the entities list
	* @param e entity to be added to end of the entities list
	*/
	void insertPlayer(Entities::Player p);

	/**
	* Adds the given entity to the entities list
	* @param e entity to be added to end of the entities list
	*/
	void insertMovingEntity(Entities::MovingEntity e);

	/**
	* Writes a serialized string containing all entities: players, then moving entities
	*/
	std::string toString();

	/**
	* Frees any allocated memory
	*/
	void cleanUp();
};

#endif
