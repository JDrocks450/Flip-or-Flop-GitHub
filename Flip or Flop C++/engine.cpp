#include "pch.h"
#include "engine.h"
#include <string>
#include "Positioner.h"

GameScreen* GAMESCR = nullptr;
vector<pair<GameObject*, Image*>> Clickables;
vector<GameObject*> SecondPass;
sf::Vector2i MOUSE_POSITION;

unsigned int RAND_SEED = time(0);

void Engine::SetRandSeed(int seed)
{
	RAND_SEED = seed;
}

unsigned int Engine::GetRandSeed()
{
	return RAND_SEED;
}

bool Engine::perpixelCheck(int ClickablesIndex)
{
	auto gameobject = get<0>(Clickables[ClickablesIndex]);
	auto image = get<1>(Clickables[ClickablesIndex]);

	if (!gameobject->IsPerpixelClickCheck || image == nullptr)
		return true;

	auto game_pos = gameobject->sprite->getPosition();
	auto pos = Vector2f(MOUSE_POSITION) - game_pos;
	auto scaledMousePos = pos / gameobject->sprite->getScale().x;
	if (image->getPixel(int(scaledMousePos.x), int(scaledMousePos.y)).a != 0)
		return true;
	return false;
}

Engine::Engine(RenderWindow* window)
{
	cout << "Engine Started..." << endl;
	this->window = window;
}


Engine::~Engine()
{
}

void Engine::LoadBackground(string FilePath)
{
	auto t = new Texture();
	t->loadFromFile(ContentLoad(FilePath));
	LoadBackground(t);
}

void Engine::LoadBackground(sf::Texture* texture)
{
	if (background != nullptr) {
		if (background->getTexture() != nullptr)
			delete background->getTexture();
		delete background;
	}
	background = new Sprite(*texture);
	texture->setSmooth(true);	
}

void Engine::LoadMusic(string FilePath)
{
	auto t = new SoundBuffer();
	t->loadFromFile(ContentLoad(FilePath));
	LoadMusic(t);
}

void Engine::LoadMusic(sf::SoundBuffer* sounddata)
{
	if (music != nullptr) {
		if (music->getBuffer() != nullptr)
			delete music->getBuffer();
		delete music;
	}
	music = new Sound(*sounddata);	
	music->setLoop(true);
}

void Engine::CreateWindowsProcess(string fileName)
{
	extern void (*SHELL_EXEA)(const char* FILENAME);
	SHELL_EXEA(fileName.c_str());
}

void Engine::gameObject_addAsSecondPass(GameObject* obj)
{
	extern vector<GameObject*> SecondPass;
	SecondPass.push_back(obj);
}

void Engine::load()
{
	GAMESCR->Load(this);
}

bool _mouseDown_disableOnClick = false;

void Engine::checkMouseClick(double gameTime)
{
	sf::IntRect MouseRect(MOUSE_POSITION, Vector2i(1, 1));
	vector<int> hits;
	int index = 0;
	for (auto tuple : Clickables) {
		auto gameobject = std::get<0>(tuple);
		if (gameobject->IsClickable)
			if (gameobject->getHitbox().intersects(MouseRect))
				hits.push_back(index);
		index++;
	}
	if (hits.size() > 0) {
		auto topHit = hits[0];
		for (auto rev_iterator = hits.begin(); rev_iterator != hits.end(); rev_iterator++)
			if (perpixelCheck(*rev_iterator)) {
				topHit = *rev_iterator;
			}
		get<0>(Clickables[topHit])->OnClick(gameTime);
		_mouseDown_disableOnClick = true;
	}
}

void Engine::update(double gameTimeSeconds)
{
	background->setScale(GameObject::getScaleFromSize(Vector2f(window->getSize()), Vector2f(background->getTexture()->getSize())));
	MOUSE_POSITION = Mouse::getPosition(*window);
	if (GAMESCR->loaded == false)
 		load();
	if (GAMESCR->Positioner != nullptr)
		GAMESCR->Positioner->Update(gameTimeSeconds);
	if (Mouse::isButtonPressed(Mouse::Button::Left)) {
		if (!_mouseDown_disableOnClick)
			checkMouseClick(gameTimeSeconds);
	}
	else if (_mouseDown_disableOnClick)
		_mouseDown_disableOnClick = false;
}

