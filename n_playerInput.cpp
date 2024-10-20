#include "n_playerInput.h"

#include "n_GameObject.h"
//#include "physicsCalculator.h"

#include <SDL.h>

namespace N_Components {

	N_PlayerInputPlatformer::N_PlayerInputPlatformer() {
		m_maxMovementSpeed = 0;
		m_jumpVector = Utils::Vector2D(1, 1);
		m_parent = nullptr;
		m_canPressInput = true;
		m_isGrounded = false;
	}

	N_PlayerInputPlatformer::N_PlayerInputPlatformer(float maxMovementSpeed, Utils::Vector2D jumpVector, N_GameObject* parentRef) {
		m_maxMovementSpeed = maxMovementSpeed;
		m_jumpVector = jumpVector;
		m_parent = parentRef;
		m_canPressInput = true;
		m_isGrounded = false;
	}

	void N_PlayerInputPlatformer::movePlayer(Utils::Vector2D movementVector, int axis, N_RigidBody* rb, N_Transform* transform) {
		// None on server
	}

	void N_PlayerInputPlatformer::update() {
		// No updating movement on server
	}

	float N_PlayerInputPlatformer::getMaxSpeed() {
		return m_maxMovementSpeed;
	}

	Utils::Vector2D N_PlayerInputPlatformer::getJumpVector() {
		return m_jumpVector;
	}
}
