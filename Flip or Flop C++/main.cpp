#include "pch.h"
#include <SFML/Graphics.hpp>
#include <windows.h>
#include <ShellApi.h>
#include "engine.h"
#include <time.h>
#include <iostream>

void SHELL_EXE(const char* PATH) {
	ShellExecuteA(GetDesktopWindow(), "open", PATH, NULL, NULL, SW_SHOW);
};
extern void (*SHELL_EXEA)(const char* FILENAME) = SHELL_EXE;

enum {
	EV_RUN,
	EV_UPDATE,
	EV_DRAW,
	EV_IDLE,
	EV_QUIT,
	EV_UPDATEGAMESCRSIZE
};
int state = 0;

int main()
{
	int scr_w = 1024, scr_h = 768;
	sf::RenderWindow window(sf::VideoMode(scr_w, scr_h), "Bloaty's Flip or Flop");
	Engine engine(&window);
	double gameTime = 0.0;
	auto GAMESCR = GameScreen::CreateGameScreen(&window, GameScreen::SCREENS::TITLESCR); //identical to extern GAMESCR
	engine.load();	
	while (window.isOpen())
	{
		GAMESCR->resized = false;
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				state = EV_QUIT;
			if (event.type == sf::Event::LostFocus)
				state = EV_IDLE;
			if (event.type == sf::Event::GainedFocus)
				state = EV_RUN;
			if (event.type == sf::Event::Resized)
				state = EV_UPDATEGAMESCRSIZE;
		}
		auto start = clock();
		window.clear();
		switch (state) {	
		case EV_QUIT:
			cout << "Game closing...";
			window.close();
			return 0;
		case EV_UPDATE:
			engine.update(gameTime);
			break;
		case EV_UPDATEGAMESCRSIZE:		
			GAMESCR->SetSize(Vector2i(window.getSize()));
			window.close();
			window.create(sf::VideoMode(GAMESCR->GetSize().x, GAMESCR->GetSize().y), "Bloaty's Flip or Flop");
			state = EV_RUN;
		case EV_RUN:
			engine.update(gameTime);
			engine.audio(gameTime);
		case EV_DRAW:
		case EV_IDLE:
			engine.draw();
			break;
		}
		window.display();
		gameTime += double(clock() - start) / CLOCKS_PER_SEC;
	}

	return 0;
}