void Engine::audio(double gameTimeSeconds)
{
	if (music->getStatus() != Sound::Playing)
		music->play();
}

void Engine::draw()
{
	if (background != nullptr)
		window->draw(*background);	
	if (GAMESCR->Positioner != nullptr)
		GAMESCR->Positioner->Draw(*window, RenderStates::Default);
	secondPass(*window, RenderStates::Default);
}

void Engine::secondPass(sf::RenderTarget &window, sf::RenderStates state)
{
	extern vector<GameObject*> SecondPass;
	if (SecondPass.size() == 0)
		return;
	vector<GameObject*> order;
	GameObject * least = SecondPass[0];
	int index = 0;
	while (index < SecondPass.size()) {
		for (int i = 0; i < SecondPass.size(); i++) {
			auto obj = SecondPass[i];
			if (obj == least)
				continue;
			if (find(order.begin(), order.end(), least) != order.end())
				continue;
			if (obj->SecondPassRenderOrderIndex < least->SecondPassRenderOrderIndex) {
				least = obj;
			}
		}
		if (find(order.begin(), order.end(), least) != order.end())
			break;
		order.push_back(least);
		index++;
	}
	for (auto obj : order) {
		obj->engine_drawingassecondpass = true;
		obj->Draw(window, state);
		obj->engine_drawingassecondpass = false;
	}
	SecondPass.clear();
}

int gameObjectsCreated = 0;

#pragma region GameObject
GameObject::GameObject()
{
}

GameObject::GameObject(string FileNameString)
{
	_fileNameString = FileNameString;
}

GameObject::~GameObject()
{
	cout << "DELETE: GameObject, Freeing: " << sizeof(*this) << " bytes" << endl;
	if (Loaded || sprite != nullptr)
		delete sprite;	
	if (removableClickablesPair != NULL)
		Clickables.erase(remove(Clickables.begin(), Clickables.end(), *removableClickablesPair), Clickables.end());
	if (!IsPerpixelClickCheck && removableClickablesPair != nullptr) {
		delete removableClickablesPair->second;
	}
	gameObjectsCreated--;
}

void GameObject::operator()(string FileNameString)
{
	_fileNameString = FileNameString;
}

sf::Vector2<float> GameObject::getScaledSize()
{
	return Vector2f(sprite->getGlobalBounds().width, sprite->getGlobalBounds().height);
	auto textsize = sprite->getTexture()->getSize();
	auto scale = sprite->getScale();
	auto x = scale.x * textsize.x;
	auto y = scale.y * textsize.y;
	return Vector2f(x,y);
}

sf::Rect<int> GameObject::getHitbox()
{
	return IntRect(sprite->getGlobalBounds());
}

Vector2f GameObject::getScaleFromSize(sf::Vector2f newsize, sf::Vector2f oldsize)
{
	return Vector2f(newsize.x / oldsize.x, newsize.y / oldsize.y);
}

void GameObject::setScaledSize(float x, float y)
{
	setScaledSize(Vector2f(x, y));
}

void GameObject::setScaledSize(sf::Vector2f newsize)
{
	auto oldsize = getScaledSize();
	sprite->setScale(getScaleFromSize(newsize, oldsize));
}

static map<string, Texture*> LoadedTextures;
static map<string, Image*> LoadedPerpixelImages;

void GameObject::Load()
{
	if (Loaded == false) {
		gameObjectsCreated++;
		cout << "CREATE: GameObject #" << gameObjectsCreated << ", " << sizeof(*this) << " bytes" << endl;
	}
	else
		cout << "GameObject RELOADING... Loaded GameObjects: " << gameObjectsCreated << endl;
	Loaded = false;
	texture = nullptr;
	auto filePath = Engine::ContentLoad(getFileNameString());
	texture = LoadTexture(filePath);
	texture->setSmooth(true);
	if (sprite != nullptr)
		sprite->setTexture(*texture);
	else
		sprite = new Sprite(*texture);
	sprite->setTextureRect(IntRect(Vector2i(0, 0), Vector2i(texture->getSize())));
	if (IsClickable && !clickableAdded) {
		clickableAdded = true;
		if (IsPerpixelClickCheck) {
			Image* image;
			if (LoadedPerpixelImages.find(filePath) != LoadedPerpixelImages.end()) {
				image = LoadedPerpixelImages[filePath];
				cout << "PERPIXEL: Load from Memory: " << filePath << endl;
			}
			else
			{
				image = new Image(texture->copyToImage()); //copy a copy ;)
				LoadedPerpixelImages.emplace(make_pair(filePath, image));
				cout << "PERPIXEL: Copy from Texture: " << filePath << endl;
			}
			Clickables.push_back(make_pair(this, image));
		}
		else		
			Clickables.push_back(make_pair(this, new Image()));		
		removableClickablesPair = new pair<GameObject*, Image*>(*(Clickables.end()-1));
	}
	Loaded = true;
}

