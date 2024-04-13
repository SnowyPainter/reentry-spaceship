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

    auto reverseForwardForce = sf::Vector2f(0, 0);
    float torque = 0.0f;
    bool uniform_flag = false, reverse_flag = false, reverse_finished_flag = false;

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
                
                if (!reverse_flag && keycode == sf::Keyboard::Space) {
                    reverse_flag = true;
                    reverseForwardForce = spaceship.getForceToReverseFoward();
                    std::cout << "로켓이 " << reverseForwardForce.x << ", " << reverseForwardForce.y << " 힘으로 역추진" << std::endl;
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
        if (reverse_flag) {
            reverseForwardForce = spaceship.getForceToReverseFoward();
        }
        if (reverse_finished_flag) {
            //역추진 완료, 지구에 착륙
        }

        spaceship.physics.updateAddForce(reverseForwardForce, deltaTime);
        spaceship.physics.updateAngularPosition(torque, altitude, reverse_finished_flag, deltaTime);
        
        if (spaceship.physics.isStopped() && reverse_flag) {
            std::cout << "로켓의 역추진 완료" << std::endl;
            reverse_finished_flag = true;
            reverse_flag = false;
            reverseForwardForce = sf::Vector2f(0, 0);
        }

        window.clear();
        spaceship.update(&window, deltaTime);
        earth.update(&window, deltaTime);

        window.display();
        
        uniform_flag = true;
    }

    return 0;
}