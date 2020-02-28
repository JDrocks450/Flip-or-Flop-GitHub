#include "pch.h"
#include "Messenger.h"

sf::Texture* _includeOnlyTexture = nullptr, *_excludeTexture = nullptr, *krabbypattyTexture = nullptr;
sf::Sprite* _includeOnlySprite = nullptr, *_excludeSprite = nullptr, * krabbypattySprite = nullptr;

FlipOrFlop::Messenger* FlipOrFlop::MESSENGER;
std::vector<FlipOrFlop::Order*> FlipOrFlop::ORDERS;


FlipOrFlop::Messenger::Messenger()
{
	MESSENGER = this;
	
}


FlipOrFlop::Messenger::~Messenger()
{
}

void FlipOrFlop::Messenger::Update(double gt)
{
	if (GetStatus())
		DisplayMessage();	
}

void FlipOrFlop::Messenger::Draw(sf::RenderTarget & target, sf::RenderStates states)
{
}

using namespace FlipOrFlop;
const Topping::TOPPING RequiredToppings[2]{ Topping::TOPPING::BUN_BOTTOM, Topping::TOPPING::BUN_TOP };
bool _ticketAdding = false;

FlipOrFlop::Order::Order(Topping::TOPPING exclusion, MODE mode, double gt)
{
	_lastgametime = gt;
	IsClickable = false;
	if (std::find(RequiredToppings, RequiredToppings + 2, exclusion) != RequiredToppings + 2)
		exclusion = Topping::TOPPING::BUN_BOTTOM;
	Exclusions = exclusion;
	Mode = mode;
	extern std::vector<Order*> ORDERS;
	ORDERS.push_back(this);
	Load();
}

FlipOrFlop::Order::~Order()
{
}

int FlipOrFlop::Order::getIndex()
{
	extern vector<Order*> ORDERS;
	return distance(ORDERS.begin(), find(ORDERS.begin(), ORDERS.end(), this));
}

bool FlipOrFlop::Order::CheckBurger(Burger* Submission)
{
	if (Exclusions != Topping::TOPPING::PATTY || Mode == MODE::EXCLUSIVE_INCLUDE)
		if (Submission->Items[1]->type == Topping::TOPPING::PATTY) {
			if (((Patty*)Submission->Items[1])->currentState != Patty::DONE)
				return false;
		}
		else return false;
	if (Exclusions == Topping::TOPPING::BUN_BOTTOM) //regular burger
	{
		if (Submission->Items[0]->type == Topping::TOPPING::BUN_BOTTOM &&
			Submission->Items[1]->type == Topping::TOPPING::PATTY &&
			Submission->Items[2]->type == Topping::TOPPING::CHEESE &&
			Submission->Items[3]->type == Topping::TOPPING::LETTUCE &&
			Submission->Items[4]->type == Topping::TOPPING::PICKLES &&
			Submission->Items[5]->type == Topping::TOPPING::KETCHUP &&
			Submission->Items[6]->type == Topping::TOPPING::MUSTARD &&
			Submission->Items[7]->type == Topping::TOPPING::MAYONAISE &&
			Submission->Items[8]->type == Topping::TOPPING::BUN_TOP) {
			MESSENGER->EnqueueMessage(Messenger::MESSAGES::NICE_JOB);
			return true;
		}			
		else {
			MESSENGER->EnqueueMessage(Messenger::MESSAGES::WRONG_COMBO);
			return false;
		}
	}
	bool exclusiveAchieved = false;
	for (auto topping : Submission->Items)
		switch (Mode) {
		case MODE::EXCLUDE:
			if (topping->type == Exclusions)
				return false;
			break;
		case MODE::EXCLUSIVE_INCLUDE:
			if (Exclusions == Topping::TOPPING::PATTY) {
				if (Submission->Items.size() == 3) {
					if (Submission->Items[0]->type == Topping::TOPPING::BUN_BOTTOM &&
						Submission->Items[1]->type == Topping::TOPPING::PATTY &&
						Submission->Items[2]->type == Topping::TOPPING::BUN_TOP) {
						MESSENGER->EnqueueMessage(Messenger::MESSAGES::NICE_JOB);
						return true;
					}
				}
			}
			else if (Submission->Items.size() == 4) {
				if (Submission->Items[0]->type == Topping::TOPPING::BUN_BOTTOM &&
					Submission->Items[1]->type == Topping::TOPPING::PATTY &&
					Submission->Items[2]->type == Exclusions &&
					Submission->Items[3]->type == Topping::TOPPING::BUN_TOP)
				{
					MESSENGER->EnqueueMessage(Messenger::MESSAGES::NICE_JOB);
					return true;
				}
				else {
					MESSENGER->EnqueueMessage(Messenger::MESSAGES::WRONG_ORDER);
					return false;
				}
			}
			MESSENGER->EnqueueMessage(Messenger::MESSAGES::WRONG_COMBO);
			return false;
		}
	return true;
}

void FlipOrFlop::Order::Complete()
{
	ORDERS.erase(remove(ORDERS.begin(), ORDERS.end(), this), ORDERS.end());
	delete this;
}

