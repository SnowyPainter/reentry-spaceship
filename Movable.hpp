#pragma once

#include <iostream>
#include <cmath>

# define M_PI 3.14159265358979323846
# define ADJUST -1.0f
# define ANGLE_ADJUST 90

namespace m {
	
	class Movable {
	private:
		float absVec(sf::Vector2f vec) {
			return sqrt(pow(vec.x, 2) + pow(vec.y, 2));
		}
		float differScalar = 0;
	protected:
		float mass, angle = 0.0f, angularVelocity = 0.0f;
		sf::Vector2f position, angularPosition;
		sf::Vector2f deltaCircularVelocity;
		sf::Vector2f velocity;
	public:

		Movable() {
			this->mass = 1.0f;
			this->position = sf::Vector2f(0, 0);
			this->angularPosition = sf::Vector2f(0, 0);
			this->velocity = sf::Vector2f(0, 0);
		}
		Movable(float mass) {
			this->mass = mass;
			this->position = sf::Vector2f(0, 0);
			this->angularPosition = sf::Vector2f(0, 0);
			this->velocity = sf::Vector2f(0, 0);
		}
		float getMass() { return mass; }

		void updateAddForce(sf::Vector2f force, float deltaTime) {
			force *= ADJUST;
			auto a = force / mass;
			this->velocity += a * deltaTime; //a * dt
			this->position += this->velocity * deltaTime; //this-velocity * dt
		}
		void updateAngularPosition(float torque, float r, bool stop_rotate, bool stop_revol, float deltaTime) {
			torque *= ADJUST;

			float alpha = torque / mass;
			this->angularVelocity += alpha * deltaTime;
			this->angle += angularVelocity * deltaTime;
			if (!stop_revol)
				this->angularPosition = sf::Vector2f(cos(angle) * r, sin(angle) * r) + this->position;
			else
				this->angularPosition = this->position;
			if (!stop_rotate) {
				sf::Vector2f tangentDirection(-angularPosition.y, angularPosition.x);
				this->deltaCircularVelocity = angularVelocity * tangentDirection;
			}
		}

		sf::Vector2f getSFMLCoordinate(sf::Vector2u windowSize) {
			auto pos = this->position;
			pos += angularPosition - this->position;
			pos += sf::Vector2f(windowSize.x / 2, windowSize.y / 2);
			return pos;
		}


		float getObjectRotation() {
			float magnitude = std::sqrt(deltaCircularVelocity.x * deltaCircularVelocity.x + deltaCircularVelocity.y * deltaCircularVelocity.y);
			sf::Vector2f unitVector(deltaCircularVelocity.x / magnitude, deltaCircularVelocity.y / magnitude);
			float rotation = std::atan2(unitVector.y, unitVector.x) * 180 / M_PI;
			return rotation + ANGLE_ADJUST;
		}

		sf::Vector2f angularDv() {
			return this->deltaCircularVelocity;
		}

		bool isStopped() {
			auto absVelocity = absVec(deltaCircularVelocity + velocity);
			auto absDv = absVec(deltaCircularVelocity);
			if (absDv - absVelocity >= differScalar)
				differScalar = absDv - absVelocity;
			else
				return true;
			return false;

		}

	};
}