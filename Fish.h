#pragma once

#include "SFML/Graphics.hpp"
#include <iostream>
#include <vector>
#include <cmath>
#include <map>
#include <cstdlib>

#include <iterator>

#include "BackGround.h"

std::string to_str(int a);

const std::string L1_file = "C:/Users/User/MIPT/TheGame/Images/L_1.png";
const std::string L2_file = "C:/Users/User/MIPT/TheGame/Images/L_2.png";
const std::string L3_file = "C:/Users/User/MIPT/TheGame/Images/L_3.png";
const std::string L4_file = "C:/Users/User/MIPT/TheGame/Images/L_4.png";

const std::string shrimp_file = "C:/Users/User/MIPT/TheGame/Images/Shrimp.png";


enum class FishType {
	L_1 = 1, L_2 = 2, L_3 = 3, L_4 = 4, SHRIMP = 5
};

static std::map<FishType, sf::Vector2f> type_scale = { {FishType::L_1, {0.1f, 0.1f}}, {FishType::L_2, {0.2f, 0.2f}},
											  {FishType::L_3, {0.3f, 0.3f}}, {FishType::L_4, {0.4f, 0.4f}}, 
											  {FishType::SHRIMP, {0.1f, 0.1f}} };

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
	
	//!!! -- added time to Eat, Draw, Detect to make "plus points" animation
	void Draw(sf::RenderWindow& window, const float& time);

	template<typename T>
	bool isTouched(const T& Creature) {
		return fish_.getGlobalBounds().intersects(Creature.GetSprite().getGlobalBounds());
	}

	bool DetectFish(std::vector<AutomaticFish>& autoFish, const float& time);
	void Eat(std::vector<AutomaticFish>& autoFish, std::vector<AutomaticFish>::iterator it_del, const float& time);
	void ChangeType();
	int GetScore();

	void SpeedBoost(const float& time, const float& factor) {
		speed_ = speed_ * factor;
		time_boost_applied = time;
		is_boost_applied = true;
		boost_factor = factor;
	}

	void CheckBoost(const float& time) {
		if ((time - time_boost_applied > 0.5) && is_boost_applied) {
			time_boost_applied = 0;
			is_boost_applied = false;
			speed_ = speed_ / boost_factor;
		}
	}

private:
	int points_ = 0;

	bool is_boost_applied = false;
	float time_boost_applied;
	float boost_factor;

	WindowText add_points_;
	int delta_pts_ = 0;
	std::string plus_pts_string;

	float time_fish_eaten_;

	void PointsAnimation(sf::RenderWindow& window, const double& time);
};


template<class T>
class Generator {
public:
	Generator() {}

	void Draw(const float& time, sf::RenderWindow& window) {
		for (int i = 0; i < autoCreature.size(); ++i) {
			if (autoCreature[i].Draw(time, window)) {
				autoCreature.erase(autoCreature.begin() + i);
			}
		}
	}

	std::vector<T> autoCreature;
};


class FishGeneration : public Generator<AutomaticFish> {
public:
	FishGeneration();
	FishType GenerateType();
	void GenerateFish(const float &time, const sf::Vector2f& current_fish);
	//void Draw(const float &time, sf::RenderWindow& window);
	//std::vector<AutomaticFish> autoFish;
private:
	float last_creation_time = 0;
};