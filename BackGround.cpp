#include "pch.h"

#include "BackGround.h"

Background::Background(sf::RenderWindow& window, const int& mode) {
	std::string path;
	if (mode == 1) {
		path = background_game;
	}
	if (mode == 2) {
		path = background_menu;
	}
	if (!BackgroundTexture.loadFromFile(path)) {
		std::cout << "background" << std::endl;
		exit(-1);
	}
	TextureSize = BackgroundTexture.getSize(); //Get size of texture.
	WindowSize = window.getSize();

	if (mode == 2) {
		//можно слегка растянуть фон :)
		float ScaleX = (float)WindowSize.x / TextureSize.x;
		float ScaleY = (float)WindowSize.y / TextureSize.y;
		background.setScale(ScaleX, ScaleY);      //Set scale.  
	}
	
	background.setTexture(BackgroundTexture);
}

void Background::Bubbles(const float& time, sf::RenderWindow& window) {
	//initializing bubbles vector
	float make_x = 200 + rand() % 201;
	sf::Vector2f pos(make_x, 500.0);

	//making bubbles at random interval
	if (time > bubble_creation_time) {
		bubbles.push_back(Bubble(pos, time));
		float dt = 1.5f + 0.1f * (float)(rand() % 11);
		bubble_creation_time += dt;
	}

	//drawing bubbles and erasing unneeded ones
	for (int i = 0; i < bubbles.size(); i++) {
		bubbles[i].Draw(window, time);
		if (!IsInsideWindow(window.getSize(), bubbles[i].GetPos(time))) {
			bubbles.erase(bubbles.begin() + i);
		}
	}
}

void Background::draw(sf::RenderWindow& window) {
	window.draw(background);
}

sf::Vector2u Background::GetBackgroundTextureSize() const {
	return TextureSize;
}

Bubble::Bubble(const sf::Vector2f& pos, const float& t) {
	coordinates = pos;
	time_created = t;
	speed.y = (float)-0.05;
	//speed.x = 0.1* (-10 + rand() % 20);
}

sf::Vector2f Bubble::GetPos(const float& current_time) {
	float dt = current_time - time_created;

	//making bubbles go like sin(t)
	speed.x = sin(5 * current_time) / (5 + 10 * dt);

	coordinates = coordinates + speed;
	return coordinates;
}

void Bubble::Draw(sf::RenderWindow& wind, const float& current_time) {
	coordinates = GetPos(current_time);

	sf::CircleShape bub(5.f);
	bub.setFillColor(sf::Color(255, 255, 255, 90));
	bub.setPosition(coordinates.x, coordinates.y);
	wind.draw(bub);
}


