#pragma once
#include "engine.h"
#include <string>
#include <map>
#include <vector>
#include <random>

using namespace std;

namespace FlipOrFlop {
	class Topping : public GameObject
	{
	public:
		static const int ToppingNumber = 8;

		enum class TOPPING {
			PATTY,
			BUN_TOP,
			BUN_BOTTOM,

			CHEESE,
			LETTUCE,
			PICKLES,
			KETCHUP,
			MUSTARD,
			MAYONAISE,
		};
		TOPPING type;

		enum TOPPING_STATE {
			UNPLACED,
			ON_GRILL,
			ON_BURGER
		};
		TOPPING_STATE t_state = UNPLACED;

		Topping(TOPPING toppingType = TOPPING::BUN_BOTTOM);
		~Topping();

		string getPlacedFileNameString() {
			switch (type) {
			case TOPPING::KETCHUP:
				return "ketchup_place.png";
			case TOPPING::MUSTARD:
				return "mustard_place.png";
			case TOPPING::MAYONAISE:
				return "mayonaise_place.png";
			default: return getFileNameString();
			}
		}

		//Switches the graphic to the placed state version for this topping and applies the texture
		void SwitchGraphic_PlacedState() {
			_fileName = getPlacedFileNameString();
			GameObject::Load();
		}

		//returns the amount to raise this topping over the previous one in a stack
		//defaultly returns 10.
		float getStackOffset() {
			switch (type) {
			case TOPPING::MUSTARD:
			case TOPPING::MAYONAISE:
			case TOPPING::KETCHUP: return 5;
			default: return 10;
			}
		}

		static std::tuple<string, float> GetFileNameTableEntry(TOPPING type);
		std::tuple<string, float> GetInfo();

		void Load();
		void OnClick(double gt);
		bool AddToBurger(int index = -1);
	protected:		
		string _fileName;
		string getFileNameString() {
			return _fileName;
		}
	};

	class Patty : public Topping {		
	public:
		const int GRILL_TIME = 3;
		const float SIZZLE_TIME = .5f;

		enum State {
			UNCOOKED,
			FLIPME,
			FLIPPING,
			RARE,
			DONE,
			BURNT,
			DELETE,
		};
		State currentState = UNCOOKED;
		
		string getFileNameString();

		Patty();
		~Patty();

		void Load();
		void Grill(double gt);
		void Flip(double gt);
		void OnClick(double gt);
		sf::Vector2f AddToGrill();
		void Draw(RenderTarget& target, RenderStates states);

		sf::Vector2f GetOpenGrillSpot();
	private:
		float _gameTime_cookStateChanged = -1;
		float _sizzleSwitch = 0;
		bool _sizzleSwitchTextureValue = false; // 0 or 1 value
		float flipAnimPercent = 0;	

		Sprite* sizzleSprite = nullptr;
		State _prevState = UNCOOKED;
	};
}

