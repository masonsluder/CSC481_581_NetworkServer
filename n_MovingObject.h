#pragma once

#ifndef N_MOVINGOBJECT_H
#define N_MOVINGOBJECT_H

#include "n_GameObject.h"
#include "networkVector2D.h"

class N_MovingObject : public N_GameObject {

	N_MovingObject(float scaleX, float scaleY, float positionX, float positionY, float width, float height, float mass,
		std::string textureFilepath, Utils::Vector2D startPosition, Utils::Vector2D endPosition, float speed, int pauseLength);
	
};

#endif
