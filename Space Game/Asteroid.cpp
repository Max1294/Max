#include "Asteroid.h"

Asteroid::Asteroid(Texture* texture,
	Vector2f position, int type)
{
	this->texture = texture;
	this->sprite.setTexture(*this->texture);
	this->sprite.setPosition(position);

	this->type = type;
}

Asteroid::~Asteroid()
{}

void Asteroid::Update()
{
	switch (this->type)
	{
	case 0:
		this->sprite.move(0.f, 5.f);
		break;
	case 1:
		this->sprite.move(0.f, 3.f);
		break;
	default:
		break;
	}
}

void Asteroid::Draw(RenderTarget& target)
{
	target.draw(this->sprite);
}
