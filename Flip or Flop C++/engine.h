#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <string>
#include <random>
#include <functional>
#include <iostream>

using namespace sf;
using namespace std;

extern Vector2i MOUSE_POSITION;

class GameObject {
protected:
	string _fileNameString = "missing.png";
	Texture* texture = nullptr;
	
	bool clickableAdded = false;
	pair<GameObject*, Image*>* removableClickablesPair = nullptr;

	virtual string getFileNameString() {
		return _fileNameString;
	}
	
public:	
	GameObject();
	GameObject(string FileNameString);
	~GameObject();

	bool Loaded = false;
	bool IsClickable = true;
	bool IsPerpixelClickCheck = false;

    //used by the engine only
	bool engine_drawingassecondpass = false;
	void operator ()(string FileNameString);

	//Second pass flags objects to be rendered after other objects with an index of -1. The render order dictates the order of the render targets.
	int SecondPassRenderOrderIndex = -1;

	sf::Vector2<float> getScaledSize();
	sf::Rect<int> getHitbox();

	static Vector2f getScaleFromSize(sf::Vector2f newsize, sf::Vector2f oldsize);
	void setScaledSize(float x, float y);
	void setScaledSize(sf::Vector2f newsize);

	Sprite* sprite = nullptr;
	virtual void Load();
	static Texture* LoadTexture(string FileName);
	virtual void Update(double gameTimeSeconds);	
	inline virtual void OnClick(double gameTimeSeconds) {};
	virtual void Draw(sf::RenderTarget &target, sf::RenderStates states);
};

extern vector<GameObject*> SecondPass;

class Engine
{
	Sprite* background = nullptr;
	Sound* music = nullptr;	

	static void SetRandSeed(int seed);
	static unsigned int GetRandSeed();

	bool perpixelCheck(int ClickablesIndex);
public:
	Engine(RenderWindow* window);
	~Engine();

	RenderWindow* window;	

	static inline string ContentLoad(string FileName) {
		auto path = string("C:\\Users\\xXJDr\\OneDrive\\Pictures\\Games\\Flip or Flop\\shipping\\");
		if (FileName.find('.', 0) > FileName.size())
		{
			cout << "CONTENTLOAD: Asset Path has no file extension... png assumed" << endl;
			FileName += ".png";
		}
		return path + FileName;
	}
	template<class T>
	inline static void DeleteVector(vector<T*>* vector) {
		for (auto tex : *vector)
			delete tex;
		vector->clear();
	}

	template <typename T>
	static T RangeRandom(T min, T max) { RangeRandom(int(min), int(max)); }
	template<> static int RangeRandom(int min, int max);
	template<> static float RangeRandom(float min, float max);

	void LoadBackground(string FilePath);
	void LoadBackground(sf::Texture* texture);
	void LoadMusic(string FilePath);
	void LoadMusic(sf::SoundBuffer* sounddata);

	static void CreateWindowsProcess(string fileName);
	static void gameObject_addAsSecondPass(GameObject* obj);

	void load();
	void checkMouseClick(double gameTime);
	void update(double gameTimeSeconds);
	void audio(double gameTimeSeconds);
	void draw();
	void secondPass(sf::RenderTarget& window, sf::RenderStates state);
};

template<>
inline int Engine::RangeRandom(int min, int max)
{
	auto r = std::bind(uniform_int_distribution(min, max), mt19937(GetRandSeed()));
	auto seed = std::bind(uniform_int_distribution(0, 64), mt19937(random_device{}()));
	SetRandSeed(seed());
	return r();
}

template<>
inline float Engine::RangeRandom(float min, float max)
{
	auto r = std::bind(uniform_real_distribution(min, max), mt19937(GetRandSeed()));
	auto seed = std::bind(uniform_int_distribution(0, 64), mt19937(random_device{}()));
	SetRandSeed(seed());
	return r();
}

struct GameScreen {
private:
	int width, height;
	float x, y;
public:
	GameObject* Positioner;

	bool loaded = false;
	bool resized = false;

	bool HasBackground = false;
	string BackgroundPath;

	bool HasBackgroundMusic = false;
	string BackgroundMusicPath;

	GameScreen(int w, int h, float x, float y, GameObject* positioner);
	~GameScreen();

	enum SCREENS {
		TITLESCR,
		TUTORIAL,
		GAMEPLAY,
		EMPLOYEEOFTHEMONTH,
		GAMEOVER
	};
	SCREENS Screen = TITLESCR;

	//Call before running Load() to see the background.
	void SetBackground(string path);
	void SetMusic(string path);

	sf::Vector2i GetSize();
	void SetSize(int width, int height);
	template <typename T>
	void SetSize(Vector2<T> size);

	void Load(Engine* engine);

	static GameScreen* CreateGameScreen(RenderWindow* window, SCREENS screen, int x = -1, int y = -1);
};
extern GameScreen* GAMESCR;

template<typename T>
inline void GameScreen::SetSize(Vector2<T> size)
{
	SetSize(int(size.x), int(size.y));
}
