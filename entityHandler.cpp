#include <sstream>
#include <map>

#include "entityHandler.h"
#include "NetworkMovingEntity.h"
#include "NetworkEntity.h"

EntityHandler::EntityHandler() {
	// creates empty entities list
	m_players = new std::map<int, Entities::Player>();
	m_movingEntities = new std::map<int, Entities::MovingEntity>();
	m_staticEntities = new std::map<int, Entities::Entity>();
	m_timeline = Timeline();
	m_idTracker = 0;
}

void EntityHandler::updateEntities() {
	// Create map iterator
	std::map<int, Entities::MovingEntity>::iterator movIter;
	// Get deltaTime
	double dt = m_timeline.getDeltaTime();

	// Moves all moving entities to their appropriate locations
	for (movIter = m_movingEntities->begin(); movIter != m_movingEntities->end(); ++movIter) {
		movIter->second.moveByTime(dt);
	}
}

/**
* Deserializes a string and adds those entities into the entity controller
*
* @param entityListString
*/
void EntityHandler::updatePlayerByString(std::string playerString) {
	// Creates a player from the string
	Entities::Player *player = Entities::Player::fromString(playerString);
	m_players->insert_or_assign(player->getUUID(), *player);
}


std::map<int, Entities::Player>* EntityHandler::getPlayers() {
	return m_players;
}

/**
 * Adds the given entity to the entities list
 * @param e entity to be added to end of the entities list
 */
int EntityHandler::insertPlayer(Entities::Player p) {
	if (p.getUUID() == -1) {
		p.setUUID(m_idTracker++);
	}
	m_players->insert_or_assign(p.getUUID(), p);
	return p.getUUID();
}

/**
 * Adds the given entity to the entities list
 * @param e entity to be added to end of the entities list
 */
void EntityHandler::insertMovingEntity(Entities::MovingEntity e) {
	if (e.getUUID() == -1) {
		e.setUUID(m_idTracker++);
	}
	m_movingEntities->insert_or_assign(e.getUUID(), e);
}

std::string EntityHandler::toString() {
	// Create stringstream
	std::stringstream ss;
	// Iterate through player list and add them to the string
	std::map<int, Entities::Player>::iterator playIter;
	for (playIter = m_players->begin(); playIter != m_players->end(); ++playIter) {
		ss << "*\n" << playIter->second.toString() << "\n";
	}
	// Separate entity types with '+'
	ss << "+";

	std::map<int, Entities::MovingEntity>::iterator movIter;
	// Iterate through each MovingEntity and add them to the string
	for (movIter = m_movingEntities->begin(); movIter != m_movingEntities->end(); ++movIter) {
		ss << "*\n" << movIter->second.toString() << "\n";
	}
	return ss.str();
}

std::string EntityHandler::toStringAll() {
	// Create stringstream
	std::stringstream ss;
	// Iterate through player list and add them to the string
	std::map<int, Entities::Player>::iterator playIter;
	for (playIter = m_players->begin(); playIter != m_players->end(); ++playIter) {
		ss << "*\n" << playIter->second.toString() << "\n";
	}
	// Separate entity types with '+'
	ss << "+";

	std::map<int, Entities::MovingEntity>::iterator movIter;
	// Iterate through each MovingEntity and add them to the string
	for (movIter = m_movingEntities->begin(); movIter != m_movingEntities->end(); ++movIter) {
		ss << "*\n" << movIter->second.toString() << "\n";
	}

	// Separate entity types with '+'
	ss << "+";

	std::map<int, Entities::Entity>::iterator statIter;
	// Iterate through each static Entity and add them to the string
	for (statIter = m_staticEntities->begin(); statIter != m_staticEntities->end(); ++statIter) {
		ss << "*\n" << statIter->second.toString() << "\n";
	}
	return ss.str();
}

void EntityHandler::cleanUp() {
	// TODO: Free allocated memory
	// TODO: free memory on entities with the destroy function
	delete m_players;
	delete m_movingEntities;
}
