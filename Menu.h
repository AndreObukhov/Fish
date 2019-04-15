#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>

#include "SFML/Graphics.hpp"

const std::string font_file = "C:/Users/User/MIPT/TheGame/varsity_regular.ttf";
const std::string button_file = "C:/Users/User/MIPT/TheGame/Images/ButtonOne.png";

class Button {
public:
	Button(const std::string& name, const float& y);		//�������� ������ � ��� ��� �� ������
	void draw(sf::RenderWindow& window);					//��������� ������ � ����
	bool IsClicked(const sf::Vector2i& MousePos);

private:
	void SetVectors();		//������ �������, ���������� ���������� ������ �� ������

	sf::Vector2f Left_Top;			//�� ���� ���� �������� ���������� �������� ��������� ������ � �� ������
	sf::Vector2f Right_Bottom;		//�����, ����� ������ ����������, ������ �� ������

	sf::Texture button_texture;		//"���" ������
	sf::Sprite button_sprite;

	sf::Font button_font;			//����� �� ������
	sf::Text button_text;
	const std::string button_name;
	
	float y_position;				//��� �� ������ � �������� ������ (�������� ����� ������ ����� 100)
};

void ShowMenu(sf::RenderWindow& window);