#include "n_GameObject.h"


/*
* Some references include: https://www.gamedeveloper.com/design/the-entity-component-system---an-awesome-game-design-pattern-in-c-part-1-
* http://entity-systems.wikidot.com/test-for-parallel-processing-of-components#cpp
* Textbook
*/

// Default constructor for GameObject. Adds a Transform component (required)
N_GameObject::N_GameObject(){
	// Add default GameObject values
	m_uuid = -1;
	addComponent<N_Components::N_Transform>(
		Utils::Vector2D(0, 0), 
		nullptr, 
		Utils::Vector2D(1, 1), 
		Utils::Vector2D(1, 1)
	);
}

// Constructor with fields for GameObject. Adds a Transform component (required)
N_GameObject::N_GameObject(float scaleX, float scaleY, float positionX, float positionY, float width, float height, Utils::Vector2D *cameraPos) {
	// Add default GameObject ID
	m_uuid = -1;
	// Add Transform component, which is required for all GameObjects
	addComponent<N_Components::N_Transform>(
		Utils::Vector2D(positionX, positionY),
		cameraPos,
		Utils::Vector2D(width, height),
		Utils::Vector2D(scaleX, scaleY)
	);
}

// Update all components attached to game object and the time step the object should be using for calculations
void N_GameObject::update(double deltaTimeInSecs) {
	m_currTimeStep = deltaTimeInSecs;

	// Iterate through each component and update it
	for (auto& [type, component] : m_components) {
		component->update();  // Update each component
	}
}

void N_GameObject::setUUID(int uuid) {
	this->m_uuid = uuid;
}

int N_GameObject::getUUID() {
	return m_uuid;
}

double N_GameObject::getDeltaTimeInSecsOfObject() {
	return m_currTimeStep;
}

void N_GameObject::from_json(const json& j) {
	// Get UUID
	if (j.contains("uuid")) {
		m_uuid = j["uuid"].get<int>();
	}
	// If it contains each component
	if (j.contains("transform")) {
		auto transformData = j["transform"];
		addComponent<N_Components::N_Transform>(
			Utils::Vector2D(transformData["position"]["x"], transformData["position"]["y"]),
			Utils::Vector2D(transformData["width"], transformData["height"]),
			Utils::Vector2D(transformData["scale"]["x"], transformData["scale"]["y"])
		);
	}

	if (j.contains("rigidbody")) {
		auto transformData = j["transform"];
		auto rigidBodyData = j["rigidbody"];
		addComponent<N_Components::N_RigidBody>(
			rigidBodyData["mass"],
			rigidBodyData["iskinematic"],
			SDL_Rect() = { // Use transform data to fill out the rectangle for now
				(int)transformData["position"]["x"],
				(int)transformData["position"]["y"],
				(int)((float)transformData["scale"]["x"] * (float)transformData["width"]),
				(int)((float)transformData["scale"]["y"] * (float)transformData["height"])
			},
			rigidBodyData["collidertype"],
			this
		);
	}

	if (j.contains("texturemesh")) {
		auto textureMeshData = j["texturemesh"];
		addComponent<N_Components::N_TextureMesh>(
			textureMeshData["texturefilepath"]
		);
	}
}

void N_GameObject::to_json(json& j) {
	// Set UUID
	j["uuid"] = m_uuid;

	// If it contains each component
	N_Components::N_Transform* transform = getComponent<N_Components::N_Transform>();
	if (transform) {
		j["transform"] = {
			{"position", {{"x", transform->getPosition()->x}, {"y", transform->getPosition()->y}}},
			{"width", transform->getSize().x},
			{"height", transform->getSize().y},
			{"scale", {{"x", transform->getScale().x}, {"y", transform->getScale().y}}}
		};
	}

	N_Components::N_RigidBody* rigidBody = getComponent<N_Components::N_RigidBody>();
	if (rigidBody) {
		j["rigidbody"] = {
			{"mass", rigidBody->getMass()},
			{"iskinematic", rigidBody->isKinematic()},
			{"collidertype", rigidBody->getColliderType()}
		};
	}

	N_Components::N_TextureMesh* textureMesh = getComponent<N_Components::N_TextureMesh>();
	if (textureMesh) {
		j["texturemesh"] = {
			{"texturefilepath", textureMesh->getTextureFilePath()}
		};
	}

}

// Example for main:
// Player player(1, 1, 0, 0, 50, 50, 10, "player_texture.png", false, true, 0, 10, 5.0f);
// StaticPlatform platform(1, 1, 100, 50, 200, 10, 100, "platform_texture.png", true, false);

// Others to add:
// - MovingPlatform: Server-side.  TextureMesh, RigidBody, MovementController?
// - SpawnPoint: DON'T need to add, can just be a GameObject, only needs Transform
// - DeathZone: RigidBody (for collisions)
// - SideBoundary: RigidBody (for collisions)
