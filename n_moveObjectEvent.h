#pragma once

#ifndef NMOVEOBJECTEVENT_H
#define NMOVEOBJECTEVENT_H


#include "n_event.h"
#include "networkVector2D.h"
#include "n_GameObject.h"

#include <zmq.hpp>

namespace N_Events {
	class N_MoveObjectEvent : public virtual N_Event {
	private:

	public:

		/**
		* Constructor for N_MoveObjectEvent
		*/
		N_MoveObjectEvent(std::vector<N_GameObject*> goRef, int64_t timeStampPriority, int priority, zmq::socket_t* socket);

		void onEvent() const override;

	};

}


#endif