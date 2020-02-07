#pragma once
#include "Bullet.h"

enum AsteroidsType { SmallAsteroid = 0, BigAsteroid = 1 };

class Asteroid
{
	Texture* texture;
	Sprite sprite;

	int type;

public:
	Asteroid(Texture* texture, Vector2f position, int type);

	virtual ~Asteroid();

	// Accessors
	inline const int GetType()const { return this->type; }
	inline FloatRect getGlobalBounds()const { return this->sprite.getGlobalBounds(); }
	inline Vector2f getPosition() const { return this->sprite.getPosition(); }
	
	// Functions
	void Update();
	void Draw(RenderTarget& target);

};
