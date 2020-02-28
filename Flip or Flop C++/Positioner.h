#pragma once
#include "engine.h"
#include "Spongebob.h"
#include "Squidward.h"
#include "Stack.h"
#include "Button.h"

namespace FlipOrFlop {
	// Used by patties to determine where they should be placed
	extern std::vector<Patty*> GrillingPatties; 
	extern sf::Rect<float> STOVE_BOUNDARY;
	extern Burger* BURGER_SLOTS[4];

	extern int DIFFICULTY, MODE;

	struct Positioner : public GameObject {
		GameScreen* Parent;
		sf::Vector2i Parent_Size;
		
		Positioner() {
			extern GameScreen* GAMESCR;
			Parent = GAMESCR;
			Parent_Size = Parent->GetSize();
			IsClickable = false;
		}

		Positioner(GameScreen* Parent) {
			Positioner::Parent = Parent;
			Parent_Size = Parent->GetSize();
			IsClickable = false;
		}
	};

	class TitlePositioner : public Positioner
	{		
		static void Start_OnClick(FlipOrFlop::Button* button) {
			GameScreen::CreateGameScreen(nullptr, GameScreen::GAMEPLAY);
		}
		static void Settings_OnClick(FlipOrFlop::Button* button) {
			Engine::CreateWindowsProcess("C://Users//xXJDr//source//repos//Flip or Flop C++//Debug//SettingsDialog.exe");
		}
		Button UIButtons[2]{ Button("start", 3, Start_OnClick), Button("settings", 3, Settings_OnClick) };
		RectangleShape backgroundRect;
		const float HEIGHT = 75;
	public:
		using Positioner::Positioner;
		~TitlePositioner();

		void Load();
		void Update(double gt);
		void Draw(RenderTarget& win, RenderStates states);
	};

	class GameplayPositioner : public Positioner
	{				
		Squidward Squidward;

	public:
		using Positioner::Positioner;
		~GameplayPositioner();

		std::vector<GameObject*> UI_Objects;
		std::vector<Stack*> UI_Stacks;		

		GameObject Stove;
		sf::RectangleShape Counter;
		Spongebob Player;

		void PlaceObjects(std::vector<FlipOrFlop::Stack*>* UI_Stacks);

		static void LoadGameContext();

		void Load();
		void Update(double gt);
		void Draw(RenderTarget &target, RenderStates states);
	};
}

