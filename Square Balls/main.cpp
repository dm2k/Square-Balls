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
	"B                       BBBB                                           BBBBBB                                       B",
	"B                                                                      BBBBBB                         SSS           B",
	"BBBB                SSS                               BBB              BBBBBB                  BB                   B",
	"B                            BBBBBBBBB     SSS        BBBB                                     BBBB                 B",
	"B           BB               BBBBBBBBBB               BBBBBBBBBBBB                             BBBBBB               B",
	"B        BBBBBBB             BBBBBBBBBB               BBBBBBBBBBBB                             BBBBBBBB        SSSSSB",
	"B        BBBBBBB             BBBBBBBBBBB         SSSSSSSSSSBBBBBBBBBBB       SSS               BBBBBBBBBB      SSSSSB",
	"B        BBBBBBB            BBBBBBBBBBBBBBB      SSSSSSSSSSBBBBBBBBBBBBBB                      BBBBBBBBBBB     SSSSSB",
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
	bool onGround;


	virtual void update() = 0;
	void draw(sf::RenderWindow & window) {
		window.draw(shape);
	};

	int Collision(int dir) {
		int res = 0;
		for (int i = shape.getPosition().y / tileSize; i < (shape.getPosition().y + shape.getSize().y) / tileSize; ++i)
			for (int j = shape.getPosition().x / tileSize; j < (shape.getPosition().x + shape.getSize().x) / tileSize; ++j)
			{

				if (dir == 0) {
					if (TileMap[i][j] == 'B') {
						if (vel.x > 0) {
							shape.setPosition(j * tileSize - shape.getSize().x, shape.getPosition().y);
							vel.x = 0.f;
							res += 1;
						}
						if (vel.x < 0) {
							shape.setPosition(j * tileSize + tileSize, shape.getPosition().y);
							vel.x = 0.f;
							res += 2;
						}

					}
				}
				else if (dir == 1) {
					if (TileMap[i][j] == 'B') {
						if (vel.y > 0) {
							shape.setPosition(shape.getPosition().x, i * tileSize - shape.getSize().y);
							vel.y = 0;
							onGround = true;
							res += 4;
						}
						if (vel.y < 0) {
							shape.setPosition(shape.getPosition().x, i * tileSize + tileSize);
							vel.y = 0;
							res += 8;
						}

					}

				}


				if (TileMap[i][j] == 'S') {
					TileMap[i][j] = ' ';
					res += 16;
				}

			}
		return res;
	};
};


class Enemy : public Entity
{
public:

	bool alive;

	Enemy(sf::Vector2f pos = { 0.f, 0.f }) {
		shape.setPosition(pos);
		shape.setSize({ tileSize*0.95f, tileSize*0.95f });
		shape.setFillColor(sf::Color::Blue);
		alive = true;
		vel.x = 1.f;
	}


	void update() {


		shape.move(vel.x, 0);

		int isHit = Collision(0);

		//if (onGround)
		if ((isHit & 1) == 1)
			vel.x = -1.f;
		if ((isHit & 2) == 2)
			vel.x = 1.f;
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

	};


};

class Player: public Entity
{
public:
	
	

	Player(sf::Vector2f pos = {0.f, 0.f}) {
		shape.setPosition(pos);
		shape.setSize({ tileSize*1.95f, tileSize*1.95f });
		shape.setFillColor(sf::Color::Green);
	}


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

	};

};



int main() {

	std::cout << "Yeah";


	sf::RenderWindow window(sf::VideoMode(1800, 600+tileSize*2.f), "Square Balls!");
	window.setFramerateLimit(120);
	
	sf::View view(sf::FloatRect(0, 0, 1800, 600 + tileSize*2.f));
	
	window.setView(view);


	Enemy goodboy({ 800,300 });	

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

		goodboy.update();
		hero.update();

		if (hero.shape.getGlobalBounds().intersects(goodboy.shape.getGlobalBounds())) {
			hero.shape.setFillColor(goodboy.shape.getFillColor());
		}

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

		goodboy.draw(window);
		hero.draw(window);

		window.display();
	}


	return 0;
}