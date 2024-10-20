#include "n_rigidBody.h"

namespace N_Components {

	N_RigidBody::N_RigidBody(float mass, bool isKinematic, SDL_Rect collider, int colliderType) {
		// Create new vectors at (0,0) for velocity and acceleration
		m_velocity = new Utils::Vector2D();
		m_acceleration = new Utils::Vector2D();
		// Set mass and whether the object is able to move/be able to be affected by physics
		m_mass = mass;
		m_isKinematic = isKinematic;
		// Set collider options and whether it serves as a trigger
		m_collider = collider;
		m_colliderType = colliderType;
	}

	N_RigidBody::~N_RigidBody() {
		// Deletes velocity and acceleration pointers
		delete m_velocity;
		delete m_acceleration;
	}

	void N_RigidBody::update() {
		// No physics are calculated server-side
	}

	/**
	* Returns the mass of the GameObject
	*/
	float N_RigidBody::getMass() {
		return m_mass;
	}

	/**
	* Sets the mas of the GameObject
	*/
	void N_RigidBody::setMass(float mass) {
		m_mass = mass;
	}

	/**
	* Returns a pointer to the Object's velocity
	*/
	Utils::Vector2D* N_RigidBody::getVelocity() {
		return m_velocity;
	}

	/**
	* Adds the given vector to the current velocity
	*/
	void N_RigidBody::updateVelocity(Utils::Vector2D other) {
		*m_velocity = m_velocity->add(other);
	}

	/**
	* Sets the velocity vector to the given parameter vector
	*/
	void N_RigidBody::setVelocity(Utils::Vector2D other) {
		*m_velocity = other;
	}

	/**
	* Returns a pointer to the acceleration of the GameObject
	*/
	Utils::Vector2D* N_RigidBody::getAcceleration() {
		return m_acceleration;
	}

	/**
	* Updates the acceleration by adding the other parameter to the current value
	*/
	void N_RigidBody::updateAcceleration(Utils::Vector2D other) {
		*m_acceleration = m_acceleration->add(other);
	}

	/**
	* Sets the acceleration value to the provided vector
	*/
	void N_RigidBody::setAcceleration(Utils::Vector2D other) {
		*m_acceleration = other;
	}

	void N_RigidBody::setColliderCoordinates(float x, float y) {
		m_collider.x = x;
		m_collider.y = y;
	}

	void N_RigidBody::setColliderCoordinates(Utils::Vector2D newCoords) {
		m_collider.x = newCoords.x;
		m_collider.y = newCoords.y;
	}

	bool N_RigidBody::isKinematic() {
		return m_isKinematic;
	}

	int N_RigidBody::getColliderType() {
		return m_colliderType;
	}

}
