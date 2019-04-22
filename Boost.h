#pragma once

#include "SFML/Graphics.hpp"
#include "Fish.h"

class Shrimp : public AutomaticFish {
public:
	Shrimp(const sf::Vector2f& pos, const float& time);
	//float GetBoost();
	void Apply(ControlledFish& fish, const float& time);

private:
	float boost_ = 2.0f;
};


class BoostGeneration : public Generator<Shrimp> {
public:
	BoostGeneration() {}
	void Generate(const float& time, ControlledFish& fish);
	void IsEaten(ControlledFish& fish, const float& time);

private:
	float last_creation_time = 0;
};