#include "Game.h"

Game::Game(RenderWindow* window)
{
	srand(time(NULL));
	this->font.loadFromFile("Textures\\arial.ttf");

	this->window = window;
	this->window->setFramerateLimit(60);
	this->runGame = true;
	
	this->score = 0;
	
	this->AsteroidSpawnTimerMax = 20;
	this->AsteroidSpawnTimer = this->AsteroidSpawnTimerMax;
	
	// init texture
	this->playertexture.loadFromFile("Textures\\ship.png");
	this->bulletTexture.loadFromFile("Textures\\bullet.png");
	this->rocketBulletTexture.loadFromFile("Textures\\rocket.png");
	this->bigAsteroidTexture.loadFromFile("Textures\\big_asteroid.png");
	this->smallAsteroidTexture.loadFromFile("Textures\\small_asteroid.png");
	this->enemyTexture.loadFromFile("Textures\\enemy.png");
	this->tripleGunTexture.loadFromFile("Textures\\TripleGunAbility.png");
	this->rocketTexture.loadFromFile("Textures\\RocketAbility.png");
	this->shieldTexture.loadFromFile("Textures\\shield.png");

	// Init Map
	this->MapTexture.loadFromFile("Textures\\Background.jpg");
	this->MapSprite.setTexture(this->MapTexture);
	this->MapSprite.setScale(1.95f, 1.65f);

	// init player
	this->player = new Player(&this->playertexture, &bulletTexture);

	// init enemies
	this->EnemySpawnTimerMax = 800;
	this->EnemySpawnTimer = 0;
	
	// init circle for shield
	this->shield.push_back(CircleShape());

	// init Abilities
	this->isRocket = false;
	this->isShield = false;
	this->isTripleGun = false;

	this->TripleGunShootTimer = 1;
	this->TripleGunShootTimerMax = 20;
	
	this->AbilityDuration = 1000;
	this->ActivationRocketTimer = 0;
	this->PossibleRocketActivation = 500;
	// init timer
	this->timer = 0;

	// init pause
	this->PauseText.setFont(this->font);
	this->PauseText.setCharacterSize(62);
	this->PauseText.setFillColor(Color::Red);
	this->PauseText.setString(std::string{ "Pause" });
	this->PauseText.setStyle(Text::Bold);
	this->PauseText.setPosition(900, 500);
	
	this->isPause = false;
}

void Game::TimeBar()
{
	this->Timeline.setSize(Vector2f(100.f, 15.f));
	this->Timeline.setFillColor(Color(100.f, 50.f, 150.f, 200.f));
	
	this->Timeline.setPosition(this->player->getPosition());
	this->Timeline.setRotation(90.f);

	if (this->runGame == true)
		this->Timeline.setScale(Vector2f(1.f - (static_cast<float>(this->timer) / this->AbilityDuration), 1.f));
	else
		this->Timeline.setScale(Vector2f(0.f, 0.f));
}

void Game::EnemiesSpawn()
{
	if (this->EnemySpawnTimer < this->EnemySpawnTimerMax)
		this->EnemySpawnTimer++;
	
	if (this->EnemySpawnTimer >= this->EnemySpawnTimerMax)
	{
		if (this->Enemies.size() == 0 && this->runGame == true)
		{
				this->Enemies.push_back(
					Enemy(&this->enemyTexture, &this->bulletTexture,
						Vector2f(700.f + rand() % 500, 50.f)));
		}
		this->EnemySpawnTimer = 0;
	}
}

void Game::AsteroidsSpawn()
{
	// Update Asteroid timer
	if (this->AsteroidSpawnTimer < this->AsteroidSpawnTimerMax)
		this->AsteroidSpawnTimer++;

	// Spawn Asteroids
	if (this->AsteroidSpawnTimer >= this->AsteroidSpawnTimerMax)
	{
		this->Asteroids.push_back(Asteroid(
			&this->bigAsteroidTexture, Vector2f(100.f + rand() % 1700, 0.f),
			AsteroidsType::BigAsteroid)
		);

		this->Asteroids.push_back(Asteroid(
			&this->smallAsteroidTexture, Vector2f(100.f + rand() % 1700, 0.f),
			AsteroidsType::SmallAsteroid)
		);

		this->AsteroidSpawnTimer = 0;
	}
}

