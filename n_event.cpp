#include "n_event.h"
#include "n_GameObject.h"

namespace N_Events {

	N_Event::N_Event() {
		this->m_goRefVector = std::vector<N_GameObject*>();
		this->m_timeStampPriority = 0;
		this->m_priority = 0;
	}

	/**
	* Constructor for the N_Event class
	*
	* @param goRef: A reference to the Gameobject that the N_Event will affect
	* @param timeStampPriority: The timestamp at which the N_Event should be deployed
	* @param priority: A secondary priority
	*/
	N_Event::N_Event(std::vector<N_GameObject*> goRefVector, int64_t timeStampPriority, int priority) {
		this->m_goRefVector = std::vector<N_GameObject*>();
		this->m_goRefVector = goRefVector;
		this->m_timeStampPriority = timeStampPriority;
		this->m_priority = priority;
	}

	/**
	* Destructor for an N_Event that gets rid of the reference to N_GameObject
	*/
	N_Event::~N_Event() {
		this->m_goRefVector.clear();
	}

	/**
	* Returns a string in JSON format for the N_Event, N_GameObjectUUID is sent as 0 (invalid) if there is no reference
	*/
	void N_Event::to_json(json& j) const {
		// Add all fields to the json, but records N_GameObject ID as 0 if there is no N_GameObject reference
		std::vector<int> idVector = std::vector<int>();
		for (N_GameObject* goRef : m_goRefVector) {
			idVector.push_back(goRef->getUUID());
		}
		j["goUUIDs"] = idVector;
		j["timeStampPriority"] = m_timeStampPriority;
		j["priority"] = m_priority;
	}

	/**
	* Return the timestamp the N_Event should execute at
	*/
	int64_t N_Event::getTimeStampPriority() const {
		return m_timeStampPriority;
	}

	/**
	* Sets the time stamp to the specified parameter
	*/
	void N_Event::setTimeStampPriority(int64_t timeStampPriority) {
		m_timeStampPriority = timeStampPriority;
	}

	/**
	* Returns the priority of the N_Event
	*/
	int N_Event::getPriority() {
		return m_priority;
	}

	/**
	* Sets the priority of the N_Event
	*/
	void N_Event::setPriority(int priority) {
		m_priority = priority;
	}
}
