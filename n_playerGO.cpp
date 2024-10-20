#include "n_playerGO.h"

#include <string>

N_PlayerGO::N_PlayerGO(float scaleX, float scaleY, float positionX, float positionY, float width, float height, float mass,
	std::string textureFilepath, bool isKinematic, float jumpVectorX, float jumpVectorY, float maxSpeed) {

	// Default UUID
	m_uuid = 0;

	// Add transform component
	addComponent<N_Components::N_Transform>(
		Utils::Vector2D(positionX, positionY),
		Utils::Vector2D(width, height),
		Utils::Vector2D(scaleX, scaleY)
	);

	// Add RigidBody component
	addComponent<N_Components::N_RigidBody>(
		mass,
		isKinematic,
		SDL_Rect() = { (int)positionX, (int)positionY, (int)(scaleX * width), (int)(scaleY * height) },
		0, // Default collider type
		this
	);

	// Add TextureMesh component
	addComponent<N_Components::N_TextureMesh>(textureFilepath);

	// Add PlayerInput Component (specifically the platforming controls currently)
	addComponent<N_Components::N_PlayerInputPlatformer>(
		maxSpeed,
		Utils::Vector2D(jumpVectorX, jumpVectorY),
		this
	);
}