Game::~Game()
{
	delete player;
}

bool operator == (const Asteroid& a, const Asteroid& b)
{
	if (a.getGlobalBounds().intersects(b.getGlobalBounds()))
	{
		return true;
	}

	return false;
}

void Game::AsteroidsColiision()
{
	for (int i = 0; i < this->Asteroids.size(); i++)
	{
		for (int j = i+1; j < this->Asteroids.size(); j++)
		{
			if (this->Asteroids[i] == this->Asteroids[j])
			{
				if (this->Asteroids[i].GetType() == AsteroidsType::BigAsteroid &&
					this->Asteroids[j].GetType() == AsteroidsType::SmallAsteroid)
				{
					this->Asteroids.erase(this->Asteroids.begin() + j);
					DestroyBigAsteroid(this->Asteroids[i], i);
				}
				else if (this->Asteroids[i].GetType() == AsteroidsType::SmallAsteroid &&
					this->Asteroids[j].GetType() == AsteroidsType::BigAsteroid)
				{
					DestroyBigAsteroid(this->Asteroids[j], j);
					this->Asteroids.erase(this->Asteroids.begin() + i);
				}
				else
				{
					this->Asteroids.erase(this->Asteroids.begin() + j);
				}
				break;
			}	
		}
	}
}

void Game::DestroyBigAsteroid(const Asteroid& asteroid, size_t index)
{
	Vector2f prevPosition = asteroid.getPosition();

	this->Asteroids.erase(this->Asteroids.begin() + index);

	this->Asteroids.push_back(Asteroid(
		&this->smallAsteroidTexture, Vector2f(prevPosition.x + 50.f, prevPosition.y + 30.f),
		AsteroidsType::SmallAsteroid)
	);

	this->Asteroids.push_back(Asteroid(
		&this->smallAsteroidTexture, Vector2f(prevPosition.x, prevPosition.y),
		AsteroidsType::SmallAsteroid)
	);
}

void Game::gameOver()
{
	this->GameOverText.setFont(this->font);
	this->GameOverText.setCharacterSize(62);
	this->GameOverText.setFillColor(Color::Red);
	this->GameOverText.setString(std::string{ "    Game Over\n   Your score: " } +
							std::to_string(static_cast<int>(this->score)) + 
							std::string{ "\nFor restart press R\n" });
	this->GameOverText.setStyle(Text::Bold);
	this->GameOverText.setPosition(700, 400);

	this->player->getBullets().erase((this->player->getBullets().begin()),
		(this->player->getBullets().end()));

	this->Asteroids.erase(this->Asteroids.begin(), this->Asteroids.end());

	for (size_t i = 0; i < this->Enemies.size(); i++)
	{
		this->Enemies[i].getBullets().erase(this->Enemies[i].getBullets().begin(), this->Enemies[i].getBullets().end());
	}

	this->Enemies.erase(this->Enemies.begin(), this->Enemies.end());
	this->Abilities.erase(this->Abilities.begin(), this->Abilities.end());

	this->isShield = false;
	this->isRocket = false;
	this->isTripleGun = false;
	this->timer = 0;

	this->Timeline.setScale(Vector2f(0.f, 0.f));
	this->player->ResetPlayerSpeed();
	this->player->setScale(Vector2f(0.f, 0.f));
	this->runGame = false;
}

