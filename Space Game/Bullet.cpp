#include "Bullet.h"

Bullet::Bullet(Texture* texture, Vector2f position,
	Vector2f direction, float initialVelocity, float maxVelocity, 
	float acceleration, int type)
{
	this->texture = texture;
	this->sprite.setTexture(*this->texture);

	this->maxVelocity = maxVelocity;
	this->acceleration = acceleration;
	this->direction = direction;
	this->currentVelocity = Vector2f(
		initialVelocity * this->direction.x,
		initialVelocity * this->direction.y
	);

	this->sprite.setPosition(position);

	this->type = type;
}

Bullet::~Bullet()
{}

void Bullet::Movement()
{

	if (this->acceleration > 0.f)
	{
		if (this->currentVelocity.x < this->maxVelocity)
			this->currentVelocity.x += this->acceleration * this->direction.x;

		if (this->currentVelocity.y < this->maxVelocity)
			this->currentVelocity.y += this->acceleration * this->direction.y;
	}
	else
	{
		this->currentVelocity = Vector2f(
			this->maxVelocity * this->direction.x,
			this->maxVelocity * this->direction.y);
	}

	this->sprite.move(this->currentVelocity);
}

void Bullet::Update()
{
	this->Movement();
}

void Bullet::Draw(RenderTarget& target)
{
	target.draw(this->sprite);
}
