#pragma once

#include <iostream>
#include "SFML/Graphics.hpp"

#include <vector>

const std::string background_game = "C:/Users/User/MIPT/TheGame/Images/panorama.jpg";
const std::string background_menu = "C:/Users/User/MIPT/TheGame/Images/menu.png";

bool IsInsideWindow(sf::Vector2u WSize, sf::Vector2f Position);

class Bubble {
public:
	Bubble(const sf::Vector2f& pos, const float& t);

	sf::Vector2f GetPos(const float& current_time);

	void Draw(sf::RenderWindow& wind, const float& current_time);

private:
	sf::Vector2f coordinates;
	sf::Vector2f speed;
	float time_created;
};

class Background {				//responsible for setting background image and drawing bubbles
public:
	Background(sf::RenderWindow& window, const int& mode);

	void Bubbles(const float& time, sf::RenderWindow& window);

	void draw(sf::RenderWindow& window);

	sf::Vector2u GetBackgroundTextureSize() const;

private:
	//background
	sf::Texture BackgroundTexture;
	sf::Sprite background;

	sf::Vector2u TextureSize;  //Added to store texture size.
	sf::Vector2u WindowSize;   //Added to store window size.

	//bubbles
	std::vector<Bubble> bubbles;
	double bubble_creation_time = 0;
};