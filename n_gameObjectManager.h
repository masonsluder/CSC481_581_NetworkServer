#pragma once

#ifndef N_GAMEOBJECTMANAGER_H
#define N_GAMEOBJECTMANAGER_H

#include "n_GameObject.h"
#include "n_PlayerGO.h"
#include "networkTimeline.h"

#include <map>

class N_GameObjectManager {
private:
	// Keeps track of the current ID and assigns it to added GameObjects
	int m_idTracker;
	// Map of GameObjects (Key: UUID, Value: GameObject)
	std::map<int, N_GameObject*>* m_objects;
	// Map of player GameObjects
	std::map<int, N_PlayerGO*>* m_players;
	// Reference to Timeline for physics calculations
	Timeline* m_timeline;
public:

	/**
	* Constructor for the GameObjectManager that takes in a reference to the Timeline
	*
	* @param timeline: Reference to the timeline
	*/
	N_GameObjectManager(Timeline* timelineRef);

	/**
	* Destructor that frees any allocated memory for the GameObjects
	*/
	~N_GameObjectManager();

	/**
	* Updates each of the GameObjects in the objects map
	*/
	void update();

	/**
	* Deserializes a string of gameObjects and inserts those GameObjects into the object map.
	* This is meant to read in gameObject information sent from the server
	*
	* @param movingEntityString: string containing movingObject information from the server
	* @param networkType: defines the type of network being used (1=client2server, 2=peer2peer)
	*/
	void deserialize(std::string gameObjectString, int networkType);

	/**
	* Deserializes a string of gameObjects and inserts those GameObjects into the object map.
	* This is meant to read in gameObject information sent from the server
	*
	* @param movingEntityString: string containing movingObject information from the server
	* @param networkType: defines the type of network being used (1=client2server, 2=peer2peer)
	*/
	void deserializeClient(std::string gameObjectString, int networkType);

	/**
	* Serializes a string of gameObjects
	*
	* @param gameObjectString: string containing movingObject information from the server
	* @param networkType: defines the type of network being used (1=client2server, 2=peer2peer)
	*/
	void serialize(std::string& outputString, bool includePlayers);

	/**
	* Returns the objects map
	*/
	std::map<int, N_GameObject*>* getObjectMap();

	/**
	* Terminates the client
	* 
	* @param UUID
	*/
	void terminateClient(int uuidKey);

	/**
	* Inserts the GameObject into the objects map
	*
	* @param go GameObject to be added to end of the object map
	*/
	void insert(N_GameObject* go);

	/**
	* Inserts the GameObject into the objects map
	*
	* @param go GameObject to be added to end of the object map
	*/
	void insertPlayer(N_PlayerGO* go);
};

#endif
