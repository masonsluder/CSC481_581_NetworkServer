#include "n_gameObjectManager.h"

#include "n_component.h"
#include "n_rigidBody.h"
#include "../CSC481-581-GameEngine/definitions.h"

/**
* Constructor for the GameObjectManager that takes in a reference to the Timeline
*
* @param timeline: Reference to the timeline
*/
N_GameObjectManager::N_GameObjectManager(Timeline* timelineRef) {
	// Set starting ID value
	m_idTracker = 0;
	// Set reference to timeline
	m_timeline = timelineRef;
	// Instantiate empty map of GameObjects with UUIDs as the key
	m_objects = new std::map<int, N_GameObject*>();
	// Instantiate empty map of GameObjects with UUIDs as the key
	m_players = new std::map<int, N_PlayerGO*>();
}

/**
* Destructor that frees any allocated memory for the GameObjects
*/
N_GameObjectManager::~N_GameObjectManager() {
	delete m_objects;
}

/**
* Updates each of the GameObjects in the objects map
*/
void N_GameObjectManager::update() {
	// Calculate current delta time in seconds
	double deltaTimeInSecs = m_timeline->getDeltaTime() / MICROSEC_PER_SEC;
	// Create iterator to iterate through the Map
	std::map<int, N_GameObject*>::iterator iter;
	// Updates each GameObject in the map
	for (iter = m_objects->begin(); iter != m_objects->end(); ++iter) {
		// Get GameObject
		N_GameObject* go = iter->second;
		// Update each GameObject component
		{
			std::lock_guard<std::mutex> lock(go->mutex);
			go->update(deltaTimeInSecs);
		}
		// No updating collisions on server
	}
}

/**
* Deserializes a string of gameObjects and inserts those GameObjects into the object map.
* This is meant to read in gameObject information sent from the server
*
* @param movingEntityString: string containing movingObject information from the server
* @param networkType: defines the type of network being used (1=client2server, 2=peer2peer)
*/
void N_GameObjectManager::deserialize(std::string gameObjectString, int networkType) {
	json j = json::parse(gameObjectString);

	// Loop through objects in JSON array
	for (const auto& obj : j) {
		auto uuid = obj["uuid"];
		if (!m_objects->count(uuid)) { // If it's a new game object
			N_GameObject* go = new N_GameObject();//
			go->from_json(obj);
			// Insert new object into the map
			insert(go);
		}
		else { // If it's an existing game object
			N_GameObject* go = m_objects->at(uuid);
			{
				std::lock_guard<std::mutex> lock(go->mutex);
				go->from_json(obj);
			}
		}
	}

	// Handle network type if necessary
}

/**
* Deserializes a string of gameObjects and inserts those GameObjects into the object map.
* This is meant to read in gameObject information sent from the server
*
* @param movingEntityString: string containing movingObject information from the server
* @param networkType: defines the type of network being used (1=client2server, 2=peer2peer)
*/
void N_GameObjectManager::deserializeClient(std::string gameObjectString, int networkType) {
	// TODO: Create new serialization function for MovingObjects once that's implemented into the Server
	json j = json::parse(gameObjectString);

	// Read in JSON array (should only be one object in peer to peer)
	//std::cout << "Game Object String: " << gameObjectString << "\n";
	int uuid = j["uuid"].get<int>();
	// Determine whether the object is new or existing
	if (!m_players->count(uuid)) { // If it's a new game object
		N_PlayerGO* go = m_players->at(uuid);
		{
			std::lock_guard<std::mutex> lock(go->mutex);
			go->from_json(j);
		}
		// Insert new object into the map
		insertPlayer(go);
	}
	else { // If it's an existing game object
		N_PlayerGO* go = m_players->at(uuid);
		{
			std::lock_guard<std::mutex> lock(go->mutex);
			go->from_json(j);
		}
	}
	// Handle network type if necessary
}

/**
* Serializes a string of gameObjects and puts them all into one string.
* This is meant to prepare the gameObject information to send (might only need player itself)
*
* @param gameObjectString: string containing movingObject information from the server
* @param networkType: defines the type of network being used (1=client2server, 2=peer2peer)
*/
void N_GameObjectManager::serialize(std::string& outputString, bool includePlayers) {
	// TODO: Adjust for use in peer-to-peer where only the player's information needs to be sent.
	// Probably don't need to iterate, but need to have the local player field for conversion
	json j;
	for (const auto& [id, go] : *m_objects) {
		if (includePlayers || !includePlayers && !go->getComponent<N_Components::N_PlayerInputPlatformer>()) {
			json gameObjectJson;
			{
				std::lock_guard<std::mutex> lock(go->mutex);
				go->to_json(gameObjectJson);
			}
			j.push_back(gameObjectJson);
		}
	}

	outputString = j.dump(); // Convert JSON to string
}

/**
* Returns a pointer to the objects map
*/
std::map<int, N_GameObject*>* N_GameObjectManager::getObjectMap() {
	return m_objects;
}

/**
* Erases the object from the client map
*/
void N_GameObjectManager::terminateClient(int uuidKey) {
	// Find GameObject, then delete it
	if (N_GameObject* go = m_objects->at(uuidKey)) {
		m_objects->erase(uuidKey);
		m_players->erase(uuidKey);
		delete go;
	}
}

/**
* Inserts the GameObject into the objects map
*
* @param go GameObject to be added to end of the object map
*/
void N_GameObjectManager::insert(N_GameObject* go) {
	// Sets UUID for the inserted object before adding it, if new
	if (go->getUUID() == 0) {
        m_idTracker++;
		go->setUUID(m_idTracker);
	}

	// Adds or inserts existing information into the Manager
	m_objects->insert_or_assign(go->getUUID(), go);
}

/**
* Inserts the Players into the players map
*
* @param go GameObject to be added to end of the object map
*/
void N_GameObjectManager::insertPlayer(N_PlayerGO* go) {
	// Sets UUID for the inserted object before adding it, if new
	/*if (go->getUUID() == 0) {
		m_idTracker++;
		go->setUUID(m_idTracker);
	}*/

	// Adds players into the map
	m_players->insert_or_assign(go->getUUID(), go);
}

/**
* Finds a GameObject with the given idea, otherwise, returns null
*
* @param uuid: The ID of the object being serached for
* @returns A reference to the GameObject or a nullptr if nothing is found
*/
N_GameObject* N_GameObjectManager::find(int uuid) {
	try { // Try finding a GameObject in the objectMap
		return m_objects->at(uuid);
	}
	catch (std::out_of_range& e) { // If there is no GameObject at the given ID, return nullptr
		return nullptr;
	}
}

/**
* Converts all objects in the object map into a vector of objects without their keys
*
* @returns A vector of references to all server GameObjects
*/
std::vector<N_GameObject*> N_GameObjectManager::convertObjectMapToVector() {
	// Create iterator to iterate through the Map
	std::map<int, N_GameObject*>::iterator iter;
	// Return vector
	std::vector<N_GameObject*> goVec = std::vector<N_GameObject*>();
	// Inserts reach GameObject into the vector
	for (iter = m_objects->begin(); iter != m_objects->end(); ++iter) {
		// Push GameObject into the GameObject vector
		goVec.push_back(iter->second);
	}
	return goVec;
}
