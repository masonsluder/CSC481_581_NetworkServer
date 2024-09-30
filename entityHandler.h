#pragma once
#include "../CSC481-581-GameEngine/player.h"
#include "../CSC481-581-GameEngine/movingEntity.h"
#include <list>
#include <SDL.h>
#include <iostream>

#pragma once
#ifndef ENTITY_HANDLER_H
#define ENTITY_HANDLER_H

/**
* Manages and updates physics and functions of all entities
*/
class EntityHandler {
private:
	/** The list of all players */
	std::list<Entities::Player>* m_players;
	/** The list of all entities */
	std::list<Entities::MovingEntity>* m_movingEntities;
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
	void updatePlayersByString(std::string playerListString);

	/**
	* Returns the entities list
	*/
	std::list<Entities::Player>* getPlayers();

	/**
	* Adds the given entity to the entities list
	* @param e entity to be added to end of the entities list
	*/
	void addPlayer(Entities::Player p);

	/**
	* Adds the given entity to the entities list
	* @param e entity to be added to end of the entities list
	*/
	void addMovingEntity(Entities::MovingEntity e);

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
