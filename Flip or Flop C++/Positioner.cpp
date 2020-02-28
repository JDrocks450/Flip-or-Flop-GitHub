#include "pch.h"
#include "Positioner.h"
#include "Messenger.h"
#include "INIReader.h"

FlipOrFlop::TitlePositioner::~TitlePositioner()
{
}

void FlipOrFlop::TitlePositioner::Load()
{
	UIButtons[0].Position = Vector2f(Parent->GetSize().x / 2, Parent->GetSize().y - 35);
	UIButtons[1].Position = Vector2f(Parent->GetSize().x / 2 + 300, Parent->GetSize().y - 35);
	backgroundRect.setFillColor(sf::Color(0, 0, 0, 150));
	backgroundRect.setSize(Vector2f(Parent_Size.x, HEIGHT));
	backgroundRect.setPosition(0, Parent_Size.y - HEIGHT);
}

void FlipOrFlop::TitlePositioner::Update(double gt)
{
	if (Parent->resized)
	{
		UIButtons[0].Position = Vector2f(Parent->GetSize().x / 2, Parent->GetSize().y - 35);
		UIButtons[1].Position = Vector2f(Parent->GetSize().x / 2 + 300, Parent->GetSize().y - 35);
		backgroundRect.setSize(Vector2f(Parent_Size.x, HEIGHT));
		backgroundRect.setPosition(0, Parent_Size.y - HEIGHT);
	}
	UIButtons[1].Update(gt);
	UIButtons[0].Update(gt);
	
}

void FlipOrFlop::TitlePositioner::Draw(RenderTarget & win, RenderStates states)
{
	win.draw(backgroundRect, states);
	UIButtons[0].Draw(win, states);
	UIButtons[1].Draw(win, states);
}

std::vector<FlipOrFlop::Patty*> FlipOrFlop::GrillingPatties;
sf::Rect<float> FlipOrFlop::STOVE_BOUNDARY;
FlipOrFlop::Burger* FlipOrFlop::BURGER_SLOTS[4]{ nullptr };

int FlipOrFlop::DIFFICULTY = 3, FlipOrFlop::MODE = 1;

FlipOrFlop::GameplayPositioner::~GameplayPositioner()
{
	Engine::DeleteVector(&UI_Objects);
	Engine::DeleteVector(&GrillingPatties);
}

//converts 800x600 screen coords for toppings on the counter into any screen size.
inline float convertY(float y) {
	return FlipOrFlop::STOVE_BOUNDARY.top + (y - 411);
}
inline float convertX(float x) {
	return FlipOrFlop::STOVE_BOUNDARY.left + (x - 166);
}

void FlipOrFlop::GameplayPositioner::PlaceObjects(std::vector<FlipOrFlop::Stack*>* UI_Stacks) {
	using namespace FlipOrFlop;
	Parent_Size.x = Parent->GetSize().x;
	Parent_Size.y = Parent->GetSize().y;
	Counter.setSize(Vector2f(Parent_Size.x, Stove.getScaledSize().y));
	Stove.sprite->setPosition(Parent_Size.x / 3 - 100, Parent_Size.y - Stove.getScaledSize().y);
	Counter.setPosition(0.F, Stove.sprite->getPosition().y);
	STOVE_BOUNDARY = Rect<float>(Stove.sprite->getPosition(), Stove.getScaledSize());
	auto bound = FlipOrFlop::STOVE_BOUNDARY;

	for (auto object : *UI_Stacks)
		if (object != nullptr) {
			bool del = true;
			for (auto burger : BURGER_SLOTS)
				if (burger == object)
					del = false;
			if (del)
				delete object;
		}
	UI_Stacks->clear();

	UI_Stacks->push_back(new Stack(Topping::TOPPING::KETCHUP, convertX(10), convertY(320), 1));
	UI_Stacks->push_back(new Stack(Topping::TOPPING::MUSTARD, convertX(-5), convertY(360), 1));
	UI_Stacks->push_back(new Stack(Topping::TOPPING::MAYONAISE, convertX(-20), convertY(400), 1));
	UI_Stacks->push_back(new Stack(Topping::TOPPING::CHEESE, convertX(60), convertY(380), 6));
	UI_Stacks->push_back(new Stack(Topping::TOPPING::LETTUCE, convertX(50), convertY(425), 6));
	UI_Stacks->push_back(new Stack(Topping::TOPPING::PICKLES, convertX(40), convertY(465), 6));
	UI_Stacks->push_back(new Stack(Topping::TOPPING::BUN_TOP, convertX(5), convertY(515), 3));
	
	int index = 0;
	for (auto burger : BURGER_SLOTS) {
		if (burger == nullptr) {
			if (index == 3) {
				BURGER_SLOTS[index] = new Burger(convertX(150), convertY(410));
			}
			else if (index < 3)
			{
				BURGER_SLOTS[index] = BURGER_SLOTS[index + 1];
				BURGER_SLOTS[index + 1] = nullptr;
			}
		}
		burger = BURGER_SLOTS[index];
		if (burger == nullptr) {
			BURGER_SLOTS[index] = new Burger(0, 0);
			burger = BURGER_SLOTS[index];
		}
		switch (index) {
		case 0:
			burger->SetPosition(Vector2f(convertX(150), convertY(410)));
			break;
		case 1:
			burger->SetPosition(Vector2f(convertX(138), convertY(440)));
			break;
		case 2:
			burger->SetPosition(Vector2f(convertX(124), convertY(470)));
			break;
		case 3:
			burger->SetPosition(Vector2f(convertX(111), convertY(500)));
			break;
		}
		UI_Stacks->push_back(burger);
		index++;
	}

	UI_Stacks->push_back(new Stack(Topping::TOPPING::PATTY, convertX(800 - 135), convertY(420), 5, false));
	UI_Stacks->push_back(new Stack(Topping::TOPPING::PATTY, convertX(800 - 120), convertY(460), 5, false));
	UI_Stacks->push_back(new Stack(Topping::TOPPING::PATTY, convertX(800 - 105), convertY(500), 5, false));

	extern Messenger * MESSENGER;
	UI_Objects.clear();
	UI_Objects.push_back(MESSENGER);
	UI_Objects.push_back(&Stove);
	UI_Objects.push_back(&Player);
	UI_Objects.push_back(&Squidward);
	for (auto stack : *UI_Stacks)
		UI_Objects.push_back(stack);
}

