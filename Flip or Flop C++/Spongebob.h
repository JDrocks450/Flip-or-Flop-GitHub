#pragma once
#include "engine.h"
#include "Stack.h"
#include <vector>
#include "Messenger.h"

namespace FlipOrFlop {
	class Spongebob : public GameObject
	{
		const float PATTYFLIP_INTERACT_TIME = .2f;
		const float IGNOREMOUSE_TIME = .2f;

		float _gameTime_mouseIgnoreStart = -1;
		float _interaction_right_StartTime = -1;

			
		bool _dropOnClick = false;

		template <typename T>
		void moveHeldObjectToHand(T* object);
		template <> void moveHeldObjectToHand(FlipOrFlop::Stack* object);
		template <> void moveHeldObjectToHand(FlipOrFlop::Burger* object) { moveHeldObjectToHand<Stack>(object); }
	public:
		Spongebob();
		~Spongebob();

		bool IgnoringMouse = false;
		bool HoldingObject = false;
		bool Interacting = false;

		Stack* heldObject = nullptr;

		static const int ScreenPositionOffset = 120;

		enum ANIM_STATE {
			RIGHT_SPATULA_HOLDING,
			RIGHT_SPATULA_FLIPPING,

			LEFT_IDLE,
			LEFT_GRAB_CLOSE,
			LEFT_GRAB_MID,
			LEFT_GRAB_FAR
		};

		string getFileNameString() {
			return "spongebob1.png";
		}

		std::vector<GameObject*> appendages;
		ANIM_STATE displayedAppendages[2];

		void Load();
		void Update(double gameTime);

		template <typename T>
		inline bool Interact(T* InteractWith, double gameTimeSeconds) {};
		template <> bool Interact(Topping* InteractWith, double gameTimeSeconds);
		template <> bool Interact(Patty* InteractWith, double gameTimeSeconds);
		void Hold(Stack* holdObject, bool releaseOnClick = true);
		void Release();

		void OnClick(double gt);
		void Draw(RenderTarget& target, RenderStates states);
	};

	template<>
	inline void Spongebob::moveHeldObjectToHand(FlipOrFlop::Stack* stack)
	{
		if (sprite->getPosition().x - ScreenPositionOffset < 0) {
			extern sf::Vector2i MOUSE_POSITION;
			stack->SetPosition(MOUSE_POSITION.x - (stack->Items[0]->getScaledSize().x / 2), MOUSE_POSITION.y);
		}
		else
		stack->SetPosition(sprite->getPosition().x + 50, sprite->getPosition().y + 325);
	}
	
	template<>
	inline void Spongebob::moveHeldObjectToHand(GameObject* object)
	{
		if (object->sprite == nullptr)
			return;
		object->sprite->setPosition(sprite->getPosition().x + 50, sprite->getPosition().y + 325);
	}	

	template<>
	inline bool Spongebob::Interact(Topping* InteractWith, double gameTimeSeconds)
	{
		if (HoldingObject)
			return false;
		Interacting = true;
		switch (InteractWith->type) {
		case Topping::TOPPING::CHEESE:
			displayedAppendages[1] = LEFT_GRAB_CLOSE;
			break;
		case Topping::TOPPING::LETTUCE:
			displayedAppendages[1] = LEFT_GRAB_MID;
			break;
		case Topping::TOPPING::PICKLES:
			displayedAppendages[1] = LEFT_GRAB_FAR;
			break;
		}		
		IgnoringMouse = true;
		_gameTime_mouseIgnoreStart = gameTimeSeconds;
		sprite->setPosition(InteractWith->sprite->getPosition().x+50, sprite->getPosition().y);
		_interaction_right_StartTime = gameTimeSeconds;
		return true;
	}

	template<>
	inline bool Spongebob::Interact(Patty* InteractWith, double gameTimeSeconds)
	{
		if (HoldingObject)
			return false;
		Interacting = true;
		sf::Vector2f pos;
		switch (InteractWith->currentState) {
		case Patty::UNPLACED:
			pos = InteractWith->AddToGrill();
			if (pos == Vector2f(-1, -1))
				return false;
			break;
		case Patty::RARE:
			if (InteractWith->AddToBurger()) {
				pos = InteractWith->sprite->getPosition();
				extern Messenger* MESSENGER;
				MESSENGER->EnqueueMessage(Messenger::UNDERCOOKED);
			}		
			break;
		case Patty::FLIPME:
			InteractWith->Flip(gameTimeSeconds);
			pos = InteractWith->sprite->getPosition();
			break;
		case Patty::DONE:			
			if (InteractWith->AddToBurger()) {
				pos = InteractWith->sprite->getPosition();
			}
			else return false;
			break;
		default:
			return false;
		}
		IgnoringMouse = true;
		_gameTime_mouseIgnoreStart = gameTimeSeconds;
		sprite->setPosition(pos.x-50, sprite->getPosition().y);
		displayedAppendages[0] = RIGHT_SPATULA_FLIPPING;
		_interaction_right_StartTime = gameTimeSeconds;
		return true;
	}
	
	extern Spongebob* SPONGEBOB;
}

