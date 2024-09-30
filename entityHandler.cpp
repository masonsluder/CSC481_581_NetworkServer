#include <sstream>
#include "entityHandler.h"

EntityHandler::EntityHandler() {
	// creates empty entities list
	m_players = new std::list<Entities::Player>();
	m_movingEntities = new std::list<Entities::MovingEntity>();
}

void EntityHandler::updateEntities() {
	std::list<Entities::MovingEntity>::iterator movIter;
	// Moves all moving entities to their appropriate locations
	for (movIter = (*m_movingEntities).begin(); movIter != (*m_movingEntities).end(); ++movIter) {
		movIter->move();
	}
}

/**
	* Deserializes a string and adds those entities into the entity controller
	*
	* @param entityListString
	*/
void EntityHandler::updatePlayersByString(std::string playerListString) {

}


std::list<Entities::Player>* EntityHandler::getPlayers() {
	return m_players;
}

/**
 * Adds the given entity to the entities list
 * @param e entity to be added to end of the entities list
 */
void EntityHandler::addPlayer(Entities::Player p) {
	m_players->emplace_back(p);
}

/**
 * Adds the given entity to the entities list
 * @param e entity to be added to end of the entities list
 */
void EntityHandler::addMovingEntity(Entities::MovingEntity e) {
	m_movingEntities->emplace_back(e);
}

std::string EntityHandler::toString() {
	// Create stringstream
	std::stringstream ss;
	// Iterate through player list and add them
	std::list<Entities::Player>::iterator playIter;
	for (playIter = (*m_players).begin(); playIter != (*m_players).end(); ++playIter) {
		ss << "*" << playIter->toString();
	}

	ss << "+";

	std::list<Entities::MovingEntity>::iterator movIter;
	// Iterate through each MovingEntity
	for (movIter = (*m_movingEntities).begin(); movIter != (*m_movingEntities).end(); ++movIter) {
		s << "*" << playIter->i;
	}
}

void EntityHandler::cleanUp() {
	// TODO: Free allocated memory
	// TODO: free memory on entities with the destroy function
	delete m_players;
}
