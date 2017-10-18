#include <SFML/Graphics.hpp>
#include <iostream>
#include <list>
#include <map>
#include <random>
#include <cmath>



sf::Vector2f right = { 1.f, 0.f },
				left = { -1.f, 0.f},
				up = { 0.f, -1.f },
				down = { 0.f, 1.f };

float tileSize = 50.f;

sf::Vector2f operator*(float m, sf::Vector2f v) {
	return { v.x * m, v.y * m };
}


std::string TileMap[25] = {
	"BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB",
	"B                       BBBB                                           BBBBBB                                       B",
	"B                                                                      BBBBBB                         SSS           B",
	"B                                                                      BBBBBB                         SSS           B",
	"B                                                                      SSSSSS                         BBBBBBBBBBB   B",
	"B                                                                                                     SSS           B",
	"B                   BBBBBBBBBBBBBBBBBBBBBB                             BBBBBB                         SSS           B",
	"B                                                                      BBBBBB                         SSS           B",
	"B                                                                      BBBBBB                         SSS           B",
	"B    BBBBBBBBBBBBBBBBBBB                                               BBBBBB                         SSS           B",
	"B                                                                                             BBBBBBBBBSBBBBBBBB    B",
	"B                                                                      BBBBBB                         SSS           B",
	"B                                                                      BBBBBB                         SSS           B",
	"BBBBBB                              BBBBBBBBBBBBBBBBB                  BBBBBB                         SSS           B",
	"BSSSSS                                                                 BBBBBB                         SSS           B",
	"B                                                                                                     SSS           B",
	"B                                                                                                     SSS           B",
	"B                                                                      BBBBBB                         SSS           B",
	"BBBBBB              SSS                               BBB              BBBBBB                  BB                   B",
	"B                            BBBBBBBBB     SSS        BBBB                                     BBBB                 B",
	"B           BB               BBBBBBBBBB               BBBBBBBBBBBB                             BBBBBB               B",
	"B        BBBBBBB             BBBBBBBBBB               BBBBBBBBBBBB                             BBBBBBBB        SSSSSB",
	"B        BBBBBBB             BBBBBBBBBBB         SSSSSSSSSSBBBBBBBBBBB       SSS               BBBBBBBBBB      SSSSSB",
	"B        BBBBBBB            BBBBBBBBBBBBBBB      SSSSSSSSSSBBBBBBBBBBBBBB                      BBBBBBBBBBB     SSSSSB",
	"BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB"
}
;

int H = 25;
int W = TileMap[0].size();

float ground = H * tileSize,
	gt = 0.01f,
	rub = 0.8f;



sf::RectangleShape tmp_rect({ tileSize, tileSize });

const int i_up = 8, i_down = 4, i_left = 2, i_right = 1;

