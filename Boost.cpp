#include "pch.h"

#include "Boost.h"

Shrimp::Shrimp(const sf::Vector2f& pos, const float& time) : AutomaticFish(pos, FishType::SHRIMP, time) {
	boost_ = 2.f;
}

void Shrimp::Apply(ControlledFish& fish, const float& time) {
	std::cout << "Booooooooost" << std::endl;
	fish.SpeedBoost(time, boost_);
}

void BoostGeneration::Generate(const float& time, ControlledFish& fish) {
	sf::Vector2f current_fish = fish.GetPosition();
	if (time > last_creation_time) {
		float x = current_fish.x + 600.f + rand() % 400;
		float y = 100 + rand() % 400;

		autoCreature.push_back(Shrimp({ x, y }, time));
		last_creation_time += 3.f;
	}
	IsEaten(fish, time);
}

void BoostGeneration::IsEaten(ControlledFish& fish, const float& time) {
	std::vector<Shrimp>::iterator it = autoCreature.begin();

	for (int i = 0; i < autoCreature.size(); i ++) {
		if (fish.isTouched(autoCreature[i])) {
			autoCreature[i].Apply(fish, time);
			it += i;
			autoCreature.erase(it);
		}
	}
}