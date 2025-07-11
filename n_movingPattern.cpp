#include "n_movingPattern.h"
#include "n_transform.h"

namespace N_Components {
	N_MovingPattern::N_MovingPattern(Utils::Vector2D startPosition, Utils::Vector2D endPosition, float speed, int pauseLength, N_GameObject* parentRef) {
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
		m_currPauseTimer = 0;
		m_isReversed = false;
	}

	void N_MovingPattern::update() {
		// Only update position if the MovingObject is not paused
		if (m_currPauseTimer <= 0) {

			// Get Transform component
			N_Components::N_Transform* transform = m_parent->getComponent<N_Components::N_Transform>();
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

			//std::cout << "Distance calculated: " << distance.toString() << "\nNormalizedDistance: " << normalizedDistance.toString() << "\n" <<
			//	"\nSpeed: " << m_speed << "\nDeltaTime: " << deltaTimeInSecs << "\n\n";
			
			Utils::Vector2D position = *transform->getPosition();

			// Cover cases in which it moves just horizontally or vertically
			if (m_startPosition.x == m_endPosition.x) { // If the object moves horizontally
				// Check for differences in start and end positions with y
				if (position.y <= m_startPosition.y) {
					m_isReversed = false;
					m_currPauseTimer = m_pauseLength;
					transform->setPosition(m_startPosition);
				}
				else if (position.y >= m_endPosition.y) {
					m_isReversed = true;
					m_currPauseTimer = m_pauseLength;
					transform->setPosition(m_endPosition);
				}
			}
			else if (m_startPosition.y == m_endPosition.y) { // If the object omves vertically
				// Check for differences in start and end positions with x
				if (position.x <= m_startPosition.x) {
					m_isReversed = false;
					m_currPauseTimer = m_pauseLength;
					transform->setPosition(m_startPosition);
				}
				else if (position.x >= m_endPosition.x) {
					m_isReversed = true;
					m_currPauseTimer = m_pauseLength;
					transform->setPosition(m_endPosition);
				}
			}
			else { // This is for if the moving platform moves on both axes
				// Detects if the object has reached either the start or end positions of its path
				if (position.lessThanOrEqualToXOrY(m_startPosition)) {
					m_isReversed = false;
					m_currPauseTimer = m_pauseLength;
					transform->setPosition(m_startPosition);
				}
				else if (position.greaterThanOrEqualToXOrY(m_endPosition)) {
					m_isReversed = true;
					m_currPauseTimer = m_pauseLength;
					transform->setPosition(m_endPosition);
				}
			}
		}
		else { // Decrement pause timer
			m_currPauseTimer--;
		}
	}

}
