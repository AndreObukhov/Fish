#pragma once

#include "SFML/Graphics.hpp"
#include <iostream>
#include <vector>
#include <cmath>
#include <map>
#include <cstdlib>

#include <iterator>

#include "background.h"


const std::string L1_file = "C:/Users/User/MIPT/TheGame/Images/L_1.png";
const std::string L2_file = "C:/Users/User/MIPT/TheGame/Images/L_2.png";
const std::string L3_file = "C:/Users/User/MIPT/TheGame/Images/L_3.png";
const std::string L4_file = "C:/Users/User/MIPT/TheGame/Images/L_4.png";


enum class FishType {
	L_1 = 1, L_2 = 2, L_3 = 3, L_4 = 4
};

static std::map<FishType, sf::Vector2f> type_scale = { {FishType::L_1, {0.1f, 0.1f}}, {FishType::L_2, {0.2f, 0.2f}},
											  {FishType::L_3, {0.3f, 0.3f}}, {FishType::L_4, {0.4f, 0.4f}} };

static std::map<FishType, int> type_points = { {FishType::L_1, 1}, {FishType::L_2, 5},
													 {FishType::L_3, 10}, {FishType::L_4, 15} };

static std::map<FishType, int> type_limit_points = { {FishType::L_1, 10}, {FishType::L_2, 50},
											 {FishType::L_3, 100}, {FishType::L_4, 150} };

static std::map<FishType, sf::Vector2f> type_speed = { {FishType::L_1, {0.04, 0}}, {FishType::L_2, {0.07, 0}},
											 {FishType::L_3, {0.09, 0}}, {FishType::L_4, {0.1, 0}} };




class Fish {
public:

	Fish(const sf::Vector2f& pos, FishType type);

	void LoadSprite();
	sf::Sprite GetSprite() const;
	sf::Vector2f GetPosition() const;
	FishType GetType() const;

protected:
	sf::Sprite fish_;
	sf::Texture tex_;
	sf::Vector2f pos_;
	sf::Vector2f scale_;
	float speed_ = 0.5;
	FishType type_;
};

class AutomaticFish : public Fish {
public:
	AutomaticFish(const sf::Vector2f& pos, FishType type, const float& time);
	bool Draw(const float &time, sf::RenderWindow& window);

private:
	float time_created_;
	sf::Vector2f  speed_;

};


class ControlledFish : public Fish {
public:
	ControlledFish(const sf::Vector2f& pos, FishType type);
	void Rotate(sf::RenderWindow& window, sf::Vector2f d);
	//сюда добавляю передачу фона, чтобы вовремя его продлевать
	void Move(sf::Vector2u& TextureSize, Background& background);

	void Laser(sf::RenderWindow& window, sf::Vector2f center, sf::Vector2f worldPos);

	//сюда добавляю передачу фона, чтобы вовремя его продлевать + TextureSize по ссылке - она меняется с продолжением фона
	void Control(sf::Vector2u& TextureSize, sf::RenderWindow& window, Background& background);

	void Draw(sf::RenderWindow& window);
	bool isTouched(const AutomaticFish& autoFish);
	bool DetectFish(std::vector<AutomaticFish>& autoFish);
	void Eat(std::vector<AutomaticFish>& autoFish, std::vector<AutomaticFish>::iterator it_del);
	void ChangeType();
	int GetScore();

private:
	int points_ = 0;
};


class FishGeneration {
public:
	FishGeneration();
	FishType GenerateType();
	void GenerateFish(const float &time, const sf::Vector2f& current_fish);
	void Draw(const float &time, sf::RenderWindow& window);
	std::vector<AutomaticFish> autoFish;

private:
	float fish_creation_time = 0;
};