void Game::EnemyUpdate()
{
	for (size_t enemy = 0; enemy < this->Enemies.size(); enemy++)
	{
		float dX = (this->player->getPosition().x + (this->player->getGlobalBounds().width) / 2)
			- (this->Enemies[enemy].getPosition().x);

		float dY = (this->player->getPosition().y + (this->player->getGlobalBounds().height / 2))
			- (this->Enemies[enemy].getPosition().y);

		double rotation = atan2(dX, dY) * 180 / 3.14159265;

		this->Enemies[enemy].Update(rotation);

		// Shoot Enemies
		for (size_t bullet = 0; bullet < this->Enemies[enemy].getBullets().size(); bullet++)
		{
			this->Enemies[enemy].getBullets()[bullet].Update();

			if (this->Enemies[enemy].getBullets()[bullet].getGlobalBounds().intersects(this->shield[0].getGlobalBounds()))
			{
				this->Enemies[enemy].getBullets().erase(this->Enemies[enemy].getBullets().begin() + bullet);
				break;
			}

			// Collision enemy bullets with player bullets
			for (size_t i = 0; i < this->player->getBullets().size(); i++)
			{
				if (this->Enemies[enemy].getBullets()[bullet].getGlobalBounds().intersects(this->player->getBullets()[i].getGlobalBounds()))
				{
					if (this->player->getBullets()[i].getType() != BulletType::rocket)
					{
						this->player->getBullets().erase((this->player->getBullets().begin() + i));
					}

					this->Enemies[enemy].getBullets().erase(this->Enemies[enemy].getBullets().begin() + bullet);
					return;
				}	
			}
			
			if (this->Enemies[enemy].getBullets()[bullet].getPosition().y >= 1000.f ||
				this->Enemies[enemy].getBullets()[bullet].getPosition().x > 1900.f ||
				this->Enemies[enemy].getBullets()[bullet].getPosition().x < 20.f)
			{
				this->Enemies[enemy].getBullets().erase((this->Enemies[enemy].getBullets().begin() + bullet));
				break;
			}

			// player is killed
			if (this->Enemies[enemy].getBullets()[bullet].getGlobalBounds().intersects(this->player->getGlobalBounds()))
			{
				this->gameOver();			
				break;
			}
			
			// Asteroids
			for (size_t j = 0; j < this->Asteroids.size(); j++)
			{
				if (this->Enemies[enemy].getBullets()[bullet].getGlobalBounds().intersects(this->Asteroids[j].getGlobalBounds()))
				{
					switch (this->Asteroids[j].GetType())
					{
					case AsteroidsType::BigAsteroid:
						DestroyBigAsteroid(this->Asteroids[j], j);
						break;

					case AsteroidsType::SmallAsteroid:	
						this->Asteroids.erase(this->Asteroids.begin() + j);
						break;
					}
					this->Enemies[enemy].getBullets().erase(this->Enemies[enemy].getBullets().begin() + bullet);
					break;
				}
			}
		}
	}
}

void Game::BulletUpdate()
{
	for (size_t i = 0; i < this->player->getBullets().size(); i++)
	{
		this->player->getBullets()[i].Update();

		// Asteroids collision
		for (size_t k = 0; k < this->Asteroids.size(); k++)
		{
			// Collision bullets with asteroids	
			if (this->player->getBullets()[i].getGlobalBounds().intersects(this->Asteroids[k].getGlobalBounds()))
			{				
				if (this->player->getBullets()[i].getType() == BulletType::rocket)
				{
					this->Asteroids.erase(this->Asteroids.begin() + k);
					break;
				}
				else
				{
					if (this->Asteroids[k].GetType() == AsteroidsType::BigAsteroid)
					{
						score += 10;
						DestroyBigAsteroid(this->Asteroids[k], k);
					}
					else
					{
						// Abilities
						switch (this->AbilitiesGeneration())
						{
						case AbilitiesType::Shield:
							// Shield
							Abilities.push_back(Ability(&this->shieldTexture,
								this->Asteroids[k].getPosition(), AbilitiesType::Shield));
							break;

						case AbilitiesType::Rocket:
							// Rocket
							Abilities.push_back(Ability(&this->rocketTexture,
								this->Asteroids[k].getPosition(), AbilitiesType::Rocket));
							break;

						case AbilitiesType::TripleGun:
							// TripleGun
							Abilities.push_back(Ability(&this->tripleGunTexture,
								this->Asteroids[k].getPosition(), AbilitiesType::TripleGun));		
							break;

						default:
							break;
						}

						score += 50;
						this->Asteroids.erase(this->Asteroids.begin() + k);
					}
					this->player->getBullets().erase((this->player->getBullets().begin() + i));
					return; // return
				}
			}
		}

		// Kill enemies
		for (int n = 0; n < this->Enemies.size(); n++)
		{
			if (this->player->getBullets()[i].getGlobalBounds().intersects(this->Enemies[n].getGlobalBounds()))
			{
				score += 100;
				this->Enemies.erase(this->Enemies.begin() + n);
				break;
			}
		}

		// Window bounds ckeck
		if (this->player->getBullets()[i].getPosition().y <= 10 ||
			this->player->getBullets()[i].getPosition().x > 1900.f ||
			this->player->getBullets()[i].getPosition().x < 70.f)
		{		

			this->player->getBullets().erase((this->player->getBullets().begin() + i));
			break;
		}
	}
}

