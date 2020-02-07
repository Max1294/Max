#pragma once
#include "Bullet.h"


enum AbilitiesType { Shield = 3, Rocket = 5, TripleGun = 7 };

class Ability
{
	Texture* abilitiesTexture;
	Sprite sprite;

	int aliveTimer;
	int aliveTimerMax;
	bool isAlive;
	int Type;

	
public:
	Ability(Texture* abilitiesTexture, Vector2f position, int type);
	~Ability();
	
	

	// Accessors
	inline Vector2f getPosition() const { return this->sprite.getPosition(); }
	inline FloatRect getGlobalBounds()const { return this->sprite.getGlobalBounds(); }
	const int getType() const { return this->Type; }
	const bool IsAlive() const { return this->isAlive; }

	// Functions
	void Update();
	void Draw(RenderTarget& target);

	bool checkCollision(FloatRect rect);
};
