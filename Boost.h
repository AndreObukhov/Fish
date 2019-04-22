#pragma once

#include "SFML/Graphics.hpp"
#include "Fish.h"

class Shrimp : public AutomaticFish {
public:
	Shrimp(const sf::Vector2f& pos, const float& time);
	float GetBoost();

private:
	float boost_;
};


class BoostGeneration : public Generator<Shrimp> {

	 
};