void Game::AsteroidsUpdate()
{
	for (size_t i = 0; i < this->Asteroids.size(); i++)
	{
		this->Asteroids[i].Update();

		if (this->shield[0].getGlobalBounds().intersects(this->Asteroids[i].getGlobalBounds()))
		{
			// shield
			this->Asteroids.erase(this->Asteroids.begin() + i);
			score += 20;
			break;
		}

		if (this->player->getGlobalBounds().intersects(this->Asteroids[i].getGlobalBounds()))
		{
			this->gameOver();
			break;
		}

		if (this->Asteroids[i].getPosition().y > 900.f)
		{
			this->Asteroids.erase(this->Asteroids.begin() + i);
			break;
		}
	}
}

void Game::AbilitiesUpdate()
{
	for (int i = 0; i < this->Abilities.size(); i++)
	{
		this->Abilities[i].Update();

		if (this->Abilities[i].IsAlive() == 0)
		{
			this->Abilities.erase(this->Abilities.begin() + i);
			break;
		}

		if (this->Abilities[i].checkCollision(this->player->getGlobalBounds()))
		{
			switch (this->Abilities[i].getType())
			{
			case AbilitiesType::Shield:
				this->isShield = true;
				this->isRocket = false;
				this->isTripleGun = false;
				this->timer = 0;
				
				if (this->isTripleGun == true)
				{
					this->player->getBullets().erase(this->player->getBullets().begin(),
						this->player->getBullets().end());
				}

				this->player->canShoot(true);
				
				break;

			case AbilitiesType::TripleGun:
				this->isTripleGun = true;
				this->isShield = false;
				this->isRocket = false;
				this->timer = 0;
				this->TripleGunShootTimer = 1;
				this->player->canShoot(false);

				this->shield.erase(this->shield.begin(), this->shield.end());
				this->shield.push_back(CircleShape());
				break;

			case AbilitiesType::Rocket:
				this->isRocket = true;		
				break;
			}
		
			this->Abilities.erase(this->Abilities.begin() + i);
			break;
		}
	}
}

void Game::UpdateScore()
{
	this->score += 0.01f;

	this->Score.setFont(this->font);
	this->Score.setCharacterSize(40);
	this->Score.setFillColor(Color::Green);
	this->Score.setString(std::string{"Score: "} +
		std::to_string(static_cast<int>(this->score)));
	this->Score.setStyle(Text::Bold);
	this->Score.setPosition(30, 10);
}

