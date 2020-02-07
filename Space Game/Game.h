#pragma once

#include "Player.h"
#include "Asteroid.h"
#include "Enemy.h"
#include "Ability.h"
#include "Bullet.h"

#include <string>

using namespace sf;

class Game
{
private:
	RenderWindow* window;

	// Textures
	Player* player;
	Texture playertexture;
	Texture bulletTexture;
	Texture smallAsteroidTexture;
	Texture rocketTexture;
	Texture bigAsteroidTexture;
	Texture enemyTexture;

	// Score
	double score;
	Text Score;

	// End of game
	Text GameOverText;
	Font font;
	bool runGame;

	// Map
	Texture MapTexture;
	Sprite MapSprite;
	
	// Asteroids
	std::vector<Asteroid> Asteroids;
	int AsteroidSpawnTimer;
	int AsteroidSpawnTimerMax;

	// Enemies
	std::vector<Enemy> Enemies;
	int EnemySpawnTimer;
	int EnemySpawnTimerMax;
	
	// Abilities
	std::vector<Ability> Abilities;
	Texture shieldTexture;
	Texture rocketBulletTexture;
	Texture tripleGunTexture;
	std::vector<CircleShape> shield;
	
	bool isShield;
	bool isRocket;
	bool isTripleGun;

	int timer;
	int TripleGunShootTimer;
	int TripleGunShootTimerMax;
	const bool Timer(const int maxTimer)
	{
		if (this->timer < maxTimer)
		{
			this->timer++;
			return true;
		}

		this->timer = 0;

		return false;
	}

	RectangleShape Timeline;
	int AbilityDuration;
	int PossibleRocketActivation;
	int ActivationRocketTimer;
	
	// Pause
	bool isPause;
	Text PauseText;
public:

	Game(RenderWindow* window);
	virtual ~Game();

	// Accessors
	inline RenderWindow& getWindow() { return *this->window; }
	
	inline const bool isGame() const { return this->runGame; }
	inline const bool getPause() const { return this->isPause; }
	inline void setPause() { this->isPause = !this->isPause; }

	// Function
	void AsteroidsColiision();
	void Update();
	void Draw();
	
	void DestroyBigAsteroid(const Asteroid& asteroid, size_t index);
	void gameOver();
	void EnemiesSpawn();
	void AsteroidsSpawn();
	void EnemyUpdate();
	void BulletUpdate();
	void AsteroidsUpdate();
	void AbilitiesUpdate();
	void UpdateScore();
	void TimeBar();
	const int AbilitiesGeneration()const { return 1 + rand() % 8; }
};
