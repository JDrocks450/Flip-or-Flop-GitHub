#pragma once
#include <iostream>
#include <queue>
#include <string>
#include <map>
#include "engine.h"
#include "Stack.h"

using namespace std;

namespace FlipOrFlop {
	struct GameMessage {
		GameMessage(string text) {
			SetText(text);
		}

		string GetText() {
			return message;
		}
		void SetText(string text) {
			message = text;
		}

	private:
		string message;
	};

	class Messenger : public GameObject
	{
		bool status = 1;
		queue<string> messages;
	public:
		Messenger();
		~Messenger();

		enum MESSAGES {
			BURNT_PATTY,
			WRONG_COMBO,
			UNDERCOOKED,
			OUT_OF_TIME,
			NICE_JOB,
			WRONG_ORDER,
			ORDER_SUBMIT
		};
	private:
		const map<MESSAGES, string> MessageList = {
			{BURNT_PATTY, "BURNT!\nWatch The Patty Laddy!"},
			{WRONG_COMBO, "Wrong\nCombination!"},
			{UNDERCOOKED, "Undercooked\nPatty!"},
			{OUT_OF_TIME, "You're Fired!"},
			{NICE_JOB, "Nice Job,\nLad!"},
			{WRONG_ORDER, "You call that\na burger?"}
		};

	public:
		inline bool GetStatus() { return status; }
		inline void SetStatus(bool value) { status = value; }

		inline void DisplayMessage() {
			if (messages.size() == 0)
				return;
			cout << messages.front() << endl;
			messages.pop();
			SetStatus(true);
		}

		inline void EnqueueMessage(string message) {
			messages.push(message);
		}

		inline void EnqueueMessage(MESSAGES showMessage) {
			EnqueueMessage(MessageList.at(showMessage));
		}

		void Update(double gt);
		void Draw(sf::RenderTarget& target, sf::RenderStates states);
	};
	extern Messenger* MESSENGER;

	class Order : public GameObject {
	private:
		float SPEED = 700;
		double _lastgametime = 0;
		Topping* displayedTopping;
		float topping_desiredScale = .09F;		
		bool Added = false, Adding = false;
	public:
		enum MODE{
			//require the topping to be avoided
			EXCLUDE,
			//require the topping to be the only topping
			EXCLUSIVE_INCLUDE
		};

		//Creates an order and adds it to ORDERS vector
		Order(Topping::TOPPING exclusions, MODE mode, double gt);
		~Order();

		Topping::TOPPING Exclusions;
		MODE Mode;		
		bool Completed = false;

		string getFileNameString() { return "order.png"; }

		int getIndex();

		bool CheckBurger(Burger* Submission);
		void Complete();

		void Load();
		void Update(double gt);
		void Draw(RenderTarget& win, RenderStates s);
	};
	extern vector<Order*> ORDERS;
}

