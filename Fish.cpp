#include "pch.h"

#include "fish.h"

Fish::Fish(const sf::Vector2f& pos, FishType type) {
	pos_ = pos;
	type_ = type;
	scale_ = type_scale[type];
	LoadSprite();

	//fish_.setTexture(tex_);
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

	//=========boosts============

	case FishType::SHRIMP: 
		if (!tex_.loadFromFile(shrimp_file))
			exit(-1);
		break;
	case FishType::OYSTER:
		if (!tex_.loadFromFile(oyster_file))
			exit(-1);
		break;
	case FishType::COIN:
		if (!tex_.loadFromFile(coin_file))
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

float Fish::GetSpeed() const {
	return speed_;
}

float Fish::GetAngle() const {
	return angle_;
}


sf::FloatRect ControlledFish::FishMouth() const {					//counts current coordinates of what should be fish mouth
	sf::FloatRect mouth = fish_.getGlobalBounds();

	//std::cout << "Before" << mouth.left << ":" << mouth.top << std::endl;

	switch (type_)
	{
	case FishType::L_1:
		mouth.top += 0.5f * fish_.getTexture()->getSize().y * scale_.y *(1.f + sin(angle_ - 180.f));
		mouth.height *= 0.4f;
		break;
	case FishType::L_2:
		mouth.top += 0.5f * fish_.getTexture()->getSize().y * scale_.y * (1.f + sin(angle_ - 180.f));
		mouth.height *= 0.35f;
		break;
	case FishType::L_3:
		mouth.top += 0.6f * fish_.getTexture()->getSize().y * scale_.y *(1.f + 0.5f * sin(angle_ - 180.f));
		mouth.height *= 0.25f;
		break;
	case FishType::L_4:
		mouth.top += 0.8f * fish_.getTexture()->getSize().y * scale_.y * (1 + 0.2f * sin(angle_ - 180.f));
		mouth.height *= 0.2f;
		break;
	
	default:
		break;
	}

	mouth.left += 0.4f * fish_.getTexture()->getSize().x * scale_.x * (1.f + cos(angle_ - 180.f));
	mouth.width *= 0.2f;

	//std::cout << "After" << mouth.left << ":" << mouth.top << std::endl;

	return mouth;
}

sf::FloatRect AutomaticFish::DangerZone() const {		//fish is eaten only "face to face"
	sf::FloatRect body = fish_.getGlobalBounds();
	body.top += 0.2f * fish_.getTexture()->getSize().y * scale_.y;
	body.height *= 0.6f;
	body.left -= 0.1 * fish_.getTexture()->getSize().x * scale_.x;
	body.width *= 0.8f;
	return body;
}


AutomaticFish::AutomaticFish(const sf::Vector2f& pos, FishType type, const float& time) : 
	Fish(pos, type), time_created_(time) {
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

	return (time - time_created_ > life_time);
}

//additionally setting size of font used to display points added
ControlledFish::ControlledFish(const sf::Vector2f& pos, FishType type) : Fish(pos, type), add_points_(20), 
						bar_(sf::Color::Green), boost_bar_(sf::Color::Red) {
	LoadSounds();
}


bool ControlledFish::CheckSharpRotate(DirectionType newDirectionType) {
	return (((previous_direction == DirectionType::UP) && (newDirectionType == DirectionType::DOWN)) ||
		((previous_direction == DirectionType::DOWN) && (newDirectionType == DirectionType::UP)) ||
		((previous_direction == DirectionType::LEFT) && (newDirectionType == DirectionType::RIGHT)) ||
		((previous_direction == DirectionType::RIGHT) && (newDirectionType == DirectionType::LEFT)));
}

void ControlledFish::GradualRotate(DirectionType newDirectionType) {
	float end_angle = directionType_angle[newDirectionType];
	float sign = end_angle - angle_;
	if (angle_ != end_angle) {
		float d_angle;
		if (((angle_ >= 0) && (angle_ < 90)) && (end_angle == 270))
			angle_ = angle_ + 360 - 1;
		else
			if ((((angle_ < 360) && (angle_ > 270))) && (end_angle == 270))
				angle_ -= 1;
			else
				if ((((angle_ < 360) && (angle_ >= 270))) && (end_angle == 0))
					angle_ += 1;
				else {
					d_angle = (sign > 0) ? 1 : -1;
					angle_ += d_angle;
				}
	}
}

void ControlledFish::Rotate(DirectionType newDirectionType) {
	current_direction = newDirectionType;
	
	if (newDirectionType != DirectionType::NOWHERE) {
		if (CheckSharpRotate(newDirectionType)) {
			angle_ = directionType_angle[newDirectionType];
		}
		GradualRotate(newDirectionType);
	}

	previous_direction = newDirectionType;
}

void ControlledFish::Move(sf::Vector2u& TextureSize, Background& background, const float& current_time) {
	float dt = (current_time - previous_control_time);
	previous_control_time = current_time;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
		Rotate(DirectionType::UP);
		pos_.y -= speed_ * dt;
		if (!IsInsideWindow(TextureSize, pos_)) {
			pos_.y += speed_ * dt;
		}
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
		Rotate(DirectionType::DOWN);
		pos_.y += speed_ * dt;
		if (!IsInsideWindow(TextureSize, pos_)) {
			pos_.y -= speed_ * dt;
		}
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
		Rotate(DirectionType::RIGHT);
		pos_.x += speed_ * dt;

		if (TextureSize.x < pos_.x + 500) {
			std::cout << "Background added" << std::endl;
			background.AddBackground();
			TextureSize = background.GetBackgroundTextureSize();
		}

	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
		Rotate(DirectionType::LEFT);
		pos_.x -= speed_ * dt;
		if (!IsInsideWindow(TextureSize, pos_)) {
			pos_.x += speed_ * dt;
		}
	} else {
		Rotate(DirectionType::NOWHERE);
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

void ControlledFish::Control(sf::Vector2u& textureSize, sf::RenderWindow& window, Background& background, const float& time) {

	sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
	sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos);
	sf::Vector2u fishSize = fish_.getTexture()->getSize();
	fish_.setOrigin(10, fishSize.y / 2);
	sf::Vector2f center = fish_.getPosition();
	sf::Vector2f d = sf::Vector2f(worldPos.x, worldPos.y) - center;

	Move(textureSize, background, time);
	Laser(window, center, worldPos);
}


//additionally getting time for "adding points animation"
void ControlledFish::Eat(std::vector<AutomaticFish>& autoFish, 
					std::vector<AutomaticFish>::iterator it_del, const float& time) {

	delta_pts_ = type_points[(*it_del).GetType()];

	points_ += delta_pts_;
	plus_pts_string = "+ " + to_str(delta_pts_);		//init'ing the string only once each time

	Resize();

	std::cout << "current points = " << points_ << std::endl;

	time_text_effect_ = time;		//for plus points animation
	
	autoFish.erase(it_del);		//seems OK

	PlaySound(collect_point_sound_);
	
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

	fish_.setTexture(tex_);
}

DirectionType ControlledFish::GetDirectionType()  const {
	return current_direction;
}

//return true if you died
bool ControlledFish::DetectFish(std::vector<AutomaticFish>& autoFish, const float& time, int& number_fish_eaten) {
	//it is here for now, need to find another place (probably)
	CheckBoost(time);

	bool imDied = false;
	auto it = autoFish.begin();

	//fixing simultaneous iteration and deleting from one vector
	std::vector<AutomaticFish>::iterator to_eat;
	bool poedanie = false;

	while (it != autoFish.end()) {
		if (isTouched(*it)) {
			if (type_ >= (*it).GetType()) {
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
		number_fish_eaten = to_eat - autoFish.begin();
		std::cout << number_fish_eaten << std::endl;
		Eat(autoFish, to_eat, time);
	}
	else {
		number_fish_eaten = -1;
	}

	return imDied;
}

void ControlledFish::Draw(sf::RenderWindow& window, const float& time) {
	fish_.setTexture(tex_);
	fish_.setScale(scale_);
	fish_.setPosition(pos_.x, pos_.y);
	fish_.setRotation(angle_);
	if (((angle_ >= 0) && (angle_ < 90)) || ((angle_ > 270) && (angle_ < 360)))
		fish_.scale(1.0f, -1.0f);
	window.draw(fish_);


	//drawing progress bar until the next level
	if (type_ != FishType::L_4) {
		if (type_ == FishType::L_1)
			bar_.Draw(window, points_, type_limit_points[type_], 80);
		else {
			int prev_type = static_cast<int> (type_) - 1;
			bar_.Draw(window, points_ - type_limit_points[static_cast<FishType> (prev_type)],
				type_limit_points[type_] - type_limit_points[static_cast<FishType> (prev_type)], 80);
		}
	}
	
	//drawing a bar indicating how much boost is left
	if (is_boost_applied) {
		boost_bar_.Draw(window, 1.f - (time - time_boost_applied), 1.f, 130);
	}

	//+pts animation above the fish
	if (time - time_text_effect_ < 0.75f)
		PointsAnimation(window, time);
}

float ControlledFish::GetSpeed() const {
	if (is_boost_applied) {
		return speed_;
	}
	else {
		return speed_ * boost_factor;
	}
}

void ControlledFish::SpeedBoost(const float& time, const float& factor) {
	if (!is_boost_applied) {		//если до этого нет ни одного буста (ускорителя)
		speed_ = speed_ * factor;
		time_boost_applied = time;
		is_boost_applied = true;
		boost_factor = factor;
	}
	else {
		if (boost_factor == factor) {		//если то же самое ускорение - просто продлеваем время
											//1.f каждый раз делает эффект длиннее (что-то вроде комбо)
			time_boost_applied += 1.0f;
		}
		else {							//если ускорение другое - создаем их комбо, которое длится полное время
										//возможно, стоит подумать над этой логикой
			boost_factor = boost_factor * factor;
			time_boost_applied = time;
		}
	}

	//std::cout << "Boost by: " << boost_factor << std::endl;		//test purposes

	time_text_effect_ = time;
	plus_pts_string = "boost x" + to_str(boost_factor);		//init'ing the string only once each time
}

void ControlledFish::CheckBoost(const float& time) {		//убирает буст по истечении времени его применения
	if ((time - time_boost_applied > 1.f) && is_boost_applied) {
		time_boost_applied = 0;
		is_boost_applied = false;
		speed_ = 500.f;
		//std::cout << "END " << std::endl;
	}
}

void ControlledFish::CoinBoost(const int& points_added, const float& time) {
	delta_pts_ = points_added;
	points_ += delta_pts_;
	plus_pts_string = "+ " + to_str(delta_pts_);		//init'ing the string only once each time

	time_text_effect_ = time;		//for plus points animation

	if (points_ >= type_limit_points[type_])
		ChangeType();
}



void ControlledFish::PointsAnimation(sf::RenderWindow& window, const double& time) {
	sf::Vector2f delta_pts_pos;
	delta_pts_pos.x = pos_.x;
	delta_pts_pos.y = pos_.y - tex_.getSize().y * scale_.y / 2 - 75 * (time - time_text_effect_);	//going up effect

	add_points_.Display(window, delta_pts_pos, plus_pts_string);
}


int ControlledFish::GetScore() {
	return points_;
}

//fish grows gradually
void ControlledFish::Resize() {
	if (type_ != FishType::L_4) {
		scale_.x += (0.002f * delta_pts_);
		scale_.y += (0.002f * delta_pts_);
	}
}


void ControlledFish::LoadSounds() {
	if (!collect_point_sound_.loadFromFile("C:/Users/User/MIPT/TheGame/Sounds/collect_point.wav"))
		exit(-1);
}

void ControlledFish::PlaySound(sf::SoundBuffer buffer) {
	sf::Sound sound;
	sound.setBuffer(buffer);
	//sound.setVolume(20);
	sound.play();
}

//AnotherPlayerFish::AnotherPlayerFish() {}

AnotherPlayerFish::AnotherPlayerFish(const sf::Vector2f& pos, FishType type, float angle,
	DirectionType directionType, float speed) : Fish(pos, type) {

	fish_.setTexture(tex_);
	fish_.setScale(scale_);

	angle_ = angle;
	current_direction = directionType;
	speed_ = speed;
}

void AnotherPlayerFish::NetUpdate(const sf::Vector2f& pos, FishType type, float angle,
									DirectionType directionType, float speed) {
	pos_ = pos;
	type_ = type;
	LoadSprite();
	scale_ = type_scale[type_];

	angle_ = angle;
	current_direction = directionType;
	speed_ = speed;
}


void AnotherPlayerFish::UpdatePosition(const float& time) {
	float dt = time - time_last_updated;

	if (current_direction == DirectionType::RIGHT) {
		pos_.x += speed_ * dt;
	}
	if (current_direction == DirectionType::LEFT) {
		pos_.x -= speed_ * dt;
	}
	if (current_direction == DirectionType::UP) {
		pos_.y -= speed_ * dt;
	}
	if (current_direction == DirectionType::DOWN) {
		pos_.y += speed_ * dt;
	}

	//pos_.x = speed_ * dt * cos(angle_ - 180);
	//pos_.y = speed_ * dt * sin(angle_ - 180);

	fish_.setPosition(pos_.x, pos_.y);

	time_last_updated = time;
}


void AnotherPlayerFish::Draw(sf::RenderWindow& window) {
	//fish_.scale(1.0f, -1.0f);
	fish_.setTexture(tex_);
	fish_.setScale(scale_);
	//fish_.setPosition(pos_.x, pos_.y);
	fish_.setRotation(angle_);
	if (((angle_ >= 0) && (angle_ < 90)) || ((angle_ > 270) && (angle_ < 360)))
		fish_.scale(1.0f, -1.0f);
	window.draw(fish_);
}

DirectionType AnotherPlayerFish::GetDirectionType()  const {
	return current_direction;
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

/*
void FishGeneration::Draw(const float &time, sf::RenderWindow& window) {

	for (int i = 0; i < autoFish.size(); ++i) {
		if (autoFish[i].Draw(time, window)) {
			autoFish.erase(autoFish.begin() + i);
		}
	}
}
*/

//сюда теперь передаем самого перса, чтобы на расстоянии от него генерились рыбы
//а не на конце окна(иначе беда)

void FishGeneration::GenerateFish(const float& time, const sf::Vector2f& current_fish) {
	if (time > last_creation_time) {
		float x = current_fish.x + 600.f + rand() % 400;
		float y = 100 + rand() % 400;			//?????

		FishType type = GenerateType();	
		
		autoCreature.push_back(AutomaticFish({ x, y }, type, time));

		float dt = rand() % 3;
		last_creation_time += dt;
	}
}