#include "pch.h"
#include "Topping.h"
#include "Spongebob.h"
#include "Positioner.h"

using namespace FlipOrFlop;

const map<Topping::TOPPING, std::tuple<string, float>> fileNameTable = {
			{Topping::TOPPING::CHEESE, make_tuple("cheese.png", .1)},
			{Topping::TOPPING::LETTUCE, make_tuple("lettuce.png", .09)},
			{Topping::TOPPING::PICKLES, make_tuple("pickles.png", .08)},
			{Topping::TOPPING::KETCHUP, make_tuple("ketchup.png", .25F)},
			{Topping::TOPPING::MUSTARD, make_tuple("mustard.png", .25F)},
			{Topping::TOPPING::MAYONAISE, make_tuple("mayonaise.png", .25F)},
			{Topping::TOPPING::BUN_BOTTOM, make_tuple("bottom bun.png", .09)},
			{Topping::TOPPING::BUN_TOP, make_tuple("top bun.png", .1)},
			{Topping::TOPPING::PATTY, make_tuple("cooked_patty.png", .1)}
};

FlipOrFlop::Topping::Topping(TOPPING toppingType)
{
	type = toppingType;
	_fileName = get<0>(fileNameTable.at(type));
}

FlipOrFlop::Topping::~Topping()
{
}

std::tuple<string, float> FlipOrFlop::Topping::GetFileNameTableEntry(Topping::TOPPING type)
{
	return fileNameTable.at(type);
}

std::tuple<string, float> FlipOrFlop::Topping::GetInfo()
{
	return GetFileNameTableEntry(type);
}

void FlipOrFlop::Topping::Load()
{
	switch (type) {
	case TOPPING::KETCHUP:
	case TOPPING::MUSTARD:
	case TOPPING::MAYONAISE:
		IsClickable = true;
		IsPerpixelClickCheck = true;
		break;
	}
	GameObject::Load();
	float scale = get<1>(fileNameTable.at(type));
	sprite->setScale(scale, scale);
}

void FlipOrFlop::Topping::OnClick(double gt)
{
	auto t = new Topping(type);
	t->Load();

	extern Spongebob* FlipOrFlop::SPONGEBOB;
	if (SPONGEBOB->Interact(this, gt)) //Check if spongebob is able to interact first before adding the topping to the burger.
		if (t->AddToBurger())
			return;
	delete t;
}

bool FlipOrFlop::Topping::AddToBurger(int index)
{
	extern Burger* BURGER_SLOTS[4];
	bool force = false;
	if (index == -1) {
		index = 0;
		force = true;
	}
	for (int i = 0; i < 4; i++)
		if (BURGER_SLOTS[index]->AddTopping(this))
		{
			SwitchGraphic_PlacedState();
			t_state = ON_BURGER;
			sprite->setOrigin(0, 0);
			sprite->setScale(.09, .09);
			extern std::vector<Patty*> GrillingPatties;
			GrillingPatties.erase(remove(GrillingPatties.begin(), GrillingPatties.end(), this), GrillingPatties.end());
			return true;
		}
		else index++;
	return false;
}

string FlipOrFlop::Patty::getFileNameString()
{
	switch (currentState) {
	case UNCOOKED: return "uncooked_patty.png";
	case FLIPPING: case FLIPME: case RARE: return "rare_patty.png";
	case DONE: return "cooked_patty.png";
	case BURNT: return "burnt_patty.png";
	case DELETE: return "missing.png";
	}	
}

FlipOrFlop::Patty::Patty()
{
	type = TOPPING::PATTY;
}

FlipOrFlop::Patty::~Patty()
{
	delete sizzleSprite;
}

void FlipOrFlop::Patty::Load()
{
	_prevState = currentState;
	GameObject::Load();	
	if (sizzleSprite == nullptr) {
		sprite->setScale(.1, .1);
		sizzleSprite = new Sprite();
		sizzleSprite->setScale(.1, .1);
	}
}

