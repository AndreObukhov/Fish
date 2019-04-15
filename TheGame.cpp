#include "pch.h"

#include "SFML/Graphics.hpp"
#include "Menu.h"
#include "BackGround.h"

#include <iostream>
#include <vector>
#include <cmath>
#include <map>
#include <cstdlib>

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

bool IsOnTheHook(const sf::Sprite& sprite, const sf::Vector2f& hook_pos) {
	if (abs(sprite.getPosition().x - hook_pos.x) < sprite.getTexture()->getSize().x * sprite.getScale().x &&
		abs(sprite.getPosition().y - hook_pos.y) < sprite.getTexture()->getSize().y * sprite.getScale().y) {
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
		//std::cout << "boat created" << std::endl;
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

		/*sf::RectangleShape boat;
		boat.setSize({ 100.f, 20.f });
		boat.setPosition(x_coordinate, 20.f);
		boat.setFillColor(sf::Color(128, 128, 0));*/

		//sf::Sprite boat(boat_figure);

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

class FisherBoat : public Boat {			//на конец "лески" нужно повесить спрайт крючка 
public:
	FisherBoat(const float& x, const float& v) : Boat(x, v) {};			//так же можно передать максимальную глубину крючка

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

		wind.draw(line, 2, sf::Lines);
		if (depth == 300) {
			hook = false;
		}
		return sf::Vector2f(x_coordinate, depth);
	}

private:
	bool hook = true;
	//float time_attacked;
	float depth = 0;
};


int main()
{
	sf::RenderWindow window(sf::VideoMode(1200, 720), "Best game ever!");

	ShowMenu(window);		//go to menu.cpp

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

	//this is Sprite that we can control --- задание спрайта уйдет в конструктор рыбы
	sf::Texture texture;

	if (!texture.loadFromFile("C:/Users/User/MIPT/TheGame/Images/FISH.png")) {
		std::cout << "texture" << std::endl;
		exit(-1);
	}

	sf::Sprite circle(texture);		//персонаж
	circle.setScale(0.25f, 0.25f);

	circle.setPosition(100, 100);

	//boat
	FisherBoat boat(100.f, 0.001);

	while (window.isOpen())
	{
		sf::Time time = clock.getElapsedTime();

		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		window.clear();

		//moving sprite towards left side of the window
		//sf::Vector2f deltaX(view_.getSize().x / 3, 0.f);
		//sf::Vector2f ratioY(0.f, )

		sf::Vector2f view_Center;

		view_Center.x = circle.getPosition().x + view_.getSize().x/ 3.f;
		view_Center.y = (TextureSize.y / 2.f) + ((circle.getPosition().y - TextureSize.y / 2)*YRatio);

		//view_.setCenter(circle.getPosition() + deltaX);		//making camera follow our sprite

		view_.setCenter(view_Center);
		window.setView(view_);						//!!!Dont forget or view is useless

		background.draw(window);
		background.Bubbles(time.asSeconds(), window);

		//boat - легко сделать вектор лодок, атакующих в разное время
		boat.draw(time.asSeconds(), window);


		sf::Vector2f hook_pos;
		if (boat.IsAttacking()) {
			hook_pos = boat.Attack(window);
			if (IsOnTheHook(circle, hook_pos)) {
				std::cout << "You fucked up" << std::endl;
			}
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
			boat.InitAttack();
		}


		// get the current mouse position in the window
		sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
		// convert it to world coordinates
		sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos);		//согласуем координаты в окне с глобальными координатами


		sf::Vector2u CircleSize = circle.getTexture()->getSize();
		circle.setOrigin(CircleSize.x / 2, CircleSize.y / 2);

		//sf::Vector2i MousePos = sf::Mouse::getPosition(window);

		sf::Vector2f center = circle.getPosition();				//где находится наш перс

		sf::Vector2f d = sf::Vector2f(worldPos.x, worldPos.y) - center;

		const double Pi = 3.14159f;
		double angle = 180 + atan2f(d.y, d.x) * 180.0 / Pi;
		circle.setRotation(angle);

		window.draw(circle);

		//fish movement
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
			circle.move(0, -0.2);
			if (!IsInsideWindow(TextureSize, circle.getPosition())) {
				circle.move(0, 0.2);
			}
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
			circle.move(0, 0.2);
			if (!IsInsideWindow(TextureSize, circle.getPosition())) {
				circle.move(0, -0.2);
			}
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
			circle.move(0.2, 0);
			if (!IsInsideWindow(TextureSize, circle.getPosition())) {
				circle.move(-0.2, 0);
			}
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
			circle.move(-0.2, 0);
			if (!IsInsideWindow(TextureSize, circle.getPosition())) {
				circle.move(0.2, 0);
			}
		}


		if (sf::Keyboard::isKeyPressed(sf::Keyboard::L)) {		//laser
			sf::Vertex line[] =
			{
				sf::Vertex(center),
				//sf::Vertex(sf::Vector2f(MousePos.x, MousePos.y))
				sf::Vertex(sf::Vector2f(worldPos.x, worldPos.y))
			};

			line->color = (sf::Color::Red);
			window.draw(line, 2, sf::Lines);
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
			circle.scale(1.002, 1.002);
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {
			circle.scale(0.995, 0.995);
		}

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
