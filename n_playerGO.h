#pragma once

#ifndef N_PLAYERGO_H
#define N_PLAYERGO_H

#include "n_GameObject.h"
#include "n_transform.h"
#include "n_rigidBody.h"
#include "n_textureMesh.h"
#include "n_playerInput.h"

// - Player: TextureMesh, RigidBody (collision), PlayerInput
class N_PlayerGO : public N_GameObject {
public:
	N_PlayerGO(float scaleX, float scaleY, float positionX, float positionY, float width, float height, float mass,
		std::string textureFilepath, bool isKinematic, float jumpVectorX, float jumpVectorY, float maxSpeed);

};

#endif