int TEX_POOL_GETSIZE(std::map<string, Texture*> map) {
	int size = 0;
	for (auto p : map)
		size += sizeof(*p.second);
	return size;
}

Texture * GameObject::LoadTexture(string filePath)
{
	Texture* texture = nullptr;
	try
	{
		texture = LoadedTextures.at(filePath);
	}
	catch (const std::exception&)
	{
		;
	}
	if (texture == nullptr) {
		texture = new Texture();
		if (!texture->loadFromFile(filePath)) {
			cout << "Setting texture to default..." << endl;
			if (!texture->loadFromFile(Engine::ContentLoad("missing.png")))
				cout << "Crucial texture load failure!" << endl;
		}
		LoadedTextures.emplace(std::make_pair(filePath, texture));
		cout << "TEX_LOAD: File: " << filePath << endl << "TEX_POOL_GETSIZE: Size: " << sizeof(LoadedTextures) + TEX_POOL_GETSIZE(LoadedTextures) << " bytes" << endl;
	}
	else
		cout << "TEX_LOAD: Memory: " << filePath << endl;
	return texture;
}

void GameObject::Update(double gameTimeSeconds)
{
	
}

void GameObject::Draw(RenderTarget &window, RenderStates states)
{	
	if (SecondPassRenderOrderIndex == -1)
		window.draw(*sprite, states);
	else
		if (engine_drawingassecondpass)
			window.draw(*sprite, states);
		else
			Engine::gameObject_addAsSecondPass(this);
}
#pragma endregion

RenderWindow* renderWindow = nullptr;

GameScreen::GameScreen(int w, int h, float x, float y, GameObject* positioner)
{
	width = w;
	height = h;
	GameScreen::x = x;
	GameScreen::y = y;
	GAMESCR = this;
	Positioner = positioner;
}


GameScreen::~GameScreen()
{
	cout << "DELETE: GameScreen " << sizeof(*this) << " bytes" << endl;
	delete Positioner;
}

void GameScreen::SetBackground(string path)
{
	HasBackground = true;
	BackgroundPath = path;
}

void GameScreen::SetMusic(string path)
{
	HasBackgroundMusic = true;
	BackgroundMusicPath = path;
}

sf::Vector2i GameScreen::GetSize()
{
	return sf::Vector2i(width, height);
}

void GameScreen::SetSize(int width, int height)
{
	GameScreen::width = width;
	GameScreen::height = height;
	resized = true;
}

void GameScreen::Load(Engine* engine)
{
	if (Positioner != nullptr)
		Positioner->Load();
	if (HasBackground)
		engine->LoadBackground(BackgroundPath);
	if (HasBackgroundMusic)
		engine->LoadMusic(BackgroundMusicPath);
	loaded = true;
}

GameScreen* GameScreen::CreateGameScreen(RenderWindow* window, SCREENS screen, int x, int y)
{
	if (window != nullptr)
		renderWindow = window;
	else
		window = renderWindow;
	if (GAMESCR != nullptr)
		delete GAMESCR;
	int w = window->getSize().x, h = window->getSize().y;
	GameScreen* p = nullptr;
	switch (screen) {
	case GAMEPLAY:
		p = new GameScreen(w, h, x, y, nullptr);
		p->Positioner = new FlipOrFlop::GameplayPositioner();
		p->SetBackground("kitchen.jpg");
		cout << "GameScreen switched to: GAMEPLAY" << endl;
		p->SetMusic("sound\\gameplay.wav");
		p->Screen = screen;
		return p;
	case TITLESCR:
		p = new GameScreen(w, h, x, y, nullptr);
		p->Positioner = new FlipOrFlop::TitlePositioner();
		p->SetBackground("mainmenu.png");
		p->SetMusic("sound\\mainmenu.wav");		
		cout << "GameScreen switched to: TITLESCR" << endl;
		return p;
	}
}
