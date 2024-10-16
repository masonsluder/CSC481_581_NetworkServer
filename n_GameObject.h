#pragma once

#ifndef N_GAMEOBJECT_H
#define N_GAMEOBJECT_H

#include "n_component.h"
#include "n_transform.h"

#include "networkVector2D.h"

#include <typeindex>
#include <iostream>
#include <map>
#include <vector>

/*
* Some references include: https://www.gamedeveloper.com/design/the-entity-component-system---an-awesome-game-design-pattern-in-c-part-1-
* http://entity-systems.wikidot.com/test-for-parallel-processing-of-components#cpp
* Textbook
*/
class N_GameObject {
protected:
    // UUID
    int m_uuid;
    // Store components in a map using std::type_index as the key
    std::map<std::type_index, std::unique_ptr<N_Components::N_Component>> m_components;
    // The deltaTime for the object passed down by the Manager
    double m_currTimeStep;

    // Put stuff in struct?
public:
    // Default constructor for GameObject. Adds a Transform component (required)
    N_GameObject();

    // Constructor with fields for GameObject. Adds a Transform component (required)
    N_GameObject(float scaleX, float scaleY, float positionX, float positionY, float width, float height, Utils::Vector2D* cameraPos);

    // Function to add component to the game object
    template<typename T, typename... Args>
    void addComponent(Args&&... args) {
        // Create a unique_ptr for the component and store it using its typeid
        m_components[typeid(T)] = std::make_unique<T>(std::forward<Args>(args)...);
    }

    /**
    * Uses the type given in the brackets to determine the component to get, then returns it:
    * Usage: getComponent<TYPE>()
    *
    * @return Returns the component specified by the template type
    */
    template<typename T>
    T* getComponent() {

        //std::cout << "I made it in getComponent\n";

        auto it = m_components.find(typeid(T));
        if (it != m_components.end()) {

            return dynamic_cast<T*>(it->second.get());  // Safely cast the component to its type
        }
        return nullptr;
    }

    // Update all components attached to game object
    void update(double deltaTimeInSecs);

    void setUUID(int uuid);

    int getUUID();

    double getDeltaTimeInSecsOfObject();
};



// Example for main:
// Player player(1, 1, 0, 0, 50, 50, 10, "player_texture.png", false, true, 0, 10, 5.0f);
// StaticPlatform platform(1, 1, 100, 50, 200, 10, 100, "platform_texture.png", true, false);

// Others to add:
// - MovingPlatform: Server-side.  TextureMesh, RigidBody, MovementController?
// - SpawnPoint: DON'T need to add, can just be a GameObject, only needs Transform
// - DeathZone: RigidBody (for collisions)
// - SideBoundary: RigidBody (for collisions)

#endif
