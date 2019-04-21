#include "pch.h"

#include "SFML/Graphics.hpp"
#include "Menu.h"
#include "BackGround.h"
#include "Fish.h"

#include <iostream>
#include <vector>
#include <cmath>
#include <map>
#include <cstdlib>
#include <fstream>
#include <sstream>


const std::string hook_image = "C:/Users/User/MIPT/TheGame/Images/hook.png";

//used to convert current score into string to show it in top corner.
std::string to_str(int a) {
	std::ostringstream ostr;
	ostr << a;
	return ostr.str();
}

void ManageRecords(const float& score) {
	std::vector<float> results;
	std::ifstream ifile("C:/Users/User/MIPT/TheGame/bin/scores.txt");

	while (!ifile.eof()) {
		float i;
		ifile >> i;
		//std::cout << i << std::endl;
		results.push_back(i);
	}
	ifile.close();

	results.pop_back();		//как сделать без этого..?
	results.push_back(score);
	//сортировка по убыванию 
	std::sort(results.begin(), results.end(), 
		[](const float& f1, const float& f2) 
		{return f2 < f1; });
	results.pop_back();

	std::ofstream ofile("C:/Users/User/MIPT/TheGame/bin/scores.txt", std::ofstream::trunc);		//чистим файл от старых значений
	for (const auto& f : results) {
		ofile << f << '\n';
	}

	ofile.close();

	//возможно, стоит делать файл постоянного размера 
	//то есть, хранить 10 лучших результатов
	//остальное выбрасывать на стадии записи в файл
	//решаестя одним pop_back после сортировки, т.к. добавляем один последний счет
}

//сюда добавить передачу текущего размера рыбы, чтобы она не заплывала плавником за границу аквариума.
bool IsInsideWindow(sf::Vector2u WSize, sf::Vector2f Position) {
	if (Position.x >= 0 && Position.y >= 0 &&
		Position.x <= WSize.x && Position.y <= WSize.y) {
		return true;
	}
	else {
		return false;
	}
}

//maybe написать единую (шаблонную?) функцию для проверки взаимного расположения рыба-окно, рыба-крючок, рыба-другая рыба

bool IsOnTheHook(const sf::Sprite& sprite, const sf::Vector2f& hook_pos) {		//с делением на 2 работает +- как надо
	if (abs(sprite.getPosition().x - hook_pos.x) < sprite.getTexture()->getSize().x * sprite.getScale().x/2 &&
		abs(sprite.getPosition().y - hook_pos.y) < sprite.getTexture()->getSize().y * sprite.getScale().y/2) {
		return true;
	}
	return false;
}


class Boat {
public:
	Boat(const float& x, const float& v) {
		x_coordinate = x;
		speed = v;
		SetSprite();
 	}

	void SetSprite() {
		if (!tex.loadFromFile("C:/Users/User/MIPT/TheGame/Images/speed-boat.png")) {
			std::cout << "boat" << std::endl;
			exit(-1);
		}
		//boat_.setTexture(tex);
	}

	void draw(const float& t, sf::RenderWindow& wind) {
		x_coordinate += speed * t;

		boat_.setTexture(tex);
		boat_.setScale(0.2f, 0.2f);
		boat_.setPosition(x_coordinate, -100.f);

		wind.draw(boat_);
	}

	//~Boat() {};

protected:
	float x_coordinate;
	float speed;
	sf::Sprite boat_;
	sf::Texture tex;
	//сюда добавить текстуру
};

class FisherBoat : public Boat {
public:
	FisherBoat(const float& x, const float& v) : Boat(x, v) {		//так же можно передать максимальную глубину крючка
		if (!hook_texture.loadFromFile(hook_image)) {
			std::cout << "hook" << std::endl;
			exit(-1);
		}
		hook_sprite.setTexture(hook_texture);
		hook_sprite.setScale(0.05f, 0.05f);
	};			
	bool IsAttacking() {
		if (depth >= 20) {
			return true;
		}
		return false;
	}

	void InitAttack() {		//setting params to start attack
		depth = 20;
		hook = true;
	}

