#include <SFML/Graphics.hpp>
#include "Movable.hpp"
#include "Object.hpp"

int main()
{
    /*
    screen +- configuration

    x:+  y:+  │ x:-  y:+
    ⚊⚊⚊⚊⚊⚊╊⚊⚊⚊⚊⚊⚊
    x:+  y:-  │ x:+  y:-
    
    */

    auto vm = sf::VideoMode(1300, 750);
    sf::RenderWindow window(vm, "Movements");
    window.setFramerateLimit(60);

    sf::Texture spaceshipTexture, earthTexture;

    if (!spaceshipTexture.loadFromFile("./spaceship.png"))
    {
        std::cerr << "failed to load spaceship texture" << std::endl;
        exit(1);
    }
    if (!earthTexture.loadFromFile("./earth.png"))
    {
        std::cerr << "failed to load earth texture" << std::endl;
        exit(1);
    }


    // 10^24 -> 10^12 - earth
    // 10^5 -> 10 - spaceship
    const sf::Vector2u earthSize(55.0f, 55.0f);
    const sf::Vector2u spaceshipSize(30, 50);
    const float earthMass = 5.72e14, spaceshipMass = 100;
    const float altitude = 200.0f;

    auto spaceship = m::SpaceShip(spaceshipMass, spaceshipTexture, spaceshipSize, altitude);
    auto earth = m::Earth(earthMass, earthTexture, earthSize);
    
    spaceship.body.setTexture(spaceshipTexture, true);
    earth.body.setTexture(earthTexture, true);

    auto force = sf::Vector2f(0, 0);
    float torque = 0.0f;
    bool uniform_flag = false;

    while (window.isOpen())
    {
        sf::Event event;
        float deltaTime = 1.0f / 60.0f;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::KeyPressed) {
                auto keycode = event.key.code;
                if (keycode == sf::Keyboard::Up) {
                    force.x += 10.0f;
                    force.y += 10.0f;
                }
            }
        }

        if (!uniform_flag) {
            torque = spaceship.getTorqueByEarth(earth);
            std::cout << "로켓이 " << torque << "(torque) 힘을 받아 등속 원운동" << std::endl;
        }
        else {
            torque = 0.0f;
        }

        spaceship.physics.updateAddForce(force, deltaTime);
        spaceship.physics.updateAngularPosition(torque, altitude, deltaTime);
        uniform_flag = true;

        window.clear();
        spaceship.update(&window, deltaTime);
        earth.update(&window, deltaTime);

        window.display();
    }

    return 0;
}