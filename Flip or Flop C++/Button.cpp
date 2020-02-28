#include "pch.h"
#include <iostream>
#include "Button.h"

using namespace FlipOrFlop;
void FlipOrFlop::Button::setup(string* arr, int leng)
{
	for (int i = 0; i < 3; i++)
	{
		Texture* t = nullptr;
		if (i >= leng && i > 0)
			t = assets[0];
		else if (i >= leng && i == 0) {
			t = new Texture();
			t->loadFromFile(Engine::ContentLoad("missing.png"));
		}
		else if (i < leng) {
			t = new Texture();
			t->loadFromFile(Engine::ContentLoad(arr[i]));
			t->setSmooth(true);
		}
		if (t == nullptr)
		{
			cout << "A really weird error occurred so this button has to stop being setup!";
			return;
		}
		assets[i] = t;
		sprites[i] = new Sprite(*t);
	}	
	sprite = sprites[0];
}

Button::Button(string* buttonTextures, int leng, OnClickFuncCall onClick)
{
	setup(buttonTextures, leng);
	OnClick = onClick;
}

FlipOrFlop::Button::Button(string incompletePath, int leng, OnClickFuncCall onClick)
{
	string assets[3]{ incompletePath + "_reg", incompletePath + "_hi", incompletePath + "_click" };
	setup(assets, 3);
	OnClick = onClick;
}

Button::~Button()
{
	for (auto texture : assets)
	{
		delete texture;
		cout << "BUTTON DELETE: Texture " << sizeof(*texture) << " bytes" << endl;
	}
	for (auto sprite : sprites)	
		delete sprite;	
}

void Button::Update(double gt)
{
	for (auto image : sprites)
		image->setPosition(Position - Vector2<float>(sprite->getGlobalBounds().width /2, sprite->getGlobalBounds().height /2));
	extern sf::Vector2i MOUSE_POSITION;
	if (sprite->getGlobalBounds().contains(Vector2<float>(MOUSE_POSITION))) {
		if (Mouse::isButtonPressed(Mouse::Button::Left)) {
			isMouseDown = true;
			CurrentState = MOUSECLICK;
		}
		else {
			if (isMouseDown && OnClick != 0) {
				OnClick(this);
				isMouseDown = false;
			}
			else
				CurrentState = MOUSEOVER;
		}
	} else CurrentState = REG;
}

void Button::Draw(sf::RenderTarget& win, sf::RenderStates states)
{
	win.draw(*sprites[CurrentState], states);
}
