#include <SDL.h>
#include <cmath>
#include <string>
#include <sstream>

#include "networkVector2D.h"


namespace Utils {

	/**
	* Vector2D default constructor
	*/
	Vector2D::Vector2D(void) {
		this->x = 0.0;
		this->y = 0.0;
	}

	/**
	 * Vector2D constructor
	 * @param x coordinate
	 * @param y coordinate
	 */
	Vector2D::Vector2D(float x, float y) {
		this->x = x;
		this->y = y;
	}

	/**
	 * Adds the given vector to this vector and returns the result.
	 * @param other Vector2D
	 * @returns The result of the addition between the two Vectors
	 */
	Vector2D Vector2D::add(Vector2D other) {
		return Vector2D(this->x + other.x, this->y + other.y);
	}

	/**
	 * Adds the given vector coordinates to this vector and returns the result.
	 * @param x coordinate to add
	 * @param y coordinate to add
	 * @returns The result of the addition between the two Vectors
	 */
	Vector2D Vector2D::add(float x, float y) {
		return Vector2D(this->x + x, this->y + y);
	}

	/**
	* Multiplies the vector by a constant and returns the result
	* @param constant The constant multiplying the Vector
	* @returns The result of the constant multiplication
	*/
	Vector2D Vector2D::multConst(float constant) {
		return Vector2D(this->x * constant, this->y * constant);
	}

	/**
	* Divides the vector by a constant and returns the result
	* @param constant The constant dividing the vector
	* @returns The result of the constant division
	*/
	Vector2D Vector2D::divideConst(float constant) {
		return Vector2D(this->x / constant, this->y / constant);
	}

	/**
	* Calculates and returns the magnitude of the vector
	* @returns The magnitude of the vector
	*/
	float Vector2D::getMagnitude(void) {
		return sqrt((pow(x, 2) + pow(y, 2)));
	}

	/**
	* Calculates the unit vector for this vector, then returns it
	* @returns The normalized vector
	*/
	Vector2D Vector2D::normalizeVector(void) {
		return this->divideConst(getMagnitude());
	}

	std::string Vector2D::toString() {
		// Create stringstream to stringify vector
		std::stringstream ss;
		// Create string in format X,Y
		ss << x << "," << y << "\n";
		return ss.str();
	}

	Vector2D* Vector2D::fromString(std::stringstream& ss) {
		std::string line;
		std::getline(ss, line);

		std::stringstream lineStream(line);
		std::string xStr, yStr;

		std::getline(lineStream, xStr, ',');
		std::getline(lineStream, yStr, '\n');

		//std::cout << "Printing Vector2D Test: " << xStr << "," << yStr << "\n";

		//std::cout << "Printing Vector2D Test LINE: " << line << "\n";

		float x = std::stof(xStr);
		float y = std::stof(yStr);

		return new Vector2D(x, y);
	}
}
