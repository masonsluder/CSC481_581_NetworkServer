#pragma once
#include <list>
#include <SDL.h>
#include <iostream>

#include "networkPlayer.h"
#include "networkMovingEntity.h"
#include "networkEntity.h"
#include "networkTimeline.h"
#include <map>

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
	/** The list of all static entities */
	std::map<int, Entities::Entity>* m_staticEntities;
	/** Timeline used for movement calculations */
	Timeline m_timeline;
	/** Integer used to track and set IDs to added entitiess */
	int m_idTracker;

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
	int insertPlayer(Entities::Player p);

	/**
	* Adds the given entity to the entities list
	* @param e entity to be added to end of the entities list
	*/
	void insertMovingEntity(Entities::MovingEntity e);

	/**
	* Writes a serialized string containing entities: players, then moving entities
	*/
	std::string toString();

	/**
	* Writes a serialized string containing entities: moving entities
	*/
	std::string toString(int networkConfiguration);

    /**
    * Writes a serialized string containing all entities: players, then moving entities, then static entities
    */
    std::string toStringAll();

	/**
	* Frees any allocated memory
	*/
	void cleanUp();
};

#endif
