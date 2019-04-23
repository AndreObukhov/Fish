#include "pch.h"

#include "Boost.h"

Booster::Booster(const sf::Vector2f& pos, const float& time, const FishType& type) : AutomaticFish(pos, type, time) {}

void Booster::Apply(ControlledFish& fish, const float& time) {
	std::cout << "No booster selected" << std::endl;	
}


Shrimp::Shrimp(const sf::Vector2f& pos, const float& time) : Booster(pos, time, FishType::SHRIMP) {
	boost_ = 2.0f;
}

void Shrimp::Apply(ControlledFish& fish, const float& time) {
	std::cout << "shrimp booooooooost" << std::endl;
	fish.SpeedBoost(time, boost_);
}


Oyster::Oyster(const sf::Vector2f& pos, const float& time) : Booster(pos, time, FishType::OYSTER) {
	boost_ = -1.f;
}

void Oyster::Apply(ControlledFish& fish, const float& time) {
	std::cout << "oyster booooooooost" << std::endl;
	fish.SpeedBoost(time, boost_);
}



void BoostGeneration::Generate(const float& time, ControlledFish& fish) {
	sf::Vector2f current_fish = fish.GetPosition();
	if (time > last_creation_time) {
		sf::Vector2f p;
		p.x = current_fish.x + 600.f + rand() % 400;
		p.y = 100.f + rand() % 400;

		//std::shared_ptr<Booster> b;
		//b = std::make_shared<Shrimp>({ x, y }, time);
		
		//for test purposes
		boosts_.push_back(std::make_shared<Shrimp>(p, time));
		p.x += 200.f;
		p.y += 100.f;
		boosts_.push_back(std::make_shared<Oyster>(p, time));

		std::cout << boosts_.size() << std::endl;

		last_creation_time += 3.f;
	}
	IsEaten(fish, time);
}

void BoostGeneration::Draw(const float& time, sf::RenderWindow& window) {
	bool check_pos = false;
	auto iter = boosts_.begin();
	int pos = 0;

	for (auto& b : boosts_) {
		b->Draw(time, window);

		if (b->GetPosition().x < window.getView().getCenter().x - window.getSize().x / 2) {
			iter += pos;
			check_pos = true;
		}
		pos++;
	}

	if (check_pos) {
		boosts_.erase(iter);
	}
}

void BoostGeneration::IsEaten(ControlledFish& fish, const float& time) {
	auto it = boosts_.begin();
	/*
	for (auto& b : boosts_) {
		if (fish.isTouched(*b)) {
			b->Apply(fish, time);
		}
	}
	*/
	for (unsigned i = 0; i < boosts_.size(); i ++) {
		if (fish.isTouched(*boosts_[i])) {
			boosts_[i]->Apply(fish, time);		//��������� ����� � ����
			it += i;
			boosts_.erase(it);
		}
	}
}