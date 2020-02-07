#include "Player.h"

Player::Player(Texture* texture, Texture* bulletTexture)
{
	this->texture = texture;
	this->bulletTexture = bulletTexture;
	this->sprite.setTexture(*this->texture);
	
	this->sprite.setScale(0.2f, 0.2f);
	
	this->maxShootTimer = 20;
	this->shootTimer = this->maxShootTimer;
	this->maxVelocity = 20.f;
	this->acceleration = 0.3f;
	this->stabilizerForce = 0.1f;

	this->sprite.setPosition(960.f - (this->sprite.getGlobalBounds().width / 2),
		540.f - (this->sprite.getGlobalBounds().height / 2));

	this->shootStatus = true;
}

Player::~Player() 
{}

void Player::Movement()
{
	
	if (Keyboard::isKeyPressed(Keyboard::W))
	{
		this->direction.x = 0.f;
		this->direction.y = 1.f;

		if (this->currentVelocity.y < this->maxVelocity)
		{
			this->currentVelocity.y += this->direction.y * this->acceleration;
		}
	}
	if (Keyboard::isKeyPressed(Keyboard::S))
	{
		this->direction.x = 0.f;
		this->direction.y = -1.f;

		if (this->currentVelocity.y > -this->maxVelocity)
		{
			this->currentVelocity.y += this->direction.y * this->acceleration;
		}
	}
	if (Keyboard::isKeyPressed(Keyboard::A))
	{
		this->direction.x = -1.f;
		this->direction.y = 0.f;

		if (this->currentVelocity.x < this->maxVelocity)
		{
			this->currentVelocity.x += this->direction.x * this->acceleration;
		}
	}
	if (Keyboard::isKeyPressed(Keyboard::D))
	{
		this->direction.x = 1.f;
		this->direction.y = 0.f;

		if (this->currentVelocity.x < this->maxVelocity)
		{
			this->currentVelocity.x += this->direction.x * this->acceleration;
		}
	}
	if ((sf::Mouse::isButtonPressed(Mouse::Left) ||
		Keyboard::isKeyPressed(Keyboard::Space))
		&& this->shootTimer >= this->maxShootTimer)
	{
		if (this->shootStatus == true)
		{
			this->bullets.push_back(Bullet(bulletTexture,
				this->sprite.getPosition()+ Vector2f(30.f, 0.f),
				Vector2f(0.f, -1.f), 0.5f, 25.f, 0.5f, BulletType::Usual));
			
			this->shootTimer = 0; // reset timer
		}		
	}

	// Stabilization
	if (this->currentVelocity.y > 0)
	{
		this->currentVelocity.y -= this->stabilizerForce;

		if (this->currentVelocity.y < 0)
		{
			this->currentVelocity.y = 0;
		}
	}
	else if (this->currentVelocity.y < 0)
	{
		this->currentVelocity.y += this->stabilizerForce;

		if (this->currentVelocity.y > 0)
		{
			this->currentVelocity.y = 0;
		}
	}

	if (this->currentVelocity.x > 0)
	{
		this->currentVelocity.x -= this->stabilizerForce;

		if (this->currentVelocity.x < 0)
		{
			this->currentVelocity.x = 0;
		}
	}
	else if (this->currentVelocity.x < 0)
	{
		this->currentVelocity.x += this->stabilizerForce;
		
		if (this->currentVelocity.x > 0)
		{
			this->currentVelocity.x = 0;
		}
	}

	if (this->getPosition().y < 70.f)
	{
		this->currentVelocity = Vector2f(0.f, 0.f);
		this->sprite.move(0.f, 1.f);
	}
	else if (this->getPosition().y > 900.f)
	{
		this->currentVelocity = Vector2f(0.f, 0.f);
		this->sprite.move(0.f, -1.f);
	}
	else if (this->getPosition().x < 70.f)
	{
		this->currentVelocity = Vector2f(0.f, 0.f);
		this->sprite.move(1.f, 0.f);
	}
	else if (this->getPosition().x > 1800.f)
	{
		this->currentVelocity = Vector2f(0.f, 0.f);
		this->sprite.move(-1.f, 0.f);
	}
	else
	{
		this->sprite.move(this->currentVelocity.x, -(this->currentVelocity.y));
	}
}

void Player::Update(Vector2u windowBounds)
{
	if (this->shootTimer < this->maxShootTimer)
	{
		this->shootTimer++;
	}

	this->Movement();

	for (size_t i = 0; i < bullets.size(); i++)
	{
		this->bullets[i].Update();
	}
}

void Player::Draw(RenderTarget& target)
{
	target.draw(this->sprite);

	for (size_t i = 0; i < bullets.size(); i++)
	{
		this->bullets[i].Draw(target);
	}
}
