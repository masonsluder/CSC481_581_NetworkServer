#pragma once

#ifndef N_MOVINGPATTERN_H
#define N_MOVINGPATTERN_H

#include "n_component.h"
#include "networkVector2D.h"
#include "n_GameObject.h"

namespace N_Components {

	class N_MovingPattern : public virtual N_Component {
	private:
		// The starting position of the GameObject when moving
		Utils::Vector2D m_startPosition;
		// The ending position that the GameObject is trying to reach as its destination
		Utils::Vector2D m_endPosition;

		// The speed at which the GameObject moves
		float m_speed;

		// The amount of time that the Object is stopped for when reaching a destination
		int m_pauseLength;
		// The current amount of time that has passed since reaching a destination
		int m_currPauseTimer;

		// Whether the platform is moving from Start->End or End->Start
		bool m_isReversed;

		// Reference to GameObject parent to communicate with other components
		N_GameObject* m_parent;

	public:

		N_MovingPattern(Utils::Vector2D startPosition, Utils::Vector2D endPosition, float speed, int pauseLength, N_GameObject *parentRef);

		~N_MovingPattern() {}

		void update() override;

	};

}

#endif
