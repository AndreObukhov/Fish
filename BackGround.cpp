#include "pch.h"

#include "BackGround.h"

Background::Background(sf::RenderWindow& window, const int& mode) {
	std::string path;		//1 is for main game background, 2 is for menu background
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

	if (mode == 2) {
		WindowSize = window.getSize();			//вписываем фон меню строго в окно
		//можно слегка растянуть фон :)
		float ScaleX = (float)WindowSize.x / TextureSize.x;
		float ScaleY = (float)WindowSize.y / TextureSize.y;
		background.setScale(ScaleX, ScaleY);      //Set scale.  
		background.setTexture(BackgroundTexture);
	}
	
	if (mode == 1) {
		for (int i = 0; i < 1; ++i) {
			sf::Sprite one_piece;
			one_piece.setTexture(BackgroundTexture);
			one_piece.setPosition(BackgroundTexture.getSize().x * i, 0.f);
			long_background.push_back(one_piece);
		}
	}
}


void Background::AddBackground() {
	sf::Sprite one_piece;
	one_piece.setTexture(BackgroundTexture);
	one_piece.setPosition(BackgroundTexture.getSize().x * long_background.size(), 0.f);
	long_background.push_back(one_piece);
}



void Background::Bubbles(const float& x_offset, const float& time, sf::RenderWindow& window) {
	//initializing bubbles vector
	float make_x = x_offset + 1000.f + rand() % 201;			//remade to draw bubbles ahead of the fish
	sf::Vector2f pos(make_x, 500.f);

	//making bubbles at random interval 1-2 sec
	if (time > bubble_creation_time) {
		bubbles.push_back(Bubble(pos, time));
		float dt = 1.f + 0.1f * (float)(rand() % 11);
		bubble_creation_time += dt;
	}

	//drawing bubbles and erasing unneeded ones
	for (int i = 0; i < bubbles.size(); i++) {
		bubbles[i].Draw(window, time);
		if (bubbles[i].GetPos(time).y < 0) {			//now delete only if higher than top of the window 
			bubbles.erase(bubbles.begin() + i);
		}
	}
}

void Background::draw(sf::RenderWindow& window) {
	if (long_background.empty()) {
		window.draw(background);
	}
	else {
		for (int i = 0; i < long_background.size(); ++i) {
			window.draw(long_background[i]);
		}
	}
}

sf::Vector2u Background::GetBackgroundTextureSize() const {
	if (long_background.empty()) {
		return TextureSize;
	}
	else {
		sf::Vector2u res;
		res.y = TextureSize.y;
		res.x = TextureSize.x * long_background.size();
		return res;
	}
}

Bubble::Bubble(const sf::Vector2f& pos, const float& t) {
	coordinates = pos;
	time_created = t;
	speed.y = -0.05f - 0.01f * (rand() % 10);			//now random as well
	//std::cout << speed.y << std::endl;
	//speed.x = 0.1* (-10 + rand() % 20);
}

sf::Vector2f Bubble::GetPos(const float& current_time) {
	float dt = current_time - time_created;

	//making bubbles go like sin(t)
	speed.x = sin(5 * current_time) / (5 + 5 * dt);

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



WindowText::WindowText(const unsigned& size) {
	font.loadFromFile(font_file);
	win_text.setFont(font);
	win_text.setCharacterSize(size);
	//win_text.setPosition(position);
}

void WindowText::Display(sf::RenderWindow& window, const sf::Vector2f& position, std::string text) {
	win_text.setPosition(position);

	win_text.setString(text);
	window.draw(win_text);
}

/*
void DisplayText(sf::RenderWindow& window, const sf::Vector2f& position, const unsigned& size, std::string text) {
	
	sf::Font font;
	font.loadFromFile(font_file);
	sf::Text win_text;
	win_text.setFont(font);
	win_text.setCharacterSize(size);

	//sf::Vector2i text_pos;
	//text_pos.x = window.getSize().x - win_text.getLocalBounds().width * 2 - 50;
	//text_pos.y = 0;
	win_text.setPosition(position);

	win_text.setString(text);
	window.draw(win_text);
}*/

StatusBar::StatusBar(sf::Color color) {
	bar.setFillColor(color);

	frame.setFillColor(sf::Color::Transparent);
	frame.setOutlineThickness(0.5f);
	frame.setOutlineColor(color);
	frame.setSize(sf::Vector2f(100.f, 10.f));
}

void StatusBar::Draw(sf::RenderWindow& window, const float& progress, const float& level_limit, const int& height) {
	bar.setPosition(window.mapPixelToCoords({ (int)window.getSize().x - 300, height }));
	bar.setSize(sf::Vector2f(progress * 100.f / level_limit, 10.f));

	frame.setPosition(window.mapPixelToCoords({ (int)window.getSize().x - 300, height }));

	window.draw(bar);
	window.draw(frame);
}
