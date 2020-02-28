#pragma once
#include "Topping.h"

namespace FlipOrFlop {
	struct Stack : public GameObject
	{
		bool REVERSE_ORDER = true;

		//Creates a generic stack that can hold any topping.
		inline Stack() {};
		Stack(Topping::TOPPING desiredStackType, int desiredAmount = 5);
		Stack(Topping::TOPPING desiredStackType, float x, float y, int desiredAmount = 5, bool reverse = true);
		~Stack();

		sf::Vector2<float> position;

		Topping::TOPPING Type;
		vector<Topping*> Items;

		void Load() {

		}
		
		void Generate(int Amount);

		inline Topping* operator[](std::size_t index) { return Items[index]; }
		
		inline void SetIsClickableState(bool state) {
			for (auto topping : Items)
				topping->IsClickable = false;
		}

		inline void SetPosition(float x, float y) { location_changed = true; position = Vector2f(x, y); }
		inline void SetPosition(sf::Vector2f location) { location_changed = true; position = location; }
		inline sf::Vector2f GetPosition() { return position; }

		inline void Remove(Topping* remove) { Items.erase(std::remove(Items.begin(), Items.end(), remove), Items.end()); }
		void Update(double gameTimeSeconds);
		void Draw(RenderTarget &target, RenderStates states);

	private:
		bool location_changed = false;
		int items_lastCount = 0;
	};

	struct Burger : public Stack {
		Burger(float x, float y);
		~Burger();

		bool Completed = false;

		bool AddedToppings[9]{ false };
		bool AddTopping(Topping* T);
		void Update(double gt);
	};
}