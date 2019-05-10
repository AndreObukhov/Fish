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


class StatusBar {
public:
	StatusBar(const sf::Vector2f& pos) {
		bar.setPosition(pos);
		bar.setFillColor(sf::Color::Green);
		progress = 0;
	}

	void Draw(sf::RenderWindow& window, const float& progress) {
		bar.setSize(sf::Vector2f(progress, 50.f));
		window.draw(bar);
	}

private:
	float progress;		//progress is from 0 to 1 - ratio until next fish level
	sf::RectangleShape bar;
};

//void DisplayText(sf::RenderWindow& window, const sf::Vector2f& position, const unsigned& size, std::string text);