	sf::Vector2f Attack(sf::RenderWindow& wind) {
		sf::Vertex line[] =						//леска
			{
				sf::Vertex(sf::Vector2f(x_coordinate, 20.f)),
				sf::Vertex(sf::Vector2f(x_coordinate, (float)depth))
			};
		if (hook) {
			depth += (float) 0.5;
		} else {
			depth -= (float) 0.5;
		}

		hook_sprite.setPosition(x_coordinate - 11.f, depth);			//11 - магия, благодаря которой все совпадает :)
		wind.draw(hook_sprite);

		wind.draw(line, 2, sf::Lines);
		if (depth == 300) {
			hook = false;
		}
		return sf::Vector2f(x_coordinate, depth);
	}

private:
	sf::Texture hook_texture;
	sf::Sprite hook_sprite;
	bool hook = true;
	//float time_attacked;
	float depth = 0;
};


int main()
{
	sf::RenderWindow window(sf::VideoMode(1500, 900), "Best game ever!");

	ShowMenu(window, true, 0);		//go to menu.cpp
									//true means that menu for game beginning is displayed

	sf::Clock clock;
	Background background(window, 1);

	sf::Vector2u TextureSize = background.GetBackgroundTextureSize();

	//setting view
	sf::View view_;
	view_.setCenter(sf::Vector2f(300.f, 200.f));
	view_.setSize(sf::Vector2f(600.f, 360.f));
	//end of view

	const float YRatio = (TextureSize.y - view_.getSize().y) / (TextureSize.y);		//магия для того, чтобы не вылетать
																					//за текстуру фона
	
	ControlledFish fish({ 100, 100 }, FishType::L_1);
	FishGeneration gen;

	//boat
	FisherBoat boat(100.f, 0.001);
	
	float score;

	WindowText score_text(20);

	while (window.isOpen())
	{
		sf::Time time = clock.getElapsedTime();

		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed) {
				score = fish.GetScore();
				ManageRecords(score);
				window.close();
			}
		}

		window.clear();

		sf::Vector2f view_Center;

		//making camera follow our sprite
		view_Center.x = fish.GetSprite().getPosition().x + view_.getSize().x / 3.f;
		view_Center.y = (TextureSize.y / 2.f) + ((fish.GetSprite().getPosition().y - TextureSize.y / 2) * YRatio);

		
		view_.setCenter(view_Center);				
		window.setView(view_);						//!!!Dont forget or view is useless

		background.draw(window);
		background.Bubbles(time.asSeconds(), window);

		//рисуем рыб
		fish.Draw(window, time.asSeconds());

		gen.GenerateFish(time.asSeconds(), fish.GetPosition());			//сюда передаем фон, чтобы с его обновлением рисовалось корректно
		gen.Draw(time.asSeconds(), window);

		//adding time into function for score animation
		if (fish.DetectFish(gen.autoFish, time.asSeconds())) {
			std::cout << "YOU ARE DEAD!" << std::endl;
			ShowMenu(window, false, fish.GetScore());

			// написать какой-то метод EndGame(), который будет все останавливать
		}
		
		//благодаря всему этому, текст привязан к правому верхнему углу
		sf::Vector2i text_pos;
		text_pos.x = window.getSize().x - 50;
		text_pos.y = 0;

		score_text.Display(window, window.mapPixelToCoords(text_pos), to_str(fish.GetScore()));


		//DisplayText(window, window.mapPixelToCoords(text_pos), 20, to_str(fish.GetScore()));

		//boat - легко сделать вектор лодок, атакующих в разное время
		boat.draw(time.asSeconds(), window);


		sf::Vector2f hook_pos;
		if (boat.IsAttacking()) {
			hook_pos = boat.Attack(window);
			if (IsOnTheHook(fish.GetSprite(), hook_pos)) {
				std::cout << "You fucked up" << std::endl;
			}
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
			boat.InitAttack();
		}
		//сюда добавляю передачу фона, чтобы вовремя его продлевать
		fish.Control(TextureSize, window, background);

		window.display();
	}

	return 0;
}

// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.
