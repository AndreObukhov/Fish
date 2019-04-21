#include "pch.h"

#include "fish.h"

Fish::Fish(const sf::Vector2f& pos, FishType type) {
	pos_ = pos;
	type_ = type;
	scale_ = type_scale[type];
	LoadSprite();
}

void Fish::LoadSprite() {
	switch (type_)
	{
	case FishType::L_1:
		if (!tex_.loadFromFile(L1_file))
			exit(-1);
		break;
	case FishType::L_2:
		if (!tex_.loadFromFile(L2_file))
			exit(-1);
		break;
	case FishType::L_3:
		if (!tex_.loadFromFile(L3_file))
			exit(-1);
		break;
	case FishType::L_4:
		if (!tex_.loadFromFile(L4_file))
			exit(-1);
		break;

	}
}



sf::Sprite Fish::GetSprite() const {
	return fish_;
}


sf::Vector2f Fish::GetPosition() const {
	return pos_;
}

FishType Fish::GetType() const {
	return type_;
}

AutomaticFish::AutomaticFish(const sf::Vector2f& pos, FishType type, const float& time) : Fish(pos, type), time_created_(time) {
	speed_ = type_speed[type];
}



//return true if fish is outside window
bool AutomaticFish::Draw(const float &time, sf::RenderWindow& window) {
	pos_.x -= speed_.x * (time - time_created_);
	//усложнить траекторию!
	speed_.y = sin(5 * time) / ((5 + rand() % 5) + (10 + rand() % 10) * (time - time_created_));
	pos_.y += speed_.y * (time - time_created_);
	fish_.setTexture(tex_);
	fish_.setScale(scale_);
	fish_.scale(1.0f, -1.0f);
	fish_.setPosition(pos_.x, pos_.y);
	window.draw(fish_);
	return (pos_.x < 0);
}

//additionally setting size of font used to display points added
ControlledFish::ControlledFish(const sf::Vector2f& pos, FishType type) : Fish(pos, type), add_points_(20) {}

void ControlledFish::Rotate(sf::RenderWindow& window, sf::Vector2f d) {
	const double Pi = 3.14159f;
	double angle = 180 + atan2f(d.y, d.x) * 180.0 / Pi;
	fish_.setRotation(angle);
}

void ControlledFish::Move(sf::Vector2u& TextureSize, Background& background) {
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
		pos_.y -= speed_;
		if (!IsInsideWindow(TextureSize, pos_)) {
			pos_.y += speed_;
		}
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
		pos_.y += speed_;
		if (!IsInsideWindow(TextureSize, pos_)) {
			pos_.y -= speed_;
		}
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
		pos_.x += speed_;

		if (TextureSize.x < pos_.x + 500) {
			std::cout << "Background added" << std::endl;
			background.AddBackground();
			TextureSize = background.GetBackgroundTextureSize();		//добавляет очередной кусок фона
		}

		/*if (!IsInsideWindow(TextureSize, fish_.getPosition())) {
			pos_.x -= speed_;
		}*/
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
		pos_.x -= speed_;
		if (!IsInsideWindow(TextureSize, pos_)) {
			pos_.x += speed_;
		}
	}

}

void ControlledFish::Laser(sf::RenderWindow& window, sf::Vector2f center, sf::Vector2f worldPos) {
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::L)) {        //laser
		sf::Vertex line[] =
		{
				sf::Vertex(center),
				//sf::Vertex(sf::Vector2f(MousePos.x, MousePos.y))
				sf::Vertex(sf::Vector2f(worldPos.x, worldPos.y))
		};

		line->color = (sf::Color::Red);
		window.draw(line, 2, sf::Lines);
	}
}

void ControlledFish::Control(sf::Vector2u& textureSize, sf::RenderWindow& window, Background& background) {

	sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
	sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos);
	sf::Vector2u fishSize = fish_.getTexture()->getSize();
	fish_.setOrigin(fishSize.x / 2, fishSize.y / 2);
	sf::Vector2f center = fish_.getPosition();
	sf::Vector2f d = sf::Vector2f(worldPos.x, worldPos.y) - center;

	//сюда добавляю передачу фона, чтобы вовремя его продлевать
	Move(textureSize, background);

	Rotate(window, d);
	Laser(window, center, worldPos);
}

