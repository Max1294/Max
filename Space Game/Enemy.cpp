#include "Enemy.h"

Enemy::Enemy(Texture* texture, Texture* bulletTexture, Vector2f position)
{
	srand(time(NULL));
	this->texture = texture;
	this->bulletTexture = bulletTexture;
	
	this->sprite.setTexture(*this->texture);
	this->sprite.setPosition(position);
	
	this->maxShootTimer = 50;
	this->shootTimer = 0;
	this->rotation = 0.f;

}

Enemy::~Enemy()
{}

void Enemy::Shoot(Vector2f direction)
{
	this->bullets.push_back(Bullet(this->bulletTexture,
		this->bulletPosition,
		direction, 0.5f, 25.f, 0.25f, BulletType::Usual));

	this->shootTimer = 0;
}

void Enemy::Update(double rotation)
{
	this->rotation = rotation;
	this->sprite.setOrigin(this->getGlobalBounds().width / 2, this->getGlobalBounds().height / 2);

	this->bulletPosition = Vector2f(this->sprite.getPosition().x - (this->sprite.getGlobalBounds().width / 4), 
		this->sprite.getPosition().y);

	this->sprite.setRotation(-this->rotation);
 
	if (this->shootTimer >= this->maxShootTimer)
	{		
		double degree = (this->rotation / 57.296f);

		this->Shoot(Vector2f(sin(degree), cos(degree)));
	}


	if (this->shootTimer < this->maxShootTimer)
	{
		this->shootTimer++;
	}

	for (size_t i = 0; i < bullets.size(); i++)
	{
		this->bullets[i].Update();
	}
}

void Enemy::Draw(RenderTarget& target)
{
	target.draw(this->sprite);

	for (int i = 0; i < this->bullets.size(); i++)
	{
		this->bullets[i].Draw(target);
	}
}
