#pragma once
#include "Engine.h"

using namespace std;

namespace FlipOrFlop{
	class Squidward : public GameObject
	{
		const string tentacle_fileName = "tentacle.png";
		Texture* tentacle_texture = nullptr;
		Sprite* tentacle_sprite = nullptr;

		const float SPEED = .6f;
	public:
		const float APPEAR_XVALUE = -53;
		const float DISAPPEAR_XVALUE = -253;

		Squidward();
		~Squidward();

		bool Received = false;

		string getFileNameString() {
			return "squidward.png";
		}

		sf::Vector2f Position;
		enum DIRECTION {
			TAKE_PATTY,
			GIVE_ORDER
		};
		DIRECTION direction = TAKE_PATTY;

		void Load();

		bool OnScreen = false;

		void OnClick(double gt);

		void Update(double gt);
		void Draw(RenderTarget& win, RenderStates states);
	};
}

