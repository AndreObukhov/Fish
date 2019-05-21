#pragma once

#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"
#include <iostream>
#include <vector>
#include <cmath>
#include <map>
#include <cstdlib>

#include <iterator>

#include "BackGround.h"

std::string to_str(int a);

//fish
const std::string L1_file = "C:/Users/User/MIPT/TheGame/Images/L_1.png";
const std::string L2_file = "C:/Users/User/MIPT/TheGame/Images/L_2.png";
const std::string L3_file = "C:/Users/User/MIPT/TheGame/Images/L_3.png";
const std::string L4_file = "C:/Users/User/MIPT/TheGame/Images/L_4.png";

//boosts
const std::string shrimp_file = "C:/Users/User/MIPT/TheGame/Images/Shrimp.png";
const std::string oyster_file = "C:/Users/User/MIPT/TheGame/Images/Oyster.png";
const std::string coin_file = "C:/Users/User/MIPT/TheGame/Images/Coin.png";

const float life_time = 30.f;


enum class FishType {
	L_1 = 1, L_2 = 2, L_3 = 3, L_4 = 4, SHRIMP = 5, OYSTER = 6, COIN = 7
};

static std::map<FishType, sf::Vector2f> type_scale = { {FishType::L_1, {0.1f, 0.1f}}, {FishType::L_2, {0.2f, 0.2f}},
											  {FishType::L_3, {0.3f, 0.3f}}, {FishType::L_4, {0.4f, 0.4f}}, 
											  {FishType::SHRIMP, {0.1f, 0.1f}}, {FishType::OYSTER, {0.05f, 0.05f}},
											  {FishType::COIN, {0.012f, 0.012f}} };

static std::map<FishType, int> type_points = { {FishType::L_1, 1}, {FishType::L_2, 5},
													 {FishType::L_3, 10}, {FishType::L_4, 15} };

static std::map<FishType, int> type_limit_points = { {FishType::L_1, 10}, {FishType::L_2, 50},
											 {FishType::L_3, 100}, {FishType::L_4, 150} };

static std::map<FishType, sf::Vector2f> type_speed = { {FishType::L_1, {0.04, 0}}, {FishType::L_2, {0.07, 0}},
											 {FishType::L_3, {0.09, 0}}, {FishType::L_4, {0.1, 0}} };



enum class DirectionType {
	UP, DOWN, LEFT, RIGHT, NOWHERE
};
static std::map<DirectionType, float> directionType_angle = { {DirectionType::UP, 90}, {DirectionType::DOWN, 270},
													   {DirectionType::RIGHT, 180}, {DirectionType::LEFT, 0}, 
													   {DirectionType::NOWHERE, 180}, };



class Fish {
public:
	Fish(const sf::Vector2f& pos, FishType type);

	void LoadSprite();
	sf::Sprite GetSprite() const;
	sf::Vector2f GetPosition() const;
	FishType GetType() const;

	virtual float GetSpeed() const;
	float GetAngle() const;

	virtual ~Fish() = default;
protected:
	sf::Sprite fish_;
	sf::Texture tex_;
	sf::Vector2f pos_;
	sf::Vector2f scale_;
	float speed_ = 500.f;
	float angle_ = 180.f;
	FishType type_;
};

class AnotherPlayerFish : public Fish {
public:
	//AnotherPlayerFish();
	AnotherPlayerFish(const sf::Vector2f& pos, FishType type, float angle, DirectionType directionType, float speed);
	void Draw(sf::RenderWindow& window);
	void NetUpdate(const sf::Vector2f& pos, FishType type, float angle, DirectionType directionType, float speed);
	void UpdatePosition(const float& time);
	DirectionType GetDirectionType() const;

	~AnotherPlayerFish() = default;
private:
	DirectionType current_direction = DirectionType::RIGHT;
	float time_last_updated = 0.f;
};

class AutomaticFish : public Fish {
public:
	AutomaticFish(const sf::Vector2f& pos, FishType type, const float& time);
	bool Draw(const float &time, sf::RenderWindow& window);

	sf::FloatRect DangerZone() const;			//to make eating more realistic

	virtual ~AutomaticFish() = default;
private:
	float time_created_;
	sf::Vector2f  speed_;
};


class ControlledFish : public Fish {
public:
	ControlledFish(const sf::Vector2f& pos, FishType type);


	bool CheckSharpRotate(DirectionType newDirectionType);
	void GradualRotate(DirectionType newDirectionType);

	void Rotate(DirectionType newDirectionType);

	void Move(sf::Vector2u& TextureSize, Background& background, const float& time);

	sf::FloatRect FishMouth() const;			//to make eating more realistic

	void Laser(sf::RenderWindow& window, sf::Vector2f center, sf::Vector2f worldPos);

	//сюда добавляю передачу фона, чтобы вовремя его продлевать + TextureSize по ссылке - она меняется с продолжением фона
	void Control(sf::Vector2u& TextureSize, sf::RenderWindow& window, Background& background, const float& time);

	
	//!!! -- added time to Eat, Draw, Detect to make "plus points" animation
	void Draw(sf::RenderWindow& window, const float& time);

	template<typename T>			//interaction with both autoFish and boost creatures
	bool isTouched(const T& Creature) {
		return FishMouth().intersects(Creature.DangerZone());
	}

	bool DetectFish(std::vector<AutomaticFish>& autoFish, const float& time, int& number_fish_eaten);
	void Eat(std::vector<AutomaticFish>& autoFish, std::vector<AutomaticFish>::iterator it_del, const float& time);
	void ChangeType();
	int GetScore();
	float GetSpeed() const;

	DirectionType GetDirectionType() const;


	void SpeedBoost(const float& time, const float& factor);
	void CoinBoost(const int& points_added, const float& time);

	void CheckBoost(const float& time);

	void LoadSounds();
	void PlaySound(sf::SoundBuffer buffer);

private:
	int points_ = 0;

	sf::SoundBuffer collect_point_sound_;


	StatusBar bar_;			//right top corner, shows how much is left until the next level
	StatusBar boost_bar_;	//r.t.c, shows how much boost is left


	//make struct FishBoost where all of this is stored
	bool is_boost_applied = false;
	float time_boost_applied;
	float boost_factor = 1.f;
	//end of boosts

	float previous_control_time = 0;
	DirectionType previous_direction = DirectionType::RIGHT;
	DirectionType current_direction = DirectionType::RIGHT;

	//variables needed for displaying text above our fish
	WindowText add_points_;
	int delta_pts_ = 0;
	std::string plus_pts_string;

	float time_text_effect_;				//was time_fish_eaten

	//function drawing text above controlled fish
	void PointsAnimation(sf::RenderWindow& window, const double& time);
	void Resize();
};

/*
//do not need this anymore as Boosts generate another way
template<class T>
class Generator {
public:
	Generator() {}

	
};
*/

class FishGeneration {
public:
	FishGeneration();
	FishType GenerateType();
	void GenerateFish(const float &time, const sf::Vector2f& current_fish);

	void Draw(const float& time, sf::RenderWindow& window) {
		for (int i = 0; i < autoCreature.size(); ++i) {
			if (autoCreature[i].Draw(time, window)) {
				autoCreature.erase(autoCreature.begin() + i);
				//std::cout << "Fish " << i << " deleted" << std::endl;
			}
		}
	}

	std::vector<AutomaticFish> autoCreature;
private:
	float last_creation_time = 0;
};