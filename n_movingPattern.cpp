#include "n_movingPattern.h"

#include "n_transform.h"

namespace N_Components {

	N_MovingPattern::N_MovingPattern(Utils::Vector2D startPosition, Utils::Vector2D endPosition, float speed, bool pauseLength, N_GameObject* parentRef) {
		// Set start and end positions for movement
		m_startPosition = startPosition;
		m_endPosition = endPosition;
		// Set speed of moving objects
		m_speed = speed;
		// Sets the time each platform is paused for
		m_pauseLength = pauseLength;

		// Sets reference to GameObject parent
		m_parent = parentRef;

		// Sets defaults
		m_currPauseTimer = pauseLength;
		m_isReversed = false;
	}

	void N_MovingPattern::update() {
		// Get Transform component
		N_Components::N_Transform *transform = m_parent->getComponent<N_Components::N_Transform>();
		// Get time from object
		double deltaTimeInSecs = m_parent->getDeltaTimeInSecsOfObject();

		// Get distance from destination
		Utils::Vector2D distance;
		// If reversed, calculate distance to start position
		if (m_isReversed) {
			distance = m_startPosition.subtract(*transform->getPosition());
		}
		else { // If not, then calculate distance to end position
			distance = m_endPosition.subtract(*transform->getPosition());
		}

		// Calculate the distance to be moved in both x and y directions using speed and timestep as a magnitude
		Utils::Vector2D normalizedDistance =
			distance.normalizeVector().multConst(m_speed).multConst(deltaTimeInSecs);

		// Updates the position of the transform using the normalized distance calculated
		transform->updatePosition(normalizedDistance);

		// TODO: Implement destination swapping and pause timer activation so that the object properly moves
		// back and forth

	}

}
