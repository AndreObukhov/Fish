#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>

#include "BackGround.h"

#include "SFML/Graphics.hpp"

//const std::string font_file = "C:/Users/User/MIPT/TheGame/varsity_regular.ttf";
const std::string button_file = "C:/Users/User/MIPT/TheGame/Images/ButtonOne.png";


std::string to_str(int a);


class Button {
public:
	Button(const std::string& name, const float& y);		//название кнопки и где она по высоте
	void draw(sf::RenderWindow& window);					//отрисовка кнопки в окне
	bool IsClicked(const sf::Vector2f& MousePos);

private:
	void SetVectors();		//задает векторы, содержащие ориентацию кнопки на экране

	sf::Vector2f Left_Top;			//по этим двум векторам однозначно понимаем положение кнопки и ее размер
	sf::Vector2f Right_Bottom;		//нужны, чтобы быстро определять, нажата ли кнопка
	
	sf::Texture button_texture;		//"фон" кнопки
	sf::Sprite button_sprite;
	
	WindowText button_text;

	//sf::Font button_font;			//текст на кнопке
	//sf::Text button_text;
	const std::string button_name;
	
	float y_position;				//где по высоте в столбике кнопок (разность лучше делать около 100)
};

void ShowMenu(sf::RenderWindow& window);

void ShowExitScreen(sf::RenderWindow& window, const int& score);