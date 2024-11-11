#pragma once

#ifndef N_UPDATEOBJECTEVENT_H
#define N_UPDATEOBJECTEVENT_H


#include "n_event.h"
#include "n_GameObject.h"
#include "n_gameObjectManager.h"

#include <zmq.hpp>

namespace N_Events {
	class N_UpdateObjectEvent : public virtual N_Event {
	private:
		// Whether or not the Event is receiving or sending out data
		bool m_isReceiving;
		// The client ID used to filter messages, this will not be used if it is passed in as 0
		int m_clientIdentifier;
		// Reference to the socket used for sending information out
		zmq::socket_ref m_socketRef;
		// JSON string to parse with Object Information in it
		std::string m_jsonString;

		// Reference to the GameObjectManager
		N_GameObjectManager* m_goManagerRef;

	public:

		/**
		* Constructor for UpdateObjectEvent for outbound events
		*/
		N_UpdateObjectEvent(std::vector<N_GameObject*> goRef, int64_t timeStampPriority, int priority, zmq::socket_ref socketRef, int clientIdentifier = 0);

		/**
		* Constructor for UpdateObjectEvent for inbound Events
		*/
		N_UpdateObjectEvent(N_GameObjectManager* goManager, int64_t timeStampPriority, int priority, std::string jsonString);

		/**
		* The function that parses or sends out information on positional data changes
		*/
		void onEvent() override;

		/*
		* Converts the Event to json for serialization across clients and server
		*/
		void to_json(json& j) const override;

	};

}


#endif
