#include <iostream>
#include <SFML/Graphics.hpp>
#include "Pool.h"
using namespace std;
using namespace sf;

/*
* A partir d'un evenement (de clic), revoie la position de la souris
* \param event le clic de la souris
* \param window fenetre active
* \param taille du tableau
* \return un vec2 des coordonnées de la souris dans la fenetre
*/
Vector2i getPosMouse(Event event, RenderWindow& window, int taille) {
    Vector2i p;
    Vector2u fenetreSize = window.getSize();
    p.x = event.mouseButton.x / (float)(fenetreSize.x/taille);
    p.y = event.mouseButton.y / (float)(fenetreSize.y/taille);
    return p;
}

int main(){

    int TAILLE = 40;
    // création de la fenetre
    sf::RenderWindow window(VideoMode(width, height), "JEU DE LA VIE");
    window.setVerticalSyncEnabled(true);

    sf::Texture texturePlay;  // Play button
    {
        if(not texturePlay.loadFromFile("../resources/play.png")){
            cout << "Error loading \"play button\" file" << endl;
        }
    }
    sf::Texture texturePause;  // Pause button
    {
        if(not texturePause.loadFromFile("../resources/pause.png")){
            cout << "Error loading \"pause button\" file" << endl;
        }
    }
    sf::Texture texturePlus;  // Plus button
    {
        if(not texturePlus.loadFromFile("../resources/plus.png")){
            cout << "Error loading \"plus button\" file" << endl;
        }
    }
    sf::Texture textureLess;  // Less button
    {
        if(not textureLess.loadFromFile("../resources/less.png")){
            cout << "Error loading \"less button\" file" << endl;
        }
    }
    Vector2u buttonsSize = texturePlay.getSize(); // Get real scale
    Vector2f buttonResized;  // Change textures scales
    {
        buttonResized.x = (float) buttonsSize.x / width * 2.8;
        buttonResized.y = (float) buttonsSize.y / height * 2.8;
    }

    sf::Sprite Play;  // "Play" button resized and positionned
    {
        Play.setTexture(texturePlay);
        Play.setScale(buttonResized);
        Play.setPosition(Vector2f(width - 50,2));
    }
    sf::Sprite Pause;  // "Pause" button resized and positionned
    {
        Pause.setTexture(texturePause);
        Pause.setScale(buttonResized);
        Pause.setPosition(Vector2f(width - 50,2));
    }
    sf::Sprite Plus;  // "Plus" button resized and positionned
    {
        Plus.setTexture(texturePlus);
        Plus.setScale(buttonResized);
        Plus.setPosition(Vector2f(((float)width / 2) + 55,0));
    }
    sf::Sprite Less;  // "Less" button resized and positionned
    {
        Less.setTexture(textureLess);
        Less.setScale(buttonResized);
        Less.setPosition(Vector2f(((float)width / 2) - 90,0));
    }

    sf::Font police;
    if(not police.loadFromFile("../resources/Avenir_Next.ttc")){
        cout << "Error loading file" << endl;
    }

    sf::Text msg;  // "day x" text positionned
    {
        msg.setFont(police);
        msg.setFillColor(Color::Red);
        msg.setStyle(Text::Bold);
        msg.setCharacterSize(30);
        msg.setString(" day 0");
    }

    sf::Text stringTick;  // tick "0.x" text positionned
    {
        stringTick.setFont(police);
        stringTick.setFillColor(Color::Red);
        stringTick.setStyle(Text::Bold);
        stringTick.setCharacterSize(30);
        stringTick.setPosition(Vector2f(((float)width / 2) - 40,0));
        stringTick.setString("0.2s");
    }


    // création de la piscine
    Pool pool = Pool();

    // Mouse clic position
    Vector2i mouse;

    // variables that manage time
    sf::Clock clock;
    float timer = 0;
    float delay = 0.2;

    // variable that manage days -> "day x" message
    int day = -1;

    // variables that manage tick
    int intDelay;
    bool tickPlus = false;
    bool tickLess = false;

    // variable that pause -> liked to play/pause button
    bool pause = true;

    bool clic = false;

    while (window.isOpen()){

        ////// time for tick //////
        float time = clock.getElapsedTime().asSeconds();
        clock.restart();
        timer += time;

        Event event;
        ////// handle all events //////
        while(window.pollEvent(event))
        {
            // pour quitter ( + q ou croix rouge)
            if (event.type == Event::Closed) window.close();

            if(event.type == Event::MouseButtonPressed)
            {
                if (event.mouseButton.button == Mouse::Left)  // left clic detected
                {
                    // mouse = les coordonnées de la souris dans la fenetre
                    mouse = getPosMouse(event, window, TAILLE);
                    clic = true;
                }
            }
        }

        ////// Top side buttons actived //////
        if(clic)
        {
            if(mouse.y < 3)
            {
                if(mouse.x > TAILLE - 4) // play/pause button
                    pause = not pause;
                else if (mouse.x > TAILLE / 2) // plus button
                    tickPlus = true;
                else if (mouse.x > TAILLE / 4) // less button
                    tickLess = true;
            }
            clic = false;
        }

        ////// "drag and drop" //////
        if(Mouse::isButtonPressed(Mouse::Left))
        {
            mouse = Mouse::getPosition(window);
            mouse.x /= window.getSize().x / TAILLE;
            mouse.y /= window.getSize().y / TAILLE;
            pool.clic(mouse);
        }

        ////// change tick //////
        if(tickPlus)
        {
            delay -= 0.05;
            tickPlus = false;
            intDelay = (delay * 100);
            stringTick.setString("0." + to_string(intDelay) + "s");
        }
        else if (tickLess)
        {
            delay += 0.05;
            tickLess = false;
            intDelay = (delay * 100);
            stringTick.setString("0." + to_string(intDelay) + "s");
        }

        ////// Tick //////
        if(timer > delay and not pause)
        {
            pool.update(TAILLE);
            day++;
            msg.setString(" day " + to_string(day) + " |");
            timer = 0;
        }

        ////// Draw //////
        window.clear();
        pool.draw(window, TAILLE);
        if(pause) { window.draw(Play); }
        else { window.draw(Pause); }
        window.draw(stringTick);
        window.draw(msg);
        window.draw(Plus);
        window.draw(Less);
        window.display();

    }
    return 0;
}
