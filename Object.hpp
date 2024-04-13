#pragma once

#include <SFML/Graphics.hpp>
#include "Movable.hpp"

#define G 0.00000000000667

namespace m {

	class Object {
	protected:
		
	public:
		sf::Vector2u size;
		sf::Sprite body = sf::Sprite();
		m::Movable physics;
		Object(float mass, sf::Texture texture, sf::Vector2u size) {
			this->size = size;
			auto textureSize = texture.getSize();
			//setting texture here, bug occurs.
			body.setScale((float)size.x / textureSize.x, (float)size.y / textureSize.y);
			body.setOrigin(sf::Vector2f((float)this->size.x / 2, (float)this->size.y / 2));
			physics = m::Movable(mass);
		}
		virtual void update(sf::RenderWindow* window, float deltaTime) {
			body.setPosition(physics.getSFMLCoordinate(window->getSize()));
			window->draw(body);
		}
	};
	
	class Earth : public Object {
	public:

		Earth(float mass, sf::Texture texture, sf::Vector2u size) : Object(mass, texture, size) {
			
		}

		float distanceFromSurface(Object obj) {
			sf::Vector2f center1 = body.getPosition();
			sf::Vector2f center2 = obj.body.getPosition();
			float dx = center2.x - center1.x;
			float dy = center2.y - center1.y;
			
			float distance = std::sqrt(dx * dx + dy * dy);
			distance -= (size.x) / 2 + (obj.size.y) / 2;
			return distance;
		}

		virtual void update(sf::RenderWindow* window, float deltaTime) {
			body.setPosition(physics.getSFMLCoordinate(window->getSize()));
			//std::cout << body.getPosition().x << " " << body.getPosition().y << std::endl;
			window->draw(body);
		}
	};

	class SpaceShip : public Object {
	public:
		float altitude = 0.0f;

		SpaceShip(float mass, sf::Texture texture, sf::Vector2u size, float altitude) : Object(mass, texture, size) {
			this->altitude = altitude;
		}

		float getTorqueByEarth(Earth earth) {
			float earthRadius = earth.size.x / 2;
			float g = (earth.physics.getMass() * G) / pow(altitude - earthRadius, 2);
			return earthRadius * (physics.getMass() * g);
		}

		float getObjectRotationFace2Earth(Earth earth) {
			sf::Vector2f diff = earth.body.getPosition() - body.getPosition();
			float angleRadians = std::atan2(diff.y, diff.x);
			return angleRadians * (180.0f / M_PI) + ANGLE_ADJUST + 180;
		}

		sf::Vector2f getForceToReverseFoward() {
			auto dv = physics.angularDv();
			auto powvec = sf::Vector2f(dv.x * dv.x, dv.y * dv.y);
			auto f = ADJUST * ((physics.getMass() * powvec) / altitude);
			float x_sign = dv.x > 0 ? 1 : -1;
			float y_sign = dv.y > 0 ? 1 : -1;
			f.x *= x_sign * ADJUST;
			f.y *= y_sign * ADJUST;
			return f;
		}

		virtual void update(sf::RenderWindow* window, float deltaTime) {
			window->draw(body);
		}
	};
}