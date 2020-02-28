#include "pch.h"
#include "Squidward.h"

using namespace FlipOrFlop;

Squidward::Squidward()
{
}


Squidward::~Squidward()
{
}

void Squidward::Load()
{
	GameObject::Load();
	if (tentacle_texture == nullptr) {
		tentacle_texture = new Texture();
		tentacle_texture->loadFromFile(Engine::ContentLoad(tentacle_fileName));
	}
	if (tentacle_sprite == nullptr)	
		tentacle_sprite = new Sprite(*tentacle_texture);
	sprite->setScale(.65, .65);
	sprite->setOrigin(.5, .5);
	tentacle_sprite->setScale(.5, .5);
	tentacle_sprite->setOrigin(.5, .5);
	Position = Vector2f(-253, 250);
}

void FlipOrFlop::Squidward::OnClick(double gt)
{
	Received = true;
}

void Squidward::Update(double gt)
{
	if (OnScreen) {
		if (Position.x < APPEAR_XVALUE)
			Position.x += SPEED;
		if (Position.x > APPEAR_XVALUE)
			Position.x = APPEAR_XVALUE;
	}
	else
	{
		if (Position.x > DISAPPEAR_XVALUE)
			Position.x -= SPEED;
	}
}

void Squidward::Draw(RenderTarget& win, RenderStates states)
{	
	Vector2f pos = Position;
	int directionMultiplier = 1;
	if (direction == TAKE_PATTY) { //quick and dirty flip horizontal
		if (sprite->getScale().x > 0) {
			auto scale = sprite->getScale();
			sprite->setScale(scale.x * -1, scale.y);
			scale = tentacle_sprite->getScale();
			tentacle_sprite->setScale(scale.x * -1, scale.y);
		}
		pos.x += sprite->getGlobalBounds().width;
		directionMultiplier = -1;
	}
	win.draw(*tentacle_sprite, states);
	GameObject::Draw(win, states);
	tentacle_sprite->setPosition(pos.x, pos.y + 180);
	sprite->setPosition(pos.x + (35 * directionMultiplier), pos.y);
	win.draw(*tentacle_sprite, states);
	tentacle_sprite->setPosition(pos.x, pos.y + 150);
}
