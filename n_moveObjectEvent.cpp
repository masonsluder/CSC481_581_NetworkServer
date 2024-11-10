#include "n_moveObjectEvent.h"


namespace N_Events {

	/**
	* Constructor for MoveObjectEvent for outbound events
	*/
	N_MoveObjectEvent::N_MoveObjectEvent(std::vector<N_GameObject*> goRef, int64_t timeStampPriority, int priority, std::string socketAddress, int clientIdentifier) {
		// GameObject reference	
		m_goRefVector = goRef;
		// Event priorities
		m_timeStampPriority = timeStampPriority;
		m_priority = priority;
		// Socket address to send out information
		m_socketAddress = socketAddress;
		// Define client ID for message sending
		m_clientIdentifier = clientIdentifier;
		// Identifier false for onEvent function
		m_isReceiving = false;
		// Empty string since it's not relevant in a send until the end
		m_jsonString = "";
		m_goManagerRef = nullptr;
	}

	/**
	* Constructor for MoveObjectEvent for inbound Events
	*/
	N_MoveObjectEvent::N_MoveObjectEvent(N_GameObjectManager* goManager, int64_t timeStampPriority, int priority, std::string jsonString) {
		// GameObject reference
		m_goRefVector = std::vector<N_GameObject*>();
		// Event priorities
		m_timeStampPriority = timeStampPriority;
		m_priority = priority;
		// Socket address is empty because it is not needed for json parsing, client identifier is also set to 0 (invalid)
		m_socketAddress = "";
		m_clientIdentifier = 0;
		// Identifier for the onEvent function
		m_isReceiving = true;
		// The jsonstring to be parsed
		m_jsonString = jsonString;
		// Set go manager reference 
		m_goManagerRef = goManager;
	}

	void N_MoveObjectEvent::onEvent() const {
		if (m_isReceiving) { // If this is receiving a JSON

			// Parse json
			json j = json::parse(m_jsonString);

			// Loop through objects in JSON array
			for (const auto& obj : j["moving"]) {
				// Get the UUID of the object
				int uuid = obj["uuid"].get<int>();

				// Check if the Object exists
				if (N_GameObject* go = m_goManagerRef->find(uuid)) {
					// Set transform position
					go->getComponent<N_Components::N_Transform>()->setPosition(obj["position"]["x"].get<float>(), obj["position"]["y"].get<float>());
				}
			}
		}
		else { // If this is sending out a JSON

			// Create reply socket for REQ-REP initial GameObject initialization on Client join
			zmq::context_t context{ 1 };
			zmq::socket_t serverToClientPublisher{ context, zmq::socket_type::pub };
			serverToClientPublisher.bind(m_socketAddress);

			// Set conflate to only send most recent message
			int conflate = 1;
			zmq_setsockopt(serverToClientPublisher, ZMQ_CONFLATE, &conflate, sizeof(conflate));

			// Convert gameObject and Event data into json format
			json j;
			to_json(j);
			// If clientIdentifier is valid (not 0), then send clientIdentifier alongside JSON string
			std::string eventInfo = m_clientIdentifier != 0 ? "Client_" + std::to_string(m_clientIdentifier) + "\n" + j.dump() : j.dump();
			zmq::message_t msg(eventInfo);
			serverToClientPublisher.send(msg, zmq::send_flags::dontwait);
			// Close the socket after use is done
			serverToClientPublisher.close();
		}
	}

	/*
	* Converts the Event to json for serialization across clients and server
	*/
	void N_MoveObjectEvent::to_json(json& j) const {
		// Add all fields to the json
		json gosJson;
		// Iterate through all of the GameObjects
		for (N_GameObject* go : m_goRefVector) {
			// JSON object to store positional information
			json gameObjectJson;
			// Set UUID
			gameObjectJson["uuid"] = go->getUUID();
			// Set Position
			Utils::Vector2D position = *go->getComponent<N_Components::N_Transform>()->getPosition();
			gameObjectJson["position"] = {
				{"x", position.x},
				{"y", position.y}
			};
			if (go->getComponent<N_Components::N_PlayerInputPlatformer>()) {
				// Push GameObject into the players list
				gosJson["players"].push_back(gameObjectJson);
			} else {
				// Push GameObject in the list of MovingObjects
				gosJson["moving"].push_back(gameObjectJson);
			}
		}
		j["gos"] = gosJson;
		j["timeStampPriority"] = m_timeStampPriority;
		j["priority"] = m_priority;
	}

}
