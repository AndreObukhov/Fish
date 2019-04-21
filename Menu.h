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
	Button(const std::string& name, const float& y);		//�������� ������ � ��� ��� �� ������
	void draw(sf::RenderWindow& window);					//��������� ������ � ����
	bool IsClicked(const sf::Vector2f& MousePos);

private:
	void SetVectors();		//������ �������, ���������� ���������� ������ �� ������

	sf::Vector2f Left_Top;			//�� ���� ���� �������� ���������� �������� ��������� ������ � �� ������
	sf::Vector2f Right_Bottom;		//�����, ����� ������ ����������, ������ �� ������
	
	sf::Texture button_texture;		//"���" ������
	sf::Sprite button_sprite;
	
	WindowText button_text;

	//sf::Font button_font;			//����� �� ������
	//sf::Text button_text;
	const std::string button_name;
	
	float y_position;				//��� �� ������ � �������� ������ (�������� ����� ������ ����� 100)
};

void ShowMenu(sf::RenderWindow& window);

void ShowExitScreen(sf::RenderWindow& window, const int& score);