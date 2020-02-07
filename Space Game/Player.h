#pragma once

#include "Bullet.h"
#include "Ability.h"


class Player
{
private:
	Texture* texture;
	Sprite sprite;

	Texture* bulletTexture;
	std::vector<Bullet> bullets;

	// Direction
	Vector2f direction;
	Vector2f currentVelocity;
	float maxVelocity;
	float acceleration;
	float stabilizerForce;

	int shootTimer;
	int maxShootTimer;

	bool shootStatus;

public:

	Player(Texture* texture, Texture* bulletTexture);
	virtual ~Player();

	// Accessors



	inline std::vector<Bullet>& getBullets()
	{
		return this->bullets;
	}

	inline const Vector2f& getPosition() const
	{
		return this->sprite.getPosition();
	}

	inline void setPosition(Vector2f position)
	{
		this->sprite.setPosition(position);
	}

	inline FloatRect getGlobalBounds() const
	{
		return this->sprite.getGlobalBounds();
	}

	inline void setScale(Vector2f scale)
	{
		this->sprite.setScale(scale);
	}

	// Functions
	const void canShoot(bool shootStatus)
	{
		this->shootStatus = shootStatus;
	}

	void ResetPlayerSpeed()
	{
		this->currentVelocity = Vector2f(0.f, 0.f);
	}

	void insertBullets(Bullet&& bullet)
	{
		this->bullets.push_back(std::move(bullet));
	}

	void Movement();
	void Update(Vector2u windowBounds);
	void Draw(RenderTarget& target);
};