bool ControlledFish::isTouched(const AutomaticFish& autoFish) {
	return fish_.getGlobalBounds().intersects(autoFish.GetSprite().getGlobalBounds());
}


//additionally getting time for "adding points animation"
void ControlledFish::Eat(std::vector<AutomaticFish>& autoFish, std::vector<AutomaticFish>::iterator it_del, 
						const float& time) {

	delta_pts_ = type_points[(*it_del).GetType()];

	points_ += delta_pts_;
	plus_pts_string = "+ " + to_str(delta_pts_);

	std::cout << "current points = " << points_ << std::endl;

	time_fish_eaten_ = time;
	
	autoFish.erase(it_del);		//seems OK
	
	if (points_ >= type_limit_points[type_])
		ChangeType();
}

void ControlledFish::ChangeType() {
	int type = static_cast<int> (type_);
	type += 1;
	if (type > 4) {
		std::cout << "You WIN!!!" << std::endl;
		return;
	}
	type_ = static_cast<FishType> (type);
	scale_ = type_scale[type_];
	LoadSprite();
}


//return true if you died
bool ControlledFish::DetectFish(std::vector<AutomaticFish>& autoFish, const float& time) {
	bool imDied = false;
	auto it = autoFish.begin();

	//fixing simultaneous iteration and deleting from one vector
	std::vector<AutomaticFish>::iterator to_eat;
	bool poedanie = false;

	while (it != autoFish.end()) {
		if (isTouched(*it)) {
			if (type_ >= (*it).GetType()) {
				//Eat(autoFish, it);
				to_eat = it;
				poedanie = true;
			}
			else {
				imDied = true;
				break;
			}
		}
		it++;
	}

	if (poedanie) {
		Eat(autoFish, to_eat, time);
	}
	/*for (AutomaticFish& fish : autoFish) {
		if (isTouched(fish)) {
			if (type_ >= fish.GetType()) {
				Eat(autoFish, it);
			}
			else {
				imDied = true;
				break;
			}
		}
		//it++;
	}*/
	return imDied;
}

void ControlledFish::Draw(sf::RenderWindow &window, const float& time) {
	fish_.setTexture(tex_);
	fish_.setScale(scale_);
	fish_.setPosition(pos_.x, pos_.y);
	window.draw(fish_);

	if (time - time_fish_eaten_ < 1.0)
		PointsAnimation(window, time);
}


void ControlledFish::PointsAnimation(sf::RenderWindow& window, const double& time) {
	sf::Vector2f delta_pts_pos;
	delta_pts_pos.x = pos_.x;
	delta_pts_pos.y = pos_.y - tex_.getSize().y * scale_.y / 2 - 75 * (time - time_fish_eaten_);	//going up effect

	add_points_.Display(window, delta_pts_pos, plus_pts_string);
}


int ControlledFish::GetScore() {
	return points_;
}





FishGeneration::FishGeneration() {}

FishType FishGeneration::GenerateType() {
	//чтобы было распределение "чем выше рыба по уровню, тем реже появляется"
	int genType = rand() % 100;
	FishType type;
	if (genType < 40)
		type = FishType::L_1;
	if (genType >= 40 && genType < 70)
		type = FishType::L_2;
	if (genType >= 70 && genType < 90)
		type = FishType::L_3;
	if (genType >= 90 && genType < 100)
		type = FishType::L_4;
	return type;
}


void FishGeneration::Draw(const float &time, sf::RenderWindow& window) {

	for (int i = 0; i < autoFish.size(); ++i) {
		if (autoFish[i].Draw(time, window)) {
			autoFish.erase(autoFish.begin() + i);
		}
	}
}

//сюда теперь передаем самого перса, чтобы на расстоянии от него генерились рыбы
//а не на конце окна(иначе беда)
void FishGeneration::GenerateFish(const float &time, const sf::Vector2f& current_fish) {
	if (time > fish_creation_time) {
		float x = current_fish.x + 600.f + rand() % 400;
		float y = 100 + rand() % 400;			//?????
		FishType type = GenerateType();
		autoFish.push_back(AutomaticFish({ x, y }, type, time));
		float dt = rand() % 3;
		fish_creation_time += dt;

	}
}