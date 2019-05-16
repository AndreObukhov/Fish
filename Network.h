#pragma once

#include "Fish.h"

#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <string>
#include <vector>
#include <iostream>

using namespace std;

class Network {
private:
	sf::IpAddress ip;
	sf::TcpSocket socket;
	char connectionType;

public:
	Network();
	void CreateConnection();
	void GetAnotherFish(AnotherPlayerFish& anotherFish, std::vector<AutomaticFish>& fishes, const float& client_time);
	void SendMyFish(const ControlledFish& myFish);
	void FishEaten(const int& index);

};


//int main() {
//
//    sf::RectangleShape rect1, rect2;
//    rect1.setSize(sf::Vector2f(20, 20));
//    rect2.setSize(sf::Vector2f(20, 20));
//
//    rect1.setFillColor(sf::Color::Red);
//    rect2.setFillColor(sf::Color::Blue);
//
//    sf::RenderWindow app(sf::VideoMode(800,600,32),"SFML_ProjectBase", sf::Style::Resize);
//    sf::Vector2f prevPosition, p2Position;
//
//    socket.setBlocking(false);
//
//    bool update = false;
//
//    while (app.isOpen()) {
//        sf::Event e;
//        while (app.pollEvent(e)) {
//            if (e.type == sf::Event::Closed || (e.type == sf::Event::KeyReleased && e.key.code == sf::Keyboard::Escape)) {
//                app.close();
//            }
//            else if (e.type == sf::Event::GainedFocus) {
//                update = true;
//            }
//            else if (e.type == sf::Event::LostFocus) {
//                update = false;
//            }
//        }
//        prevPosition = rect1.getPosition();
//
//        if (update) {
//            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
//                rect1.move(0.2f, 0.0f);
//            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
//                rect1.move(-0.2f, 0.0f);
//            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
//                rect1.move(0.0f, -0.2);
//            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
//                rect1.move(0.0f, 0.2f);
//        }
//
//        sf::Packet packet;
//
//        if (prevPosition != rect1.getPosition()) {
//            packet << rect1.getPosition().x << rect1.getPosition().y;
//            socket.send(packet);
//        }

//        socket.receive(packet);
//        if (packet >> p2Position.x >> p2Position.y) {
//            rect2.setPosition(p2Position);
//        }
//        app.draw(rect1);
//        app.draw(rect2);
//
//        app.display();
//        app.clear();
//    }
//    system("pause");
//    return 0;
//