void Game::Update()
{
	if (this->runGame == true)
	{
		// Update score
		this->UpdateScore();

		// Update player
		this->player->Update(this->window->getSize());
		
		// Enemies Spawn
		this->EnemiesSpawn();

		// Asteroids Spawn
		this->AsteroidsSpawn();

		// Collision Asteroids
		this->AsteroidsColiision();

		// Enemies update
		this->EnemyUpdate();

		// Bullets update
		this->BulletUpdate();

		// Asteroids update
		this->AsteroidsUpdate();

		// Abilities update
		this->AbilitiesUpdate();

		//Rocket
		if (this->isRocket == true)
		{
			this->ActivationRocketTimer++;

			if (this->ActivationRocketTimer >= this->PossibleRocketActivation)
			{
				this->isRocket = false;
				this->ActivationRocketTimer = 0;
				return;
			}

			if (sf::Mouse::isButtonPressed(sf::Mouse::Right) == true)
			{
				this->Timeline.setScale(0.f, 0.f);
				this->shield.erase(this->shield.begin(), this->shield.end());
				this->shield.push_back(CircleShape());

				this->isShield = false;
				this->isTripleGun = false;

				this->player->getBullets().erase(this->player->getBullets().begin(),
					this->player->getBullets().end());

				this->player->insertBullets(Bullet(&rocketBulletTexture,
					this->player->getPosition(),
					Vector2f(0.f, -1.f), 0.5f, 25.f, 1.f, BulletType::rocket));

				this->player->insertBullets(Bullet(&rocketBulletTexture,
					this->player->getPosition() + Vector2f(38.f, 0.f),
					Vector2f(0.f, -1.f), 0.5f, 25.f, 1.f, BulletType::rocket));

				this->isRocket == false;
				this->player->canShoot(true);
				this->ActivationRocketTimer = 0;
				return;
			}
		}

		// Shield
		if (this->isShield == true)
		{
			if (this->Timer(this->AbilityDuration) == false)
			{
				this->isShield = false;
				this->shield.erase(this->shield.begin(), this->shield.end());
				this->shield.push_back(CircleShape());
				return;
			}

			this->TimeBar();

			this->shield[0].setRadius(80);
			this->shield[0].setFillColor(Color::Transparent);
			this->shield[0].setPosition(this->player->getPosition() + Vector2f(-40.f, -30.f));
			this->shield[0].setOutlineThickness(5);
			this->shield[0].setOutlineColor(Color(250, 150, 100));
		}

		// TripleGun
		if (this->isTripleGun == true)
		{
			if (this->Timer(this->AbilityDuration) == false)
			{
				this->isTripleGun = false;
				this->player->canShoot(true);
				return;
			}

			if (this->TripleGunShootTimer < TripleGunShootTimerMax)
				this->TripleGunShootTimer++;
			
			this->TimeBar();

			if (this->TripleGunShootTimer >= this->TripleGunShootTimerMax)
			{
				this->player->insertBullets(Bullet(&bulletTexture,
					this->player->getPosition() + Vector2f(6.f, 0.f),
					Vector2f(-0.1f, -1.f), 0.5f, 25.f, 0.5f, BulletType::Usual));

				this->player->insertBullets(Bullet(&bulletTexture,
					this->player->getPosition() + Vector2f(25.f, 0.f),
					Vector2f(0.f, -1.f), 0.5f, 25.f, 0.5f, BulletType::Usual));

				this->player->insertBullets(Bullet(&bulletTexture,
					this->player->getPosition() + Vector2f(44.f, 0.f),
					Vector2f(0.1f, -1.f), 0.5f, 25.f, 0.5f, BulletType::Usual));

				this->TripleGunShootTimer = 1;
			}
		}
	}

	// Restart game
	if (Keyboard::isKeyPressed(Keyboard::R) &&
		this->runGame == false)
	{
		this->score = 0;
		this->runGame = true;
		this->EnemySpawnTimer = 0;
		this->ActivationRocketTimer = 0;
		this->player->canShoot(true);
		this->player->setScale(Vector2f(0.2f, 0.2f));
		this->player->setPosition(Vector2f(960.f, 540.f));
		return;
	}
}

void Game::Draw()
{
	window->clear();
	
	// Draw background
	window->draw(this->MapSprite);

	// Draw Timer
	window->draw(this->Timeline);

	// Draw player
	player->Draw(*this->window);

	// Game Over
	if (this->runGame == false) 
		window->draw(this->GameOverText);

	// Draw Asteroids
	for (size_t i = 0; i < Asteroids.size(); i++)
	{
		this->Asteroids[i].Draw(*this->window);
	}

	// Draw Enemies
	for (int j = 0; j < Enemies.size(); j++)
	{
		this->Enemies[j].Draw(*this->window);
	}

	// Draw Shield	
	if (this->isShield == true)
	{
		this->window->draw(this->shield[0]);
	}

	// Draw abilities
	for (int i = 0; i < Abilities.size(); i++)
	{
		this->Abilities[i].Draw(*this->window);
	}

	// Draw Score
	if (this->runGame == true)
	{
		window->draw(this->Score);

		// Pause
		if (this->isPause == true)
			window->draw(this->PauseText);
	}

	window->display();
}
