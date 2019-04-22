#include "pch.h"

#include "Menu.h"

Button::Button(const std::string& name, const float& y) : button_name(name), button_text(40) {
	y_position = y;
	
	if (!button_texture.loadFromFile(button_file)) {
		std::cout << "button" << std::endl;
	}

	button_sprite.setTexture(button_texture);
	button_sprite.setPosition(10.0f, y_position);
	button_sprite.setScale(0.18f, 0.1f);

	SetVectors();
}

Button::Button(const std::string& file_name, const float& x, const float& y) : button_name(""), button_text(40) {
	y_position = y;

	if (!button_texture.loadFromFile(file_name)) {
		std::cout << "button" << std::endl;
	}

	button_sprite.setTexture(button_texture);
	button_sprite.setPosition(x, y_position);
	button_sprite.setScale(0.2f, 0.2f);

	SetVectors();
}

void Button::draw(sf::RenderWindow& window) {
	window.draw(button_sprite);
	
	sf::Vector2f text_pos(60.f, y_position + 22.f);
	button_text.Display(window, text_pos, button_name);
	//DisplayText(window, text_pos, 40, button_name);
}

void Button::dynamicDraw(sf::RenderWindow& window, const sf::Vector2f& pos) {
	button_sprite.setPosition(pos);
	window.draw(button_sprite);
	SetVectors();
}


bool Button::IsClicked(const sf::Vector2f& MousePos) {
	if ((MousePos.x > Left_Top.x &&
		MousePos.x < Right_Bottom.x) && 
		(MousePos.y > Left_Top.y &&
		MousePos.y < Right_Bottom.y - (Right_Bottom.y - Left_Top.y) * 0.2f)) {			//because button textures are not ideal
		return true;
	}
	return false;
}
/*
void Button::ResetTexture(const std::string& new_file) {
	if (!button_texture.loadFromFile(new_file)) {
		std::cout << "button" << std::endl;
	}

	button_sprite.setTexture(button_texture);
	SetVectors();
}
*/

void Button::SetVectors() {
	Left_Top.x = button_sprite.getPosition().x;
	Left_Top.y = button_sprite.getPosition().y;
	
	Right_Bottom.x = button_sprite.getPosition().x + button_texture.getSize().x*button_sprite.getScale().x;
	Right_Bottom.y = button_sprite.getPosition().y + button_texture.getSize().y*button_sprite.getScale().y;
}


//finally done right
std::string GetHighscores() {
	//reading results
	float num;

	std::string score_string = "best scores list:\n";

	//пока штука показывает 5 лучших результатов
	std::ifstream file("C:/Users/User/MIPT/TheGame/bin/scores.txt");
	for (int i = 0; i < 5; ++i) {			//нужно разобраться с циклом!!
		file >> num;
		score_string += to_str(num);
		score_string += "\n";

		//std::cout << num << std::endl;		//уже отсортированы в конце игры
	}
	file.close();

	return score_string;
}


void ShowMenu(sf::RenderWindow& window, bool EntryMenu, const int& score) {
	Button ButtonStart("start new game", 200.f);
	Button Highscore("show highscore", 300.f);
	Button Exit("exit", 400.f);
	Button Back("back", 700.f);

	//сюда же добавить выбор одно/многопользовательского режима

	Background background(window, 2);
	
	//sets initial view that allows to display exit menu correctly
	sf::View exit_view = window.getDefaultView();
	window.setView(exit_view);

	std::string score_string = "your score was:\n " + to_str(score);

	WindowText game_result(40);

	
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

		//displaying buttons
		if(EntryMenu)
			ButtonStart.draw(window);

		Highscore.draw(window);
		Exit.draw(window);
		//Back.draw(window);

		if(!EntryMenu)
			game_result.Display(window, { 75.f, 200.f }, score_string);


		sf::Vector2i MousePos = sf::Mouse::getPosition(window);

		// convert it to world coordinates *fullscreen now works correctly*
		sf::Vector2f worldPos = window.mapPixelToCoords(MousePos);

		if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Right)) {
			break;
		}
		
		if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
			if (EntryMenu && ButtonStart.IsClicked(worldPos)) {
				std::cout << "Good luck buddy" << std::endl;
				break;
			}

			if (Highscore.IsClicked(worldPos)) {
				std::cout << "Highscores" << std::endl;
				WindowText game_result(40);

				while (1) {
					MousePos = sf::Mouse::getPosition(window);
					worldPos = window.mapPixelToCoords(MousePos);

					//needed if we want to leave the game from highscore screen
					while (window.pollEvent(event))
					{
						if (event.type == sf::Event::Closed) {
							window.close();
							exit(0);
						}
					}

					window.clear();
					background.draw(window);
					Back.draw(window);
					game_result.Display(window, { 75.f, 50.f }, GetHighscores());

					if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) && Back.IsClicked(worldPos)) {
						break;
					}
					window.display();
				}
			}

			if (Exit.IsClicked(worldPos)) {
				std::cout << "See you next time" << std::endl;
				exit(0);
			}
		}

		window.display();
	}
}
