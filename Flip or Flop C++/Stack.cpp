#include "pch.h"
#include "Stack.h"
#include <algorithm>

FlipOrFlop::Stack::Stack(Topping::TOPPING desiredStackType, int amount)
{
	Type = desiredStackType;
	Generate(amount);
}

FlipOrFlop::Stack::Stack(Topping::TOPPING desiredStackType, float x, float y, int desiredAmount, bool reverse)
{
	Type = desiredStackType;
	Generate(desiredAmount);
	SetPosition(x, y);
	REVERSE_ORDER = reverse;
}

FlipOrFlop::Stack::~Stack()
{
	for (int i = 0; i < Items.size(); i++)
		delete Items[i];	
}

void FlipOrFlop::Stack::Generate(int Amount)
{
	for (int i = 0; i < Amount; i++) {
		if (Type != Topping::TOPPING::PATTY)
			Items.push_back(new Topping(Type));
		else
			Items.push_back(new Patty());
		Items.back()->Load();
	}
}

void FlipOrFlop::Stack::Update(double gameTimeSeconds)
{
	float lastY = position.y;
	int count = 0;
	if (items_lastCount != Items.size() || location_changed) {		
		for (auto i : Items) {			
			lastY = lastY + (REVERSE_ORDER ? 1 : -1) * i->getStackOffset();
			i->sprite->setPosition(position.x, lastY);
			count++;
		}
		location_changed = false;
		items_lastCount = Items.size();
	}
}

void FlipOrFlop::Stack::Draw(RenderTarget &target, RenderStates states)
{
	if (REVERSE_ORDER)
		std::reverse(Items.begin(), Items.end());
	for (auto i : Items)		
		i->Draw(target, states);
	if (REVERSE_ORDER)
		std::reverse(Items.begin(), Items.end());
}


FlipOrFlop::Burger::Burger(float x, float y)
{
	SetPosition(x, y);
	Type = Topping::TOPPING::BUN_BOTTOM;
	Generate(1);
	REVERSE_ORDER = false;
	AddedToppings[int(Type)] = true;
}

FlipOrFlop::Burger::~Burger()
{
}

bool FlipOrFlop::Burger::AddTopping(Topping * T)
{
	if (Completed)
		return false;
	if (AddedToppings[(int)T->type])
		return false;
	else
		Items.push_back(T);
	AddedToppings[int(T->type)] = true;
	if (T->type == Topping::TOPPING::BUN_TOP)
		Completed = true;
	return true;
}

void FlipOrFlop::Burger::Update(double gt)
{
	Stack::Update(gt);
	if (Completed)
		SetIsClickableState(false);
}
