#include "pch.h"
#include "Spongebob.h"

FlipOrFlop::Spongebob* FlipOrFlop::SPONGEBOB;

FlipOrFlop::Spongebob::Spongebob()
{
	SPONGEBOB = this;
}


FlipOrFlop::Spongebob::~Spongebob()
{
	for (auto gameobject : appendages)
		delete gameobject;
}

void FlipOrFlop::Spongebob::Load()
{
	GameObject::Load();
	appendages.push_back(new GameObject("spongebob_spatula.png"));
	appendages.push_back(new GameObject("spongebob_spatula_down.png"));
	appendages.push_back(new GameObject("spongebob_hand.png"));
	appendages.push_back(new GameObject("spongebob_cheesegrab.png"));
	appendages.push_back(new GameObject("spongebob_lettucegrab.png"));
	appendages.push_back(new GameObject("spongebob_lettucegrab.png"));
	for (auto gameobject : appendages)
		if (gameobject != nullptr) {
			gameobject->IsClickable = false;
			gameobject->Load();		
		}
	displayedAppendages[0] = RIGHT_SPATULA_HOLDING;
	displayedAppendages[1] = LEFT_IDLE;
}

bool mouseLeftButtonLifted = true;

void FlipOrFlop::Spongebob::Update(double gameTime)
{
	extern sf::Vector2i MOUSE_POSITION;
	auto size = getScaledSize();
	auto newX = MOUSE_POSITION.x - size.x / 2;
	auto oldPos = sprite->getPosition();	

	if (!IgnoringMouse)
		sprite->setPosition(newX, sprite->getPosition().y);

	if (IgnoringMouse && _gameTime_mouseIgnoreStart + IGNOREMOUSE_TIME <= gameTime)
		IgnoringMouse = false;

	if (Interacting) {
		if (_interaction_right_StartTime + PATTYFLIP_INTERACT_TIME <= gameTime)
		{
			displayedAppendages[0] = RIGHT_SPATULA_HOLDING;
			displayedAppendages[1] = LEFT_IDLE;
			IgnoringMouse = false;
			Interacting = false;
		}
	}
	
	if (HoldingObject) //force spatula animation when holding an object.
	{
		displayedAppendages[0] = RIGHT_SPATULA_FLIPPING;
		moveHeldObjectToHand(heldObject);
		if (mouseLeftButtonLifted)
			if (Mouse::isButtonPressed(Mouse::Button::Left))
				OnClick(gameTime);
		Interacting = true;
	}

	if (!mouseLeftButtonLifted)
		if (!Mouse::isButtonPressed(Mouse::Left))
			mouseLeftButtonLifted = true;

	for(auto state : displayedAppendages)
		switch(state){			 
		case RIGHT_SPATULA_HOLDING:
			appendages[state]->sprite->setScale(.2, .2);
			appendages[state]->sprite->setPosition(oldPos.x + size.x - 70, oldPos.y + size.y / 2 - appendages[state]->getScaledSize().y / 2 + 20);
			break;
		case RIGHT_SPATULA_FLIPPING:
			appendages[state]->sprite->setScale(.2, .2);
			appendages[state]->sprite->setPosition(oldPos.x+40, oldPos.y + size.y / 2 + 50);
			break;
		case LEFT_IDLE:
			appendages[state]->sprite->setScale(.15, .15);
			appendages[state]->sprite->setPosition(oldPos.x - appendages[state]->getScaledSize().x + 10, oldPos.y + size.y / 2 + 10);
			break;
		case LEFT_GRAB_CLOSE:
		case LEFT_GRAB_MID:
		case LEFT_GRAB_FAR:	
			appendages[state]->SecondPassRenderOrderIndex = 1;
			appendages[state]->sprite->setScale(.15, .15);
			appendages[state]->sprite->setPosition(oldPos.x - appendages[state]->getScaledSize().x+23, oldPos.y + size.y / 2 + 80);
			break;
		}	
}

void FlipOrFlop::Spongebob::Hold(FlipOrFlop::Stack* holdObject, bool releaseOnClick)
{
	if (HoldingObject)
		return;
	HoldingObject = true;
	_dropOnClick = releaseOnClick;
	heldObject = holdObject;
	mouseLeftButtonLifted = false;
}

void FlipOrFlop::Spongebob::Release()
{
	HoldingObject = false;
	heldObject = nullptr;
}

void FlipOrFlop::Spongebob::OnClick(double gt)
{
	if (_dropOnClick && HoldingObject)
		Release();
}

void FlipOrFlop::Spongebob::Draw(RenderTarget & target, RenderStates states)
{
	appendages[displayedAppendages[1]]->Draw(target, states);
	GameObject::Draw(target, states);
	appendages[displayedAppendages[0]]->Draw(target, states);
}
