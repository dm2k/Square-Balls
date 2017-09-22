#include <SFML/Graphics.hpp>
#include <iostream>

float ground = 600.f, 
	gt = 0.01f, 
	rub = 0.8f;

sf::Vector2f right = { 1.f, 0.f },
				left = { -1.f, 0.f},
				up = { 0.f, -1.f },
				down = { 0.f, 1.f };

float tileSize = 50.f;

std::string TileMap[10] = {
	"BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBb",
	"BBBB                    BBBB                                           B    B                                       B",
	"B                                                                      B    B                                       B",
	"B                   SSS                                 B              BBBBBB                  B                    B",
	"BB                           BBBB                      BBB                                                         BB",
	"B           BB               BBBBBBBBBB                  BBBBBBBBB                              B                   B",
	"B                                 BBB                   B   B  B                                  B                 B",
	"B                             BBBBB  BBB                B    B       B                            B    B            B",
	"B                           BB BBBB    BBBB            B      B         B                           B    B          B",
	"BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB"
}
;

int H = 10;
int W = TileMap[0].size();

sf::RectangleShape tmp_rect({ tileSize, tileSize });


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
	
	bool onGround;

	Player(sf::Vector2f pos = {0.f, 0.f}) {
		shape.setPosition(pos);
		shape.setSize({ tileSize*1.95f, tileSize*1.95f });
		shape.setFillColor(sf::Color::Green);
	}


	void Collision(int dir) {
		for (int i = shape.getPosition().y / tileSize; i < (shape.getPosition().y + shape.getSize().y) / tileSize; ++i)
			for (int j = shape.getPosition().x / tileSize; j < (shape.getPosition().x + shape.getSize().x) / tileSize; ++j)
			{

				if (dir == 0) {
					if (TileMap[i][j] == 'B') {
						if (vel.x > 0) {
							shape.setPosition(j * tileSize - shape.getSize().x, shape.getPosition().y);
							vel.x = 0;
						}
						if (vel.x < 0) {
							shape.setPosition(j * tileSize + tileSize, shape.getPosition().y);
							vel.x = 0;
						}

					}
				} else if (dir == 1) {
						if (TileMap[i][j] == 'B') {
							if (vel.y > 0) {
								shape.setPosition(shape.getPosition().x, i * tileSize - shape.getSize().y);
								vel.y = 0;
								onGround = true;

							}
							if (vel.y < 0) {
								shape.setPosition(shape.getPosition().x, i * tileSize + tileSize);
								vel.y = 0;
							}

						}

				}


				if (TileMap[i][j] == 'S') {
					TileMap[i][j] = ' ';
				}

			}
	};


	void update() {


		shape.move(vel.x, 0);

		Collision(0);

		//if (onGround)
			vel.x *= rub;
		//else
			//vel.x *= rub * 1.5f;


		if (!onGround && shape.getPosition().y >= ground) {
			shape.move(0, ground - shape.getPosition().y);
			onGround = true;
			vel.y = 0;
		}
		else {
			onGround = false;
			vel.y += gt;
			shape.move(0, vel.y);
		}

		Collision(1);

	}

};



int main() {

	std::cout << "Yeah";


	sf::RenderWindow window(sf::VideoMode(1800, 600+tileSize*2.f), "Square Balls!");
	window.setFramerateLimit(120);
	
	sf::View view(sf::FloatRect(0, 0, 1800, 600 + tileSize*2.f));
	
	window.setView(view);




	Player hero({ 250,250 });

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
			 hero.vel += left;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		{
			hero.vel += right;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		{
			if (hero.onGround) hero.vel += 5.f*(up);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		{
			hero.vel += (down);
		}

		hero.update();

		view.setCenter(hero.shape.getPosition());
		window.setView(view);

		window.clear();
		
		for (int i = 0; i < H; ++i)
		{
			for (int j = 0; j < W; ++j)
			{
				if (TileMap[i][j] == 'B')
				{
					tmp_rect.setFillColor(sf::Color::Red);
				}
				if (TileMap[i][j] == 'S')
				{
					tmp_rect.setFillColor(sf::Color::Cyan);
				}
				if (TileMap[i][j] == ' ')
				{
					continue;
				}
				tmp_rect.setPosition({ j*tileSize, i*tileSize });
				window.draw(tmp_rect);
			}
		}


		hero.draw(window);

		window.display();
	}


	return 0;
}