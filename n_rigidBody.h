#pragma once

#ifndef N_RIGIDBODY_H
#define N_RIGIDBODY_H

#include "n_component.h"
#include "networkVector2D.h"

#include <SDL.h>

namespace N_Components {

	class N_RigidBody : public virtual N_Component {
	protected:
		// Whether the object moves and is affected by physics
		bool m_isKinematic;
		// The mass of the object
		float m_mass;

		// The velocity of the object
		Utils::Vector2D *m_velocity;
		// The acceleration of the object
		Utils::Vector2D *m_acceleration;

		// Determines the type of collider is being used for the RigidBody (0 = normal collider, 1 = death zone, 2 = boundary)
		int m_colliderType;
		// The collider of the GameObject
		SDL_Rect m_collider;

	public:

		N_RigidBody(float mass, bool isKinematic, SDL_Rect collider, bool isTrigger);

		~N_RigidBody() override;

		void update() override;

		/**
		* Returns the mass of the GameObject
		*/
		float getMass();

		/**
		* Sets the mas of the GameObject
		*/
		void setMass(float mass);

		/**
		* Returns a pointer to the Object's velocity
		*/
		Utils::Vector2D* getVelocity();

		/**
		* Adds the given vector to the current velocity
		*/
		void updateVelocity(Utils::Vector2D other);

		/**
		* Sets the velocity vector to the given parameter vector
		*/
		void setVelocity(Utils::Vector2D other);

		/**
		* Returns a pointer to the acceleration of the GameObject
		*/
		Utils::Vector2D* getAcceleration();

		/**
		* Updates the acceleration by adding the other parameter to the current value
		*/
		void updateAcceleration(Utils::Vector2D other);

		/**
		* Sets the acceleration value to the provided vector
		*/
		void setAcceleration(Utils::Vector2D other);
	};

}

#endif