void FlipOrFlop::Patty::Grill(double gt)
{
	if (_gameTime_cookStateChanged == -1) {
		_gameTime_cookStateChanged = gt;
		sprite->setOrigin(Vector2f(texture->getSize() / (unsigned int)2)); 
		sizzleSprite->setOrigin(Vector2f(texture->getSize() / (unsigned int)2));
		sizzleSprite->setPosition(sprite->getPosition());
	}	
	
	switch (currentState) {
	case UNCOOKED:
	case RARE:
	case DONE:
	case BURNT:
		if (_gameTime_cookStateChanged + GRILL_TIME <= gt) {
			currentState = State(currentState + 1);
			_gameTime_cookStateChanged = gt;
			if (currentState == BURNT)
			{
				extern Messenger* MESSENGER;
				MESSENGER->EnqueueMessage(Messenger::BURNT_PATTY);
			}
		}
		break;
	case FLIPME:
		if (_gameTime_cookStateChanged + GRILL_TIME <= gt)
		{
			currentState = BURNT;
			_gameTime_cookStateChanged = gt;
		}
		break;
	case FLIPPING:
		const float FLIP_CHANGE = .5, FLIP_TIME = .5;
		flipAnimPercent = (gt - _gameTime_cookStateChanged) / FLIP_TIME;
		float offset =
			(flipAnimPercent < .5 ? -1 : 1) * (FLIP_CHANGE * (abs(flipAnimPercent - .5)));
		sprite->move(0, offset);
		sprite->setRotation(360 * -flipAnimPercent);
		if (flipAnimPercent >= 1.0F)
		{
			currentState = RARE;
		}
		break;
	}

	//sizzle animation
	if (currentState != DELETE && currentState != BURNT && currentState != FLIPPING)
		if (_sizzleSwitch + SIZZLE_TIME < gt)
		{
			_sizzleSwitch = gt;
			_sizzleSwitchTextureValue = !_sizzleSwitchTextureValue;
			switch (_sizzleSwitchTextureValue) {
			case true:
				sizzleSprite->setTexture(*LoadTexture(Engine::ContentLoad("sizzle1.png")));
				break;
			case false:
				sizzleSprite->setTexture(*LoadTexture(Engine::ContentLoad("sizzle2.png")));
				break;
			}
		}
}

void FlipOrFlop::Patty::Flip(double gt)
{
	if (currentState == FLIPME) {
		_gameTime_cookStateChanged = gt;
		currentState = FLIPPING;
	}
}

void FlipOrFlop::Patty::OnClick(double gt)
{
	extern Spongebob* FlipOrFlop::SPONGEBOB;
	SPONGEBOB->Interact(this, gt);
}

sf::Vector2f FlipOrFlop::Patty::AddToGrill()
{
	extern std::vector<Patty*> GrillingPatties;
	auto pos = GetOpenGrillSpot();
	if (pos == Vector2f(-1, -1))
		return pos;
	sprite->setPosition(pos);
	GrillingPatties.push_back(this);
	t_state = ON_GRILL;
	currentState = UNCOOKED;
	return pos;
}

void FlipOrFlop::Patty::Draw(RenderTarget & target, RenderStates states)
{
	if (_prevState != currentState)
		Load();
	GameObject::Draw(target, states);
	if (t_state != ON_BURGER && currentState != DELETE && currentState != BURNT && currentState != FLIPPING)
		target.draw(*sizzleSprite);
}

sf::Vector2f FlipOrFlop::Patty::GetOpenGrillSpot()
{
	extern std::vector<Patty*> GrillingPatties;
	extern sf::Rect<float> STOVE_BOUNDARY;

	std::random_device dev;
	std::default_random_engine engine(dev());
	std::uniform_int_distribution<int> distX(0, STOVE_BOUNDARY.width - 170);
	std::uniform_int_distribution<int> distY(0, STOVE_BOUNDARY.height - 50);

	int retries = 0;

	while (true) {
		if (retries > 10)
			return Vector2f(-1, -1);
		auto pos = Vector2f(STOVE_BOUNDARY.left + 60 + distX(engine), STOVE_BOUNDARY.top + distY(engine));
		bool match = false;
		for (auto patty : GrillingPatties)
			if (sf::Rect<float>(pos, getScaledSize()).intersects(sf::FloatRect(patty->getHitbox()))) {
				match = true;
				break;
			}
		if (!match)
			return pos;
		retries++;
	}
}
