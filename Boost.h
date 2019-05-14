#pragma once

#include "SFML/Graphics.hpp"
#include "Fish.h"

#include <memory>

class Booster : public AutomaticFish {
public:
	Booster(const sf::Vector2f& pos, const float& time, const FishType& type);
	sf::FloatRect DangerZone() const;
	//float GetBoost();
	virtual void Apply(ControlledFish& fish, const float& time);
	virtual ~Booster() = default;
};

class Shrimp : public Booster {
public:
	Shrimp(const sf::Vector2f& pos, const float& time);
	void Apply(ControlledFish& fish, const float& time) override;

private:
	float boost_;
};

class Oyster : public Booster {
public:
	Oyster(const sf::Vector2f& pos, const float& time);
	void Apply(ControlledFish& fish, const float& time) override;

private:
	float boost_;
};

class Coin : public Booster {
public:
	Coin(const sf::Vector2f& pos, const float& time);
	void Apply(ControlledFish& fish, const float& time) override;

private:
	int points_ = 10;
};

class BoostGeneration {
public:
	BoostGeneration() {}
	void Generate(const float& time, ControlledFish& fish);
	void IsEaten(ControlledFish& fish, const float& time);
	
	std::vector<std::shared_ptr<Booster>> boosts_;


	//add DELETING from the vector if a thing is outside of the window
	void Draw(const float& time, sf::RenderWindow& window);
private:
	float last_creation_time = 0;
};

FishType GenerateBoost();