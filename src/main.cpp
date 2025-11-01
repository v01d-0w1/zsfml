#include <SFML/Graphics.hpp>

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "Centered Green Circle");
    sf::CircleShape circle(150.0f); // Larger radius
    circle.setFillColor(sf::Color::Green);
    circle.setOutlineColor(sf::Color::Black);
    circle.setOutlineThickness(3.0f);

    // Center the circle by setting its origin to its center
    circle.setOrigin(circle.getRadius(), circle.getRadius());
    circle.setPosition(window.getSize().x / 2.0f, window.getSize().y / 2.0f);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event)) // Fixed: pollEvent (not pol1Event)
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        window.clear(sf::Color::Black);
        window.draw(circle);
        window.display();
    }

    return 0;
}