void FlipOrFlop::GameplayPositioner::LoadGameContext()
{
	INIReader r(Engine::ContentLoad("settings.ini"));	
	DIFFICULTY = r.GetInteger("gameplay", "difficulty", 0);
	MODE = r.GetInteger("gameplay", "mode", 0);
	cout << "GameSettings Loaded... DIFF: " << DIFFICULTY << " MODE: " << MODE << endl;
}

void FlipOrFlop::GameplayPositioner::Load()
{
	extern Messenger* MESSENGER;
	MESSENGER = new Messenger;	
	LoadGameContext();
	Counter.setFillColor(sf::Color(100, 100, 100, 255));
	Stove("stove.png");
	Stove.IsClickable = false;
	Stove.Load();	
	Player.Load();
	Player.sprite->setScale(.3, .3);
	Stove.sprite->setScale(.7F, .7F);	
	Squidward.Load();	
	PlaceObjects(&UI_Stacks);	
}

int SPONGEBOB_HoldingBurgerSlot = 0;

void FlipOrFlop::GameplayPositioner::Update(double gt)
{	
	if (Parent->resized)
		PlaceObjects(&UI_Stacks);
	for (auto stack : UI_Stacks)
		for (auto obj : stack->Items) {
			auto check = std::find(GrillingPatties.begin(), GrillingPatties.end(), obj);
			if (check != GrillingPatties.end())
			{
				stack->Items.erase(std::remove(stack->Items.begin(), stack->Items.end(), obj), stack->Items.end());
				GrillingPatties.push_back((Patty*)obj);
				continue;
			}
		}
	for (auto obj : UI_Objects)
		obj->Update(gt);
	for (auto it = GrillingPatties.begin(); it != GrillingPatties.end();) {
		auto patty = GrillingPatties[it - GrillingPatties.begin()];
		patty->Grill(gt);
		if (patty->currentState == Patty::DELETE) {
			GrillingPatties.erase(std::remove(GrillingPatties.begin(), GrillingPatties.end(), patty), GrillingPatties.end());
			delete patty;	
			if (GrillingPatties.size() == 0)
				it = GrillingPatties.end();
			else
				it++;
		}
		else it++;
	}
	if (!Player.IgnoringMouse) {
		int offset = Spongebob::ScreenPositionOffset;
		auto sPos = Player.sprite->getPosition();
		if (sPos.x - offset < 0)
			sPos.x = offset;
		else if (sPos.x + offset > Parent_Size.x - Player.getScaledSize().x)
			sPos.x = Parent_Size.x - Player.getScaledSize().x - offset;
		sPos.y = Counter.getPosition().y - Player.getScaledSize().y;
		Player.sprite->setPosition(sPos);
	}
	
	if (ORDERS.size() == 0)
		new Order(Topping::TOPPING::BUN_BOTTOM, Order::MODE::EXCLUDE, gt);
	for (auto order : ORDERS)
		order->Update(gt);

	if (!SPONGEBOB->HoldingObject) {
		bool found = false;
		int index = 0;
		for (auto patty : BURGER_SLOTS) {
			if (patty->Completed)
			{
				SPONGEBOB->Hold(patty, false);
				Squidward.OnScreen = true;
				found = true;
				SPONGEBOB_HoldingBurgerSlot = index;
				break;
			}
			index++;
		}
		if (!found)
			Squidward.OnScreen = false;
	}

	if (Squidward.OnScreen && Squidward.Received)
	{
		int index = 0;
		while (ORDERS[index]->Completed) {
			if (index < ORDERS.size() - 1)
				index++;
			else
				return;
		}
		Squidward.Received = false;
		if (ORDERS[index]->CheckBurger((Burger*)SPONGEBOB->heldObject))
			ORDERS[index]->Complete();
		//delete SPONGEBOB->heldObject;
		SPONGEBOB->Release();
		BURGER_SLOTS[SPONGEBOB_HoldingBurgerSlot] = nullptr;
		PlaceObjects(&UI_Stacks);
		Squidward.OnScreen = false;

		extern vector<Order*> ORDERS;
		for (int i = ORDERS.size(); i < DIFFICULTY + 1; i++)
			switch (DIFFICULTY) {
			case 1:
				new Order(Topping::TOPPING::BUN_BOTTOM, Order::MODE::EXCLUDE, gt);
				break;
			case 2:
			case 3:
				new Order(Topping::TOPPING(Engine::RangeRandom(0, Topping::ToppingNumber)), Order::MODE(Engine::RangeRandom(0, 1)), gt);
			}
	}
}

void FlipOrFlop::GameplayPositioner::Draw(RenderTarget& win, RenderStates states)
{
	win.draw(Counter, states);
	for (auto obj : UI_Objects)
		if (obj != nullptr)
			obj->Draw(win, states);
	for (auto obj : GrillingPatties)
		obj->Draw(win, states);
	extern vector<Order*> ORDERS;
	for (auto order : ORDERS)
		order->Draw(win, states);
}
