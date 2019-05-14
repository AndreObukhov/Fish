#pragma once

#include <iostream>
#include "SFML/Graphics.hpp"

#include <vector>

const std::string background_game = "C:/Users/User/MIPT/TheGame/Images/panorama.jpg";
const std::string background_menu = "C:/Users/User/MIPT/TheGame/Images/menu.png";
const std::string font_file = "C:/Users/User/MIPT/TheGame/varsity_regular.ttf";


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

	void AddBackground();

	void Bubbles(const float& x_offset, const float& time, sf::RenderWindow& window);

	void draw(sf::RenderWindow& window);

	sf::Vector2u GetBackgroundTextureSize() const;

private:
	//background
	sf::Texture BackgroundTexture;
	sf::Sprite background;
	std::vector<sf::Sprite> long_background;

	sf::Vector2u TextureSize;  //Added to store texture size.
	sf::Vector2u WindowSize;   //Added to store window size.

	//bubbles
	std::vector<Bubble> bubbles;
	double bubble_creation_time = 0;
};


class WindowText {			//needed to load font from file once, not every time it is drawn
public:
	WindowText(const unsigned& size);
	void Display(sf::RenderWindow& window, const sf::Vector2f& position, std::string text);
private:
	sf::Font font;
	sf::Text win_text;
	unsigned font_size;
};


//draws bars in right side of the window (height ajusted as a param of Draw function)
//color must be declared in a constructor
class StatusBar {
public:
	StatusBar(sf::Color color);

	void Draw(sf::RenderWindow& window, const float& progress, const float& level_limit, const int& height);

private:
	//bar consists of two parts: transparent frame and a rectangle of changing size that "fills" it
	sf::RectangleShape bar;
	sf::RectangleShape frame;
};

//void DisplayText(sf::RenderWindow& window, const sf::Vector2f& position, const unsigned& size, std::string text);