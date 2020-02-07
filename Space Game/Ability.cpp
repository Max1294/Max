#include "Ability.h"

Ability::Ability(Texture* abilitiesTexture, Vector2f position, int type)
{
	this->abilitiesTexture = abilitiesTexture;
	this->sprite.setTexture(*this->abilitiesTexture);
	this->sprite.setPosition(position);

	this->aliveTimerMax = 200;
	this->aliveTimer = 0;

	this->Type = type;

	this->isAlive = true;
}

bool Ability::checkCollision(FloatRect rect)
{
	if (this->sprite.getGlobalBounds().intersects(rect))
		return true;

	return false;
}

Ability::~Ability()
{}

void Ability::Update()
{
	if (this->aliveTimer < this->aliveTimerMax)
		this->aliveTimer++;
	else
		this->isAlive = false;
}

void Ability::Draw(RenderTarget& target)
{
	target.draw(this->sprite);
}
