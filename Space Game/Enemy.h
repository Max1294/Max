#pragma once
#include "Bullet.h"
#include "Player.h"

class Enemy
{
	Texture* texture;
	Sprite sprite;
	
	Texture* bulletTexture;
	std::vector<Bullet> bullets;

	double rotation;

	int shootTimer;
	int maxShootTimer;

	Vector2f bulletPosition;

public:
	Enemy(Texture* texture, Texture* bulletTexture, Vector2f position);
	virtual ~Enemy();

	// Accessors
	inline FloatRect getGlobalBounds()const { return this->sprite.getGlobalBounds(); }
	inline Vector2f getPosition() const { return this->sprite.getPosition(); }
	inline std::vector<Bullet>& getBullets() { return this->bullets; }

	// Function
	void Shoot(Vector2f direction);
	void Update(double rotation);
	void Draw(RenderTarget& target);

};
