#include "n_MovingObject.h"


N_MovingObject::N_MovingObject(float scaleX, float scaleY, float positionX, float positionY,
	float width, float height, float mass, std::string textureFilepath,
	Utils::Vector2D startPosition, Utils::Vector2D endPosition, float speed, int pauseLength) {

	// Add Transform Component
	addComponent<N_Components::N_Transform>(
		Utils::Vector2D(positionX, positionY),
		Utils::Vector2D(width, height),
		Utils::Vector2D(scaleX, scaleY)
	);
	// Add RigidBody Component
	addComponent<N_Components::N_RigidBody>(
		mass,
		true, // MovingObject should always be kinematic for the time being as collisions are handled client-side
		SDL_Rect() = { (int)positionX, (int)positionY, (int)(scaleX * width), (int)(scaleY * height) },
		0 // Default collider type (normal collision)
	);
	// Add TextureMesh Component
	addComponent<N_Components::N_TextureMesh>(textureFilepath);
	// Add MovingPattern Component
	addComponent<N_Components::N_MovingPattern>(
		startPosition,
		endPosition,
		speed,
		pauseLength,
		this // gives component reference to parent GameObject
	);
}
