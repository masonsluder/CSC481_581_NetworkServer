#pragma once

#ifndef N_INSTANTIATEOBJECTEVENT_H
#define N_INSTANTIATEOBJECTEVENT_H

#include "n_event.h"
#include "n_gameObjectManager.h"

#include <zmq.hpp>
#include <string>

namespace N_Events {

	class N_InstantiateObjectEvent : public virtual N_Event {
	private:

		// Whether or not the Event is receiving or sending out data
		bool m_isReceiving;
		// The player ID for the player being instantiated if applicable, unused if 0
		int m_playerID;
		// The client ID used to filter messages, this will not be used if it is passed in as 0
		int m_clientIdentifier;
		// A reference to the socket being used for a particular data transfer
		zmq::socket_ref m_socketRef;
		// JSON string to parse with Object Information in it
		std::string m_jsonString;

		// Reference to the GameObjectManager
		N_GameObjectManager* m_goManagerRef;

	public:

		// Constructor used for sending out Event information through a send socket and direct GameObject reference
		N_InstantiateObjectEvent(std::vector<N_GameObject*> goRef, int64_t timeStampPriority, int priority, zmq::socket_ref socketRef, int playerID = 0, int clientIdentifier = 0);

		// Constructor used for receiving Event information through an already-obtained message and direct GameObject reference
		N_InstantiateObjectEvent(N_GameObjectManager* goManager, int64_t timeStampPriority, int priority, std::string jsonString, int playerID = 0);

		// Overridden onEvent function that parses or sends out a message
		void onEvent() override;

		// Converts the Event to json for serialization across clients and server
		void to_json(json& j) const override;

	};

}

#endif
