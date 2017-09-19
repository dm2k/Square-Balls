#include <SFML/Graphics.hpp>
#include <iostream>

sf::Vector2f right = { 1.f, 0.f },
				left = { -1.f, 0.f},
				up = { 0.f, -1.f },
				down = { 0.f, 1.f };

float tileSize = 100.f;
std::string TileMap[10] = {
	" BB                       BBB           BBBBB",
	"  BB                    BBBB              BBB",
	"                                             ",
	"                    SSS                      ",
	" B                           BBBB          BB",
	"            BB               BBBB          BB",
	"B                                            ",
	"                                             ",
	"                                             ",
	" B                                         BB"
}
;


class Entity {
public:
	sf::Vector2f vel;
	sf::RectangleShape shape;


	virtual void update() = 0;
	void draw(sf::RenderWindow & window) {
		window.draw(shape);
	};
};

class Player: public Entity
{
public:
	
	Player(sf::Vector2f pos = {0.f, 0.f}) {
		shape.setPosition(pos);
		shape.setSize({ 100.f, 100.f });
		shape.setFillColor(sf::Color::Green);
	}

	void update() {
		shape.move(vel);
		vel *= 0.75f;
	}

};

int main() {

	std::cout << "Yeah";


	sf::RenderWindow window(sf::VideoMode(800, 600), "Square Balls!");
	window.setFramerateLimit(120);
	
	Player squareBall({ 50,50 });

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();


		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		{
			squareBall.vel += left;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		{
			squareBall.vel += right;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		{
			squareBall.vel += (up);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		{
			squareBall.vel += (down);
		}

		squareBall.update();

		window.clear();
		
		squareBall.draw(window);

		window.display();
	}


	return 0;
}