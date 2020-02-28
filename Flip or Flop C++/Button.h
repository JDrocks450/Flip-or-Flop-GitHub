#pragma once
#include "Engine.h"
#include <cstdarg>

namespace FlipOrFlop {
	class Button : public GameObject
	{
		Texture* assets[3]{ nullptr };
		Sprite* sprites[3]{ nullptr };
		bool isMouseDown = false;

		void setup(string* arr, int leng);
	public:
		typedef void (*OnClickFuncCall)(Button* sender);
		OnClickFuncCall OnClick;

		Vector2f Position{ 0,0 };

		enum STATE {
			REG,
			MOUSEOVER,
			MOUSECLICK,
		};
		STATE CurrentState = REG;

		Button(string* buttonTextures, int leng = 3, OnClickFuncCall onClick = 0);
		//Automatically concatinates "_reg", "_hi", "_click" and ".png" to the submitted asset name
		Button(string incompletePath, int leng = 3, OnClickFuncCall onClick = 0);
		~Button();

		inline void Load() {};
		void Update(double gt);
		void Draw(sf::RenderTarget& win, sf::RenderStates states);
	};
}

