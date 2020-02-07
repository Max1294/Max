#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <vector>

using namespace sf;

enum BulletType {Usual = 0, rocket};
class Bullet
{
private:
	Texture* texture;
	Sprite sprite;

	Vector2f currentVelocity;
	float maxVelocity;
	float acceleration;
	Vector2f direction;

	int type;

public:
	Bullet(Texture* texture, Vector2f position,
		Vector2f direction, float initialVelocity, 
		float maxVelocity, float acceleration, int type);
	
	virtual ~Bullet();


	// Accessors
	inline FloatRect getGlobalBounds() const { return this->sprite.getGlobalBounds(); }
	inline Vector2f getPosition() { return this->sprite.getPosition(); }
	inline const int getType() const { return this->type; }

	// Functions
	void Movement();
	void Update();
	void Draw(RenderTarget& target);

};
