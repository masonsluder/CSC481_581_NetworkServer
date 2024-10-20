#include "n_transform.h"

namespace N_Components {

	/**
	* Constructs transform with all necessary fields
	*/
	N_Transform::N_Transform(Utils::Vector2D position, Utils::Vector2D size, Utils::Vector2D scale) {
		// Set position values
		m_position = new Utils::Vector2D();
		*m_position = position;

		// Set size and scale
		m_size = size;
		m_scale = scale;
	}

	/**
	* Destructor for the Transform
	*/
	N_Transform::~N_Transform() {
		delete m_position;
	}

	void N_Transform::update(void) {
		// modify position using offset
	}

	/**
	* Returns the position
	*/
	Utils::Vector2D* N_Transform::getPosition(void) {
		return m_position;
	}

	/**
	* Adds the parameter position vector to the Entity's current position vector
	* @param position: The position to add to the current position
	*/
	void N_Transform::updatePosition(Utils::Vector2D position) {
		*m_position = m_position->add(position);
	}

	/**
	* Sets the position using the two float values provided
	* @param positionX: The x component of the position
	* @param positionY: The y component of the position
	*/
	void N_Transform::setPosition(float positionX, float positionY) {
		m_position = new Utils::Vector2D(positionX, positionY);
	}

	/**
	* Sets the position using the two float values provided
	* @param positionX: The x component of the position
	* @param positionY: The y component of the position
	*/
	void N_Transform::setPosition(Utils::Vector2D other) {
		*m_position = other;
	}

	/**
	* Returns the height and width
	* @return m_size
	*/
	Utils::Vector2D N_Transform::getSize(void) {
		return m_size;
	}

	/**
	* Sets the height and width
	* @param width to set
	* @param height to set
	*/
	void N_Transform::setSize(float width, float height) {
		m_size = Utils::Vector2D(width, height);
	}

	/**
	* Returns the scale ratio of the entity
	* @returns m_scale the scale ratio of the entity as a 2D vector
	*/
	Utils::Vector2D N_Transform::getScale(void) {
		return m_scale;
	}

	/**
	* Sets the scale values
	* @param scaleX
	* @param scaleY
	*/
	void N_Transform::setScale(float scaleX, float scaleY) {
		m_scale = Utils::Vector2D(scaleX, scaleY);
	}

}