class Entity {
public:
	sf::Vector2f vel;
	sf::RectangleShape shape;
	bool onGround;
	bool exists;

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
							shape.setPosition(j * tileSize - shape.getSize().x - 3, shape.getPosition().y);
							//vel.x = 0.f;
							res += i_right;
						}
						if (vel.x < 0) {
							shape.setPosition(j * tileSize + tileSize + 3, shape.getPosition().y);
							//vel.x = 0.f;
							res += i_left;
						}

					}
				}
				else if (dir == 1) {
					if (TileMap[i][j] == 'B') {
						if (vel.y > 0) {
							shape.setPosition(shape.getPosition().x, i * tileSize - shape.getSize().y-1);
							//vel.y = 0;
							onGround = true;
							res += i_down;
						}
						if (vel.y < 0) {
							shape.setPosition(shape.getPosition().x, i * tileSize + tileSize + 1);
							//vel.y = 0;
							res += i_up;
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

class Bullet : public Entity
{
public:

	bool active;
	sf::Vector2f base_vel;
	int timer;

	Bullet(sf::Vector2f pos = { 0.f, 0.f }, sf::Vector2f v = {10.f, 0.f}) {
		exists = true;
		timer = 500;
		shape.setPosition(pos);
		shape.setSize({ tileSize, tileSize/8 });
		shape.setFillColor(sf::Color::Magenta);
		active = true;
		vel = v;
		base_vel = v;
	}


	void update() {
		
		shape.move(vel.x, 0);

		int isHit;

		isHit= Collision(0);

		if (isHit == 1) {
			vel.x = -vel.x;
		}
		else if (isHit == 2) {
			vel.x = - vel.x;
		}

		shape.move(0, vel.y);

		isHit = Collision(1);

		if (isHit == 4 || isHit == 8) {
			vel.y = -vel.y;
		}

		vel.y += gt;

		--timer;

		if (timer <= 0) {
			exists = false;
		}

	};


};

class Enemy : public Entity
{
public:

	bool alive;

	Enemy(sf::Vector2f pos = { 0.f, 0.f }, sf::Color ecolor = sf::Color::Blue) {
		exists = true;
		shape.setPosition(pos);
		shape.setSize({ tileSize / 4 * 3, tileSize / 4 * 3 });
		shape.setFillColor(ecolor);
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


		if (!onGround  && shape.getPosition().y >= ground) {
			shape.move(0, ground - shape.getPosition().y);
			onGround = true;
			vel.y = 0;
		}
		else {
			onGround = false;
			vel.y += gt;
			shape.move(0, vel.y);
		}

		isHit = Collision(1);

		if (isHit & 4) {
			vel.y = 3.f * (up.y);
		}

	};


};

const int weapon_delay = 10;

class Player: public Entity
{
public:

	float dir;
	int weapon_timer;
	bool weapon_ready;

	Player(sf::Vector2f pos = {0.f, 0.f}) {
		exists = true;
		shape.setPosition(pos);
		shape.setSize({ tileSize/2 - 5.f, tileSize - 5.f });
		shape.setFillColor(sf::Color::Green);
		dir = 1.f;
		weapon_ready = true;
		weapon_timer = weapon_delay;
	}


	void update() {

		if (!weapon_ready) {
			--weapon_timer;

			if (weapon_timer == 0) {
				weapon_ready = true;
				weapon_timer = weapon_delay;
			}
		}


		if (vel.x != 0) {
			dir = vel.x > 0 ? 1.f : -1.f;
		}

		shape.move(vel.x, 0);

		Collision(0);

		//if (onGround)
			vel.x *= rub;
		//else
			//vel.x *= rub * 1.5f;

		/*
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
		*/

			

		if (!onGround) {
			shape.move(0, vel.y);
			vel.y += gt;
		}

		int ay = Collision(1);

		if (ay == 4 || ay == 8) {
			vel.y = 0.f;
			onGround = true;
		}
		else {
			onGround = false;
		}

	};

};



int main() {

	std::cout << "Yeah";


	sf::RenderWindow window(sf::VideoMode(1800, 600+tileSize*2.f), "Square Balls!");
	window.setFramerateLimit(120);
	
	sf::View view(sf::FloatRect(0, 0, 1800, 600 + tileSize*2.f));
	
	window.setView(view);

	//Bullet maimer({ -1000,-1000 });

	//Enemy goodboy({ 800,300 });	

	Player hero({ 250,250 });


	std::list<Entity*> entities;
	std::list<Entity*>::iterator it;

	std::list<Entity*> enemies;
	std::list<Entity*>::iterator it_enemy;

	std::list<Entity*> bullets;
	std::list<Entity*>::iterator it_bullet;

	enemies.push_back(new Enemy({ 800,300 }));
	enemies.push_back(new Enemy({ 300,200 }, sf::Color::White));
	
	bool pop_enemy = false;
	//entities.push_back(&maimer);


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
			if (hero.onGround) { hero.onGround = false; hero.vel = 5.f*(up); }
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		{
			hero.vel += (down);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
			//maimer.shape.setPosition( hero.shape.getPosition());
			//maimer.active = true;

			if (hero.weapon_ready) {
				sf::Vector2f s;
				if (hero.dir == 1.f) s = { 10.f, 0.f };
				else  s = { -10.f, 0.f };
				bullets.push_back(new Bullet(hero.shape.getPosition(), s));
				hero.weapon_ready = false;
			}
		}

		if (pop_enemy)
		{
			enemies.push_back(new Enemy({ 300,200 }, sf::Color::White));
			enemies.push_back(new Enemy({ 800,500 }, sf::Color::White));
			pop_enemy = false;
		}


		// if (goodboy.onGround) goodboy.vel += 5.f*(up);


		for (it_enemy = enemies.begin(); it_enemy != enemies.end();++it_enemy) {
			Entity *enemy = *it_enemy;
			for (it_bullet = bullets.begin(); it_bullet != bullets.end();++it_bullet) {
				Entity *bullet = *it_bullet;
					if (enemy->shape.getGlobalBounds().intersects(bullet->shape.getGlobalBounds())) {
						//bullet->shape.setFillColor(enemy->shape.getFillColor());
						bullet->exists = false;
						//enemy->shape.setFillColor(bullet->shape.getFillColor());
						enemy->exists = false;

						pop_enemy = true;
					}

			}
		}

		
		for (it = bullets.begin(); it != bullets.end();) {
			Entity *e = *it;

			e->update();

			if (e->exists) {
				++it;
			} else {
				it = bullets.erase(it);
				delete e;
			}

		}
		

		for (it = enemies.begin(); it != enemies.end();) {
			Entity *e = *it;

			e->update();

			if (e->exists) {
				++it;
			}
			else {
				it = enemies.erase(it);
				delete e;
			}

		}




		//goodboy.update();
		
		hero.update();
		
		//if (maimer.active) maimer.update();

		/*
		if (hero.shape.getGlobalBounds().intersects(goodboy.shape.getGlobalBounds())) {
			hero.shape.setFillColor(goodboy.shape.getFillColor());
		}
		*/

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

		for (it = bullets.begin(); it != bullets.end(); ++it) {
			(*it)->draw(window);
		}

		for (it = enemies.begin(); it != enemies.end(); ++it) {
			(*it)->draw(window);
		}


		// goodboy.draw(window);
		hero.draw(window);
		// maimer.draw(window);

		window.display();
	}


	return 0;
}