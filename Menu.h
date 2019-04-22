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
	Button(const std::string& file_name, const float& x, const float& y);		//constructor for simple buttons without text
	void draw(sf::RenderWindow& window);					//��������� ������ � ����
	void dynamicDraw(sf::RenderWindow& window, const sf::Vector2f& pos);
	bool IsClicked(const sf::Vector2f& MousePos);
	//void ResetTexture(const std::string& new_file);

private:
	void SetVectors();		//������ �������, ���������� ���������� ������ �� ������

	sf::Vector2f Left_Top;			//�� ���� ���� �������� ���������� �������� ��������� ������ � �� ������
	sf::Vector2f Right_Bottom;		//�����, ����� ������ ����������, ������ �� ������
	
	sf::Texture button_texture;		//"���" ������
	sf::Sprite button_sprite;
	
	WindowText button_text;			//text on a button made in a new way

	const std::string button_name;
	
	float y_position;				//��� �� ������ � �������� ������ (�������� ����� ������ ����� 100)
};

void ShowMenu(sf::RenderWindow& window, bool EntryMenu, const int& score);