int getDesiredX(int index, unsigned int texturesizeX, float scaledsize) {
	return 30 + ((index > 0) ? (texturesizeX * scaledsize) + ((-1 + index) * (texturesizeX * .35) + 10) : 0);
}

void FlipOrFlop::Order::Load()
{
	GameObject::Load();
	sprite->setPosition(getDesiredX(getIndex(), texture->getSize().x, 0.000732 * GAMESCR->GetSize().x), -sprite->getGlobalBounds().height);
	displayedTopping = new Topping(Exclusions);
	displayedTopping->SwitchGraphic_PlacedState();
	displayedTopping->Load();

	//setup exclude/include sprites
	if (_excludeTexture == nullptr) {
		_excludeTexture = new Texture();
		_excludeTexture->loadFromFile(Engine::ContentLoad("exclude.png"));
		_excludeSprite = new Sprite(*_excludeTexture);
	}
	if (_includeOnlyTexture == nullptr) {
		_includeOnlyTexture = new Texture();
		_includeOnlyTexture->loadFromFile(Engine::ContentLoad("only.png"));
		_includeOnlySprite = new Sprite(*_includeOnlyTexture);
	}
	if (krabbypattyTexture == nullptr) {
		krabbypattyTexture = new Texture();
		krabbypattyTexture->loadFromFile(Engine::ContentLoad("krabbypatty.png"));
		krabbypattySprite = new Sprite(*krabbypattyTexture);
	}
}

extern GameScreen* GAMESCR;

void FlipOrFlop::Order::Update(double gt)
{
	double delta = gt - _lastgametime;	
	_lastgametime = gt;
	int i = getIndex();
	float currentTicket_ScaledSize = 0.000732 * GAMESCR->GetSize().x;
	if (currentTicket_ScaledSize > .75)
		currentTicket_ScaledSize = .75;
	else if (currentTicket_ScaledSize < .35)
		currentTicket_ScaledSize = .35;	
	if (i == 0) {
		if (sprite->getScale().x < currentTicket_ScaledSize)
			sprite->setScale((sprite->getScale().x + SPEED / 5) * delta, (sprite->getScale().x + SPEED / 5) * delta);
		if (sprite->getScale().x > currentTicket_ScaledSize)
			sprite->setScale(currentTicket_ScaledSize, currentTicket_ScaledSize);
		displayedTopping->sprite->setScale(topping_desiredScale + .08, topping_desiredScale + .08);
	}
	else {
		sprite->setScale(.35, .35);
		displayedTopping->sprite->setScale(topping_desiredScale - .02, topping_desiredScale - .02);
	}
	if (!Added)
		if (!_ticketAdding || Adding) {
			Adding = true;
			_ticketAdding = true;
			if (sprite->getPosition().y < 0)
				sprite->move(0, SPEED * delta);
			else if (sprite->getPosition().y > 0)
				sprite->setPosition(sprite->getPosition().x, 0);
			else {
				_ticketAdding = false;
				Adding = false;
				Added = true;
			}
		}

	float desiredX = getDesiredX(getIndex(), texture->getSize().x, currentTicket_ScaledSize);
	if (sprite->getPosition().x > desiredX)
		sprite->move(-SPEED * delta, 0);
	if (sprite->getPosition().x < desiredX)
		sprite->setPosition(desiredX, sprite->getPosition().y);
	displayedTopping->sprite->setPosition(
		sprite->getPosition().x + (sprite->getGlobalBounds().width / 2) - (displayedTopping->sprite->getGlobalBounds().width / 2),
		15 + sprite->getPosition().y + (sprite->getGlobalBounds().height / 2) - (displayedTopping->sprite->getGlobalBounds().height / 2));
}

void FlipOrFlop::Order::Draw(RenderTarget& win, RenderStates s)
{
	GameObject::Draw(win, s);
	if (Exclusions == Topping::TOPPING::BUN_BOTTOM) {
		if (getIndex() != 0)
			krabbypattySprite->setScale(.5, .5);
		else
			krabbypattySprite->setScale(1, 1);
		krabbypattySprite->setPosition(
			sprite->getPosition().x + (sprite->getGlobalBounds().width / 2) - (krabbypattySprite->getGlobalBounds().width / 2),
			15 + sprite->getPosition().y + (sprite->getGlobalBounds().height / 2) - (krabbypattySprite->getGlobalBounds().height / 2));
		win.draw(*krabbypattySprite, s);
		return;
	}
	else
		displayedTopping->Draw(win, s);
	switch (Mode) {
	case EXCLUDE:
		_excludeSprite->setPosition(sprite->getPosition());
		_excludeSprite->setScale(sprite->getScale());
		win.draw(*_excludeSprite, s);
		break;
	case EXCLUSIVE_INCLUDE:
		_includeOnlySprite->setPosition(sprite->getPosition());
		_includeOnlySprite->setScale(sprite->getScale());
		win.draw(*_includeOnlySprite, s);
		break;
	}
}
