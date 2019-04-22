#include "pch.h"

#include "Boost.h"

Shrimp::Shrimp(const sf::Vector2f& pos, const float& time) : AutomaticFish(pos, FishType::SHRIMP, time) {
	boost_ = 2.f;
}

float Shrimp::GetBoost() {
	return boost_;
}

