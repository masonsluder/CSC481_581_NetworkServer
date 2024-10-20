#pragma once

#ifndef N_PLAYERINPUT_H
#define N_PLAYERINPUT_H

#include "n_GameObject.h"
#include "n_component.h"
#include "n_rigidBody.h"
#include "n_transform.h"

//#include "input.h"
#include "networkvector2D.h"

namespace N_Components {

	class N_PlayerInputPlatformer : public virtual N_Component {
	protected:
		// Whether the player can input
		bool m_canPressInput;
		// Whether the player is on the ground
		bool m_isGrounded;
		// The maximum speed the player moves at
		float m_maxMovementSpeed;
		// The vector that defines the force with which the palyer jumps
		Utils::Vector2D m_jumpVector;
		// Struct that takes in player inputs
		//InputHandler* m_inputHandler;
		// A reference to the GameObject parent that allows for inter-Object communication between components
		N_GameObject* m_parent;

	public:
		/* Default constructor */
		N_PlayerInputPlatformer();

		/* Constructor */
		N_PlayerInputPlatformer(float maxMovementSpeed, Utils::Vector2D jumpVector, N_GameObject* parentRef);

		void update() override;

		// Helper method to move the player transform
		void movePlayer(Utils::Vector2D movementVector, int axis, N_RigidBody* rb, N_Transform* transform);

		float getMaxSpeed();

		Utils::Vector2D getJumpVector();
	};

}

#endif
