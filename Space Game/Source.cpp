#include <SFML/Graphics.hpp>
#include "Game.h"

int main()
{
    sf::RenderWindow window(sf::VideoMode(1920, 1080), "", sf::Style::Fullscreen);
    window.setKeyRepeatEnabled(false);

    Game game(&window);

    // Game loop
    while (window.isOpen())
    { 
        sf::Event event;
        
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::KeyPressed)
            {
                if (Keyboard::isKeyPressed(Keyboard::P))
                {
                    if(game.isGame() == true)
                        game.setPause();
                }
                if (Keyboard::isKeyPressed(Keyboard::Escape))
                {
                    window.close();
                }
            }
        }
  
        if (game.getPause() == false)
            game.Update();

        game.Draw();
    }

    return 0;
}
