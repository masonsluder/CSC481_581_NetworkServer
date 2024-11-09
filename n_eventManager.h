#pragma once

#ifndef N_EVENTMANAGER_H
#define N_EVENTMANAGER_H

#include "n_event.h"
//#include "gameObject.h"

#include <queue>
#include <vector>
#include <typeindex>

class N_GameObject;

class N_EventManager {
private:
	// Queue of Events that are dispatched when their timestamp is less than or equal to the current time
	// PriorityQueue<Event, Storage container, Greatest to least/least to greatest>
	std::priority_queue<N_Events::N_Event*, std::vector<N_Events::N_Event*>, std::greater<N_Events::N_Event*>> m_eventQueue;
	// Maintains the history of all GameObjects that are registered to a given Event
	std::map<std::type_index, std::vector<N_GameObject*>> m_eventRegistry;

public:
	/**
	* Constructor for EventManager that intializes the event queue
	*/
	N_EventManager();

	/**
	* Destructor for EventManager that deletes each Event stored in the manager
	*/
	~N_EventManager();

	/**
	* Registers a vector of GameObjects to a given Event
	*/
	template <typename E>
	void registerEvent(N_GameObject* gameObject);

	/**
	* Adds the given event into the event queue
	* @param Event to add
	*/
	void raiseEvent(N_Events::N_Event* event);

	/**
	* Instantly raises an Event and activates it, skips queue (primarily to preserve multi-threaded nature of networked events, maybe temp)
	* @param Event to activate
	*/
	void raiseEventInstantly(N_Events::N_Event* event);

	/**
	* Adds the given event into the event queue
	* @param timeStampPriority: The time that the event should be executed
	* @param priority: The priority of each event outside of the time priority
	*/
	template <typename E>
	void raiseEventAll(int64_t timeStampPriority, int priority);

	/**
	* Dispatches any events in the priority queue which
	* meet the time and priority requirements
	*/
	void dispatchEvents(int64_t timeStampPriority);
};

#endif
