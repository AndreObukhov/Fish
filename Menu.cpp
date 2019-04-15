#include "pch.h"

#include "Menu.h"
#include "BackGround.h"

Button::Button(const std::string& name, const float& y) : button_name(name) {
	y_position = y;
	
	if (!button_font.loadFromFile(font_file)) {
		std::cout << "Font" << std::endl;
	}
	button_text.setFont(button_font);
	button_text.setString(button_name);

	button_text.setCharacterSize(40);
	button_text.setPosition(60.0f, y_position + 22.f);

	if (!button_texture.loadFromFile(button_file)) {
		std::cout << "button" << std::endl;
	}

	button_sprite.setTexture(button_texture);
	button_sprite.setPosition(10.0f, y_position);
	button_sprite.setScale(0.18f, 0.1f);

	SetVectors();
}

void Button::draw(sf::RenderWindow& window) {
	window.draw(button_sprite);
	window.draw(button_text);
}

bool Button::IsClicked(const sf::Vector2i& MousePos) {
	if ((MousePos.x > Left_Top.x &&
		MousePos.x < Right_Bottom.x) && 
		(MousePos.y > Left_Top.y &&
		MousePos.y < Right_Bottom.y - 30.f)) {
		return true;
	}
	return false;
}

void Button::SetVectors() {
	Left_Top.x = button_sprite.getPosition().x;
	Left_Top.y = button_sprite.getPosition().y;
	
	Right_Bottom.x = button_sprite.getPosition().x + button_texture.getSize().x*button_sprite.getScale().x;
	Right_Bottom.y = button_sprite.getPosition().y + button_texture.getSize().y*button_sprite.getScale().y;
}

void ShowMenu(sf::RenderWindow& window) {
	Button ButtonStart("create new game", 200.f);
	Button Highscore("show highscore", 300.f);
	Button Exit("exit", 400.f);
	//сюда же добавить выбор одно/многопользовательского режима

	Background background(window, 2);

	while (window.isOpen()) {

		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed) {
				window.close();
				exit(0);
			}
		}

		window.clear();

		background.draw(window);

		ButtonStart.draw(window);
		Highscore.draw(window);
		Exit.draw(window);

		sf::Vector2i MousePos = sf::Mouse::getPosition(window);

		if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Right)) {
			break;
		}
		
		if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
			if (ButtonStart.IsClicked(MousePos)) {
				std::cout << "Good luck buddy" << std::endl;
				break;
			}

			if (Highscore.IsClicked(MousePos)) {
				std::cout << "no highscores yet" << std::endl;
			}

			if (Exit.IsClicked(MousePos)) {
				exit(0);
			}
		}

		window.display();
	}
}