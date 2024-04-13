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

		virtual void update(sf::RenderWindow* window, float deltaTime) {
			body.setRotation(physics.getObjectRotation());
			body.setPosition(physics.getSFMLCoordinate(window->getSize()));
			//std::cout << body.getPosition().x << " " << body.getPosition().y << std::endl;
			window->draw(body);
		}
	};
}